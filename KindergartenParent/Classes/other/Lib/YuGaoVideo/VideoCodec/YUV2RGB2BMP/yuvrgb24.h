/*
 *  yuvrgb24.h
 *  Camera
 *
 *  Created by apple on 11-1-21.
 *  Copyright 2011 HuangSiwen. All rights reserved.
 *
 */

#ifndef _YUV_RGG_24_H
#define _YUV_RGG_24_H

#include "DebugLog.h" 

#ifdef __cplusplus
extern "C"
{
#endif
    
extern void ConvertRGBtoYUV(unsigned char *dst0, unsigned char *dst1, unsigned char *dst2,
                      unsigned char *origrgb,int width,int height );
extern void ConvertYUVtoRGB (unsigned char *src0, unsigned char *src1, unsigned char *src2,
                      unsigned char *dst_ori,int width,int height);
extern void ConvertYUVtoRGBWithEdge (unsigned char *src0, unsigned char *src1, unsigned char *src2,
							 unsigned char *dst_ori,int width,int height, int total_y, int total_uv);

#ifdef __cplusplus
}
#endif


#endif
