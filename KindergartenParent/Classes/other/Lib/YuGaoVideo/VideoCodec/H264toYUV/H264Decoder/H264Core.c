
#include <memory.h>
#include <assert.h>
#include <stdio.h>

#include "global.h"
#include "common.h"
#include "h264Core.h"
#include "bitstream.h"
#include "parset.h"
#include "slice.h"
#include "access.h"
#include "nalu.h"
#include "h264dec.h"
#include "dpb.h"
#include "dct.h"
#include "recon.h"
#include "interpolate.h"
#include "loopfilter.h"


long  init_decoder(decoder_t *decoder)
{		
    ////long cpu_flag;

	decoder->cur_nalu = align_malloc(sizeof(nalu_t));
	decoder->bs = align_malloc(sizeof(bs_t));
	decoder->cur_slice = align_malloc(sizeof(slice_t));  
	decoder->mb_access = align_malloc(sizeof(mb_access_t));
	decoder->poc = align_malloc(sizeof(pic_order_cnt_t));
	decoder->mb_date = align_malloc(sizeof(macroblock_t)*MAX_SIZE_IN_MBS);
	decoder->ipred_mode = align_malloc(sizeof(uchar)*MAX_SIZE_IN_MBS*16+16);
	memset(decoder->ipred_mode,DC_PRED,sizeof(uchar)*MAX_SIZE_IN_MBS*16+16);
	decoder->ipred_mode += 16;
	decoder->nz_coeffs_y = align_malloc(sizeof(char)*MAX_SIZE_IN_MBS*16+16);	
	decoder->nz_coeffs_y += 16;
	decoder->nz_coeffs_u = align_malloc(sizeof(char)*MAX_SIZE_IN_MBS*4+16);
	decoder->nz_coeffs_u += 16;
	decoder->nz_coeffs_v = align_malloc(sizeof(char)*MAX_SIZE_IN_MBS*4+16);
	decoder->nz_coeffs_v += 16;
	decoder->dpb = align_malloc(sizeof(dpb_t));

	decoder->next_header = SOP;
	decoder->mb_skip_run = -1;

    //cpu_flag = check_cpu();
	
	////if( cpu_flag & CPU_SSE2 ){
    if (0) {
        printf("=== Assembly language ====\n");
        /*
		memcpy_4x4_dc = memcpy_4x4_dc_mmx;
		iquant_4x4 = iquant_4x4_mmx;
		idct_4x4 = idct_4x4_mmx;
		idct_4x4_dc = idct_4x4_dc_mmx;
		recon_mb_p = recon_mb_p_sse2;
		get_block_16x16_luma = get_block_16x16_luma_sse2;
		get_block_8x8_chroma = get_block_8x8_chroma_sse2;
		get_block_8x8_luma = get_block_8x8_luma_sse2;
		get_block_4x4_chroma = get_block_4x4_chroma_sse2;
		filter_luma_ver = filter_luma_ver_mmx;
		filter_luma_hor = filter_luma_hor_mmx;
		filter_chroma_ver = filter_chroma_ver_mmx;
		filter_chroma_hor = filter_chroma_hor_mmx;
		recon_chroma_for_mb_4x4 = recon_chroma_for_mb_4x4_sse2;
		get_block_4x4_luma = get_block_4x4_luma_sse2;
         */
        
	} else {
        printf("====== C language =====\n");
		memcpy_4x4_dc = memcpy_4x4_dc_c;
		iquant_4x4 = iquant_4x4_c;
		idct_4x4 = idct_4x4_c;
		idct_4x4_dc = idct_4x4_dc_c;
		recon_mb_p = recon_mb_p_c;
		get_block_16x16_luma = get_block_16x16_luma_c;
		get_block_8x8_chroma = get_block_8x8_chroma_c;
		get_block_8x8_luma = get_block_8x8_luma_c;
		get_block_4x4_chroma = get_block_4x4_chroma_c;
		filter_luma_ver = filter_luma_ver_c;
		filter_luma_hor = filter_luma_hor_c;
		filter_chroma_ver = filter_chroma_ver_c;
		filter_chroma_hor = filter_chroma_hor_c;
		recon_chroma_for_mb_4x4 = recon_chroma_for_mb_4x4_c;
		get_block_4x4_luma = get_block_4x4_luma_c;
	}
	
	return 0;
}


long  decode_nalu(decoder_t *decoder,nalu_t *nalu)
{	
	bs_t *bs = decoder->bs;
	bs_init(bs,nalu->buf+1,nalu->len-1);
	switch( nalu->nal_unit_type )
	{
	case NALU_TYPE_SPS:    
		process_sps(decoder,bs);		
		break;
	case NALU_TYPE_PPS:
		process_pps(decoder,bs);
		break;
	case NALU_TYPE_IDR:
	case NALU_TYPE_SLICE:
		rbsp_to_sodb(nalu);
		decoder->idr_flag = (nalu->nal_unit_type == NALU_TYPE_IDR);
		decoder->nalu_ref_idc = nalu->nalu_ref_idc;
		decode_slice(decoder,bs);
		decoder->cur_slice_nr ++;
		break;		
	default:
		break;
	}
	if( decoder->next_header == SOP ){
		exit_picture(decoder);
	}
	decoder->num_dec_nalu ++;
	return 0;
}

long free_decoder(decoder_t *decoder)
{
	align_free(decoder->cur_nalu);
	align_free(decoder->bs);
	free_slice(decoder->cur_slice);
	align_free(decoder->mb_access);
	align_free(decoder->poc);
	align_free(decoder->mb_date);
	align_free(decoder->ipred_mode-16);
	align_free(decoder->nz_coeffs_y-16);
	align_free(decoder->nz_coeffs_u-16);
	align_free(decoder->nz_coeffs_v-16);
	free_dpb(decoder,decoder->dpb);
	align_free(decoder->dpb);
	return 0;
}