
#include <assert.h>
#include <stdlib.h>

#include "global.h"
#include "common.h"
#include "access.h"
#include "loopfilter.h"


#define  MV_LIMIT     4   // frame only
__attribute__((aligned(16))) static const long long two64 = 0x0002000200020002;
__attribute__((aligned(16))) static const long long four64 = 0x0004000400040004;



__attribute__((aligned(16))) static const uchar ALPHA_TABLE[52]  = {0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,4,4,5,6,  7,8,9,10,12,13,15,17,  20,22,25,28,32,36,40,45,  50,56,63,71,80,90,101,113,  127,144,162,182,203,226,255,255} ;
__attribute__((aligned(16))) static const uchar BETA_TABLE[52]  = {0,0,0,0,0,0,0,0,0,0,0,0, 0,0,0,0,2,2,2,3,  3,3,3, 4, 4, 4, 6, 6,   7, 7, 8, 8, 9, 9,10,10,  11,11,12,12,13,13, 14, 14,   15, 15, 16, 16, 17, 17, 18, 18} ;
__attribute__((aligned(16))) static const uchar CLIP_TABLE[52][5]  =
{
  { 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},
  { 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},{ 0, 0, 0, 0, 0},
  { 0, 0, 0, 0, 0},{ 0, 0, 0, 1, 1},{ 0, 0, 0, 1, 1},{ 0, 0, 0, 1, 1},{ 0, 0, 0, 1, 1},{ 0, 0, 1, 1, 1},{ 0, 0, 1, 1, 1},{ 0, 1, 1, 1, 1},
  { 0, 1, 1, 1, 1},{ 0, 1, 1, 1, 1},{ 0, 1, 1, 1, 1},{ 0, 1, 1, 2, 2},{ 0, 1, 1, 2, 2},{ 0, 1, 1, 2, 2},{ 0, 1, 1, 2, 2},{ 0, 1, 2, 3, 3},
  { 0, 1, 2, 3, 3},{ 0, 2, 2, 3, 3},{ 0, 2, 2, 4, 4},{ 0, 2, 3, 4, 4},{ 0, 2, 3, 4, 4},{ 0, 3, 3, 5, 5},{ 0, 3, 4, 6, 6},{ 0, 3, 4, 6, 6},
  { 0, 4, 5, 7, 7},{ 0, 4, 5, 8, 8},{ 0, 4, 6, 9, 9},{ 0, 5, 7,10,10},{ 0, 6, 8,11,11},{ 0, 6, 8,13,13},{ 0, 7,10,14,14},{ 0, 8,11,16,16},
  { 0, 9,12,18,18},{ 0,10,13,20,20},{ 0,11,15,23,23},{ 0,13,17,25,25}
} ;


void get_strength_ver_pmb(decoder_t *decoder,uchar strength[],loop_filter_t *loop_filter,long edge)
{
	long mb_addr = loop_filter->mb_addr;
	macroblock_t *mb_q,*mb_p;	
	long i;
	long blk_q, blk_p;
	long sub_idx = mb_addr << 4;
	long sub_idx_p;

	mb_q = &decoder->mb_date[mb_addr];
	if ( edge == 0 ){
		mb_p = &decoder->mb_date[mb_addr-1];
		if( mb_p->mb_type==I4MB || mb_p->mb_type==I16MB ){
			strength[0] = strength[1] = strength[2] = strength[3] = 4;
			return;
		}
		sub_idx_p = sub_idx - 16;
	} else {
		mb_p = mb_q;
		sub_idx_p = sub_idx;
	}
    
	for( i=0;i<4;i++ ){
		blk_q = map_to_submb_idx[i][edge];
		blk_p = map_to_submb_idx[i][(edge+3)&0x3];
		if( ((mb_q->cbp_blk&(1<<blk_q))!=0) || ((mb_p->cbp_blk&(1<<blk_p))!=0) ){
			strength[i] = 2;
		} else {
			long ref_q,ref_p;
			ref_q = loop_filter->front_ref_idx[sub_idx+blk_q];
			ref_p = loop_filter->front_ref_idx[sub_idx+blk_p];
			strength[i] = (ref_q!=ref_p) | (abs(loop_filter->front_mv[sub_idx+blk_q].x - loop_filter->front_mv[sub_idx_p
				+blk_p].x)>=MV_LIMIT) | (abs(loop_filter->front_mv[sub_idx+blk_q].y - loop_filter->front_mv[sub_idx_p
				+blk_p].y)>=MV_LIMIT);
		}
	}
}

void get_strength_hor_pmb(decoder_t *decoder,uchar strength[],loop_filter_t *loop_filter,long edge)
{
    long mb_addr = loop_filter->mb_addr;
	macroblock_t *mb_q,*mb_p;	
	long i;
	long blk_q, blk_p;
	long sub_idx = mb_addr << 4;
	long sub_idx_p;

	mb_q = &decoder->mb_date[mb_addr];
	if ( edge == 0 ){
		mb_p = &decoder->mb_date[mb_addr-decoder->pic_width_in_mbs];
		if( mb_p->mb_type==I4MB || mb_p->mb_type==I16MB ){
			strength[0] = strength[1] = strength[2] = strength[3] = 4;
			return;
		}
		sub_idx_p = sub_idx - (decoder->pic_width_in_mbs*16);
	} else {
		mb_p = mb_q;
		sub_idx_p = sub_idx;
	}
    
	for( i=0;i<4;i++ ){
		blk_q = map_to_submb_idx[edge][i];
		blk_p = map_to_submb_idx[(edge+3)&0x3][i];
		if( ((mb_q->cbp_blk&(1<<blk_q))!=0) || ((mb_p->cbp_blk&(1<<blk_p))!=0) ){
			strength[i] = 2;
		} else {
			long ref_q,ref_p;
			ref_q = loop_filter->front_ref_idx[sub_idx+blk_q];
			ref_p = loop_filter->front_ref_idx[sub_idx+blk_p];
			strength[i] = (ref_q!=ref_p) | (abs(loop_filter->front_mv[sub_idx+blk_q].x - loop_filter->front_mv[sub_idx_p
				+blk_p].x)>=MV_LIMIT) | (abs(loop_filter->front_mv[sub_idx+blk_q].y - loop_filter->front_mv[sub_idx_p
				+blk_p].y)>=MV_LIMIT);
		}
	}
}

void filter_luma_ver_c(loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge)
{
	const uchar *clip_tab;
    long alpha , beta;
	long x_pos,y_pos;
	long pel;
	uchar  *src_ptr_p, *src_ptr_q;
	long  l0,l1,l2,l3,r0,r1,r2,r3,rl0;  
	uchar  **img_y = loop_filter->img_y;
	uchar  strng;
	long  abs_delta;
	long  delta;
	long  C0,c0;
	long  ap,aq;
	long  small_gap,dif;

	x_pos = (loop_filter->mb_x<<4) + (edge<<2) - 4;
	y_pos = loop_filter->mb_y<<4;
	alpha = ALPHA_TABLE[index_a];
	beta = BETA_TABLE[index_b];
	clip_tab = CLIP_TABLE[index_a];

	for( pel=0;pel<16;pel++ ){
		src_ptr_q = & img_y[y_pos][x_pos+4];
		src_ptr_p = & img_y[y_pos][x_pos+3];

		l0 = src_ptr_p[0];
		l1 = src_ptr_p[-1];
		l2 = src_ptr_p[-2];
		l3 = src_ptr_p[-3];
		r0 = src_ptr_q[0];
		r1 = src_ptr_q[1];
		r2 = src_ptr_q[2];
		r3 = src_ptr_q[3];

		if( (strng = strength[pel>>2])>0 ){
            abs_delta = abs( delta = r0 - l0 );
			if( abs_delta < alpha ){
                C0 = clip_tab[strng];
				if( ((abs( r0 - r1) - beta )  & (abs(l0 - l1) - beta )) < 0 ){
					aq  = (abs( r0 - r2) - beta ) < 0;
					ap  = (abs( l0 - l2) - beta ) < 0;
					rl0             = l0 + r0 ;
					if( strng == 4 ){
						small_gap = (abs_delta < ((alpha >> 2) + 2));
						aq &= small_gap;
						ap &= small_gap;
						
						src_ptr_q[0]   = aq ? ( l1 + ((r1 + rl0) << 1) +  r2 + 4) >> 3 : ((r1 << 1) + r0 + l1 + 2) >> 2 ;
						src_ptr_p[0]   = ap ? ( r1 + ((l1 + rl0) << 1) +  l2 + 4) >> 3 : ((l1 << 1) + l0 + r1 + 2) >> 2 ;
						
						src_ptr_q[ 1] =   aq  ? ( r2 + r0 + r1 + l0 + 2) >> 2 : r1;
						src_ptr_p[-1] =   ap  ? ( l2 + l1 + l0 + r0 + 2) >> 2 : l1;
						
						src_ptr_q[ 2] = aq ? (((r3 + r2) <<1) + r2 + r1 + rl0 + 4) >> 3 : r2;
						src_ptr_p[-2] = ap ? (((l3 + l2) <<1) + l2 + l1 + rl0 + 4) >> 3 : l2;
					} else {
						c0               = (C0 + ap + aq) ;
						dif              = CLIP3( -c0, c0, ( (delta << 2) + (l1 - r1) + 4) >> 3 ) ;
						src_ptr_p[0]  = CLIP3(0, 255, l0 + dif) ;
						src_ptr_q[0]  = CLIP3(0, 255, r0 - dif) ;
						if( ap )
							src_ptr_p[-1] += CLIP3( -C0,  C0, ( l2 + ((rl0 + 1) >> 1) - (l1<<1)) >> 1 );
						if( aq )
							src_ptr_q[1] += CLIP3( -C0,  C0, ( r2 + ((rl0 + 1) >> 1) - (r1<<1)) >> 1 );
					}
				}
			}
		}
		y_pos ++;
	}
}


