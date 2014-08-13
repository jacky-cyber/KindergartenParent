
#include <assert.h>

#include "global.h"
#include "common.h"
#include "poc.h"




static void decode_poc_type0(decoder_t *decoder,pic_order_cnt_t *poc)
{
	slice_t *slice = decoder->cur_slice;
	long max_pic_order_cnt_lsb = 1<<(decoder->active_sps->log2_max_pic_order_cnt_lsb_minus4+4);
	long  pic_order_cnt_msb;
	if( decoder->idr_flag ){
		poc->prev_pic_order_cnt_lsb = 0;
		poc->prev_pic_order_cnt_msb = 0;
	} else {
		if( poc->last_mmco5_flag ){
			if( poc->last_pic_bottom_field_flag ){
				poc->prev_pic_order_cnt_lsb = 0;
				poc->prev_pic_order_cnt_msb = 0;
			} else {
				poc->prev_pic_order_cnt_lsb = poc->top_field_poc;
				poc->prev_pic_order_cnt_msb = 0;
			}
		} 
	}

	if( (slice->pic_order_cnt_lsb<poc->prev_pic_order_cnt_lsb) && ((poc->prev_pic_order_cnt_lsb-
		slice->pic_order_cnt_lsb)>=(max_pic_order_cnt_lsb>>1)) ){
		pic_order_cnt_msb = poc->prev_pic_order_cnt_msb + max_pic_order_cnt_lsb;
	} else if( (slice->pic_order_cnt_lsb>poc->prev_pic_order_cnt_lsb) && ((slice->pic_order_cnt_lsb-
		poc->prev_pic_order_cnt_lsb)>=(max_pic_order_cnt_lsb>>1)) ) {
		pic_order_cnt_msb = poc->prev_pic_order_cnt_msb - max_pic_order_cnt_lsb;
	} else {
		pic_order_cnt_msb = poc->prev_pic_order_cnt_msb;
	}

	if( !slice->bottom_field_flag || !slice->pic_field_flag){
		poc->top_field_poc = pic_order_cnt_msb + slice->pic_order_cnt_lsb;
	} 
	if( !slice->pic_field_flag ){
		poc->this_poc = poc->bottom_field_poc = poc->top_field_poc + slice->delta_pic_order_cnt_bottom;
	} else {
		if( !slice->bottom_field_flag ){
			poc->this_poc = poc->bottom_field_poc = pic_order_cnt_msb + slice->pic_order_cnt_lsb;
		}
	}
	if( !slice->pic_field_flag ){
		poc->frame_poc = poc->top_field_poc;
	} else {
		poc->frame_poc = poc->bottom_field_poc;
	}
	if( decoder->cur_nalu->nalu_ref_idc != NALU_PRIORITY_DISPOSABLE ){
		poc->prev_pic_order_cnt_lsb = slice->pic_order_cnt_lsb;
		poc->prev_pic_order_cnt_msb = pic_order_cnt_msb;
	}
}

