
#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>


#include "global.h"
#include "common.h"
#include "bitstream.h"
#include "vlc.h"
#include "slice.h"
#include "parset.h"
#include "access.h"
#include "poc.h"
#include "macroblock.h"
#include "h264dec.h"
#include "dpb.h"
#include "loopfilter.h"
#include "recon.h"


#ifdef TRACE
//#include <windows.h>
#endif


static  inline ulong ceil_log2(ulong ui_val)
{
	ulong ui_tmp = ui_val-1;
	ulong ui_ret = 0;

	while( ui_tmp != 0 ){
		ui_tmp >>= 1;
		ui_ret++;
	}
	return ui_ret;
}

static void ref_pic_list_reordering(decoder_t *decoder,slice_t *slice,bs_t *bs)
{
	long val,i;
	if( slice->slice_type!=I_SLICE ){
		slice->ref_pic_list_reordering_flag_l0 = val = get_bits(bs,1);
		if( val ){
			i = 0;
			do {
				slice->remapping_of_pic_nums_idc_l0[i] = val = golumb_ue(bs);
				if( val==0 || val==1 ){
					slice->abs_diff_pic_num_minus1_l0[i] = golumb_ue(bs);
				} else if( val == 2 ){
					slice->long_term_pic_num_l0[i] = golumb_ue(bs);
				}
				i++;
			} while (val != 3 );
		}
	}
	if( slice->slice_type == B_SLICE ){
		slice->ref_pic_list_reordering_flag_l1 = val = get_bits(bs,1);
		if( val ){
			i = 0;
			do {
				slice->remapping_of_pic_nums_idc_l1[i] = val = golumb_ue(bs);
				if( val==0 || val==1 ){
					slice->abs_diff_pic_num_minus1_l1[i] = golumb_ue(bs);
				} else if( val == 2 ){
					slice->long_term_pic_num_l1[i] = golumb_ue(bs);
				}
				i++;
			} while (val != 3 );
		}
	}
}

static void dec_ref_pic_marking(decoder_t *decoder,slice_t *slice,bs_t *bs)
{
	long val = 0;
	
	dec_ref_pic_marking_t *tmp = slice->dec_ref_pic_marking_buf;
	while( slice->dec_ref_pic_marking_buf!=0 ){
		slice->dec_ref_pic_marking_buf = slice->dec_ref_pic_marking_buf->next;
		align_free(tmp);
	}
	slice->dec_ref_pic_marking_buf = 0;

	if( decoder->idr_flag ){
		slice->no_output_of_prior_pics_flag = get_bits(bs,1);
		slice->long_term_reference_flag = get_bits(bs,1);
	} else {
		slice->adaptive_ref_pic_buffering_flag = get_bits(bs,1);
		if( slice->adaptive_ref_pic_buffering_flag ){
			do {
                dec_ref_pic_marking_t *tmp2 = 0;
				tmp = align_malloc(sizeof(dec_ref_pic_marking_t));
				tmp->next = 0;
				val = tmp->memory_management_control_operation = golumb_ue(bs);
				if( val==1 || val==3 ){
					tmp->difference_of_pic_nums_minus1 = golumb_ue(bs);
				}
				if( val==2 ){
					tmp->long_term_pic_num = golumb_ue(bs);
				}
				if( val==3 || val==6 ){
					tmp->long_term_frame_idx = golumb_ue(bs);
				}
				if( val==4 ){
					tmp->max_long_term_frame_idx_plus1 = golumb_ue(bs);
				}
				if( slice->dec_ref_pic_marking_buf = 0 ){
					slice->dec_ref_pic_marking_buf = tmp;
				} else {
					tmp2 = slice->dec_ref_pic_marking_buf;
					while(tmp2->next!=0) tmp2 = tmp2->next;
					tmp2->next = tmp;
				}
			} while (val!=0);
		}
	}
}

static long interpret_slice_header_first_part(slice_t *slice,bs_t *bs)
{
	ulong tmp;
	slice->start_mb_nr = golumb_ue(bs);
	tmp = golumb_ue(bs);
	if( tmp > 4 ){
		tmp -= 5;
	}
	slice->slice_type = tmp;
	slice->pic_parameter_set_id = golumb_ue(bs);
	return 0;
}

static long use_parameter_set(decoder_t *decoder,slice_t *slice)
{    
	pic_parameter_set_t *pps = &decoder->pps[slice->pic_parameter_set_id];
	seq_parameter_set_t *sps = &decoder->sps[pps->seq_parameter_set_id];
	active_sps(decoder,sps);
	active_pps(decoder,pps);
	return 0;
}

