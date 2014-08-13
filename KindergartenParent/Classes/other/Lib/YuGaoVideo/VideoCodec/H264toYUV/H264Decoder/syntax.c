


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>


#include "global.h"
#include "common.h"
#include "syntax.h"
#include "bitstream.h"
#include "vlc.h"
#include "access.h"
#include "dct.h"



__attribute__((aligned(16))) static const long ICBPTab[] = { 0,16,32,15,31,47 };
__attribute__((aligned(16))) static const long p_v2b8[] = { 4 , 5, 6, 7, IBLOCK };
__attribute__((aligned(16))) static const long pred_type_mapping[] = { 
	MVPRED_MEDIAN,MVPRED_L,MVPRED_U,MVPRED_MEDIAN,MVPRED_UR,MVPRED_MEDIAN,MVPRED_MEDIAN,MVPRED_MEDIAN 
};
__attribute__((aligned(16))) static const long sub_mb_part_16x8[][2] = { { 0,5 } , { 8,13 } };
__attribute__((aligned(16))) static const long sub_mb_part_8x16[][2] = { { 0,1 } , { 4,5 } };
__attribute__((aligned(16))) static const long sub_mb_part_8x8[][2] = { {0,1}, {4,5},{8,9},{12,13} };
__attribute__((aligned(16))) static const char  zigzag_scan_for_4x4[2][16] = {
	{ 0,4,1,2,5,8,12,9,6,3,7,10,13,14,11,15},
	{ 0,1,4,2,3,5,6,7,8,9,10,11,12,13,14,15}
};
__attribute__((aligned(16))) const uchar zigzag_scan[2][16] = {	
	{ 0,16,1,2,17,32,48,33,18,3,19,34,49,50,35,51 } ,
	{ 0,1,16,2,3,17,18,19,32,33,34,35,48,49,50,51 }
};

static inline long get_pred_type(long ref_frame,long r_frame_l,long r_frame_u,long r_frame_ur)
{
	long mask = 0;
	mask |= r_frame_l==ref_frame;
	mask |= (r_frame_u==ref_frame)<<1;
	mask |= (r_frame_ur==ref_frame)<<2;
	return pred_type_mapping[mask];
}



static inline long  pred_nnz_y(decoder_t *decoder,long idx)
{	
	long  flag = (decoder->nnz_offset_a[idx]>=0)&(decoder->nnz_offset_b[idx]>=0);
    
	return (decoder->nz_coeffs_y[decoder->nnz_offset_a[idx]] + 
		decoder->nz_coeffs_y[decoder->nnz_offset_b[idx]] + flag)>> flag;

}


static inline long  pred_nnz_u(decoder_t *decoder,long idx)
{
	long flag = (decoder->nnz_offset_a[idx<<2]>=0)&(decoder->nnz_offset_b[idx<<2]>=0);

	return (decoder->nz_coeffs_u[decoder->nnz_offset_a[idx<<2]>>2] + 
		decoder->nz_coeffs_u[decoder->nnz_offset_b[idx<<2]>>2] + flag)>>flag;		
}


static inline long  pred_nnz_v(decoder_t *decoder,long idx)
{	
	long flag = (decoder->nnz_offset_a[idx<<2]>=0)&(decoder->nnz_offset_b[idx<<2]>=0);

	return (decoder->nz_coeffs_v[decoder->nnz_offset_a[idx<<2]>>2] + 
		decoder->nz_coeffs_v[decoder->nnz_offset_b[idx<<2]>>2] + flag)>>flag;		
}



static inline void interpret_mb_type_I(macroblock_t *cur_mb)
{
	long mode = cur_mb->mb_type;
	long i;

	if( mode == 0 ){
		cur_mb->mb_type = I4MB;
	} else if( mode == 25 ){
		cur_mb->mb_type = IPCM;
		assert( 0==1 );		
	} else {
		cur_mb->mb_type = I16MB;
		cur_mb->i16mode = (mode-1) & 0x3;
		cur_mb->cbp = ICBPTab[(mode-1)>>2];
	}
	for( i=0;i<4;i++ ){
		cur_mb->b8mode[i] = IBLOCK;
		cur_mb->b8pdir[i] = -1;
	}
}

static inline void interpret_mb_type_P(macroblock_t *cur_mb)
{
	long i;
	long mode = cur_mb->mb_type;
	if( mode < 4 ){
		for( i=0;i<4;i++ ){
			cur_mb->b8mode[i] = mode;
			cur_mb->b8pdir[i] = 0;			
		}
	} else if( mode==4 || mode==5 ){
		cur_mb->all_ref_zero = (mode==5);
		cur_mb->mb_type = P8x8;
	} else {
		cur_mb->mb_type -= 6;
		interpret_mb_type_I(cur_mb);
	}
}

static inline void interpret_mb_type_B(macroblock_t *cur_mb)
{
	assert( 0==1 );
}


void interpret_mb_type(ulong slice_type,macroblock_t *cur_mb)
{
	switch(slice_type)
	{
	case I_SLICE:
		interpret_mb_type_I(cur_mb);
		break;
	case P_SLICE:
		interpret_mb_type_P(cur_mb);
		break;
	case B_SLICE:
		interpret_mb_type_B(cur_mb);
		break;
	default:
		assert(0==1);

	}
}


