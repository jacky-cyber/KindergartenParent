
#include <assert.h>
#include <memory.h>

#include "global.h"
#include "common.h"
#include "macroblock.h"
#include "bitstream.h"
#include "vlc.h"
#include "access.h"
#include "syntax.h"
#include "intrapred.h"
#include "recon.h"
#include "interpolate.h"



static void decode_intra_mb_16x16(decoder_t *decoder,macroblock_t *cur_mb)
{
	intra_pred_luma_16x16( decoder,cur_mb);
	intra_pred_chroma(decoder,cur_mb,0);
	intra_pred_chroma(decoder,cur_mb,1);
	recon_mb_p(decoder);	
}

static void decode_intra_mb_4x4(decoder_t *decoder,macroblock_t *cur_mb)
{
	long i, ii,jj;
	long ioff,joff,i4,j4;
	picture_t *pic = decoder->cur_pic;

	for( i=0;i<16;i++ ){
	    intra_pred_luma_4x4(decoder,cur_mb,i);
		i4 = submb_pos_x[i]<<2;
		j4 = submb_pos_y[i]<<2;
		ioff = i4 + (decoder->mb_x<<4);
		joff = j4 + (decoder->mb_y<<4);
		for( jj=0;jj<4;jj++ ){
			for( ii=0;ii<4;ii++ ){
				pic->img_y[joff + jj][ioff + ii] = CLIP1(decoder->cof[j4+jj][i4+ii] + decoder->mpr[j4+jj][i4+ii]);
				decoder->cof[j4+jj][i4+ii] = 0;
			}
		}
	}
    intra_pred_chroma(decoder,cur_mb,0);
	intra_pred_chroma(decoder,cur_mb,1);
	recon_chroma_for_mb_4x4(decoder);
}

static void decode_inter_mb_16x16(decoder_t *decoder,macroblock_t *cur_mb)
{
    long  x_pos,y_pos;
	long  ref;
	long  sub_idx = decoder->cur_mb_nr<<4;
	picture_t *pic = decoder->cur_pic;
	picture_t *ref_pic;

	x_pos = (decoder->mb_x<<6) + pic->front_mv[sub_idx].x;
	y_pos = (decoder->mb_y<<6) + pic->front_mv[sub_idx].y;
	ref = pic->front_ref_idx[sub_idx];
	ref_pic = decoder->dpb->listX[ref];

	get_block_16x16_luma(ref_pic->img_y,x_pos,y_pos,decoder->mpr,pic->width,pic->height);

	y_pos += ref_pic->chroma_vector_adjustment;

	get_block_8x8_chroma(ref_pic->img_u,x_pos,y_pos,decoder->mpr,pic->width_cr,pic->height_cr,0);
	get_block_8x8_chroma(ref_pic->img_v,x_pos,y_pos,decoder->mpr,pic->width_cr,pic->height_cr,1);	
}

static void decode_inter_mb_16x8(decoder_t *decoder,macroblock_t *cur_mb,long k)
{
	// TODO
	long x_pos,y_pos;
	long ref;
	long sub_idx = k<<3;
	picture_t *pic = decoder->cur_pic;
	picture_t *ref_pic;

	x_pos = (decoder->mb_x<<6) + pic->front_mv[sub_idx].x;
	y_pos = (decoder->mb_y<<6) + (k<<5) + pic->front_mv[sub_idx].y;
	ref = pic->front_ref_idx[sub_idx];
	ref_pic = decoder->dpb->listX[ref];

}

static void decode_inter_mb_8x16(decoder_t *decoder,macroblock_t *cur_mb,long k)
{
	// TODO
}

static void decoder_inter_mb_8x4(decoder_t *decoder,macroblock_t *cur_mb,long k)
{
	// TODO
}

static void decoder_inter_mb_4x8(decoder_t *decoder,macroblock_t *cur_mb,long k)
{
	// TODO
}

static void decode_inter_mb_8x8(decoder_t *decoder,macroblock_t *cur_mb,long k)
{
	long x_pos,y_pos;
	long ref;
	long sub_idx = (decoder->cur_mb_nr<<4) + (k<<2);
	picture_t *pic = decoder->cur_pic;
	picture_t *ref_pic;

	x_pos = (decoder->mb_x<<6) + ((k&0x1)<<5) + pic->front_mv[sub_idx].x;
	y_pos = (decoder->mb_y<<6) + ((k&0x2)<<4) + pic->front_mv[sub_idx].y;
	ref = pic->front_ref_idx[sub_idx];
	ref_pic = decoder->dpb->listX[ref];

	get_block_8x8_luma(ref_pic->img_y,x_pos,y_pos,decoder->mpr,pic->width,pic->height,k);
	y_pos += ref_pic->chroma_vector_adjustment;

	get_block_4x4_chroma(ref_pic->img_u,x_pos,y_pos,decoder->mpr,pic->width_cr,pic->height_cr,0,k);
	get_block_4x4_chroma(ref_pic->img_v,x_pos,y_pos,decoder->mpr,pic->width_cr,pic->height_cr,1,k);	
}