static long interpret_slice_header_rest_part(decoder_t *decoder,slice_t *slice,bs_t *bs)
{
	seq_parameter_set_t *sps = decoder->active_sps;
	pic_parameter_set_t *pps = decoder->active_pps;
	ulong   old_pic_size_in_mbs = decoder->pic_size_in_mbs;
	ulong bits_to_get = sps->log2_max_frame_num_minus4 + 4;

	decoder->frame_num = slice->frame_num = get_bits(bs,bits_to_get);    	
	if( sps->frame_mbs_only_flag ){
		decoder->struction = FRAME;
	} else {
		slice->pic_field_flag = get_bits(bs,1);
		if( slice->pic_field_flag ){
			slice->bottom_field_flag = get_bits(bs,1);
			decoder->struction = slice->bottom_field_flag ? BOTTOM_FIELD : TOP_FIELD;
		} else{
			decoder->struction = FRAME;
			slice->bottom_field_flag = 0;
		}
	}
	if( decoder->idr_flag ){
		slice->idr_pic_id = golumb_ue(bs);
	}

	if( sps->pic_order_cnt_type == 0 ){
		bits_to_get = sps->log2_max_pic_order_cnt_lsb_minus4 + 4;
		slice->pic_order_cnt_lsb = golumb_u(bs,bits_to_get);
		if( pps->poc_order_present_flag && (!slice->pic_field_flag) ){
			slice->delta_pic_order_cnt_bottom = golumb_se(bs);
		}
	} else if( sps->pic_order_cnt_type==1  &&  (!sps->delta_pic_order_always_zero_flag) ){
		slice->delta_pic_order_cnt[0] = golumb_se(bs);
		if( pps->poc_order_present_flag && (!slice->pic_field_flag) ){
			slice->delta_pic_order_cnt[1] = golumb_se(bs);
		}
	}
	if( pps->redundant_pic_cnt_present_flag ){
		slice->redundant_pic_cnt = golumb_ue(bs);
	}
	if( slice->slice_type == B_SLICE ){
		slice->direct_spatial_mv_pred_flag = get_bits(bs,1);
	}
	slice->num_ref_idx_l0_active = pps->num_ref_idx_l0_active_minus1 + 1;
	if( slice->slice_type==P_SLICE || slice->slice_type==B_SLICE ){
		ulong val = get_bits(bs,1);
		if( val ){
			slice->num_ref_idx_l0_active = 1 + golumb_ue(bs);
			if( slice->slice_type == B_SLICE ){
				slice->num_ref_idx_l1_active = 1 + golumb_ue(bs);
			} else {
				slice->num_ref_idx_l1_active = 0;
			}
		}
	}

	ref_pic_list_reordering(decoder,slice,bs);

	if( (pps->weighted_pred_flag && slice->slice_type==P_SLICE) || 
		(pps->weighted_bipred_idc==1 && slice->slice_type==B_SLICE) ){
        // TODO  
		assert( 0==1 );
	}
	if( decoder->nalu_ref_idc!=0 ){
        dec_ref_pic_marking(decoder,slice,bs);
	}
	if( pps->entropy_coding_flag && slice->slice_type!=I_SLICE ){
		slice->cabac_init_idc = golumb_ue(bs);
	} else {
		slice->cabac_init_idc = 0;
	}
	decoder->qp = slice->slice_qp = golumb_se(bs) + 26 + pps->pic_init_qp_minus26;
	if( pps->deblocking_filter_control_present_flag ){
		slice->disable_deblocking_filter_idc = golumb_ue(bs);
		if( slice->disable_deblocking_filter_idc != 1 ){
			slice->slice_alpha_c0_offset_div2 = golumb_se(bs);
			slice->slice_beta_offset_div2 = golumb_se(bs);
		} else {
			slice->slice_alpha_c0_offset_div2 = 0;
			slice->slice_beta_offset_div2 = 0;
		}
	}
	if( pps->num_slice_groups_minus1>0 && pps->slice_group_map_type>=3 && pps->slice_group_map_type<=5 ){
		ulong len = (sps->pic_height_in_map_units_minus1+1)*(sps->pic_width_in_mbs_minus1+1)
			/ (pps->slice_group_change_rate_minus1+1);
		if( (sps->pic_height_in_map_units_minus1+1)*(sps->pic_width_in_mbs_minus1+1)
			% (pps->slice_group_change_rate_minus1+1) ){
			len += 1;
		}
        len = ceil_log2(len+1);
		slice->slice_group_change_cycle = get_bits(bs,len);
	}
	decoder->pic_height_in_mbs = decoder->frame_height_in_mbs/(1+slice->pic_field_flag);
	decoder->pic_size_in_mbs = decoder->pic_width_in_mbs * decoder->pic_height_in_mbs;

	return (old_pic_size_in_mbs != decoder->pic_size_in_mbs);
}