void filter_luma_hor_c(loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge)
{
	const uchar *clip_tab;
    long alpha , beta;
	long x_pos,y_pos;
	long pel;
	uchar  *src_ptr_p, *src_ptr_q;
	long  l0,l1,l2,l3,r0,r1,r2,r3,rl0;  
	uchar  **img_y = loop_filter->img_y;
	uchar  strng;
	long  abs_delta;
	long  delta;
	long  C0,c0;
	long  ap,aq;
	long  small_gap,dif;
	long  width , width2,width3;

	width = loop_filter->stride_luma;
	width2 = width << 1 ;
	width3 = width2 + width;
	x_pos = (loop_filter->mb_x<<4);
	y_pos = (loop_filter->mb_y<<4) + (edge<<2) - 4;
	alpha = ALPHA_TABLE[index_a];
	beta = BETA_TABLE[index_b];
	clip_tab = CLIP_TABLE[index_a];

	for( pel=0;pel<16;pel++ ){
		if( (strng = strength[pel>>2])>0 ){
			src_ptr_q = & img_y[y_pos+4][x_pos];
			src_ptr_p = & img_y[y_pos+3][x_pos];

			l0 = src_ptr_p[0];
			l1 = src_ptr_p[-width];
			l2 = src_ptr_p[-width2];
			l3 = src_ptr_p[-width3];
			r0 = src_ptr_q[0];
			r1 = src_ptr_q[width];
			r2 = src_ptr_q[width2];
			r3 = src_ptr_q[width3];
			abs_delta = abs( delta = r0 - l0 );
			if( abs_delta < alpha ){
                C0 = clip_tab[strng];
				if( ((abs( r0 - r1) - beta )  & (abs(l0 - l1) - beta )) < 0 ){
					aq  = (abs( r0 - r2) - beta ) < 0;
					ap  = (abs( l0 - l2) - beta ) < 0;
					rl0             = l0 + r0 ;
					if( strng == 4 ){
						small_gap = (abs_delta < ((alpha >> 2) + 2));
						aq &= small_gap;
						ap &= small_gap;
						
						src_ptr_q[0]   = aq ? ( l1 + ((r1 + rl0) << 1) +  r2 + 4) >> 3 : ((r1 << 1) + r0 + l1 + 2) >> 2 ;
						src_ptr_p[0]   = ap ? ( r1 + ((l1 + rl0) << 1) +  l2 + 4) >> 3 : ((l1 << 1) + l0 + r1 + 2) >> 2 ;
						
						src_ptr_q[ width] =   aq  ? ( r2 + r0 + r1 + l0 + 2) >> 2 : r1;
						src_ptr_p[-width] =   ap  ? ( l2 + l1 + l0 + r0 + 2) >> 2 : l1;
						
						src_ptr_q[ width2] = aq ? (((r3 + r2) <<1) + r2 + r1 + rl0 + 4) >> 3 : r2;
						src_ptr_p[-width2] = ap ? (((l3 + l2) <<1) + l2 + l1 + rl0 + 4) >> 3 : l2;
					} else {
						c0               = (C0 + ap + aq) ;
						dif              = CLIP3( -c0, c0, ( (delta << 2) + (l1 - r1) + 4) >> 3 ) ;
						src_ptr_p[0]  = CLIP3(0, 255, l0 + dif) ;
						src_ptr_q[0]  = CLIP3(0, 255, r0 - dif) ;
						if( ap )
							src_ptr_p[-width] += CLIP3( -C0,  C0, ( l2 + ((rl0 + 1) >> 1) - (l1<<1)) >> 1 );
						if( aq )
							src_ptr_q[width] += CLIP3( -C0,  C0, ( r2 + ((rl0 + 1) >> 1) - (r1<<1)) >> 1 );
					}
				}
			}
		}
		x_pos ++;
	}
}


void filter_chroma_ver_c(decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge)
{
	long mb_addr = loop_filter->mb_addr;
	macroblock_t *mb_q,*mb_p;
	long qp;
	long  index_a,index_b;
	long  alpha,beta;
	const uchar  *clip_tab;
	long  l0,l1,r0,r1;
	long  C0,c0;
	long  delta,abs_delta;
	long  dif;
	long  x_pos,y_pos;
	uchar  *src_ptr_p,*src_ptr_q;	
	long  strng;
	long pel;
	uchar **img_u = loop_filter->img_u;
	uchar **img_v = loop_filter->img_v;

	mb_q = &decoder->mb_date[mb_addr];
	if( edge == 0 ){
		mb_p = &decoder->mb_date[mb_addr-1];
	} else {
		mb_p = mb_q;
	}

	x_pos = (loop_filter->mb_x<<3) + (edge<<2);
	y_pos = loop_filter->mb_y<<3;

	qp = (QP_SCALE_CR[CLIP3(0,MAX_QP,mb_q->qp + decoder->active_pps->chroma_qp_index_offset)] + 
		QP_SCALE_CR[CLIP3(0,MAX_QP,mb_p->qp + decoder->active_pps->chroma_qp_index_offset)] + 1) >> 1;
	index_a = CLIP3(0,MAX_QP,qp+mb_q->alpha_c0_offset);
	index_b = CLIP3(0,MAX_QP,qp+mb_q->beta_offset);
	alpha = ALPHA_TABLE[index_a];
	beta = BETA_TABLE[index_b];
	clip_tab = CLIP_TABLE[index_a];
	
	for( pel=0;pel<8;pel++ ){
		if( (strng = strength[pel>>1])>0 ){
			src_ptr_q = &img_u[y_pos][x_pos];
            src_ptr_p = &img_u[y_pos][x_pos-1];
			l0 = src_ptr_p[0];
			l1 = src_ptr_p[-1];
			r0 = src_ptr_q[0];
			r1 = src_ptr_q[1];

			abs_delta = abs( delta = r0 - l0 );
			if( abs_delta < alpha )
			{
				if( ((abs( r0 - r1) - beta )  & (abs(l0 - l1) - beta )) < 0  ) 
				{
					if(strng == 4 )    // INTRA strong filtering
					{
						src_ptr_q[0] = ((r1 << 1) + r0 + l1 + 2) >> 2;
						src_ptr_p[0] = ((l1 << 1) + l0 + r1 + 2) >> 2;						
					}
					else
					{
						C0			= clip_tab[strng];
						c0          = C0 + 1;
						dif         = CLIP3( -c0, c0, ( (delta << 2) + (l1 - r1) + 4) >> 3 ) ;
						src_ptr_p[0]  = CLIP3(0, 255, l0 + dif) ;
						src_ptr_q[0]  = CLIP3(0, 255, r0 - dif) ;
					} 
				}
			}
		}        
		y_pos ++;
	}

	y_pos = loop_filter->mb_y<<3;
	for( pel=0;pel<8;pel++ ){
		if( (strng = strength[pel>>1])>0 ){
			src_ptr_q = &img_v[y_pos][x_pos];
            src_ptr_p = &img_v[y_pos][x_pos-1];
			l0 = src_ptr_p[0];
			l1 = src_ptr_p[-1];
			r0 = src_ptr_q[0];
			r1 = src_ptr_q[1];

			abs_delta = abs( delta = r0 - l0 );
			if( abs_delta < alpha )
			{
				if( ((abs( r0 - r1) - beta )  & (abs(l0 - l1) - beta )) < 0  ) 
				{
					if(strng == 4 )    // INTRA strong filtering
					{
						src_ptr_q[0] = ((r1 << 1) + r0 + l1 + 2) >> 2;
						src_ptr_p[0] = ((l1 << 1) + l0 + r1 + 2) >> 2;						
					}
					else
					{
						C0			= clip_tab[strng];
						c0          = C0 + 1;
						dif         = CLIP3( -c0, c0, ( (delta << 2) + (l1 - r1) + 4) >> 3 ) ;
						src_ptr_p[0]  = CLIP3(0, 255, l0 + dif) ;
						src_ptr_q[0]  = CLIP3(0, 255, r0 - dif) ;
					} 
				}
			}
		}        
		y_pos ++;
	}
}


void filter_chroma_hor_c(decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge)
{
	long mb_addr = loop_filter->mb_addr;
	macroblock_t *mb_q,*mb_p;
	long qp;
	long  index_a,index_b;
	long  alpha,beta;
	const uchar  *clip_tab;
	long  l0,l1,r0,r1;
	long  C0,c0;
	long  delta,abs_delta;
	long  dif;
	long  x_pos,y_pos;
	uchar  *src_ptr_p,*src_ptr_q;	
	long  strng;
	long width,width2,width3;	
	long pel;
	uchar **img_u = loop_filter->img_u;
	uchar **img_v = loop_filter->img_v;

	width = loop_filter->stride_chroma;
	width2 = width << 1;
	width3 = width + width2;
	mb_q = &decoder->mb_date[mb_addr];
	if( edge == 0 ){
		mb_p = &decoder->mb_date[mb_addr-decoder->pic_width_in_mbs];
	} else {
		mb_p = mb_q;
	}
	x_pos = (loop_filter->mb_x<<3);
	y_pos = (loop_filter->mb_y<<3) + (edge<<2);
	qp = (QP_SCALE_CR[CLIP3(0,MAX_QP,mb_q->qp + decoder->active_pps->chroma_qp_index_offset)] + 
		QP_SCALE_CR[CLIP3(0,MAX_QP,mb_p->qp + decoder->active_pps->chroma_qp_index_offset)] + 1) >> 1;
	index_a = CLIP3(0,MAX_QP,qp+mb_q->alpha_c0_offset);
	index_b = CLIP3(0,MAX_QP,qp+mb_q->beta_offset);
	alpha = ALPHA_TABLE[index_a];
	beta = BETA_TABLE[index_b];
	clip_tab = CLIP_TABLE[index_a];
	for( pel=0;pel<8;pel++ ){
		if( (strng = strength[pel>>1])>0 ){
			src_ptr_q = &img_u[y_pos][x_pos];
            src_ptr_p = &img_u[y_pos-1][x_pos];
			l0 = src_ptr_p[0];
			l1 = src_ptr_p[-width];
			r0 = src_ptr_q[0];
			r1 = src_ptr_q[width];

			abs_delta = abs( delta = r0 - l0 );
			if( abs_delta < alpha )
			{
				if( ((abs( r0 - r1) - beta )  & (abs(l0 - l1) - beta )) < 0  ) 
				{
					if(strng == 4 )    // INTRA strong filtering
					{
						src_ptr_q[0] = ((r1 << 1) + r0 + l1 + 2) >> 2;
						src_ptr_p[0] = ((l1 << 1) + l0 + r1 + 2) >> 2;						
					}
					else
					{
						C0			= clip_tab[strng];
						c0          = C0 + 1;
						dif         = CLIP3( -c0, c0, ( (delta << 2) + (l1 - r1) + 4) >> 3 ) ;
						src_ptr_p[0]  = CLIP3(0, 255, l0 + dif) ;
						src_ptr_q[0]  = CLIP3(0, 255, r0 - dif) ;
					} 
				}
			}
		}        
		x_pos ++;
	}
	x_pos = loop_filter->mb_x<<3;
	for( pel=0;pel<8;pel++ ){
		if( (strng = strength[pel>>1])>0 ){
			src_ptr_q = &img_v[y_pos][x_pos];
            src_ptr_p = &img_v[y_pos-1][x_pos];
			l0 = src_ptr_p[0];
			l1 = src_ptr_p[-width];
			r0 = src_ptr_q[0];
			r1 = src_ptr_q[width];

			abs_delta = abs( delta = r0 - l0 );
			if( abs_delta < alpha )
			{
				if( ((abs( r0 - r1) - beta )  & (abs(l0 - l1) - beta )) < 0  ) 
				{
					if(strng == 4 )    // INTRA strong filtering
					{
						src_ptr_q[0] = ((r1 << 1) + r0 + l1 + 2) >> 2;
						src_ptr_p[0] = ((l1 << 1) + l0 + r1 + 2) >> 2;						
					}
					else
					{
						C0			= clip_tab[strng];
						c0          = C0 + 1;
						dif         = CLIP3( -c0, c0, ( (delta << 2) + (l1 - r1) + 4) >> 3 ) ;
						src_ptr_p[0]  = CLIP3(0, 255, l0 + dif) ;
						src_ptr_q[0]  = CLIP3(0, 255, r0 - dif) ;
					} 
				}
			}
		}        
		x_pos ++;
	}
}


