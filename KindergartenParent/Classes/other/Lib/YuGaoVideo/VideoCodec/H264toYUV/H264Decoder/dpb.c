
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <limits.h>
#include <stdio.h>

#include "global.h"
#include "common.h"
#include "h264dec.h"
#include "dpb.h"




#define DPB_PRE_ALLOC_SIZE 2



static int compare_pic_by_pic_num_desc( const void *arg1, const void *arg2 )
{
	if ( ((picture_t*)arg1)->pic_num < ((picture_t*)arg2)->pic_num)
		return 1;
	if ( ((picture_t*)arg1)->pic_num > ((picture_t*)arg1)->pic_num)
		return -1;
	else
		return 0;
}

static int compare_pic_by_lt_pic_num_asc( const void *arg1, const void *arg2 )
{
	if ( ((picture_t*)arg1)->long_term_frame_idx < ((picture_t*)arg2)->long_term_frame_idx)
		return 1;
	if ( ((picture_t*)arg1)->long_term_frame_idx > ((picture_t*)arg1)->long_term_frame_idx)
		return -1;
	else
		return 0;
}


static long  get_dpb_size(decoder_t *decoder)
{
	long size = 0;
	long pic_size = decoder->frame_size_in_mbs * 364;
	switch (decoder->active_sps->level_idc)
	{
	case 10:
		size = 152064;
		break;
	case 11:
		size = 345600;
		break;
	case 12:
		size = 912384;
		break;
	case 13:
		size = 912384;
		break;
	case 20:
		size = 912384;
		break;
	case 21:
		size = 1824768;
		break;
	case 22:
		size = 3110400;
		break;
	case 30:
		size = 3110400;
		break;
	case 31:
		size = 6912000;
		break;
	case 32:
		size = 7864320;
		break;
	case 40:
		size = 12582912;
		break;
	case 41:
		size = 12582912;
		break;
	case 42:
		size = 12582912;
		break;
	case 50:
		size = 42393600;
		break;
	case 51:
		size = 70778880;
		break;
	default:
		size = 70778880;
		break;
	}
	return min(size/pic_size,16);
}

static void idr_memory_manager(decoder_t *decoder,dpb_t *dpb)
{
	long i;
	long max_poc = 0;
	if( decoder->cur_slice->no_output_of_prior_pics_flag ){
		for( i=0;i<dpb->max_size;i++ ){
			if( dpb->pic_closet[i] ){
				dpb->pic_closet[i]->is_output = 1;
				dpb->pic_closet[i]->is_reference = 0;
				dpb->pic_closet[i]->is_long_term = 0;
			}
		}
		dpb->last_output_poc = -2;
	} else {
		for( i=0;i<dpb->max_size;i++ ){
			if( dpb->pic_closet[i] ){
				dpb->pic_closet[i]->is_reference = 0;
				dpb->pic_closet[i]->is_long_term = 0;
				if( max_poc < dpb->pic_closet[i]->frame_poc ){
					max_poc = dpb->pic_closet[i]->frame_poc ;
				}				
			}
		}

		for( i=0;i<dpb->max_size;i++ ){
			if( dpb->pic_closet[i] ){
				dpb->pic_closet[i]->frame_poc -= max_poc + 2;
			}
		}
		dpb->last_output_poc = - max_poc - 2;
	}

	if( decoder->cur_slice->long_term_reference_flag ){
		decoder->cur_pic->is_long_term = 1;
		decoder->cur_pic->long_term_frame_idx = 0;
	} else {
		decoder->cur_pic->is_long_term = 0;
		decoder->cur_pic->long_term_frame_idx = -1;
	}
}

static void adaptive_memory_manager(decoder_t *decoder,dpb_t *dpb)
{
	assert( 0==1 );
}

static void sliding_memory_manager(decoder_t *decoder,dpb_t *dpb)
{
    long i,k;
	long ref_counter = 0;
	long frame_num_wrap_min = INT_MAX;
	for( i=0;i<dpb->max_size;i++ ){
		if( dpb->pic_closet[i] && dpb->pic_closet[i]->is_reference ){
			ref_counter ++;
		}
	}
	if( ref_counter == dpb->num_ref_frames ){
		for ( i=0;i<dpb->max_size;i++ ){
			if( dpb->pic_closet[i] ){
				if( dpb->pic_closet[i]->is_reference && (!dpb->pic_closet[i]->is_long_term) && 
					(dpb->pic_closet[i]->frame_num_wrap < frame_num_wrap_min ) ){
					k = i;
					frame_num_wrap_min = dpb->pic_closet[i]->frame_num_wrap;
				}
			}
		}
		dpb->pic_closet[k]->is_reference = 0;
	}
}


void init_dpb(decoder_t *decoder,dpb_t *dpb)
{
	long i;

	dpb->num_ref_frames = decoder->active_sps->num_ref_frames;
	dpb->dpb_size = get_dpb_size(decoder);
	dpb->max_size = dpb->dpb_size + dpb->num_ref_frames;

	dpb->pic_closet = (picture_t**)align_malloc(sizeof(picture_t*)*dpb->max_size);

	for( i=0;i<dpb->max_size;i++ ){
		dpb->pic_closet[i] = 0;
	}

	for( i=0;i<DPB_PRE_ALLOC_SIZE;i++ ){
		dpb->pic_closet[i] = alloc_picture(decoder->width,decoder->height);
		dpb->pic_closet[i]->is_output = 1;
		dpb->pic_closet[i]->is_reference = 0;
	}

	for( i=0;i<MAX_LIST_SIZE;i++ ){
		dpb->listX[i] = 0;
	}
	dpb->last_output_poc = -2;	
}