static inline void get_pred_16x16_mv(decoder_t *decoder,long ref_frame,mv_t *pmv)
{
	mb_access_t *access = decoder->mb_access;
	long mbnr = decoder->cur_mb_nr;
	long  sub_idx_a,sub_idx_b,sub_idx_c;
	picture_t *cur_pic = decoder->cur_pic;
	////----long  sub_idx = mbnr << 4;
	////----mv_t *mv_tab = &cur_pic->front_mv[sub_idx];
	long  mv_pred_type;	
	long  r_frame_l,r_frame_u,r_frame_ur;
	

	sub_idx_a = get_neighbour4x4(access,mbnr,0,0);
	sub_idx_b = get_neighbour4x4(access,mbnr,0,1);
	sub_idx_c = get_neighbour4x4(access,mbnr,5,2);
	if( sub_idx_c < 0 ){
		sub_idx_c = get_neighbour4x4(access,mbnr,0,3);
	}
    
	if( sub_idx_b<0 && sub_idx_c<0 ){
		mv_pred_type = MVPRED_L;
	} else {
		r_frame_l = cur_pic->front_ref_idx[sub_idx_a];
		r_frame_u = cur_pic->front_ref_idx[sub_idx_b];
		r_frame_ur = cur_pic->front_ref_idx[sub_idx_c];
		mv_pred_type = get_pred_type(ref_frame,r_frame_l,r_frame_u,r_frame_ur);
	}

	
	
	switch( mv_pred_type )
	{
	case MVPRED_MEDIAN:
		pmv->x = cur_pic->front_mv[sub_idx_a].x + cur_pic->front_mv[sub_idx_b].x + cur_pic->front_mv[sub_idx_c].x
				- max(cur_pic->front_mv[sub_idx_a].x,max(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x))
				- min(cur_pic->front_mv[sub_idx_a].x,min(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x));
		pmv->y = cur_pic->front_mv[sub_idx_a].y + cur_pic->front_mv[sub_idx_b].y + cur_pic->front_mv[sub_idx_c].y
				- max(cur_pic->front_mv[sub_idx_a].y,max(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y))
				- min(cur_pic->front_mv[sub_idx_a].y,min(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y));                		
		break;
	case MVPRED_L:
		pmv->x = cur_pic->front_mv[sub_idx_a].x;
		pmv->y = cur_pic->front_mv[sub_idx_a].y;
		break;
	case MVPRED_U:
		pmv->x = cur_pic->front_mv[sub_idx_b].x;
		pmv->y = cur_pic->front_mv[sub_idx_b].y;
		break;
	case MVPRED_UR:
		pmv->x = cur_pic->front_mv[sub_idx_c].x;
		pmv->y = cur_pic->front_mv[sub_idx_c].y;
		break;
	}	
}

static inline void get_pred_16x8_mv(decoder_t *decoder,long ref_frame,mv_t *pmv,long index)
{
	mb_access_t *access = decoder->mb_access;
	long mbnr = decoder->cur_mb_nr;
	long  sub_idx_a , sub_idx_b , sub_idx_c;
	picture_t *cur_pic = decoder->cur_pic;
	////----long  sub_idx = mbnr << 4;
    ////----mv_t *mv_tab = &cur_pic->front_mv[sub_idx];
	long  mv_pred_type;	
	long  r_frame_l,r_frame_u,r_frame_ur;


	sub_idx_a = get_neighbour4x4(access,mbnr,sub_mb_part_16x8[index][0],0);
	sub_idx_b = get_neighbour4x4(access,mbnr,sub_mb_part_16x8[index][0],1);
	sub_idx_c = get_neighbour4x4(access,mbnr,sub_mb_part_16x8[index][1],2);
	if( sub_idx_c < 0 ){
		sub_idx_c = get_neighbour4x4(access,mbnr,sub_mb_part_16x8[index][0],3);
	}
	if( sub_idx_b<0 && sub_idx_c<0 ){
		mv_pred_type = MVPRED_L;
	} else {
		r_frame_l = cur_pic->front_ref_idx[sub_idx_a];
		r_frame_u = cur_pic->front_ref_idx[sub_idx_b];
		r_frame_ur = cur_pic->front_ref_idx[sub_idx_c];
		
		if( index==0 && ref_frame==r_frame_u){
			mv_pred_type = MVPRED_U;
		} else if( index==1 && ref_frame==r_frame_l ){
            mv_pred_type = MVPRED_L;
	    } else {
			mv_pred_type = get_pred_type(ref_frame,r_frame_l,r_frame_u,r_frame_ur);
	    }
	}

    switch( mv_pred_type )
	{
	case MVPRED_MEDIAN:
		pmv->x = cur_pic->front_mv[sub_idx_a].x + cur_pic->front_mv[sub_idx_b].x + cur_pic->front_mv[sub_idx_c].x
				- max(cur_pic->front_mv[sub_idx_a].x,max(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x))
				- min(cur_pic->front_mv[sub_idx_a].x,min(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x));
		pmv->y = cur_pic->front_mv[sub_idx_a].y + cur_pic->front_mv[sub_idx_b].y + cur_pic->front_mv[sub_idx_c].y
				- max(cur_pic->front_mv[sub_idx_a].y,max(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y))
				- min(cur_pic->front_mv[sub_idx_a].y,min(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y));                		
		break;
	case MVPRED_L:
		pmv->x = cur_pic->front_mv[sub_idx_a].x;
		pmv->y = cur_pic->front_mv[sub_idx_a].y;
		break;
	case MVPRED_U:
		pmv->x = cur_pic->front_mv[sub_idx_b].x;
		pmv->y = cur_pic->front_mv[sub_idx_b].y;
		break;
	case MVPRED_UR:
		pmv->x = cur_pic->front_mv[sub_idx_c].x;
		pmv->y = cur_pic->front_mv[sub_idx_c].y;
		break;
	}
}

