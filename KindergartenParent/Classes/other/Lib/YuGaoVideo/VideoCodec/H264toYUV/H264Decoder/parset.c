
#include <assert.h>
#include <memory.h>

#include "global.h"
#include "common.h"
#include "parset.h"
#include "bitstream.h"
#include "vlc.h"
#include "h264dec.h"
#include "dpb.h"


static long interpret_sps(seq_parameter_set_t *sps,bs_t *bs)
{
	ulong i;
	long  reserved = 0;
	sps->profile_idc = get_bits(bs,8);
	sps->constraint_set0_flag = get_bits(bs,1);
	sps->constraint_set1_flag = get_bits(bs,1);
	sps->constraint_set2_flag = get_bits(bs,1);
	sps->constraint_set3_flag = get_bits(bs,1);
	reserved = get_bits(bs,4);
	assert( reserved == 0 );
	sps->level_idc = get_bits(bs,8);
	sps->seq_parameter_set_id = golumb_ue(bs);

	if( sps->profile_idc==PROFILE_HIGH || sps->profile_idc==PROFILE_HIGH10 ||
		sps->profile_idc==PROFILE_HIGH422 || sps->profile_idc==PROFILE_HIGH444 ) {
        return -1;
	}
	sps->log2_max_frame_num_minus4 = golumb_ue(bs);
	sps->pic_order_cnt_type = golumb_ue(bs);
	if( sps->pic_order_cnt_type == 0 ){ 
		sps->log2_max_pic_order_cnt_lsb_minus4 = golumb_ue(bs);
	} else if( sps->pic_order_cnt_type == 1 ){
		sps->delta_pic_order_always_zero_flag = get_bits(bs,1);
		sps->offset_for_non_ref_pic = golumb_se(bs);
		sps->offset_for_top_to_bottom_field = golumb_se(bs);
		sps->num_ref_frames_in_pic_order_cnt_cycle = golumb_ue(bs);
		for( i=0;i<sps->num_ref_frames_in_pic_order_cnt_cycle;i++ ){
			sps->offset_for_ref_frame[i] = golumb_se(bs);
		}		
	}
	sps->num_ref_frames = golumb_ue(bs);
	sps->gaps_in_frame_num_value_allowed_flag = get_bits(bs,1);
	sps->pic_width_in_mbs_minus1 = golumb_ue(bs);
	sps->pic_height_in_map_units_minus1 = golumb_ue(bs);
	sps->frame_mbs_only_flag = get_bits(bs,1);
	if( !sps->frame_mbs_only_flag ){
		sps->mb_adaptive_frame_field_flag = get_bits(bs,1);
	}
	sps->direct_8x8_inference_flag = get_bits(bs,1);
	sps->frame_cropping_flag = get_bits(bs,1);
	if( sps->frame_cropping_flag ){
		sps->frame_cropping_rect_left_offset = golumb_ue(bs);		
		sps->frame_cropping_rect_right_offset = golumb_ue(bs);
		sps->frame_cropping_rect_top_offset = golumb_ue(bs);
		sps->frame_cropping_rect_bottom_offset = golumb_ue(bs);
	}	
	sps->vui_parameters_present_flag = get_bits(bs,1);
	if( sps->vui_parameters_present_flag ){
	}
	return 0;
}


static void  mark_sps_available(decoder_t *decoder,seq_parameter_set_t *sps)
{
	memcpy(&decoder->sps[sps->seq_parameter_set_id],sps,sizeof(seq_parameter_set_t));
}

static long interpret_pps(pic_parameter_set_t *pps,bs_t *bs)
{
	pps->pic_parameter_set_id = golumb_ue(bs);
	pps->seq_parameter_set_id = golumb_ue(bs);
	pps->entropy_coding_flag = get_bits(bs,1);
	pps->poc_order_present_flag = get_bits(bs,1);
	pps->num_slice_groups_minus1 = golumb_ue(bs);
	
	if( pps->num_slice_groups_minus1 > 1 ){
		// TODO
	}
	pps->num_ref_idx_l0_active_minus1 = golumb_ue(bs);
	pps->num_ref_idx_l1_active_minus1 = golumb_ue(bs);
	pps->weighted_pred_flag = get_bits(bs,1);
	pps->weighted_bipred_idc = get_bits(bs,2);
	pps->pic_init_qp_minus26 = golumb_se(bs);
	pps->pic_init_qs_minus26 = golumb_se(bs);
	pps->chroma_qp_index_offset = golumb_se(bs);
	pps->deblocking_filter_control_present_flag = get_bits(bs,1);
	pps->constrained_intra_pred_flag = get_bits(bs,1);
	pps->redundant_pic_cnt_present_flag = get_bits(bs,1);

	// TODO

	return 0;
}

static void mark_pps_available(decoder_t *decoder,pic_parameter_set_t *pps)
{
	ulong id = pps->pic_parameter_set_id;
	if( decoder->pps[id].slice_group_id != 0 ){
		align_free(decoder->pps[id].slice_group_id);
	}
	memcpy(&decoder->pps[id],pps,sizeof(pic_parameter_set_t));
	if( pps->num_slice_group_map_units_minus1 > 0 ){
		// TODO 
	}
}

long process_sps(decoder_t *decoder,bs_t *bs)
{
	seq_parameter_set_t  *sps = align_malloc(sizeof(seq_parameter_set_t));

    interpret_sps(sps,bs);

	if( sps->profile_idc != PROFILE_BASELINE ){		
        align_free(sps);
		return -1;
	}

	if( decoder->active_sps != 0 ){
		if( decoder->active_sps->seq_parameter_set_id == sps->seq_parameter_set_id ){
			//TODO			
		}
	}
    mark_sps_available(decoder,sps);

	align_free(sps);
	return 0;
}

long process_pps(decoder_t *decoder,bs_t *bs)
{
	pic_parameter_set_t *pps = align_malloc(sizeof(pic_parameter_set_t));

	interpret_pps(pps,bs);

	if( decoder->active_pps != 0 ){
		if( decoder->active_pps->pic_parameter_set_id == pps->pic_parameter_set_id ){
			// TODO
		}
	}

	mark_pps_available(decoder,pps);
	align_free(pps);
	return 0;
}


void active_sps(decoder_t *decoder,seq_parameter_set_t *sps)
{
	if( decoder->active_sps != sps ){
		// TODO
		decoder->max_frame_num = 1<<(sps->log2_max_frame_num_minus4+4);
		decoder->pic_width_in_mbs = sps->pic_width_in_mbs_minus1 + 1;
		decoder->pic_height_in_map_units = sps->pic_height_in_map_units_minus1 + 1;
		decoder->frame_height_in_mbs = (2-sps->frame_mbs_only_flag)*decoder->pic_height_in_map_units;
		decoder->frame_size_in_mbs = decoder->pic_width_in_mbs * decoder->frame_height_in_mbs;
		decoder->width = decoder->pic_width_in_mbs << 4;
		decoder->width_cr = decoder->width / 2;
		decoder->height = decoder->frame_height_in_mbs << 4;
		decoder->height_cr = decoder->height / 2;

		decoder->active_sps = sps;

		free_dpb(decoder,decoder->dpb);
		init_dpb(decoder,decoder->dpb);
	}
}

void active_pps(decoder_t *decoder,pic_parameter_set_t *pps)
{
	if( decoder->active_pps != pps ){
		// TODO  
		decoder->active_pps = pps;
	}
}