void exit_picture(decoder_t *decoder)
{
	if( decoder->cur_pic == 0 )
		return;

	deblock_picture(decoder);	
	expand_pic(decoder->cur_pic);
	store_picture(decoder,decoder->dpb);
	
#ifdef TRACE
	{
		long tmp;
		//// QueryPerformanceCounter((LARGE_INTEGER*)&decoder->stop);
		decoder->stop = clock();
        
        ////---- * 3
		tmp = 3 * 10000 * (decoder->stop - decoder->start)/((double)decoder->frequency);
		decoder->total += tmp;
		if ( decoder->cur_slice->slice_type == I_SLICE ) {
            printf(" %ld  %ld I  time %ld , total %ld\n",decoder->num_dec_frame,decoder->poc->frame_poc,tmp,decoder->total);
		} else if( decoder->cur_slice->slice_type == P_SLICE ){
            printf(" %ld  %ld P  time %ld , total %ld\n",decoder->num_dec_frame,decoder->poc->frame_poc,tmp,decoder->total);
		}
	}
#endif
	decoder->cur_pic = 0;
	decoder->num_dec_frame ++ ;
}

void init_picture(decoder_t *decoder)
{
    picture_t *pic;

#ifdef TRACE
	//// QueryPerformanceFrequency((LARGE_INTEGER*)&decoder->frequency);
	//// QueryPerformanceCounter((LARGE_INTEGER*)&decoder->start);
    
    decoder->frequency = CLOCKS_PER_SEC;
    decoder->start = clock();

#endif
    
	if( decoder->cur_pic != 0 ){
        exit_picture(decoder);
	}

	pic = decoder->cur_pic = get_unused_pic(decoder,decoder->dpb);
	pic->pic_structure = decoder->struction;
	pic->frame_num = decoder->frame_num;	
	pic->is_reference = decoder->cur_nalu->nalu_ref_idc != NALU_PRIORITY_DISPOSABLE;	
	pic->frame_poc = decoder->poc->frame_poc;
	decoder->num_dec_mb = 0;

	memset(decoder->ipred_mode,DC_PRED,sizeof(uchar)*decoder->frame_size_in_mbs*16);
	memset(decoder->nz_coeffs_y,0,sizeof(char)*decoder->frame_size_in_mbs*16);
	memset(decoder->nz_coeffs_u,0,sizeof(char)*decoder->frame_size_in_mbs*4);
	memset(decoder->nz_coeffs_v,0,sizeof(char)*decoder->frame_size_in_mbs*4);
}



long  decode_slice(decoder_t *decoder,bs_t *bs)
{
	long bReset = 0;
	slice_t *slice = decoder->cur_slice;
    long end_of_slice = 0;

	interpret_slice_header_first_part(slice,bs);
	use_parameter_set(decoder,slice);
	bReset = interpret_slice_header_rest_part(decoder,slice,bs);
	if( bReset ){
		init_mb_access(decoder,decoder->mb_access);
	}
	decoder->cur_mb_nr = slice->start_mb_nr;

	if( decoder->next_header = SOP ){
		decode_poc(decoder,decoder->poc);
		decoder->cur_slice_nr = 0;
		init_picture(decoder);
	}

	init_dpb_list(decoder,decoder->dpb,slice->slice_type);
	if( slice->slice_type != I_SLICE ){
		reorder_dpb_list(decoder,decoder->dpb,slice);
	}

	decoder->mb_skip_run = -1;

	while ( !end_of_slice ) {			
		init_macroblock(decoder);	
		read_macroblock(decoder,bs);
		decode_macroblock(decoder);
		end_of_slice = exit_macroblock(decoder);
	}

	return 0;
}


void    free_slice(slice_t *slice)
{
	dec_ref_pic_marking_t *tmp = slice->dec_ref_pic_marking_buf;
	while( slice->dec_ref_pic_marking_buf!=0 ){
		slice->dec_ref_pic_marking_buf = slice->dec_ref_pic_marking_buf->next;
		align_free(tmp);
	}
	align_free(slice);
}