static inline void get_pred_8x16_mv(decoder_t *decoder,long ref_frame,mv_t *pmv,long index)
{
	mb_access_t *access = decoder->mb_access;
	long mbnr = decoder->cur_mb_nr;
	long  sub_idx_a , sub_idx_b , sub_idx_c;
	picture_t *cur_pic = decoder->cur_pic;
	////----    long  sub_idx = mbnr << 4;
	////----	mv_t *mv_tab = &cur_pic->front_mv[sub_idx];
	long  mv_pred_type;	
	long  r_frame_l,r_frame_u,r_frame_ur;


	sub_idx_a = get_neighbour4x4(access,mbnr,sub_mb_part_8x16[index][0],0);
	sub_idx_b = get_neighbour4x4(access,mbnr,sub_mb_part_8x16[index][0],1);
	sub_idx_c = get_neighbour4x4(access,mbnr,sub_mb_part_8x16[index][1],2);
	if( sub_idx_c < 0 ){
		sub_idx_c = get_neighbour4x4(access,mbnr,sub_mb_part_8x16[index][0],3);
	}
	if( sub_idx_b<0 && sub_idx_c<0 ){
		mv_pred_type = MVPRED_L;
	} else {
		r_frame_l = cur_pic->front_ref_idx[sub_idx_a];
		r_frame_u = cur_pic->front_ref_idx[sub_idx_b];
		r_frame_ur = cur_pic->front_ref_idx[sub_idx_c];
		
		if( index==0 && ref_frame==r_frame_l){
			mv_pred_type = MVPRED_L;
		} else if( index==1 && ref_frame==r_frame_ur ){
            mv_pred_type = MVPRED_UR;
	    } else {
			mv_pred_type = get_pred_type(ref_frame,r_frame_l,r_frame_u,r_frame_ur);
	    }
	}

    switch( mv_pred_type )
	{
	case MVPRED_MEDIAN:
		pmv->x = cur_pic->front_mv[sub_idx_a].x + cur_pic->front_mv[sub_idx_b].x + cur_pic->front_mv[sub_idx_c].x
				- max(cur_pic->front_mv[sub_idx_a].x,max(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x))
				- min(cur_pic->front_mv[sub_idx_a].x,min(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x));
		pmv->y = cur_pic->front_mv[sub_idx_a].y + cur_pic->front_mv[sub_idx_b].y + cur_pic->front_mv[sub_idx_c].y
				- max(cur_pic->front_mv[sub_idx_a].y,max(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y))
				- min(cur_pic->front_mv[sub_idx_a].y,min(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y));                		
		break;
	case MVPRED_L:
		pmv->x = cur_pic->front_mv[sub_idx_a].x;
		pmv->y = cur_pic->front_mv[sub_idx_a].y;
		break;
	case MVPRED_U:
		pmv->x = cur_pic->front_mv[sub_idx_b].x;
		pmv->y = cur_pic->front_mv[sub_idx_b].y;
		break;
	case MVPRED_UR:
		pmv->x = cur_pic->front_mv[sub_idx_c].x;
		pmv->y = cur_pic->front_mv[sub_idx_c].y;
		break;
	}
}

static inline void get_pred_8x8_mv(decoder_t *decoder,long ref_frame,mv_t *pmv,long index)
{
	mb_access_t *access = decoder->mb_access;
	long mbnr = decoder->cur_mb_nr;
	long  sub_idx_a,sub_idx_b,sub_idx_c;
	picture_t *cur_pic = decoder->cur_pic;
	////----    long  sub_idx = mbnr << 4;
	////----	mv_t *mv_tab = &cur_pic->front_mv[sub_idx];
	long  mv_pred_type;	
	long  r_frame_l,r_frame_u,r_frame_ur;
	

	sub_idx_a = get_neighbour4x4(access,mbnr,sub_mb_part_8x8[index][0],0);
	sub_idx_b = get_neighbour4x4(access,mbnr,sub_mb_part_8x8[index][0],1);
	sub_idx_c = get_neighbour4x4(access,mbnr,sub_mb_part_8x8[index][1],2);
	if( sub_idx_c < 0 ){
		sub_idx_c = get_neighbour4x4(access,mbnr,sub_mb_part_8x8[index][0],3);
	}
    
	if( sub_idx_b<0 && sub_idx_c<0 ){
		mv_pred_type = MVPRED_L;
	} else {
		r_frame_l = cur_pic->front_ref_idx[sub_idx_a];
		r_frame_u = cur_pic->front_ref_idx[sub_idx_b];
		r_frame_ur = cur_pic->front_ref_idx[sub_idx_c];
		mv_pred_type = get_pred_type(ref_frame,r_frame_l,r_frame_u,r_frame_ur);
	}

	
	
	switch( mv_pred_type )
	{
	case MVPRED_MEDIAN:
		pmv->x = cur_pic->front_mv[sub_idx_a].x + cur_pic->front_mv[sub_idx_b].x + cur_pic->front_mv[sub_idx_c].x
				- max(cur_pic->front_mv[sub_idx_a].x,max(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x))
				- min(cur_pic->front_mv[sub_idx_a].x,min(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x));
		pmv->y = cur_pic->front_mv[sub_idx_a].y + cur_pic->front_mv[sub_idx_b].y + cur_pic->front_mv[sub_idx_c].y
				- max(cur_pic->front_mv[sub_idx_a].y,max(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y))
				- min(cur_pic->front_mv[sub_idx_a].y,min(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y));                		
		break;
	case MVPRED_L:
		pmv->x = cur_pic->front_mv[sub_idx_a].x;
		pmv->y = cur_pic->front_mv[sub_idx_a].y;
		break;
	case MVPRED_U:
		pmv->x = cur_pic->front_mv[sub_idx_b].x;
		pmv->y = cur_pic->front_mv[sub_idx_b].y;
		break;
	case MVPRED_UR:
		pmv->x = cur_pic->front_mv[sub_idx_c].x;
		pmv->y = cur_pic->front_mv[sub_idx_c].y;
		break;
	}
}