void free_dpb(decoder_t *decoder,dpb_t *dpb)
{
	long i;

	for( i=0;i<dpb->max_size;i++ ){
		if( dpb->pic_closet[i] ){
			free_picture(dpb->pic_closet[i]);
			dpb->pic_closet[i] = 0;
		}
	}
}

picture_t *get_unused_pic(decoder_t *decoder,dpb_t *dpb)
{
	long i;
	picture_t *pic = 0;

	for( i=0;i<dpb->max_size;i++ ){
		if( dpb->pic_closet[i] ){
			if( dpb->pic_closet[i]->is_output  && 
				(!dpb->pic_closet[i]->is_reference) ){

				dpb->pic_closet[i]->is_output = 0;
				dpb->pic_closet[i]->is_reference = 0;
				dpb->pic_closet[i]->is_long_term = 0;
				pic = dpb->pic_closet[i];
				dpb->pic_closet[i] = 0;
				return pic;
			}
		} 
	}
	
	pic = alloc_picture(decoder->width,decoder->height);
	return pic;	
}


void store_picture(decoder_t *decoder,dpb_t *dpb)
{
	picture_t *pic = decoder->cur_pic;
	long i;
	
	decoder->poc->last_mmco5_flag = 0;

	if( decoder->idr_flag ){
		idr_memory_manager(decoder,dpb);
	} else if( decoder->cur_slice->adaptive_ref_pic_buffering_flag ){
		adaptive_memory_manager(decoder,dpb);
	} else if( pic->is_reference ){
		sliding_memory_manager(decoder,dpb);
	}

	for( i=0;i<dpb->max_size;i++ ){
		if( dpb->pic_closet[i]==0 ){
			dpb->pic_closet[i] = pic;
			break;
		}
	}    
	assert( i<dpb->max_size );
	decoder->cur_pic = 0;
}


void init_dpb_list(decoder_t *decoder,dpb_t *dpb,long slice_type)
{
	long  max_frame_num = decoder->max_frame_num;
    long  i;
	long  idx = 0;
	long  idx_lt;


	if( !decoder->idr_flag ){
		for( i=0;i<dpb->max_size;i++ ){
			if( dpb->pic_closet[i] ){
				if( dpb->pic_closet[i]->is_reference && (!dpb->pic_closet[i]->is_long_term) ){
					if( dpb->pic_closet[i]->frame_num > decoder->frame_num ){
						dpb->pic_closet[i]->frame_num_wrap = dpb->pic_closet[i]->frame_num_wrap - max_frame_num;
					} else {
						dpb->pic_closet[i]->frame_num_wrap = dpb->pic_closet[i]->frame_num;
					}
					dpb->pic_closet[i]->pic_num = dpb->pic_closet[i]->frame_num_wrap;
				}
			}
		}
	}
	if( slice_type == I_SLICE ){
		memset(dpb->listX,0,sizeof(picture_t*)*MAX_LIST_SIZE);
	} else if( slice_type == P_SLICE ){
		for( i=0;i<dpb->max_size;i++ ){
			if( dpb->pic_closet[i] && dpb->pic_closet[i]->is_reference && (!dpb->pic_closet[i]->is_long_term)){
				dpb->listX[idx++] = dpb->pic_closet[i];
			}
		}
		qsort(dpb->listX,idx,sizeof(picture_t*),compare_pic_by_pic_num_desc);
		idx_lt = idx;
		for( i=0;i<dpb->max_size;i++ ){
			if( dpb->pic_closet[i] && dpb->pic_closet[i]->is_long_term ){
				dpb->listX[idx_lt++] = dpb->pic_closet[i];
			}
		}
		qsort(&dpb->listX[idx],idx_lt,sizeof(picture_t*),compare_pic_by_lt_pic_num_asc);
	}
}



void reorder_dpb_list(decoder_t *decoder,dpb_t *dpb,slice_t *slice)
{
	if( slice->slice_type == P_SLICE ){
		if( slice->ref_pic_list_reordering_flag_l0 ){
			assert( 0==1 );
		}
	}
}

void output_picture(dpb_t *dpb,h264_dec_param_t *param)
{
	long  i;
	long flush = param->nal == 0;
	long  min_poc = INT_MAX;
	long  k = -1;
	long  counter = 0;

	for( i=0;i<dpb->max_size;i++ ){
		if( dpb->pic_closet[i] && (!dpb->pic_closet[i]->is_output) ){
			counter ++;
			if( min_poc > dpb->pic_closet[i]->frame_poc ){
				k = i;
				min_poc = dpb->pic_closet[i]->frame_poc;
			}
		}
	}    

	if( (flush || (counter>=DPB_PRE_ALLOC_SIZE)) && (k>=0) ){
		param->width = dpb->pic_closet[k]->width;
		param->height = dpb->pic_closet[k]->height;
		param->stride = param->width + PAD_SIZE*2;		
		param->img[0] = dpb->pic_closet[k]->img_y[0];
		param->img[1] = dpb->pic_closet[k]->img_u[0];
		param->img[2] = dpb->pic_closet[k]->img_v[0];
		dpb->pic_closet[k]->is_output = 1;       	
	} else {
		param->img[0] = param->img[1] = param->img[2] = 0;
	}
}