void edge_loop_for_skip_mb(decoder_t *decoder, loop_filter_t *loop_filter, uchar deblock_strength[2][4][4])
{
	long mb_addr = loop_filter->mb_addr;
	macroblock_t *mb_q = &decoder->mb_date[mb_addr];
    macroblock_t *mb_left,*mb_top;
	long  index_a,index_b,qp;
	long  strength_sum;	
	if( loop_filter->filter_left_edge_flag ){
		strength_sum = deblock_strength[0][0][0] + deblock_strength[0][0][1] + deblock_strength[0][0][2] + deblock_strength[0][0][3];
		if( strength_sum ){
			mb_left = &decoder->mb_date[mb_addr-1];
			qp = (mb_q->qp + mb_left->qp + 1) >> 1;
			index_a = CLIP3(0,MAX_QP,qp + mb_q->alpha_c0_offset);
			index_b = CLIP3(0,MAX_QP,qp + mb_q->beta_offset);
			filter_luma_ver(loop_filter,deblock_strength[0][0],index_a,index_b,0);
			filter_chroma_ver(decoder,loop_filter,deblock_strength[0][0],0);
		}
	}
	if( loop_filter->filter_top_edge_flag ){
		mb_top = &decoder->mb_date[mb_addr - decoder->pic_width_in_mbs];
		qp = (mb_q->qp + mb_top->qp + 1) >> 1;
		index_a = CLIP3(0,MAX_QP,qp+mb_q->alpha_c0_offset);
		index_b = CLIP3(0,MAX_QP,qp+mb_q->beta_offset);
		strength_sum = deblock_strength[1][0][0] + deblock_strength[1][0][1] + deblock_strength[1][0][2] + deblock_strength[1][0][3];
		if( strength_sum ){
			filter_luma_hor(loop_filter,deblock_strength[1][0],index_a,index_b,0);
			filter_chroma_hor(decoder,loop_filter,deblock_strength[1][0],0);		
		}
	}
}

void edge_loop(decoder_t *decoder,loop_filter_t *loop_filter,uchar deblock_strength[2][4][4])
{
	long mb_addr = loop_filter->mb_addr;
	macroblock_t *mb_q = &decoder->mb_date[mb_addr];
    macroblock_t *mb_left,*mb_top;
	long  index_a,index_b,qp;
	long  strength_sum;	

	if( loop_filter->filter_left_edge_flag ){
		strength_sum = deblock_strength[0][0][0] + deblock_strength[0][0][1] + deblock_strength[0][0][2] + deblock_strength[0][0][3];
		if( strength_sum ){
			mb_left = &decoder->mb_date[mb_addr-1];
			qp = (mb_q->qp + mb_left->qp + 1) >> 1;
			index_a = CLIP3(0,MAX_QP,qp + mb_q->alpha_c0_offset);
			index_b = CLIP3(0,MAX_QP,qp + mb_q->beta_offset);
			filter_luma_ver(loop_filter,deblock_strength[0][0],index_a,index_b,0);
			filter_chroma_ver(decoder,loop_filter,deblock_strength[0][0],0);
		}
	}

	qp = mb_q->qp;
	index_a = CLIP3(0,MAX_QP,qp + mb_q->alpha_c0_offset);
	index_b = CLIP3(0,MAX_QP,qp + mb_q->beta_offset);
	strength_sum = 	deblock_strength[0][1][0] + deblock_strength[0][1][1] + deblock_strength[0][1][2] + deblock_strength[0][1][3];
	if( strength_sum ){
		filter_luma_ver(loop_filter,deblock_strength[0][1],index_a,index_b,1);
	}
	strength_sum = 	deblock_strength[0][2][0] + deblock_strength[0][2][1] + deblock_strength[0][2][2] + deblock_strength[0][2][3];
	if( strength_sum ){
		filter_luma_ver(loop_filter,deblock_strength[0][2],index_a,index_b,2);
		filter_chroma_ver(decoder,loop_filter,deblock_strength[0][2],1);
	}
	strength_sum = 	deblock_strength[0][3][0] + deblock_strength[0][3][1] + deblock_strength[0][3][2] + deblock_strength[0][3][3];
	if( strength_sum ){
		filter_luma_ver(loop_filter,deblock_strength[0][3],index_a,index_b,3);
	}	

	if( loop_filter->filter_top_edge_flag ){
		mb_top = &decoder->mb_date[mb_addr - decoder->pic_width_in_mbs];
		qp = (mb_q->qp + mb_top->qp + 1) >> 1;
		index_a = CLIP3(0,MAX_QP,qp+mb_q->alpha_c0_offset);
		index_b = CLIP3(0,MAX_QP,qp+mb_q->beta_offset);
		strength_sum = deblock_strength[1][0][0] + deblock_strength[1][0][1] + deblock_strength[1][0][2] + deblock_strength[1][0][3];
		if( strength_sum ){
			filter_luma_hor(loop_filter,deblock_strength[1][0],index_a,index_b,0);
			filter_chroma_hor(decoder,loop_filter,deblock_strength[1][0],0);		
		}
	}
	qp = mb_q->qp;
	index_a = CLIP3(0,MAX_QP,qp+mb_q->alpha_c0_offset);
	index_b = CLIP3(0,MAX_QP,qp+mb_q->beta_offset);
	strength_sum = 	deblock_strength[1][1][0] + deblock_strength[1][1][1] + deblock_strength[1][1][2] + deblock_strength[1][1][3];
	if( strength_sum ){
		filter_luma_hor(loop_filter,deblock_strength[1][1],index_a,index_b,1);
	}

	strength_sum = 	deblock_strength[1][2][0] + deblock_strength[1][2][1] + deblock_strength[1][2][2] + deblock_strength[1][2][3];
	if( strength_sum ){
		filter_luma_hor(loop_filter,deblock_strength[1][2],index_a,index_b,2);
		filter_chroma_hor(decoder,loop_filter,deblock_strength[1][2],1);		
	}

	strength_sum = 	deblock_strength[1][3][0] + deblock_strength[1][3][1] + deblock_strength[1][3][2] + deblock_strength[1][3][3];
	if( strength_sum ){
		filter_luma_hor(loop_filter,deblock_strength[1][3],index_a,index_b,3);
	}
}

void deblock_intra_mb(decoder_t *decoder,loop_filter_t *loop_filter)
{
	long mb_addr = loop_filter->mb_addr;
	uchar deblock_strength[2][4][4];
	macroblock_t *mb_q = &decoder->mb_date[mb_addr];
	mb_access_t  *access = decoder->mb_access;		

	if( mb_q->disable_deblock_filter_idc == 1 ){
		return;
	}

	get_mb_pos(access,mb_addr,&loop_filter->mb_x,&loop_filter->mb_y);
	loop_filter->filter_left_edge_flag = (loop_filter->mb_x!=0);
	loop_filter->filter_top_edge_flag = (loop_filter->mb_y!=0);	

	if( mb_q->disable_deblock_filter_idc == 2 ){
		loop_filter->filter_left_edge_flag = (access->neighbour_mb_addr[mb_addr][0] >= 0);
		loop_filter->filter_top_edge_flag = (access->neighbour_mb_addr[mb_addr][1] >= 0);
	}
	deblock_strength[1][0][0] = deblock_strength[1][0][1] = deblock_strength[1][0][2] = deblock_strength[1][0][3] = 4;
	deblock_strength[0][0][0] = deblock_strength[0][0][1] = deblock_strength[0][0][2] = deblock_strength[0][0][3] = 4;
	deblock_strength[0][1][0] = deblock_strength[0][1][1] = deblock_strength[0][1][2] = deblock_strength[0][1][3] = 3;
	deblock_strength[1][1][0] = deblock_strength[1][1][1] = deblock_strength[1][1][2] = deblock_strength[1][1][3] = 3;
	deblock_strength[0][2][0] = deblock_strength[0][2][1] = deblock_strength[0][2][2] = deblock_strength[0][2][3] = 3;
	deblock_strength[1][2][0] = deblock_strength[1][2][1] = deblock_strength[1][2][2] = deblock_strength[1][2][3] = 3;
	deblock_strength[0][3][0] = deblock_strength[0][3][1] = deblock_strength[0][3][2] = deblock_strength[0][3][3] = 3;
	deblock_strength[1][3][0] = deblock_strength[1][3][1] = deblock_strength[1][3][2] = deblock_strength[1][3][3] = 3;
    
	edge_loop(decoder,loop_filter,deblock_strength);
}