static inline void get_pred_8x4_mv(decoder_t *decoder,long ref_frame,mv_t *pmv,long index,long sub_mb_idx)
{
	mb_access_t *access = decoder->mb_access;
	long mbnr = decoder->cur_mb_nr;
	long  sub_idx_a,sub_idx_b,sub_idx_c;
	picture_t *cur_pic = decoder->cur_pic;
	////----    long  sub_idx = mbnr << 4;
	////----	mv_t *mv_tab = &cur_pic->front_mv[sub_idx];
	long  mv_pred_type;	
	long  r_frame_l,r_frame_u,r_frame_ur;
	

	sub_idx_a = get_neighbour4x4(access,mbnr,sub_mb_part_8x8[index][0]+(sub_mb_idx<<1),0);
	sub_idx_b = get_neighbour4x4(access,mbnr,sub_mb_part_8x8[index][0]+(sub_mb_idx<<1),1);
	sub_idx_c = get_neighbour4x4(access,mbnr,sub_mb_part_8x8[index][1]+(sub_mb_idx<<1),2);
	if( sub_idx_c < 0 ){
		sub_idx_c = get_neighbour4x4(access,mbnr,sub_mb_part_8x8[index][0]+(sub_mb_idx<<1),3);
	}
    
	if( sub_idx_b<0 && sub_idx_c<0 ){
		mv_pred_type = MVPRED_L;
	} else {
		r_frame_l = cur_pic->front_ref_idx[sub_idx_a];
		r_frame_u = cur_pic->front_ref_idx[sub_idx_b];
		r_frame_ur = cur_pic->front_ref_idx[sub_idx_c];
		mv_pred_type = get_pred_type(ref_frame,r_frame_l,r_frame_u,r_frame_ur);
	}

	
	
	switch( mv_pred_type )
	{
	case MVPRED_MEDIAN:
		pmv->x = cur_pic->front_mv[sub_idx_a].x + cur_pic->front_mv[sub_idx_b].x + cur_pic->front_mv[sub_idx_c].x
				- max(cur_pic->front_mv[sub_idx_a].x,max(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x))
				- min(cur_pic->front_mv[sub_idx_a].x,min(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x));
		pmv->y = cur_pic->front_mv[sub_idx_a].y + cur_pic->front_mv[sub_idx_b].y + cur_pic->front_mv[sub_idx_c].y
				- max(cur_pic->front_mv[sub_idx_a].y,max(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y))
				- min(cur_pic->front_mv[sub_idx_a].y,min(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y));                		
		break;
	case MVPRED_L:
		pmv->x = cur_pic->front_mv[sub_idx_a].x;
		pmv->y = cur_pic->front_mv[sub_idx_a].y;
		break;
	case MVPRED_U:
		pmv->x = cur_pic->front_mv[sub_idx_b].x;
		pmv->y = cur_pic->front_mv[sub_idx_b].y;
		break;
	case MVPRED_UR:
		pmv->x = cur_pic->front_mv[sub_idx_c].x;
		pmv->y = cur_pic->front_mv[sub_idx_c].y;
		break;
	}
}

static inline void get_pred_4x8_mv(decoder_t *decoder,long ref_frame,mv_t *pmv,long index,long sub_mb_idx)
{
	mb_access_t *access = decoder->mb_access;
	long mbnr = decoder->cur_mb_nr;
	long  sub_idx_a,sub_idx_b,sub_idx_c;
	picture_t *cur_pic = decoder->cur_pic;
	////----    long  sub_idx = mbnr << 4;
	////----	mv_t *mv_tab = &cur_pic->front_mv[sub_idx];
	long  mv_pred_type;	
	long  r_frame_l,r_frame_u,r_frame_ur;
	long  position = sub_mb_part_8x8[index][0] + sub_mb_idx;

	sub_idx_a = get_neighbour4x4(access,mbnr,position,0);
	sub_idx_b = get_neighbour4x4(access,mbnr,position,1);
	sub_idx_c = get_neighbour4x4(access,mbnr,position,2);
	if( sub_idx_c < 0 ){
		sub_idx_c = get_neighbour4x4(access,mbnr,position,3);
	}
    
	if( sub_idx_b<0 && sub_idx_c<0 ){
		mv_pred_type = MVPRED_L;
	} else {
		r_frame_l = cur_pic->front_ref_idx[sub_idx_a];
		r_frame_u = cur_pic->front_ref_idx[sub_idx_b];
		r_frame_ur = cur_pic->front_ref_idx[sub_idx_c];
		mv_pred_type = get_pred_type(ref_frame,r_frame_l,r_frame_u,r_frame_ur);
	}

	
	
	switch( mv_pred_type )
	{
	case MVPRED_MEDIAN:
		pmv->x = cur_pic->front_mv[sub_idx_a].x + cur_pic->front_mv[sub_idx_b].x + cur_pic->front_mv[sub_idx_c].x
				- max(cur_pic->front_mv[sub_idx_a].x,max(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x))
				- min(cur_pic->front_mv[sub_idx_a].x,min(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x));
		pmv->y = cur_pic->front_mv[sub_idx_a].y + cur_pic->front_mv[sub_idx_b].y + cur_pic->front_mv[sub_idx_c].y
				- max(cur_pic->front_mv[sub_idx_a].y,max(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y))
				- min(cur_pic->front_mv[sub_idx_a].y,min(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y));                		
		break;
	case MVPRED_L:
		pmv->x = cur_pic->front_mv[sub_idx_a].x;
		pmv->y = cur_pic->front_mv[sub_idx_a].y;
		break;
	case MVPRED_U:
		pmv->x = cur_pic->front_mv[sub_idx_b].x;
		pmv->y = cur_pic->front_mv[sub_idx_b].y;
		break;
	case MVPRED_UR:
		pmv->x = cur_pic->front_mv[sub_idx_c].x;
		pmv->y = cur_pic->front_mv[sub_idx_c].y;
		break;
	}
}

