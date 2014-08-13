
#include <memory.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "global.h"
#include "common.h"
#include "nalu.h"
#include "h264dec.h"
#include "h264Core.h"
#include "dpb.h"





h264_dec_handle_t h264_dec_open()
{
	decoder_t  *decoder = align_malloc(sizeof(decoder_t));
	init_decoder(decoder);
	return decoder;
}


long h264_dec_decode(h264_dec_handle_t handle,h264_dec_param_t *param)
{
	decoder_t *decoder = (decoder_t*)handle;
	uchar *p = param->nal;
	long len = param->nalLen;
	ulong tmp; 
	nalu_t *nalu = decoder->cur_nalu;
	if (len>0) {
		tmp = get_nalu(nalu,p,len);		
		decode_nalu(decoder,nalu);		
		p += tmp;
		len -= tmp;
		if( len > 3 ){
		    p[0] = 0;
		    p[1] = 0;
		    p[2] = 1;
		}		
		param->nal = p;
		param->nalLen = len;		
	}
	output_picture(decoder->dpb,param);
	return 0;
}


void h264_dec_close(h264_dec_handle_t handle)
{
	if( handle != 0 ){		
		free_decoder(handle);
		align_free(handle);
	}
}