void deblock_p_mb(decoder_t *decoder,loop_filter_t *loop_filter)
{
	long mb_addr = loop_filter->mb_addr;
	uchar  deblock_strength[2][4][4];
	macroblock_t *mb_q = &decoder->mb_date[mb_addr];	
	mb_access_t *access = decoder->mb_access;	

	if( mb_q->disable_deblock_filter_idc == 1 ){
		return;
	}

	if( mb_q->mb_type==I4MB || mb_q->mb_type==I16MB ){
		deblock_intra_mb(decoder,loop_filter);
		return;
	}

	get_mb_pos(access,mb_addr,&loop_filter->mb_x,&loop_filter->mb_y);
	loop_filter->filter_left_edge_flag = (loop_filter->mb_x!=0);
	loop_filter->filter_top_edge_flag = (loop_filter->mb_y!=0);


	if( mb_q->disable_deblock_filter_idc == 2 ){
		loop_filter->filter_left_edge_flag = (access->neighbour_mb_addr[mb_addr][0] >= 0);
		loop_filter->filter_top_edge_flag = (access->neighbour_mb_addr[mb_addr][1] >= 0);
	}

	if( mb_q->mb_type == 0 ){
		if( loop_filter->filter_left_edge_flag ){
			macroblock_t *mb_left = &decoder->mb_date[mb_addr-1];
			if( mb_left->mb_type==I4MB || mb_left->mb_type==I16MB ){
				deblock_strength[0][0][0] = deblock_strength[0][0][1] = deblock_strength[0][0][2] = deblock_strength[0][0][3] = 4;
			} else {
				get_strength_ver_pmb(decoder,deblock_strength[0][0],loop_filter,0);
			}
		}
		if( loop_filter->filter_top_edge_flag ){
			macroblock_t *mb_up = &decoder->mb_date[mb_addr-decoder->pic_width_in_mbs];
			if( mb_up->mb_type==I4MB || mb_up->mb_type==I16MB ){
				deblock_strength[1][0][0] = deblock_strength[1][0][1] = deblock_strength[1][0][2] = deblock_strength[1][0][3] = 4;
			} else {
				get_strength_hor_pmb(decoder,deblock_strength[1][0],loop_filter,0);
			}
		}
		edge_loop_for_skip_mb(decoder,loop_filter,deblock_strength);
	} else if( mb_q->mb_type == 1) {
		long cbp_blk = mb_q->cbp_blk;
		if( loop_filter->filter_left_edge_flag ){
			macroblock_t *mb_left = &decoder->mb_date[mb_addr-1];
			if( mb_left->mb_type==I4MB || mb_left->mb_type==I16MB ){
				deblock_strength[0][0][0] = deblock_strength[0][0][1] = deblock_strength[0][0][2] = deblock_strength[0][0][3] = 4;
			} else {
				get_strength_ver_pmb(decoder,deblock_strength[0][0],loop_filter,0);
			}
		}
		deblock_strength[0][1][0] = ((cbp_blk&0x1)|((cbp_blk>>1)&0x1))<<1;
		deblock_strength[0][1][1] = (((cbp_blk>>2)&0x1)|((cbp_blk>>3)&0x1))<<1;
		deblock_strength[0][1][2] = (((cbp_blk>>8)&0x1)|((cbp_blk>>9)&0x1))<<1;
		deblock_strength[0][1][3] = (((cbp_blk>>10)&0x1)|((cbp_blk>>11)&0x1))<<1;
		deblock_strength[0][2][0] = (((cbp_blk>>1)&0x1)|((cbp_blk>>4)&0x1))<<1;
		deblock_strength[0][2][1] = (((cbp_blk>>3)&0x1)|((cbp_blk>>6)&0x1))<<1;
		deblock_strength[0][2][2] = (((cbp_blk>>9)&0x1)|((cbp_blk>>12)&0x1))<<1;
		deblock_strength[0][2][3] = (((cbp_blk>>11)&0x1)|((cbp_blk>>14)&0x1))<<1;
		deblock_strength[0][3][0] = (((cbp_blk>>4)&0x1)|((cbp_blk>>5)&0x1))<<1;
		deblock_strength[0][3][1] = (((cbp_blk>>6)&0x1)|((cbp_blk>>7)&0x1))<<1;
		deblock_strength[0][3][2] = (((cbp_blk>>12)&0x1)|((cbp_blk>>13)&0x1))<<1;
		deblock_strength[0][3][3] = (((cbp_blk>>14)&0x1)|((cbp_blk>>15)&0x1))<<1;

		if( loop_filter->filter_top_edge_flag ){
			macroblock_t *mb_up = &decoder->mb_date[mb_addr-decoder->pic_width_in_mbs];
			if( mb_up->mb_type==I4MB || mb_up->mb_type==I16MB ){
				deblock_strength[1][0][0] = deblock_strength[1][0][1] = deblock_strength[1][0][2] = deblock_strength[1][0][3] = 4;
			} else {
				get_strength_hor_pmb(decoder,deblock_strength[1][0],loop_filter,0);
			}
		}
		deblock_strength[1][1][0] = (((cbp_blk)&0x1)|((cbp_blk>>2)&0x1))<<1;
		deblock_strength[1][1][1] = (((cbp_blk>>1)&0x1)|((cbp_blk>>3)&0x1))<<1;
		deblock_strength[1][1][2] = (((cbp_blk>>4)&0x1)|((cbp_blk>>6)&0x1))<<1;
		deblock_strength[1][1][3] = (((cbp_blk>>5)&0x1)|((cbp_blk>>7)&0x1))<<1;
		deblock_strength[1][2][0] = (((cbp_blk>>2)&0x1)|((cbp_blk>>8)&0x1))<<1;
		deblock_strength[1][2][1] = (((cbp_blk>>3)&0x1)|((cbp_blk>>9)&0x1))<<1;
        deblock_strength[1][2][2] = (((cbp_blk>>6)&0x1)|((cbp_blk>>12)&0x1))<<1;
		deblock_strength[1][2][3] = (((cbp_blk>>7)&0x1)|((cbp_blk>>13)&0x1))<<1;
		deblock_strength[1][3][0] = (((cbp_blk>>8)&0x1)|((cbp_blk>>10)&0x1))<<1;
		deblock_strength[1][3][1] = (((cbp_blk>>9)&0x1)|((cbp_blk>>11)&0x1))<<1;
		deblock_strength[1][3][2] = (((cbp_blk>>12)&0x1)|((cbp_blk>>14)&0x1))<<1;
		deblock_strength[1][3][3] = (((cbp_blk>>13)&0x1)|((cbp_blk>>15)&0x1))<<1;
		
        edge_loop(decoder,loop_filter,deblock_strength);
	} else {
		if( loop_filter->filter_left_edge_flag ){
			get_strength_ver_pmb(decoder,deblock_strength[0][0],loop_filter,0);
		}
		get_strength_ver_pmb(decoder,deblock_strength[0][1],loop_filter,1);
		get_strength_ver_pmb(decoder,deblock_strength[0][2],loop_filter,2);
		get_strength_ver_pmb(decoder,deblock_strength[0][3],loop_filter,3);

		if( loop_filter->filter_top_edge_flag ){
			get_strength_hor_pmb(decoder,deblock_strength[1][0],loop_filter,0);
		}
		get_strength_hor_pmb(decoder,deblock_strength[1][1],loop_filter,1);
		get_strength_hor_pmb(decoder,deblock_strength[1][2],loop_filter,2);
		get_strength_hor_pmb(decoder,deblock_strength[1][3],loop_filter,3);

		edge_loop(decoder,loop_filter,deblock_strength);
	}
}



void  deblock_picture(decoder_t *decoder)
{
	long i;    
	slice_t *slice = decoder->cur_slice;
	loop_filter_t loop_filter;
	picture_t *pic = decoder->cur_pic;	

	loop_filter.img_y = pic->img_y;
	loop_filter.img_u = pic->img_u;
	loop_filter.img_v = pic->img_v;
	loop_filter.stride_luma = pic->stride_luma;
	loop_filter.stride_chroma = pic->stride_chroma;
	loop_filter.front_ref_idx = pic->front_ref_idx;
	loop_filter.front_mv = pic->front_mv;
	if( slice->slice_type == I_SLICE ){
		for( i=0;i<decoder->pic_size_in_mbs;i++ ){
			loop_filter.mb_addr = i;
			deblock_intra_mb(decoder,&loop_filter);
		}
	} else if( slice->slice_type == P_SLICE ){
		for( i=0;i<decoder->pic_size_in_mbs;i++ ){
			loop_filter.mb_addr = i;			
			deblock_p_mb(decoder,&loop_filter);
		}
	}

	return;
}

/*
void filter_luma_ver_mmx(loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge)
{	
    long  x_pos, y_pos;
	uchar  *src;
	long  stride = loop_filter->stride_luma;
	long  alpha, beta;
	const  uchar * clip_table = NULL;
	uchar ** img_y = loop_filter->img_y;	
	long i;
	long strng;
	long c0;

	alpha = ALPHA_TABLE[index_a];
	beta = BETA_TABLE[index_b];
	clip_table = CLIP_TABLE[index_a];
	x_pos = (loop_filter->mb_x<<4) + (edge<<2) - 4;
	y_pos = loop_filter->mb_y<<4;
   
	for( i=0;i<4;i++ ){
		if( (strng = strength[i])>0 ){
			 src = & img_y[y_pos][x_pos];
			 if ( strng == 4 ){
				 filter_luma_ver_eq4_mmx(src,alpha,beta,stride);
			 } else {
				 c0 = clip_table[strng];
				 filter_luma_ver_lt4_mmx(src,alpha,beta,c0,stride);
			 }
		}
		y_pos += 4;
	}
}

void filter_luma_hor_mmx(loop_filter_t *loop_filter,uchar strength[4],long index_a,long index_b,long edge)
{	
	long  x_pos, y_pos;
	uchar  *src;
	long  stride = loop_filter->stride_luma;
	long  alpha, beta;
	const  uchar * clip_table = NULL;
	uchar ** img_y = loop_filter->img_y;	
	long i;
	long strng;
	long c0;

	alpha = ALPHA_TABLE[index_a];
	beta = BETA_TABLE[index_b];
	clip_table = CLIP_TABLE[index_a];
	x_pos = (loop_filter->mb_x<<4);
	y_pos = (loop_filter->mb_y<<4) + (edge<<2) - 4;

	for( i=0;i<4;i++ ){
		if( (strng = strength[i])>0 ){
			src = & img_y[y_pos][x_pos];
			if( strng==4 ){
				filter_luma_hor_eq4_mmx(src,alpha,beta,stride);
			} else {
				c0 = clip_table[strng];
				filter_luma_hor_lt4_mmx(src,alpha,beta,c0,stride);
			}
		}
		x_pos += 4;
	}
}

void filter_chroma_ver_mmx(decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge)
{
	long mb_addr = loop_filter->mb_addr;
	macroblock_t *mb_q,*mb_p;
	long qp;
	long  index_a,index_b;
	long  alpha,beta;
	const uchar  *clip_tab;
	long  C0;	
	long  x_pos,y_pos;	
	long  strng;
	long pel;
	uchar **img_u = loop_filter->img_u;
	uchar **img_v = loop_filter->img_v;
	long stride = loop_filter->stride_chroma;
	uchar *u_src,*v_src;

	mb_q = &decoder->mb_date[mb_addr];
	if( edge == 0 ){
		mb_p = &decoder->mb_date[mb_addr-1];
	} else {
		mb_p = mb_q;
	}

	x_pos = (loop_filter->mb_x<<3) + (edge<<2) - 4;
	y_pos = loop_filter->mb_y<<3;

	qp = (QP_SCALE_CR[CLIP3(0,MAX_QP,mb_q->qp + decoder->active_pps->chroma_qp_index_offset)] + 
		QP_SCALE_CR[CLIP3(0,MAX_QP,mb_p->qp + decoder->active_pps->chroma_qp_index_offset)] + 1) >> 1;
	index_a = CLIP3(0,MAX_QP,qp+mb_q->alpha_c0_offset);
	index_b = CLIP3(0,MAX_QP,qp+mb_q->beta_offset);
	alpha = ALPHA_TABLE[index_a];
	beta = BETA_TABLE[index_b];
	clip_tab = CLIP_TABLE[index_a];

	for( pel=0;pel<4;pel++ ){
		if( (strng = strength[pel])>0 ){
			u_src = &img_u[y_pos][x_pos];
			v_src = &img_v[y_pos][x_pos];
			if( strng == 4 ){
				filter_chroma_ver_eq4_mmx(u_src,v_src,alpha,beta,stride);
			} else {
				C0 = clip_tab[strng];
				filter_chroma_ver_lt4_mmx(u_src,v_src,alpha,beta,C0,stride);
			}
		}
		y_pos += 2;
	}
}

void filter_chroma_hor_mmx(decoder_t *decoder,loop_filter_t *loop_filter,uchar strength[4],long edge)
{
	long mb_addr = loop_filter->mb_addr;
	macroblock_t *mb_q,*mb_p;
	long qp;
	long  index_a,index_b;
	long  alpha,beta;
	const uchar  *clip_tab;
	long  C0;
	long  x_pos,y_pos;
	long  strng;	
	long pel;
	uchar **img_u = loop_filter->img_u;
	uchar **img_v = loop_filter->img_v;
	long stride = loop_filter->stride_chroma;
	uchar *u_src,*v_src;
	
	mb_q = &decoder->mb_date[mb_addr];
	if( edge == 0 ){
		mb_p = &decoder->mb_date[mb_addr-decoder->pic_width_in_mbs];
	} else {
		mb_p = mb_q;
	}
	x_pos = (loop_filter->mb_x<<3);
	y_pos = (loop_filter->mb_y<<3) + (edge<<2) - 2;
	qp = (QP_SCALE_CR[CLIP3(0,MAX_QP,mb_q->qp + decoder->active_pps->chroma_qp_index_offset)] + 
		QP_SCALE_CR[CLIP3(0,MAX_QP,mb_p->qp + decoder->active_pps->chroma_qp_index_offset)] + 1) >> 1;
	index_a = CLIP3(0,MAX_QP,qp+mb_q->alpha_c0_offset);
	index_b = CLIP3(0,MAX_QP,qp+mb_q->beta_offset);
	alpha = ALPHA_TABLE[index_a];
	beta = BETA_TABLE[index_b];
	clip_tab = CLIP_TABLE[index_a];

	for( pel=0;pel<4;pel++ ){
		if( (strng = strength[pel])>0 ){
			u_src = &img_u[y_pos][x_pos];
			v_src = &img_v[y_pos][x_pos];
			if( strng == 4 ){
				filter_chroma_hor_eq4_mmx(u_src,v_src,alpha,beta,stride);
			} else {
				C0 = clip_tab[strng];
				filter_chroma_hor_lt4_mmx(u_src,v_src,alpha,beta,C0,stride);
			}
		}
		x_pos += 2;
	}
}
*/