static inline void get_pred_4x4_mv(decoder_t *decoder,long ref_frame,mv_t *pmv,long sub_mb_idx)
{
	mb_access_t *access = decoder->mb_access;
	long mbnr = decoder->cur_mb_nr;
	long  sub_idx_a,sub_idx_b,sub_idx_c;
	picture_t *cur_pic = decoder->cur_pic;
	////----    long  sub_idx = mbnr << 4;
	////----	mv_t *mv_tab = &cur_pic->front_mv[sub_idx];
	long  mv_pred_type;	
	long  r_frame_l,r_frame_u,r_frame_ur;
	

	sub_idx_a = get_neighbour4x4(access,mbnr,sub_mb_idx,0);
	sub_idx_b = get_neighbour4x4(access,mbnr,sub_mb_idx,1);
	sub_idx_c = get_neighbour4x4(access,mbnr,sub_mb_idx,2);
	if( sub_idx_c < 0 ){
		sub_idx_c = get_neighbour4x4(access,mbnr,sub_mb_idx,3);
	}
    
	if( sub_idx_b<0 && sub_idx_c<0 ){
		mv_pred_type = MVPRED_L;
	} else {
		r_frame_l = cur_pic->front_ref_idx[sub_idx_a];
		r_frame_u = cur_pic->front_ref_idx[sub_idx_b];
		r_frame_ur = cur_pic->front_ref_idx[sub_idx_c];
		mv_pred_type = get_pred_type(ref_frame,r_frame_l,r_frame_u,r_frame_ur);
	}

	
	
	switch( mv_pred_type )
	{
	case MVPRED_MEDIAN:
		pmv->x = cur_pic->front_mv[sub_idx_a].x + cur_pic->front_mv[sub_idx_b].x + cur_pic->front_mv[sub_idx_c].x
				- max(cur_pic->front_mv[sub_idx_a].x,max(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x))
				- min(cur_pic->front_mv[sub_idx_a].x,min(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x));
		pmv->y = cur_pic->front_mv[sub_idx_a].y + cur_pic->front_mv[sub_idx_b].y + cur_pic->front_mv[sub_idx_c].y
				- max(cur_pic->front_mv[sub_idx_a].y,max(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y))
				- min(cur_pic->front_mv[sub_idx_a].y,min(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y));                		
		break;
	case MVPRED_L:
		pmv->x = cur_pic->front_mv[sub_idx_a].x;
		pmv->y = cur_pic->front_mv[sub_idx_a].y;
		break;
	case MVPRED_U:
		pmv->x = cur_pic->front_mv[sub_idx_b].x;
		pmv->y = cur_pic->front_mv[sub_idx_b].y;
		break;
	case MVPRED_UR:
		pmv->x = cur_pic->front_mv[sub_idx_c].x;
		pmv->y = cur_pic->front_mv[sub_idx_c].y;
		break;
	}	
}


void  read_mb_type_cavlc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs)
{
	slice_t *slice = decoder->cur_slice;
	if( slice->slice_type != I_SLICE ){
		if( decoder->mb_skip_run == -1 ){
			decoder->mb_skip_run = golumb_ue(bs);
		}
		if( decoder->mb_skip_run == 0 ){
			decoder->mb_skip_run --;
		} else {
			decoder->mb_skip_run --;
			cur_mb->mb_type = 0;
			return ;
		}
	} 
	cur_mb->mb_type = golumb_ue(bs);

	if( slice->slice_type == P_SLICE ){
		cur_mb->mb_type ++;
	}
	
	return ;
}


void read_submb_type_cavlc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs)
{
	long  submb_type;
	long  i;
	
	for( i=0;i<4;i++ ){
		submb_type = golumb_ue(bs);
		cur_mb->b8mode[i] = p_v2b8[submb_type];
		cur_mb->b8pdir[i] = 0;
	}
	return;
}

void  get_pskip_mv(decoder_t *decoder,macroblock_t *cur_mb)
{
	long   sub_idx_a,sub_idx_b,sub_idx_c;
	long   mbnr = decoder->cur_mb_nr;
	mb_access_t  *access = decoder->mb_access;
	picture_t *cur_pic = decoder->cur_pic;
	mv_t      mv;
	long   mv_pred_type = MVPRED_MEDIAN;
	long   r_frame_l, r_frame_u, r_frame_ur;
	long   *ref_array = cur_pic->front_ref_idx + (mbnr<<4);
	mv_t      *mv_array = cur_pic->front_mv + (mbnr<<4);
    
	sub_idx_a = get_neighbour4x4(access,mbnr,0,0);
	sub_idx_b = get_neighbour4x4(access,mbnr,0,1);



	if( (sub_idx_a<0) || (sub_idx_b<0) || 
		( (cur_pic->front_ref_idx[sub_idx_a]==0) && is_zero_motion_vector(&cur_pic->front_mv[sub_idx_a]) )
		|| ((cur_pic->front_ref_idx[sub_idx_b]==0) && is_zero_motion_vector(&cur_pic->front_mv[sub_idx_b])) ){
        mv.x = 0;
		mv.y = 0;
	}  else {
        sub_idx_c = get_neighbour4x4(access,mbnr,5,2);
		if( sub_idx_c < 0 ) {
			sub_idx_c = get_neighbour4x4(access,mbnr,0,3);
		}
		r_frame_l =  cur_pic->front_ref_idx[sub_idx_a];
		r_frame_u =  cur_pic->front_ref_idx[sub_idx_b];
		r_frame_ur =  cur_pic->front_ref_idx[sub_idx_c];		
       
		mv_pred_type = get_pred_type(0,r_frame_l,r_frame_u,r_frame_ur);

		switch( mv_pred_type )
		{
		case MVPRED_MEDIAN:
			if( !((sub_idx_b>=0) || (sub_idx_c>=0)) ){
                mv.x = cur_pic->front_mv[sub_idx_a].x;
			    mv.y = cur_pic->front_mv[sub_idx_a].y;
			} else {
				mv.x = cur_pic->front_mv[sub_idx_a].x + cur_pic->front_mv[sub_idx_b].x + cur_pic->front_mv[sub_idx_c].x
					- max(cur_pic->front_mv[sub_idx_a].x,max(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x))
					- min(cur_pic->front_mv[sub_idx_a].x,min(cur_pic->front_mv[sub_idx_b].x,cur_pic->front_mv[sub_idx_c].x));
                mv.y = cur_pic->front_mv[sub_idx_a].y + cur_pic->front_mv[sub_idx_b].y + cur_pic->front_mv[sub_idx_c].y
					- max(cur_pic->front_mv[sub_idx_a].y,max(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y))
					- min(cur_pic->front_mv[sub_idx_a].y,min(cur_pic->front_mv[sub_idx_b].y,cur_pic->front_mv[sub_idx_c].y));                
			}
			break;
		case MVPRED_L:
			mv.x = cur_pic->front_mv[sub_idx_a].x;
			mv.y = cur_pic->front_mv[sub_idx_a].y;
			break;
		case MVPRED_U:
			mv.x = cur_pic->front_mv[sub_idx_b].x;
			mv.y = cur_pic->front_mv[sub_idx_b].y;
			break;
		case MVPRED_UR:
			mv.x = cur_pic->front_mv[sub_idx_c].x;
			mv.y = cur_pic->front_mv[sub_idx_c].y;
			break;
		}
	}
	
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;
	*ref_array++ = 0;
	*mv_array++  = mv;       

}



