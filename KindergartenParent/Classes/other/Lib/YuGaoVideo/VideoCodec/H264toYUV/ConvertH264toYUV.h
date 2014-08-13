/*
 *  ConvertH264toYUV.h
 *  Camera
 *
 *  Created by apple on 11-1-18.
 *  Copyright 2011 HuangSiwen. All rights reserved.
 *
 */

#ifndef __ConvertH264toYUV_H__
#define __ConvertH264toYUV_H__

#include "DebugLog.h"

#include "h264dec.h"

#ifdef __cplusplus
extern "C" {
#endif


int ConvertH264toYUV(h264_dec_handle_t handle, unsigned char *pH264Data, int nH264Len,
                     unsigned char *pYUVData,   int *pYUVLen,
                     int *pWidth, int *pHeight, int *pTotalY, int *pTotalUV);

    
#ifdef __cplusplus
}
#endif


#endif