/*
void filter_luma_ver_eq4_mmx(uchar *src,long alpha, long beta,long stride)
{    
	long long tempL3;
	long long tempL2;
	long long tempL1;
	long long tempL0;
	long long tempR3;
	long long tempR2;
	long long tempR1;
	long long tempR0;
	long long temp1;
	long long temp2;
	long   stride3 = (stride<<1) + stride;

	__asm {
		mov     edx, stride
		mov     eax , stride3
		mov     esi , dword ptr [src]
		movd	mm1, dword ptr [esi]			; 00000000 L00L12L02L03
		movd	mm4, dword ptr [esi+4]			; 00000000 R03R02R01R00
		movd	mm2, dword ptr [esi+edx]		; 00000000 L10L11L12L13
		movd	mm7, dword ptr [esi+edx+4]		; 00000000 R13R12R11R10
		movd	mm3, dword ptr [esi+edx*2]		; 00000000 L20L21L22L23
		movd	mm5, dword ptr [esi+edx*2+4]	; 00000000 R23R22R21R20
		movd	mm6, dword ptr [esi+eax]		; 00000000 L30L31L32L33
		movd	mm0, dword ptr [esi+eax+4]		; 00000000 R33R32R31R30
		
		punpcklbw mm1, mm2						; L10L00L11L12 L12L02L13L03
		punpcklbw mm4, mm7						; R13R03R12R02 R11R01R10R00 
		movq	mm2, mm1						; 
		pxor	mm7, mm7
		punpcklbw mm3, mm6						; L30L20L31L21 L32L22L33L23
		punpcklbw mm5, mm0						; R33R23R32R22 R31R21R30R20

		movq	mm6, mm4
		punpcklwd mm1, mm3						; L32L22L12L02 L33L23L13L03
		punpckhwd mm2, mm3						; L30L20L10L00 L31L21L11L01
		movq	mm0, mm1
		movq	mm3, mm2
		punpcklwd mm4, mm5						; R31R21R11R01 R30R20R10R00
		punpckhwd mm6, mm5						; R33R23R13R03 R32R22R12R02
		
		punpcklbw mm1, mm7						; 00L3300L23 00L1300L03 
		punpckhbw mm0, mm7						; 00L3200L22 00L1200L02
		movq	mm5, mm4
		punpcklbw mm2, mm7						; 00L3100L21 00L1100L01
		punpckhbw mm3, mm7						; 00L3000L20 00L1000L00
		punpcklbw mm4, mm7						; 00R3000R20 00R1000R00
		punpckhbw mm5, mm7						; 00R3100R21 00R1100R01
		movq	qword ptr [tempL3], mm1			; L3
		movq	qword ptr [tempL2], mm0			; L2
		movq	qword ptr [tempL1], mm2			; L1
		movq	qword ptr [tempL0], mm3			; L0
		movq	qword ptr [tempR0], mm4			; R0
		movq	qword ptr [tempR1], mm5			; R1
		movq	mm0, mm6
		punpcklbw mm6, mm7						; 00R3200R22 00R1200R02
		punpckhbw mm0, mm7						; 00R3300R23 00R1300R03
		movq	qword ptr [tempR2], mm6			; R2
		movq	qword ptr [tempR3], mm0			; R3
		
		movq	mm0, mm2
		movq	mm1, mm5
		psubw	mm0, mm3						; L1 - L0
		psubw	mm1, mm4						; R1 - R0
		movq	mm6, mm0
		movq	mm7, mm1
		psraw	mm0, 0Fh
		psraw	mm1, 0Fh
		pxor	mm6, mm0
		pxor	mm7, mm1
		psubw	mm6, mm0
		psubw	mm7, mm1
		movd	mm0, beta						; Beta
		pshufw	mm0, mm0, 0
		movq	mm1, mm0
		pcmpgtw	mm0, mm6
		pcmpgtw	mm1, mm7
		pand	mm0, mm1
		
		movd	mm7, alpha						; Alpha
		movq	mm1, mm3
		pshufw	mm7, mm7, 0
		psubw	mm1, mm4						; L0 - R0
		movq	mm6, mm1
		psraw	mm6, 0Fh
		pxor	mm1, mm6
		psubw	mm1, mm6
		movq	mm6, mm7
		pcmpgtw	mm7, mm1
		pand	mm0, mm7						; Alpha > |L0 - R0|
		
		psrlw	mm6, 2							; small_gap = (|R0 - L0| < ((Alpha >> 2) + 2))
		paddw	mm6, two64
		pcmpgtw	mm6, mm1
		psubw	mm3, qword ptr [tempL2]			; L0 - L2
		psubw	mm4, qword ptr [tempR2]			; R0 - R2
		movq	mm1, mm3
		movq	mm5, mm4
		psraw	mm3, 0Fh
		psraw	mm4, 0Fh
		pxor	mm1, mm3
		pxor	mm5, mm4
		psubw	mm1, mm3
		psubw	mm5, mm4
		movd	mm3, beta
		pshufw	mm3, mm3, 0
		movq	mm4, mm3
		pcmpgtw	mm3, mm1
		pcmpgtw	mm4, mm5
		pand	mm3, mm6						; ap &= small_gap, for BS = 4
		pand	mm4, mm6						; aq &= small_gap, for BS = 4
		
		movq	mm6, qword ptr [tempL3]
		movq	mm2, qword ptr [tempL2]	
		movq	mm1, qword ptr [tempL1]
		paddw	mm6, mm2
		movq	mm7, mm1
		paddw	mm1, qword ptr [tempL0]			; L1 + L0
		paddw	mm6, mm6						; (L2 + L3) << 1
		paddw	mm7, mm1						; 2*L1 + L0
		paddw	mm1, qword ptr [tempR0]			; L1 + L0 + R0
		movq	mm5, qword ptr [tempR1]			; R1
		paddw	mm7, mm5						; 2*L1 + L0 + R1
		paddw	mm5, mm1						; L1 + L0 + R0 + R1
		paddw	mm1, mm2						; L1 + L0 + R0 + L2
		paddw	mm5, mm1						; L2 + 2*L1 + 2*L0 + 2*R0 + R1 
		paddw	mm7, two64
		paddw	mm5, four64
		psrlw	mm7, 2							; (2*L1 + L0 + R1 + 2) >> 2,              L0 for ap = 0
		psrlw	mm5, 3							; (L2 + 2*L1 + 2*L0 + 2*R0 + R + 4) >> 3, L0 for ap = 1
		paddw	mm6, mm1
		paddw	mm1, two64
		paddw	mm6, four64
		psrlw	mm1, 2							; (L1 + L0 + R0 + L2 + 2) >>2,            L1 for ap = 1
		psrlw	mm6, 3							; (2*L3 + 3*L2 + L1 + L0 + R0 + 4) >>3,   L2 for ap = 1

		movq	mm2, qword ptr [tempL2]
		psubw	mm6, mm2
		pand	mm6, mm3
		pand	mm6, mm0
		paddw	mm2, mm6
		movq	qword ptr [tempL2], mm2			; L2

		movq	mm2, qword ptr [tempL1]
		psubw	mm1, mm2
		pand	mm1, mm3
		pand	mm1, mm0
		paddw	mm2, mm1
		movq	qword ptr [temp1], mm2			; L1, store in new addr, old L1 should reserve for Rx

		movq	mm2, qword ptr [tempL0]
		psubw	mm5, mm7
		pand	mm5, mm3
		paddw	mm7, mm5
		psubw	mm7, mm2
		pand	mm7, mm0
		paddw	mm2, mm7
		movq	qword ptr [temp2], mm2			; L0, store in new addr, old L1 should reserve for Rx

		movq	mm6, qword ptr [tempR3]
		movq	mm2, qword ptr [tempR2]
		movq	mm1, qword ptr [tempR1]
		paddw	mm6, mm2
		movq	mm7, mm1
		paddw	mm1, qword ptr [tempR0]
		paddw	mm6, mm6
		paddw	mm7, mm1
		paddw	mm1, qword ptr [tempL0]
		movq	mm5, qword ptr [tempL1]
		paddw	mm7, mm5
		paddw	mm5, mm1
		paddw	mm1, mm2
		paddw	mm5, mm1
		paddw	mm7, two64
		paddw	mm5, four64
		psrlw	mm7, 2
		psrlw	mm5, 3
		paddw	mm6, mm1
		paddw	mm1, two64
		paddw	mm6, four64
		psrlw	mm1, 2
		psrlw	mm6, 3
		movq	mm2, qword ptr [tempR0]
		psubw	mm5, mm7
		pand	mm5, mm4
		paddw	mm7, mm5
		psubw	mm7, mm2
		pand	mm7, mm0
		paddw	mm2, mm7
		movq	qword ptr [tempR0], mm2			; R0
		movq	mm2, qword ptr [tempR1]
		psubw	mm1, mm2
		pand	mm1, mm4
		pand	mm1, mm0
		paddw	mm2, mm1
		movq	qword ptr [tempR1], mm2			; R1
		movq	mm2, qword ptr [tempR2]
		psubw	mm6, mm2
		pand	mm6, mm4
		pand	mm6, mm0
		paddw	mm2, mm6
		movq	qword ptr [tempR2], mm2			; R2

		movq	mm0, qword ptr [tempL3]
		movq	mm1, qword ptr [tempL2]
		movq	mm2, qword ptr [temp1]
		movq	mm3, qword ptr [temp2]
		psllq	mm1, 8
		psllq	mm3, 8
		por	mm0, mm1
		por	mm2, mm3
		movq	mm1, mm0
		punpcklwd mm0, mm2
		punpckhwd mm1, mm2
		movq	mm4, qword ptr [tempR0]
		movq	mm5, qword ptr [tempR1]
		movq	mm6, qword ptr [tempR2]
		movq	mm7, qword ptr [tempR3]
		psllq	mm5, 8
		psllq	mm7, 8
		por	mm4, mm5
		por	mm6, mm7
		movq	mm5, mm4
		punpcklwd mm4, mm6
		punpckhwd mm5, mm6
		movq	mm2, mm0
		punpckldq mm0, mm4
		punpckhdq mm2, mm4
		movq	mm3, mm1
		punpckldq mm1, mm5
		punpckhdq mm3, mm5

		mov     edx, stride;
		mov		eax, stride3;
		movq	qword ptr [esi], mm0
		movq	qword ptr [esi+edx], mm2
		movq	qword ptr [esi+edx*2], mm1
		movq	qword ptr [esi+eax], mm3
		emms
	}
}

void filter_luma_ver_lt4_mmx(uchar *src,long alpha, long beta, long C0, long width)
{
	long long tempL2;
	long long tempL1;
	long long tempL0;
	long long tempR2;
	long long tempR1;
	long long tempR0;	

	__asm {
		mov     edx, width
		mov		ecx, src
		lea	eax, [edx+edx*2]
		movd	mm1, dword ptr [ecx]
		movd	mm4, dword ptr [ecx+4]
		movd	mm2, dword ptr [ecx+edx]
		movd	mm7, dword ptr [ecx+edx+4]
		movd	mm3, dword ptr [ecx+edx*2]
		movd	mm5, dword ptr [ecx+edx*2+4]
		movd	mm6, dword ptr [ecx+eax]
		movd	mm0, dword ptr [ecx+eax+4]
		punpcklbw mm1, mm2
		punpcklbw mm4, mm7
		movq	mm2, mm1
		pxor	mm7, mm7
		punpcklbw mm3, mm6
		punpcklbw mm5, mm0
		movq	mm6, mm4
		punpcklwd mm1, mm3
		punpckhwd mm2, mm3
		punpcklwd mm4, mm5
		punpckhwd mm6, mm5
		movq	mm3, mm2
		punpckhbw mm1, mm7
		movq	mm5, mm4
		punpcklbw mm2, mm7
		punpckhbw mm3, mm7
		punpcklbw mm4, mm7
		punpckhbw mm5, mm7
		punpcklbw mm6, mm7
		movq	qword ptr [tempL2], mm1		// L2
		movq	qword ptr [tempL1], mm2		// L1
		movq	qword ptr [tempL0], mm3		// L0
		movq	qword ptr [tempR0], mm4		// R0
		movq	qword ptr [tempR1], mm5		// R1
		movq	qword ptr [tempR2], mm6		// R2

		movq	mm0, mm2
		movq	mm1, mm5
		psubw	mm0, mm3
		psubw	mm1, mm4
		movq	mm6, mm0
		movq	mm7, mm1
		psraw	mm0, 0fh
		psraw	mm1, 0fh
		pxor	mm6, mm0
		pxor	mm7, mm1
		psubw	mm6, mm0
		psubw	mm7, mm1
		movd	mm0, beta
		pshufw	mm0, mm0, 0
		movq	mm1, mm0
		pcmpgtw	mm0, mm6
		pcmpgtw	mm1, mm7
		pand	mm0, mm1
		movd	mm7, alpha
		pshufw	mm7, mm7, 0
		movq	mm1, mm3
		psubw	mm1, mm4
		movq	mm6, mm1
		psraw	mm6, 0fh
		pxor	mm1, mm6
		psubw	mm1, mm6
		pcmpgtw	mm7, mm1
		pand	mm0, mm7
		movq	mm6, mm3
		pavgw	mm6, mm4
		movq	mm7, mm6
		paddw	mm6, qword ptr [tempL2]
		paddw	mm7, qword ptr [tempR2]
		psubw	mm6, mm2
		psubw	mm7, mm5
		psubw	mm6, mm2
		psubw	mm7, mm5
		psraw	mm6, 1
		psraw	mm7, 1
		movq	mm1, mm4
		psubw	mm1, mm3
		psubw	mm2, mm5
		psllw	mm1, 2
		paddw	mm2, mm1
		paddw	mm2, four64
		psraw	mm2, 3
		psubw	mm3, qword ptr [tempL2]
		psubw	mm4, qword ptr [tempR2]
		movq	mm1, mm3
		movq	mm5, mm4
		psraw	mm3, 0Fh
		psraw	mm4, 0Fh
		pxor	mm1, mm3
		pxor	mm5, mm4
		psubw	mm1, mm3
		psubw	mm5, mm4
		movd	mm3, beta
		pshufw	mm3, mm3, 0
		movq	mm4, mm3
		pcmpgtw	mm3, mm1
		pcmpgtw	mm4, mm5
		movd	mm1, C0
		pxor	mm5, mm5
		pshufw	mm1, mm1, 0
		psubw	mm5, mm1
		pmaxsw	mm6, mm5
		pmaxsw	mm7, mm5
		pminsw	mm6, mm1
		pminsw	mm7, mm1
		pand	mm6, mm3
		pand	mm7, mm4
		pand	mm6, mm0
		pand	mm7, mm0
		paddw	mm3, mm4
		paddw	mm5, mm3
		psubw	mm1, mm3
		pmaxsw	mm2, mm5
		pminsw	mm1, mm2
		pand	mm1, mm0
		paddw	mm6, qword ptr [tempL1]
		pshufw	mm4, qword ptr [tempR0], 0e4h
		psubw	mm4, mm1
		paddw	mm1, qword ptr [tempL0]
		paddw	mm7, qword ptr [tempR1]
		add	ecx, 2
		lea	eax, [edx+edx*2]
		packuswb mm6, mm6
		packuswb mm1, mm1
		packuswb mm4, mm4
		packuswb mm7, mm7
		punpcklbw mm6, mm1
		punpcklbw mm4, mm7
		movq	mm3, mm6
		punpcklwd mm6, mm4
		punpckhwd mm3, mm4
		pshufw	mm1, mm6, 4eh
		pshufw	mm5, mm3, 4eh
		movd	dword ptr [ecx], mm6
		movd	dword ptr [ecx+edx], mm1
		movd	dword ptr [ecx+edx*2], mm3
		movd	dword ptr [ecx+eax], mm5
		emms
	}
}

void filter_luma_hor_eq4_mmx(uchar *src,long alpha, long beta, long stride)
{
	long long tempL2;
	long long tempL1;
	long long tempL0;
	long long tempR2;
	long long tempR1;
	long long tempR0;
	long long tempL3;
	long long tempR3;
	long long temp1;
	long long temp2;

	__asm {
		mov		ecx, src
		mov		edx, stride
		lea		eax, [edx+edx*2]
		lea		esi, [ecx+edx*4]
		pxor	mm7, mm7
		movd	mm0, dword ptr [ecx]		// Hor
		movd	mm1, dword ptr [ecx+edx]
		movd	mm2, dword ptr [ecx+edx*2]
		movd	mm3, dword ptr [ecx+eax]
		movd	mm4, dword ptr [esi]
		movd	mm5, dword ptr [esi+edx]
		movd	mm6, dword ptr [esi+edx*2]
		punpcklbw mm0, mm7
		punpcklbw mm1, mm7
		punpcklbw mm2, mm7
		punpcklbw mm3, mm7
		punpcklbw mm4, mm7
		punpcklbw mm5, mm7
		punpcklbw mm6, mm7
		movq	qword ptr [tempL3], mm0
		movq	qword ptr [tempL2], mm1
		movq	qword ptr [tempL1], mm2
		movq	qword ptr [tempL0], mm3
		movq	qword ptr [tempR0], mm4
		movq	qword ptr [tempR1], mm5
		movq	qword ptr [tempR2], mm6
		pxor	mm0, mm0
		movd	mm7, dword ptr [esi+eax]
		punpcklbw mm7, mm0
		movq	qword ptr [tempR3], mm7
		movq	mm0, mm2
		movq	mm1, mm5
		psubw	mm0, mm3
		psubw	mm1, mm4
		movq	mm6, mm0
		movq	mm7, mm1
		psraw	mm0, 0Fh
		psraw	mm1, 0Fh
		pxor	mm6, mm0
		pxor	mm7, mm1
		psubw	mm6, mm0
		psubw	mm7, mm1
		movd	mm0, beta
		pshufw	mm0, mm0, 0
		movq	mm1, mm0
		pcmpgtw	mm0, mm6
		pcmpgtw	mm1, mm7
		pand	mm0, mm1
		movd	mm7, alpha
		movq	mm1, mm3
		pshufw	mm7, mm7, 0
		psubw	mm1, mm4
		movq	mm6, mm1
		psraw	mm6, 0Fh
		pxor	mm1, mm6
		psubw	mm1, mm6
		movq	mm6, mm7
		pcmpgtw	mm7, mm1
		pand	mm0, mm7
		psrlw	mm6, 2
		paddw	mm6, two64
		pcmpgtw	mm6, mm1
		psubw	mm3, qword ptr [tempL2]
		psubw	mm4, qword ptr [tempR2]
		movq	mm1, mm3
		movq	mm5, mm4
		psraw	mm3, 0Fh
		psraw	mm4, 0Fh
		pxor	mm1, mm3
		pxor	mm5, mm4
		psubw	mm1, mm3
		psubw	mm5, mm4
		movd	mm3, beta
		pshufw	mm3, mm3, 0
		movq	mm4, mm3
		pcmpgtw	mm3, mm1
		pcmpgtw	mm4, mm5
		pand	mm3, mm6
		pand	mm4, mm6
		movq	mm6, qword ptr [tempL3]
		movq	mm2, qword ptr [tempL2]
		movq	mm1, qword ptr [tempL1]
		paddw	mm6, mm2
		movq	mm7, mm1
		paddw	mm1, qword ptr [tempL0]
		paddw	mm6, mm6
		paddw	mm7, mm1
		paddw	mm1, qword ptr [tempR0]
		movq	mm5, qword ptr [tempR1]
		paddw	mm7, mm5
		paddw	mm5, mm1
		paddw	mm1, mm2
		paddw	mm5, mm1
		paddw	mm7, two64
		paddw	mm5, four64
		psrlw	mm7, 2
		psrlw	mm5, 3
		paddw	mm6, mm1
		paddw	mm1, two64
		paddw	mm6, four64
		psrlw	mm1, 2
		psrlw	mm6, 3
		movq	mm2, qword ptr [tempL2]
		psubw	mm6, mm2
		pand	mm6, mm3
		pand	mm6, mm0
		paddw	mm2, mm6
		movq	qword ptr [tempL2], mm2
		movq	mm2, qword ptr [tempL1]
		psubw	mm1, mm2
		pand	mm1, mm3
		pand	mm1, mm0
		paddw	mm2, mm1
		movq	qword ptr [temp1], mm2
		movq	mm2, qword ptr [tempL0]
		psubw	mm5, mm7
		pand	mm5, mm3
		paddw	mm7, mm5
		psubw	mm7, mm2
		pand	mm7, mm0
		paddw	mm2, mm7
		movq	qword ptr [temp2], mm2
		movq	mm6, qword ptr [tempR3]
		movq	mm2, qword ptr [tempR2]
		movq	mm1, qword ptr [tempR1]
		paddw	mm6, mm2
		movq	mm7, mm1
		paddw	mm1, qword ptr [tempR0]
		paddw	mm6, mm6
		paddw	mm7, mm1
		paddw	mm1, qword ptr [tempL0]
		movq	mm5, qword ptr [tempL1]
		paddw	mm7, mm5
		paddw	mm5, mm1
		paddw	mm1, mm2
		paddw	mm5, mm1
		paddw	mm7, two64
		paddw	mm5, four64
		psrlw	mm7, 2
		psrlw	mm5, 3
		paddw	mm6, mm1
		paddw	mm1, two64
		paddw	mm6, four64
		psrlw	mm1, 2
		psrlw	mm6, 3
		movq	mm2, qword ptr [tempR0]
		psubw	mm5, mm7
		pand	mm5, mm4
		paddw	mm7, mm5
		psubw	mm7, mm2
		pand	mm7, mm0
		paddw	mm2, mm7
		movq	qword ptr [tempR0], mm2
		movq	mm2, qword ptr [tempR1]
		psubw	mm1, mm2
		pand	mm1, mm4
		pand	mm1, mm0
		paddw	mm2, mm1
		movq	qword ptr [tempR1], mm2
		movq	mm2, qword ptr [tempR2]
		psubw	mm6, mm2
		pand	mm6, mm4
		pand	mm6, mm0
		paddw	mm2, mm6
		movq	qword ptr [tempR2], mm2
		movq	mm1, qword ptr [tempL2]
		movq	mm4, qword ptr [tempR0]
		movq	mm5, qword ptr [tempR1]
		movq	mm6, qword ptr [tempR2]
		movq	mm2, qword ptr [temp1]
		movq	mm3, qword ptr [temp2]
		add	ecx, edx
		packuswb mm1, mm0
		packuswb mm2, mm0
		packuswb mm3, mm0
		packuswb mm4, mm0
		packuswb mm5, mm0
		packuswb mm6, mm0
		movd	dword ptr [ecx], mm1
		movd	dword ptr [ecx+edx], mm2
		movd	dword ptr [ecx+edx*2], mm3
		movd	dword ptr [esi], mm4
		movd	dword ptr [esi+edx], mm5
		movd	dword ptr [esi+edx*2], mm6
		emms
	}
}


void filter_luma_hor_lt4_mmx(uchar *src,long alpha, long beta, long C0, long stride)
{
	long long tempL2;
	long long tempL1;
	long long tempL0;
	long long tempR2;
	long long tempR1;
	long long tempR0;	

	__asm {
		mov     ecx, src
		mov		edx, stride
		add		ecx, edx
		lea	eax, [edx+edx*2]
		add	eax, ecx
		pxor	mm7, mm7
		movd	mm1, dword ptr [ecx]
		movd	mm2, dword ptr [ecx+edx]
		movd	mm3, dword ptr [ecx+edx*2]
		movd	mm4, dword ptr [eax]
		movd	mm5, dword ptr [eax+edx]
		movd	mm6, dword ptr [eax+edx*2]

		punpcklbw mm1, mm7
		punpcklbw mm2, mm7
		punpcklbw mm3, mm7
		punpcklbw mm4, mm7
		punpcklbw mm5, mm7
		punpcklbw mm6, mm7
		movq	qword ptr [tempL2], mm1
		movq	qword ptr [tempL1], mm2
		movq	qword ptr [tempL0], mm3
		movq	qword ptr [tempR0], mm4
		movq	qword ptr [tempR1], mm5
		movq	qword ptr [tempR2], mm6
		movq	mm0, mm2
		movq	mm1, mm5
		psubw	mm0, mm3
		psubw	mm1, mm4
		movq	mm6, mm0
		movq	mm7, mm1
		psraw	mm0, 0Fh
		psraw	mm1, 0Fh
		pxor	mm6, mm0
		pxor	mm7, mm1
		psubw	mm6, mm0
		psubw	mm7, mm1
		movd	mm0, beta
		pshufw	mm0, mm0, 0
		movq	mm1, mm0
		pcmpgtw	mm0, mm6
		pcmpgtw	mm1, mm7
		pand	mm0, mm1

		movd	mm7, alpha
		pshufw	mm7, mm7, 0
		movq	mm1, mm3
		psubw	mm1, mm4
		movq	mm6, mm1
		psraw	mm6, 0fh
		pxor	mm1, mm6
		psubw	mm1, mm6
		pcmpgtw	mm7, mm1
		pand	mm0, mm7

		movq	mm6, mm3
		pavgw	mm6, mm4
		movq	mm7, mm6
		paddw	mm6, qword ptr [tempL2]
		paddw	mm7, qword ptr [tempR2]
		psubw	mm6, mm2
		psubw	mm7, mm5
		psubw	mm6, mm2
		psubw	mm7, mm5
		psraw	mm6, 1
		psraw	mm7, 1
		movq	mm1, mm4
		psubw	mm1, mm3
		psubw	mm2, mm5
		psllw	mm1, 2
		paddw	mm2, mm1
		paddw	mm2, four64
		psraw	mm2, 3
		psubw	mm3, qword ptr [tempL2]
		psubw	mm4, qword ptr [tempR2]
		movq	mm1, mm3
		movq	mm5, mm4
		psraw	mm3, 0fh
		psraw	mm4, 0fh
		pxor	mm1, mm3
		pxor	mm5, mm4
		psubw	mm1, mm3
		psubw	mm5, mm4
		movd	mm3, beta
		pshufw	mm3, mm3, 0
		movq	mm4, mm3
		pcmpgtw	mm3, mm1
		pcmpgtw	mm4, mm5
		movd	mm1, C0
		pxor	mm5, mm5
		pshufw	mm1, mm1, 0
		psubw	mm5, mm1
		pmaxsw	mm6, mm5
		pmaxsw	mm7, mm5
		pminsw	mm6, mm1
		pminsw	mm7, mm1
		pand	mm6, mm3
		pand	mm7, mm4
		pand	mm6, mm0
		pand	mm7, mm0
		paddw	mm3, mm4
		paddw	mm5, mm3
		psubw	mm1, mm3
		pmaxsw	mm2, mm5
		pminsw	mm1, mm2
		pand	mm1, mm0
		paddw	mm6, qword ptr [tempL1]
		movq	mm3, qword ptr [tempL0]
		movq	mm4, qword ptr [tempR0]
		paddw	mm7, qword ptr [tempR1]
		add	ecx, edx
		lea	eax, [edx+edx*2]
		packuswb mm6, mm6
		paddw	mm3, mm1
		psubw	mm4, mm1
		packuswb mm3, mm3
		packuswb mm4, mm4
		packuswb mm7, mm7
		movd	dword ptr [ecx], mm6
		movd	dword ptr [ecx+edx], mm3
		movd	dword ptr [ecx+edx*2], mm4
		movd	dword ptr [ecx+eax], mm7
		emms
	}
}

void filter_chroma_ver_eq4_mmx(uchar *u_src, uchar *v_src, long alpha,long beta,long width)
{
	__asm {
		mov     edx, width
		mov		edi, u_src
		mov		esi, v_src
		movd	mm1, dword ptr [edi]
		movd	mm4, dword ptr [edi+4]
		movd	mm2, dword ptr [edi+edx]
		movd	mm7, dword ptr [edi+edx+4]
		movd	mm3, dword ptr [esi]
		movd	mm5, dword ptr [esi+4]
		movd	mm6, dword ptr [esi+edx]
		movd	mm0, dword ptr [esi+edx+4]

		punpcklbw mm1, mm2
		punpcklbw mm4, mm7
		movq	mm2, mm1
		pxor	mm7, mm7
		punpcklbw mm3, mm6
		punpcklbw mm5, mm0
		movq	mm6, mm4
		punpcklwd mm1, mm3
		punpckhwd mm2, mm3
		punpcklwd mm4, mm5
		punpckhwd mm6, mm5
		movq	mm3, mm2
		punpckhbw mm1, mm7
		movq	mm5, mm4
		punpcklbw mm2, mm7
		punpckhbw mm3, mm7
		punpcklbw mm4, mm7
		punpckhbw mm5, mm7
		punpcklbw mm6, mm7

		movq	mm0, mm2
		movq	mm1, mm5
		psubw	mm0, mm3
		psubw	mm1, mm4
		movq	mm6, mm0
		movq	mm7, mm1
		psraw	mm0, 0Fh
		psraw	mm1, 0Fh
		pxor	mm6, mm0
		pxor	mm7, mm1
		psubw	mm6, mm0
		psubw	mm7, mm1
		movd	mm0, beta
		pshufw	mm0, mm0, 0
		movq	mm1, mm0
		pcmpgtw	mm0, mm6
		pcmpgtw	mm1, mm7
		pand	mm0, mm1

		movd	mm7, alpha
		pshufw	mm7, mm7, 0
		movq	mm1, mm3
		psubw	mm1, mm4
		movq	mm6, mm1
		psraw	mm6, 0Fh
		pxor	mm1, mm6
		psubw	mm1, mm6
		pcmpgtw	mm7, mm1
		pand	mm0, mm7

		movq	mm7, mm2
		paddw	mm7, mm5
		paddw	mm7, two64
		movq	mm1, mm2
		movq	mm6, mm5
		paddw	mm1, mm7
		paddw	mm6, mm7
		paddw	mm1, mm3
		paddw	mm6, mm4
		psraw	mm1, 2
		psraw	mm6, 2
		psubw	mm1, mm3
		psubw	mm6, mm4
		pand	mm1, mm0
		pand	mm6, mm0
		paddw	mm3, mm1			; L0
		paddw	mm4, mm6			; R0

		pextrw	eax, mm3, 0
		pextrw	ecx, mm4, 0
		mov		byte ptr [edi+3], al
		mov		byte ptr [edi+4], cl
		pextrw	eax, mm3, 1
		pextrw	ecx, mm4, 1
		mov		byte ptr [edi+edx+3], al
		mov		byte ptr [edi+edx+4], cl

		pextrw	eax, mm3, 2
		pextrw	ecx, mm4, 2
		mov		byte ptr [esi+3], al
		mov		byte ptr [esi+4], cl
		pextrw	eax, mm3, 3
		pextrw	ecx, mm4, 3
		mov		byte ptr [esi+edx+3], al
		mov		byte ptr [esi+edx+4], cl
		emms
	}
}

void filter_chroma_ver_lt4_mmx(uchar *u_src, uchar *v_src, long alpha,long beta,long C0, long width)
{
	__asm {
		mov		eax, C0
		add		eax, 1
		mov		C0, eax
		mov     edx, width
		mov		edi, u_src
		mov		esi, v_src
		movd	mm1, dword ptr [edi]
		movd	mm4, dword ptr [edi+4]
		movd	mm2, dword ptr [edi+edx]
		movd	mm7, dword ptr [edi+edx+4]
		movd	mm3, dword ptr [esi]
		movd	mm5, dword ptr [esi+4]
		movd	mm6, dword ptr [esi+edx]
		movd	mm0, dword ptr [esi+edx+4]

		punpcklbw mm1, mm2
		punpcklbw mm4, mm7
		movq	mm2, mm1
		pxor	mm7, mm7
		punpcklbw mm3, mm6
		punpcklbw mm5, mm0
		movq	mm6, mm4
		punpcklwd mm1, mm3
		punpckhwd mm2, mm3
		punpcklwd mm4, mm5
		punpckhwd mm6, mm5
		movq	mm3, mm2
		punpckhbw mm1, mm7
		movq	mm5, mm4
		punpcklbw mm2, mm7
		punpckhbw mm3, mm7
		punpcklbw mm4, mm7
		punpckhbw mm5, mm7
		punpcklbw mm6, mm7

		movq	mm0, mm2
		movq	mm1, mm5
		psubw	mm0, mm3
		psubw	mm1, mm4
		movq	mm6, mm0
		movq	mm7, mm1
		psraw	mm0, 0Fh
		psraw	mm1, 0Fh
		pxor	mm6, mm0
		pxor	mm7, mm1
		psubw	mm6, mm0
		psubw	mm7, mm1
		movd	mm0, beta
		pshufw	mm0, mm0, 0
		movq	mm1, mm0
		pcmpgtw	mm0, mm6
		pcmpgtw	mm1, mm7
		pand	mm0, mm1

		movd	mm7, alpha
		pshufw	mm7, mm7, 0
		movq	mm1, mm3
		psubw	mm1, mm4
		movq	mm6, mm1
		psraw	mm6, 0Fh
		pxor	mm1, mm6
		psubw	mm1, mm6
		pcmpgtw	mm7, mm1
		pand	mm0, mm7

		movq	mm1, mm4
		psubw	mm1, mm3
		psubw	mm2, mm5
		psllw	mm1, 2
		paddw	mm2, mm1
		paddw	mm2, four64
		psraw	mm2, 3
		movd	mm1, C0
		pxor	mm5, mm5
		pshufw	mm1, mm1, 0
		psubw	mm5, mm1
		pmaxsw	mm2, mm5
		pminsw	mm2, mm1				; dif

		pxor	mm7, mm7
		movq	mm1, mm3
		movq	mm5, mm4
		paddw	mm1, mm2
		psubw	mm5, mm2
		packuswb  mm1, mm1
		packuswb  mm5, mm5
		punpcklbw mm1, mm7
		punpcklbw mm5, mm7

		psubw	mm1, mm3
		psubw	mm5, mm4
		pand	mm1, mm0
		pand	mm5, mm0
		paddw	mm3, mm1				; L0
		paddw	mm4, mm5				; R0

		pextrw	eax, mm3, 0
		pextrw	ecx, mm4, 0
		mov		byte ptr [edi+3], al
		mov		byte ptr [edi+4], cl
		pextrw	eax, mm3, 1
		pextrw	ecx, mm4, 1
		mov		byte ptr [edi+edx+3], al
		mov		byte ptr [edi+edx+4], cl

		pextrw	eax, mm3, 2
		pextrw	ecx, mm4, 2
		mov		byte ptr [esi+3], al
		mov		byte ptr [esi+4], cl
		pextrw	eax, mm3, 3
		pextrw	ecx, mm4, 3
		mov		byte ptr [esi+edx+3], al
		mov		byte ptr [esi+edx+4], cl
		emms
	}
}

void filter_chroma_hor_eq4_mmx(uchar *u_src, uchar *v_src, long alpha,long beta,long stride)
{
	__asm {
		mov     edx, stride
		lea		eax, [edx+edx*2]
		mov		edi, u_src
		mov		esi, v_src
		movd	mm2, dword ptr [edi]
		movd	mm3, dword ptr [edi+edx]
		movd	mm4, dword ptr [edi+edx*2]
		movd	mm5, dword ptr [edi+eax]
		movd	mm1, dword ptr [esi]
		movd	mm6, dword ptr [esi+edx]
		movd	mm7, dword ptr [esi+edx*2]
		movd	mm0, dword ptr [esi+eax]

		punpcklwd mm2, mm1
		punpcklwd mm3, mm6
		punpcklwd mm4, mm7
		punpcklwd mm5, mm0
		pxor	mm7, mm7
		punpcklbw mm2, mm7			; L1
		punpcklbw mm3, mm7			; L0
		punpcklbw mm4, mm7			; R0
		punpcklbw mm5, mm7			; R1

		movq	mm0, mm2
		movq	mm1, mm5
		psubw	mm0, mm3
		psubw	mm1, mm4
		movq	mm6, mm0
		movq	mm7, mm1
		psraw	mm0, 0Fh
		psraw	mm1, 0Fh
		pxor	mm6, mm0
		pxor	mm7, mm1
		psubw	mm6, mm0
		psubw	mm7, mm1
		movd	mm0, beta
		pshufw	mm0, mm0, 0
		movq	mm1, mm0
		pcmpgtw	mm0, mm6
		pcmpgtw	mm1, mm7
		pand	mm0, mm1

		movd	mm7, alpha
		pshufw	mm7, mm7, 0
		movq	mm1, mm3
		psubw	mm1, mm4
		movq	mm6, mm1
		psraw	mm6, 0Fh
		pxor	mm1, mm6
		psubw	mm1, mm6
		pcmpgtw	mm7, mm1
		pand	mm0, mm7

		movq	mm7, mm2
		paddw	mm7, mm5
		paddw	mm7, two64
		movq	mm1, mm2
		movq	mm6, mm5
		paddw	mm1, mm7
		paddw	mm6, mm7
		paddw	mm1, mm3
		paddw	mm6, mm4
		psraw	mm1, 2
		psraw	mm6, 2
		psubw	mm1, mm3
		psubw	mm6, mm4
		pand	mm1, mm0
		pand	mm6, mm0
		paddw	mm3, mm1			; L0
		paddw	mm4, mm6			; R0

		pextrw	eax, mm3, 0
		pextrw	ecx, mm3, 1
		mov		byte ptr [edi+edx], al
		mov		byte ptr [edi+edx+1], cl
		pextrw	eax, mm4, 0
		pextrw	ecx, mm4, 1
		mov		byte ptr [edi+edx*2], al
		mov		byte ptr [edi+edx*2+1], cl

		pextrw	eax, mm3, 2
		pextrw	ecx, mm3, 3
		mov		byte ptr [esi+edx], al
		mov		byte ptr [esi+edx+1], cl
		pextrw	eax, mm4, 2
		pextrw	ecx, mm4, 3
		mov		byte ptr [esi+edx*2], al
		mov		byte ptr [esi+edx*2+1], cl
		emms
	}
}

void filter_chroma_hor_lt4_mmx(uchar *u_src, uchar *v_src, long alpha,long beta,long C0, long stride)
{
	__asm {
		mov		eax, C0
		add		eax, 1
		mov		C0, eax
		mov     edx, stride
		lea		eax, [edx+edx*2]
		mov		edi, u_src
		mov		esi, v_src
		movd	mm2, dword ptr [edi]
		movd	mm3, dword ptr [edi+edx]
		movd	mm4, dword ptr [edi+edx*2]
		movd	mm5, dword ptr [edi+eax]
		movd	mm1, dword ptr [esi]
		movd	mm6, dword ptr [esi+edx]
		movd	mm7, dword ptr [esi+edx*2]
		movd	mm0, dword ptr [esi+eax]

		punpcklwd mm2, mm1
		punpcklwd mm3, mm6
		punpcklwd mm4, mm7
		punpcklwd mm5, mm0
		pxor	mm7, mm7
		punpcklbw mm2, mm7			; L1
		punpcklbw mm3, mm7			; L0
		punpcklbw mm4, mm7			; R0
		punpcklbw mm5, mm7			; R1

		movq	mm0, mm2
		movq	mm1, mm5
		psubw	mm0, mm3
		psubw	mm1, mm4
		movq	mm6, mm0
		movq	mm7, mm1
		psraw	mm0, 0Fh
		psraw	mm1, 0Fh
		pxor	mm6, mm0
		pxor	mm7, mm1
		psubw	mm6, mm0
		psubw	mm7, mm1
		movd	mm0, beta
		pshufw	mm0, mm0, 0
		movq	mm1, mm0
		pcmpgtw	mm0, mm6
		pcmpgtw	mm1, mm7
		pand	mm0, mm1

		movd	mm7, alpha
		pshufw	mm7, mm7, 0
		movq	mm1, mm3
		psubw	mm1, mm4
		movq	mm6, mm1
		psraw	mm6, 0Fh
		pxor	mm1, mm6
		psubw	mm1, mm6
		pcmpgtw	mm7, mm1
		pand	mm0, mm7

		movq	mm1, mm4
		psubw	mm1, mm3
		psubw	mm2, mm5
		psllw	mm1, 2
		paddw	mm2, mm1
		paddw	mm2, four64
		psraw	mm2, 3
		movd	mm1, C0
		pxor	mm5, mm5
		pshufw	mm1, mm1, 0
		psubw	mm5, mm1
		pmaxsw	mm2, mm5
		pminsw	mm2, mm1				; dif

		pxor	mm7, mm7
		movq	mm1, mm3
		movq	mm5, mm4
		paddw	mm1, mm2
		psubw	mm5, mm2
		packuswb  mm1, mm7
		packuswb  mm5, mm7
		punpcklbw mm1, mm7
		punpcklbw mm5, mm7

		psubw	mm1, mm3
		psubw	mm5, mm4
		pand	mm1, mm0
		pand	mm5, mm0
		paddw	mm3, mm1				; L0
		paddw	mm4, mm5				; R0

		pextrw	eax, mm3, 0
		pextrw	ecx, mm3, 1
		mov		byte ptr [edi+edx], al
		mov		byte ptr [edi+edx+1], cl
		pextrw	eax, mm4, 0
		pextrw	ecx, mm4, 1
		mov		byte ptr [edi+edx*2], al
		mov		byte ptr [edi+edx*2+1], cl

		pextrw	eax, mm3, 2
		pextrw	ecx, mm3, 3
		mov		byte ptr [esi+edx], al
		mov		byte ptr [esi+edx+1], cl
		pextrw	eax, mm4, 2
		pextrw	ecx, mm4, 3
		mov		byte ptr [esi+edx*2], al
		mov		byte ptr [esi+edx*2+1], cl
		emms
	}
}
*/