void read_ipred_type_cavlc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs)
{
	long  i;
	long  ipred_mode;
	long  sub_idx_a,sub_idx_b;
	long  mbnr = decoder->cur_mb_nr;
	long  left_intra_pred_mode,top_intra_pred_mode;
	long  most_probable_mode ;
	mb_access_t  *access = decoder->mb_access;
	uchar  *ipred_mode_tab = decoder->ipred_mode;
    long  sub_idx = mbnr << 4;

	if( cur_mb->mb_type == I4MB ){
		for( i=0;i<16;i++ ){
			ipred_mode = read_intra4x4_pred_mode_cavlc(bs);
            sub_idx_a = get_neighbour4x4(access,mbnr,i,0);
			sub_idx_b = get_neighbour4x4(access,mbnr,i,1);
            
			left_intra_pred_mode = ipred_mode_tab[sub_idx_a];
			top_intra_pred_mode = ipred_mode_tab[sub_idx_b];
		
			if( sub_idx_a<0 || sub_idx_b<0 ){
				most_probable_mode = DC_PRED;
			} else {
                most_probable_mode = min(left_intra_pred_mode,top_intra_pred_mode);
			}
			ipred_mode_tab[sub_idx + i] = (ipred_mode==-1) ? most_probable_mode : 
			                  ipred_mode + (ipred_mode >= most_probable_mode);
		}
	}
	cur_mb->c_ipred_mode = golumb_ue(bs);	
}




void  read_inter_pred_ref_idx_cavlc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs)
{
	picture_t *cur_pic = decoder->cur_pic;
	long   sub_idx = decoder->cur_mb_nr << 4 ;
	long  *ref_idx_tab = cur_pic->front_ref_idx;	
	slice_t  *slice = decoder->cur_slice;
	long  * ref_array;

	if( slice->num_ref_idx_l0_active == 1 ){
		ref_array = &ref_idx_tab[sub_idx];
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		*ref_array++ = 0;
		return;
	}

	switch ( cur_mb->mb_type )
	{
	case 1:
		{
			long ref_frame = 0;
			
			ref_frame = read_ref_idx_cavlc(bs,slice->num_ref_idx_l0_active==2);			
			ref_array = &ref_idx_tab[sub_idx];
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
			*ref_array++ = ref_frame;
		}
		break;
	case 2:		
		{
			long  ref_frame[2];			
			ref_frame[0] = read_ref_idx_cavlc(bs,slice->num_ref_idx_l0_active==2);
			ref_frame[1] = read_ref_idx_cavlc(bs,slice->num_ref_idx_l0_active==2);			
			ref_array = &ref_idx_tab[sub_idx];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
		}
		break;
	case 3:
		{
			long  ref_frame[2];
			ref_frame[0] = read_ref_idx_cavlc(bs,slice->num_ref_idx_l0_active==2);
			ref_frame[1] = read_ref_idx_cavlc(bs,slice->num_ref_idx_l0_active==2);			
			ref_array = &ref_idx_tab[sub_idx];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
		}
		break;
	case P8x8:
		{
			long ref_frame[4];
			ref_frame[0] = read_ref_idx_cavlc(bs,slice->num_ref_idx_l0_active==2);
			ref_frame[1] = read_ref_idx_cavlc(bs,slice->num_ref_idx_l0_active==2);
			ref_frame[2] = read_ref_idx_cavlc(bs,slice->num_ref_idx_l0_active==2);
			ref_frame[3] = read_ref_idx_cavlc(bs,slice->num_ref_idx_l0_active==2);							
			ref_array = &ref_idx_tab[sub_idx];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[0];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[1];
			*ref_array++ = ref_frame[2];
			*ref_array++ = ref_frame[2];
			*ref_array++ = ref_frame[2];
			*ref_array++ = ref_frame[2];
			*ref_array++ = ref_frame[3];
			*ref_array++ = ref_frame[3];
			*ref_array++ = ref_frame[3];
			*ref_array++ = ref_frame[3];
		}
		break;
	}

	return;
}