static void decode_inter_mb_4x4(decoder_t *decoder,macroblock_t *cur_mb,long k)
{
	long x_pos,y_pos;
	long ref;
	long sub_idx = (decoder->cur_mb_nr<<4) + k;
	picture_t *pic = decoder->cur_pic;
	picture_t *ref_pic;

	x_pos = (decoder->mb_x<<6) + (submb_pos_x[k]<<4) + pic->front_mv[sub_idx].x;
	y_pos = (decoder->mb_y<<6) + (submb_pos_y[k]<<4) + pic->front_mv[sub_idx].y;
	ref = pic->front_ref_idx[sub_idx];
	ref_pic = decoder->dpb->listX[ref];

	get_block_4x4_luma(ref_pic->img_y,x_pos,y_pos,decoder->mpr,pic->width,pic->height,k);

	y_pos += ref_pic->chroma_vector_adjustment;
	get_block_2x2_chroma_c(ref_pic->img_u,x_pos,y_pos,decoder->mpr,pic->width_cr,pic->height_cr,0,k);
	get_block_2x2_chroma_c(ref_pic->img_v,x_pos,y_pos,decoder->mpr,pic->width_cr,pic->height_cr,1,k);	
}


static void   decode_inter_mb(decoder_t *decoder,macroblock_t *cur_mb)
{
	long k,j;
	if( cur_mb->mb_type==0 || cur_mb->mb_type==1 ){
		decode_inter_mb_16x16(decoder,cur_mb);
	} else if( cur_mb->mb_type==2 || cur_mb->mb_type==3 ){
		for( k=0;k<4;k++ ){
			decode_inter_mb_8x8(decoder,cur_mb,k);
		}
	} else if( cur_mb->mb_type == P8x8 ){
		for( k=0;k<4;k++ ){
			if( cur_mb->b8mode[k] == 4 ){
				decode_inter_mb_8x8(decoder,cur_mb,k);
			} else {
				for( j=k*4;j<k*4+4;j++ ){
				    decode_inter_mb_4x4(decoder,cur_mb,j);
				}
			}
		}
	}
	recon_mb_p(decoder);	
}

void   init_macroblock(decoder_t *decoder)
{
	long mb_nr = decoder->cur_mb_nr;
	slice_t *slice = decoder->cur_slice;
	macroblock_t * cur_mb = &decoder->mb_date[mb_nr];	

	get_mb_pos(decoder->mb_access,mb_nr,&decoder->mb_x,&decoder->mb_y);
	cur_mb->slice_nr = decoder->cur_slice_nr;
	cur_mb->qp = decoder->qp;
	cur_mb->c_ipred_mode = INTRA_CHROMA_PRED_MODE_DC;
	cur_mb->mb_type = 0;
	cur_mb->delta_qp = 0;
	cur_mb->cbp = 0;
	cur_mb->cbp_bits = 0;
	cur_mb->cbp_blk = 0;
	cur_mb->disable_deblock_filter_idc = slice->disable_deblocking_filter_idc;
	cur_mb->alpha_c0_offset = slice->slice_alpha_c0_offset_div2 * 2;
	cur_mb->beta_offset = slice->slice_beta_offset_div2 * 2;    
}