static void decode_poc_type1(decoder_t *decoder,pic_order_cnt_t *poc)
{
	long expected_delta_per_pic_order_cnt_cycle = 0;
	ulong i;
	long expected_pic_order_cnt = 0;
	long pic_order_cnt_cycle_cnt = 0;
	long frame_num_in_pic_order_cnt_cycle = 0;
	slice_t *slice = decoder->cur_slice;
	seq_parameter_set_t *sps = decoder->active_sps;
	long  max_frame_num = 1<<(sps->log2_max_frame_num_minus4+4);
	if( decoder->idr_flag ){
		poc->prev_frame_num_offset = 0;
		slice->delta_pic_order_cnt[0] = 0;  // ignore
	} else {
		if( poc->last_mmco5_flag ){
			poc->prev_frame_num_offset = 0;
			poc->prev_frame_num = 0;
		} 
		if( poc->prev_frame_num < slice->frame_num ){
			poc->frame_num_offset = poc->prev_frame_num_offset + max_frame_num;
		} else {
			poc->frame_num_offset = poc->prev_frame_num_offset;
		}
	}
	if( sps->num_ref_frames_in_pic_order_cnt_cycle ){
		poc->abs_frame_num = slice->frame_num + poc->frame_num_offset;
	} else {
		poc->abs_frame_num = 0;
	}
	if( (decoder->cur_nalu->nal_unit_type==NALU_PRIORITY_DISPOSABLE) && (poc->abs_frame_num>0) ){
		poc->abs_frame_num --;
	}	
	for( i=0;i<sps->num_ref_frames_in_pic_order_cnt_cycle;i++ ){
        expected_delta_per_pic_order_cnt_cycle += sps->offset_for_ref_frame[i];
	}
	if( poc->abs_frame_num ){
		pic_order_cnt_cycle_cnt = (poc->abs_frame_num-1)/sps->num_ref_frames_in_pic_order_cnt_cycle;
		frame_num_in_pic_order_cnt_cycle = (poc->abs_frame_num-1)%sps->num_ref_frames_in_pic_order_cnt_cycle;
		expected_pic_order_cnt = pic_order_cnt_cycle_cnt * expected_delta_per_pic_order_cnt_cycle;
		for(i=0;i<sps->num_ref_frames_in_pic_order_cnt_cycle;i++){
			expected_pic_order_cnt += sps->offset_for_ref_frame[i];	
		}
	} else {
		expected_pic_order_cnt = 0;
	}
	if( decoder->cur_nalu->nal_unit_type == NALU_PRIORITY_DISPOSABLE ){
		expected_pic_order_cnt += sps->offset_for_non_ref_pic;
	}
	if( !slice->pic_field_flag ){
		poc->top_field_poc = expected_pic_order_cnt + slice->delta_pic_order_cnt[0];
		poc->bottom_field_poc = poc->top_field_poc + sps->offset_for_top_to_bottom_field + slice->delta_pic_order_cnt[1];
		poc->this_poc = poc->frame_poc = (poc->top_field_poc < poc->bottom_field_poc ) ? poc->top_field_poc : poc->bottom_field_poc;
	} else {
		if( !slice->bottom_field_flag ){
			poc->this_poc = poc->top_field_poc = expected_pic_order_cnt + slice->delta_pic_order_cnt[0];
		} else {
			poc->this_poc = poc->bottom_field_poc = expected_pic_order_cnt + sps->offset_for_top_to_bottom_field + slice->delta_pic_order_cnt[1];
		}
		poc->frame_poc = poc->this_poc;
	}
	poc->prev_frame_num = slice->frame_num;
	poc->prev_frame_num_offset = poc->frame_num_offset;
}

static void decode_poc_type2(decoder_t *decoder,pic_order_cnt_t *poc)
{
	slice_t *slice = decoder->cur_slice;
	seq_parameter_set_t *sps = decoder->active_sps;
	long  max_frame_num = 1<<(sps->log2_max_frame_num_minus4+4);
	if( decoder->idr_flag ){
		poc->frame_num_offset = 0;
		poc->this_poc = poc->top_field_poc = poc->bottom_field_poc = poc->frame_poc = 0;
	} else {
		if( poc->last_mmco5_flag ){
			poc->prev_frame_num = 0;
			poc->prev_frame_num_offset = 0;
		} 
		if( slice->frame_num < poc->prev_frame_num ){
			poc->frame_num_offset = poc->prev_frame_num_offset + max_frame_num;
		} else {
			poc->frame_num_offset = poc->prev_frame_num_offset;
		}
		poc->abs_frame_num = poc->frame_num_offset + slice->frame_num;
		if( decoder->cur_nalu->nal_unit_type == NALU_PRIORITY_DISPOSABLE ){
			poc->this_poc = 2 * poc->abs_frame_num - 1;
		} else {
			poc->this_poc = 2 * poc->abs_frame_num;
		}
		if( !slice->pic_field_flag ){
			poc->frame_poc = poc->top_field_poc = poc->bottom_field_poc = poc->this_poc;
		} else if( slice->bottom_field_flag ){
			poc->bottom_field_poc = poc->frame_poc = poc->this_poc;
		} else {
			poc->top_field_poc = poc->frame_poc = poc->this_poc;
		}
	}

	if( decoder->cur_nalu->nal_unit_type != NALU_PRIORITY_DISPOSABLE ){
		poc->prev_frame_num = slice->frame_num;
	}
	poc->prev_frame_num_offset = poc->frame_num_offset;
}


void decode_poc(decoder_t *decoder,pic_order_cnt_t *poc)
{
	seq_parameter_set_t *sps = decoder->active_sps;
	switch( sps->pic_order_cnt_type ){
	case 0:
        decode_poc_type0(decoder,poc);
		break;
	case 1:
		decode_poc_type1(decoder,poc);
		break;
	case 2:
		decode_poc_type2(decoder,poc);
		break;
	default:
        assert(0==1);
	}
}