void  read_inter_pred_mvd_calvc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs)
{
	picture_t *cur_pic = decoder->cur_pic;
	long   mbnr = decoder->cur_mb_nr;
	long   sub_idx = mbnr << 4;		
	mv_t      *mv_array;

	switch( cur_mb->mb_type )
	{
	case 1:
		{
			mv_t      pmv;
			long   ref_frame ;
			ref_frame = cur_pic->front_ref_idx[sub_idx];
			get_pred_16x16_mv(decoder,ref_frame,&pmv);
			mv_array = &cur_pic->front_mv[sub_idx];
			pmv.x += golumb_se(bs);
			pmv.y += golumb_se(bs);
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
			*mv_array++ = pmv;
		}
		break;
	case 2:
		{
            mv_t pmv[2];
			long ref_frame[2];
			ref_frame[0] = cur_pic->front_ref_idx[sub_idx];
			ref_frame[1] = cur_pic->front_ref_idx[sub_idx+8];
			mv_array = &cur_pic->front_mv[sub_idx];

			get_pred_16x8_mv(decoder,ref_frame[0],&pmv[0],0);						
			pmv[0].x += golumb_se(bs);
			pmv[0].y += golumb_se(bs);
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];

			get_pred_16x8_mv(decoder,ref_frame[1],&pmv[1],1);
			pmv[1].x += golumb_se(bs);
			pmv[1].y += golumb_se(bs);			
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
		}
		break;
	case 3:
		{
		    mv_t pmv[2];
			long ref_frame[2];
			ref_frame[0] = cur_pic->front_ref_idx[sub_idx];
			ref_frame[1] = cur_pic->front_ref_idx[sub_idx+4];
			mv_array = &cur_pic->front_mv[sub_idx];

			get_pred_8x16_mv(decoder,ref_frame[0],&pmv[0],0);
			pmv[0].x += golumb_se(bs);
			pmv[0].y += golumb_se(bs);
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			mv_array += 4;
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];
			*mv_array++ = pmv[0];

			get_pred_8x16_mv(decoder,ref_frame[1],&pmv[1],1);						
			pmv[1].x += golumb_se(bs);
			pmv[1].y += golumb_se(bs);
			mv_array = &cur_pic->front_mv[sub_idx];
			mv_array += 4;			
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			mv_array += 4;
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
			*mv_array++ = pmv[1];
		}
		break;
	case P8x8:
		{
			long k;
			for( k=0;k<4;k++ ){
				switch( cur_mb->b8mode[k] )
				{
				case 4:
					{
						mv_t pmv;
						long ref_frame = cur_pic->front_ref_idx[sub_idx+(k<<2)];
						get_pred_8x8_mv(decoder,ref_frame,&pmv,k);
						pmv.x += golumb_se(bs);
						pmv.y += golumb_se(bs);
						mv_array = &cur_pic->front_mv[sub_idx+(k<<2)];
                        *mv_array++ = pmv;
						*mv_array++ = pmv;
						*mv_array++ = pmv;
						*mv_array++ = pmv;
					}
					break;
				case 5:
					{
						mv_t pmv[2];
						long ref_frame;
						ref_frame = cur_pic->front_ref_idx[sub_idx+(k<<2)];
						mv_array = &cur_pic->front_mv[sub_idx+(k<<2)];
						
						get_pred_8x4_mv(decoder,ref_frame,&pmv[0],k,0);						
						pmv[0].x += golumb_se(bs);
						pmv[0].y += golumb_se(bs);
						*mv_array++ = pmv[0];
						*mv_array++ = pmv[0];

						get_pred_8x4_mv(decoder,ref_frame,&pmv[1],k,1);
						pmv[1].x += golumb_se(bs);
						pmv[1].y += golumb_se(bs);                       
						*mv_array++ = pmv[1];
						*mv_array++ = pmv[1];
					}
					break;
				case 6:
					{
						mv_t pmv[2];
						long ref_frame;
						ref_frame = cur_pic->front_ref_idx[sub_idx+(k<<2)];
						mv_array = &cur_pic->front_mv[sub_idx+(k<<2)];						
						get_pred_4x8_mv(decoder,ref_frame,&pmv[0],k,0);
						pmv[0].x += golumb_se(bs);
						pmv[0].y += golumb_se(bs);
						*mv_array = pmv[0];
						*(mv_array+2) = pmv[0];
						get_pred_4x8_mv(decoder,ref_frame,&pmv[1],k,1);						
						pmv[1].x += golumb_se(bs);
						pmv[1].y += golumb_se(bs);						
						*(mv_array+1) = pmv[1];						
						*(mv_array+3) = pmv[1];
					}
					break;
				case 7:
					{
						mv_t pmv[4];
						long ref_frame;
						ref_frame = cur_pic->front_ref_idx[sub_idx+(k<<2)];	
						mv_array = &cur_pic->front_mv[sub_idx+(k<<2)];
						get_pred_4x4_mv(decoder,ref_frame,&pmv[0],(k<<2));
						pmv[0].x += golumb_se(bs);
						pmv[0].y += golumb_se(bs);
						*mv_array++ = pmv[0];
						get_pred_4x4_mv(decoder,ref_frame,&pmv[1],(k<<2)+1);
						pmv[1].x += golumb_se(bs);
						pmv[1].y += golumb_se(bs);
						*mv_array++ = pmv[1];
						get_pred_4x4_mv(decoder,ref_frame,&pmv[2],(k<<2)+2);
						pmv[2].x += golumb_se(bs);
						pmv[2].y += golumb_se(bs);
						*mv_array++ = pmv[2];
						get_pred_4x4_mv(decoder,ref_frame,&pmv[3],(k<<2)+3);						
						pmv[3].x += golumb_se(bs);
						pmv[3].y += golumb_se(bs);
						*mv_array++ = pmv[3];      									
					}
					break;
				}
			}
		}
		break;
	}

	return;
}