void init_coef_nnz(decoder_t *decoder)
{
	mb_access_t *access = decoder->mb_access;
	long mbnr = decoder->cur_mb_nr;
	long cur_sub_idx = mbnr << 4;

	decoder->nnz_offset_a[0] = get_neighbour4x4(access,mbnr,0,0);
	decoder->nnz_offset_a[2] = decoder->nnz_offset_a[0] + 2;
	decoder->nnz_offset_a[8] = decoder->nnz_offset_a[0] + 8;
	decoder->nnz_offset_a[10] = decoder->nnz_offset_a[0] + 10;
	decoder->nnz_offset_a[1] = cur_sub_idx + submb_4x4[0][1].sub_idx;
	decoder->nnz_offset_a[3] = cur_sub_idx + submb_4x4[0][3].sub_idx;
	decoder->nnz_offset_a[4] = cur_sub_idx + submb_4x4[0][4].sub_idx;
	decoder->nnz_offset_a[5] = cur_sub_idx + submb_4x4[0][5].sub_idx;
	decoder->nnz_offset_a[6] = cur_sub_idx + submb_4x4[0][6].sub_idx;
	decoder->nnz_offset_a[7] = cur_sub_idx + submb_4x4[0][7].sub_idx;
	decoder->nnz_offset_a[9] = cur_sub_idx + submb_4x4[0][9].sub_idx;
	decoder->nnz_offset_a[11] = cur_sub_idx + submb_4x4[0][11].sub_idx;
	decoder->nnz_offset_a[12] = cur_sub_idx + submb_4x4[0][12].sub_idx;
	decoder->nnz_offset_a[13] = cur_sub_idx + submb_4x4[0][13].sub_idx;
	decoder->nnz_offset_a[14] = cur_sub_idx + submb_4x4[0][14].sub_idx;
	decoder->nnz_offset_a[15] = cur_sub_idx + submb_4x4[0][15].sub_idx;
	decoder->nnz_offset_b[0] = get_neighbour4x4(access,mbnr,0,1);
	decoder->nnz_offset_b[1] = decoder->nnz_offset_b[0] + 1;
	decoder->nnz_offset_b[4] = decoder->nnz_offset_b[0] + 4;
	decoder->nnz_offset_b[5] = decoder->nnz_offset_b[0] + 5;
	decoder->nnz_offset_b[2] = cur_sub_idx + submb_4x4[1][2].sub_idx;
	decoder->nnz_offset_b[3] = cur_sub_idx + submb_4x4[1][3].sub_idx;
	decoder->nnz_offset_b[6] = cur_sub_idx + submb_4x4[1][6].sub_idx;
	decoder->nnz_offset_b[7] = cur_sub_idx + submb_4x4[1][7].sub_idx;
	decoder->nnz_offset_b[8] = cur_sub_idx + submb_4x4[1][8].sub_idx;
	decoder->nnz_offset_b[9] = cur_sub_idx + submb_4x4[1][9].sub_idx;
	decoder->nnz_offset_b[10] = cur_sub_idx + submb_4x4[1][10].sub_idx;
	decoder->nnz_offset_b[11] = cur_sub_idx + submb_4x4[1][11].sub_idx;
	decoder->nnz_offset_b[12] = cur_sub_idx + submb_4x4[1][12].sub_idx;
	decoder->nnz_offset_b[13] = cur_sub_idx + submb_4x4[1][13].sub_idx;
	decoder->nnz_offset_b[14] = cur_sub_idx + submb_4x4[1][14].sub_idx;
	decoder->nnz_offset_b[15] = cur_sub_idx + submb_4x4[1][15].sub_idx;	
}

void  read_macroblock(decoder_t *decoder,bs_t *bs)
{
	macroblock_t *cur_mb = &decoder->mb_date[decoder->cur_mb_nr];
	slice_t *slice = decoder->cur_slice;

    read_mb_type_cavlc(decoder,cur_mb,bs);
	interpret_mb_type(slice->slice_type,cur_mb);

	if( cur_mb->mb_type == P8x8 ){
		read_submb_type_cavlc(decoder,cur_mb,bs);
	}
	if( slice->slice_type==P_SLICE && cur_mb->mb_type==0 ){
		get_pskip_mv(decoder,cur_mb);
		return;
	}

	if( (cur_mb->mb_type==I4MB) || (cur_mb->mb_type==I16MB) ){
		read_ipred_type_cavlc(decoder,cur_mb,bs);
		if( slice->slice_type != I_SLICE ){
			mv_t *mv_array = &decoder->cur_pic->front_mv[decoder->cur_mb_nr<<4];
			long *idx_array = &decoder->cur_pic->front_ref_idx[decoder->cur_mb_nr<<4];
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;
			*idx_array++ = -1;			
			memset(mv_array,0,sizeof(mv_t)*16);
		}
	} else {
		read_inter_pred_ref_idx_cavlc(decoder,cur_mb,bs);
		read_inter_pred_mvd_calvc(decoder,cur_mb,bs);
	}
	init_coef_nnz(decoder);

	read_coeffs_cavlc(decoder,cur_mb,bs);
}

void decode_macroblock(decoder_t *decoder)
{
	macroblock_t *cur_mb = &decoder->mb_date[decoder->cur_mb_nr];
	
	if( cur_mb->mb_type == I16MB ){
		decode_intra_mb_16x16(decoder,cur_mb);
		return;
	}

	if( cur_mb->mb_type == I4MB ){
		decode_intra_mb_4x4(decoder,cur_mb);
		return;
	}

	decode_inter_mb(decoder,cur_mb);
	return;
}

long exit_macroblock(decoder_t *decoder)
{
	decoder->num_dec_mb ++;

	if( decoder->num_dec_mb == decoder->pic_size_in_mbs ){
		decoder->next_header = SOP;
		return 1;
	} 
	
	decoder->cur_mb_nr ++;
	
	return 0;
}


