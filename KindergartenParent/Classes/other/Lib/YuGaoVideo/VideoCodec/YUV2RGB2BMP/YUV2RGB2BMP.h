/*
 *  YUV2RGB2BMP.h
 *  Camera
 *
 *  Created by apple on 11-1-21.
 *  Copyright 2011 HuangSiwen. All rights reserved.
 *
 */

#ifndef __YUV2RGB_H__
#define __YUV2RGB_H__

#include "DebugLog.h" 


int ConvertYUVtoRGBtoBMP(unsigned char *pBMPData, unsigned char *pYUVData, int nYUVLen, int nWidth, int nHeight);


#endif