void read_coeffs_cavlc(decoder_t *decoder,macroblock_t *cur_mb,bs_t *bs)
{	    
	long  max_num_coeffs;
	long  start_scan;
	long  cbp ;
	long  num_coeffs;
	long  pred_nnz;
	long  coef_ctr;
	long  runarr[18],levarr[18];
	int16_t  DCcofy[16] = { 0 };
	int16_t  DCcofuv[8] = { 0 }; 
	long  i,k;
	long  qp_per,qp_rem;
	long  qp_uv;
	long  qp_per_uv,qp_rem_uv;
	int16_t  *cof_start = (int16_t*)decoder->cof;
	int16_t  *cof;
	long  sub_idx = decoder->cur_mb_nr << 4;
	pic_parameter_set_t *pps = decoder->active_pps;
	long  b8;
	long  DCcof_off;



	if( cur_mb->mb_type != I16MB ){
		if( cur_mb->mb_type == I4MB ){
			cbp = cur_mb->cbp = read_cbp_intra_cavlc(bs);
		} else {
			cbp = cur_mb->cbp = read_cbp_inter_cavlc(bs);
		}
		if( cbp != 0 ) {
			cur_mb->delta_qp = golumb_se(bs);      
			decoder->qp = (cur_mb->delta_qp + decoder->qp + 52)%52;
		}
		max_num_coeffs = 16;
		start_scan = 0;
	} else {
		cbp = cur_mb->cbp;
		cur_mb->delta_qp = golumb_se(bs);
		decoder->qp = (cur_mb->delta_qp + decoder->qp + 52)%52;
		pred_nnz = pred_nnz_y(decoder,0);
		num_coeffs = read_coeffs_4x4_block_cavlc(bs,pred_nnz,16,0,levarr,runarr);
		coef_ctr = -1;
		if( num_coeffs ){
			for ( k=0;k<num_coeffs;k++ ){
				coef_ctr += runarr[k] + 1;
				DCcofy[zigzag_scan_for_4x4[0][coef_ctr]] = levarr[k];
			}
			idct_4x4_dc(DCcofy,decoder->qp);
		}
		max_num_coeffs = 15;
		start_scan = 1;
	}

	qp_per = decoder->qp / 6;
	qp_rem = decoder->qp % 6;
	qp_uv = decoder->qp + pps->chroma_qp_index_offset;
	qp_uv = CLIP3(0,51,qp_uv);
	qp_per_uv = QP_SCALE_CR[qp_uv]/6;
	qp_rem_uv = QP_SCALE_CR[qp_uv]%6;
	cur_mb->qp = decoder->qp;
	for( i=0;i<16;i++ ){
		b8 = i>>2;
		cof = cof_start + ( submb_pos_y[i]<<6 ) + (submb_pos_x[i]<<2);
		DCcof_off = (submb_pos_y[i]<<2) + submb_pos_x[i];
		if( cbp & (1<<b8) ){
			pred_nnz = pred_nnz_y(decoder,i);
			num_coeffs = read_coeffs_4x4_block_cavlc(bs,pred_nnz,max_num_coeffs,0,levarr,runarr);
			decoder->nz_coeffs_y[sub_idx+i] = num_coeffs;
			coef_ctr = start_scan - 1;
			if( num_coeffs ){
				cur_mb->cbp_blk |= 1 << i;
				for( k=0;k<num_coeffs;k++ ){
					coef_ctr += runarr[k] + 1;
                    cof[zigzag_scan[0][coef_ctr]] = levarr[k];
				}
				iquant_4x4(cof,qp_per,qp_rem);
				if( start_scan ){
					cof[0] = DCcofy[DCcof_off];
				}
				idct_4x4(cof);
			} else {
				if ( start_scan ){
					memcpy_4x4_dc(cof,DCcofy[DCcof_off]);
				}
			}
		} else {
			if( start_scan ){
				memcpy_4x4_dc(cof,DCcofy[DCcof_off]);
			}
		}
	}

	if ( cbp > 15 ){
		for( i=0;i<2;i++ ){
			num_coeffs = read_coeffs_4x4_block_cavlc(bs,0,4,1,levarr,runarr);
			coef_ctr = -1;
			if( num_coeffs ){
				for( k=0;k<num_coeffs;k++ ){
					coef_ctr += runarr[k] + 1;
					DCcofuv[coef_ctr+(i<<2)] = levarr[k];
				}
				idct_2x2_dc_c(DCcofuv+(i<<2),qp_per_uv,qp_rem_uv);
			}
		}
	}
	if ( cbp <= 31 ){
		for( i=0;i<8;i++ ){
			cof = &decoder->cof[(submb_pos_y[i]<<2)+16][submb_pos_x[i]<<2];
            memcpy_4x4_dc(cof,DCcofuv[i]);
		}
	} else {
		for( i=0;i<4;i++ ){
			pred_nnz = pred_nnz_u(decoder,i);
			num_coeffs = read_coeffs_4x4_block_cavlc(bs,pred_nnz,15,0,levarr,runarr);
			decoder->nz_coeffs_u[(sub_idx>>2)+i] = num_coeffs;
			coef_ctr = 0;
			cof = cof_start + ((16+((i&0x2)<<1))<<4) + ((i&0x1)<<2);
			if( num_coeffs ){
				for( k=0;k<num_coeffs;k++ ){
					coef_ctr += runarr[k] + 1;
					cof[zigzag_scan[0][coef_ctr]] = levarr[k];
				}
				iquant_4x4(cof,qp_per_uv,qp_rem_uv);
				cof[0] = DCcofuv[i];
				idct_4x4(cof);
			} else {
				memcpy_4x4_dc(cof,DCcofuv[i]);
			}
		}
		for( i=0;i<4;i++ ){
			pred_nnz = pred_nnz_v(decoder,i);
			num_coeffs = read_coeffs_4x4_block_cavlc(bs,pred_nnz,15,0,levarr,runarr);
			decoder->nz_coeffs_v[(sub_idx>>2)+i] = num_coeffs;
			coef_ctr = 0;
			cof = cof_start + ((16+((i&0x2)<<1))<<4) + (((i&0x1)<<2)+8);
			if( num_coeffs ){
				for( k=0;k<num_coeffs;k++ ){
					coef_ctr += runarr[k] + 1;
					cof[zigzag_scan[0][coef_ctr]] = levarr[k];
				}
				iquant_4x4(cof,qp_per_uv,qp_rem_uv);
				cof[0] = DCcofuv[i+4];
				idct_4x4(cof);
			} else {
				memcpy_4x4_dc(cof,DCcofuv[i+4]);
			}			
		}
	}
    
	return;
}





