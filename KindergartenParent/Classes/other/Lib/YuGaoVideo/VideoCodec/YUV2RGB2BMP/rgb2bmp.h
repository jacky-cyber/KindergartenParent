/*
 *  rgb2bmp.h
 *  Camera
 *
 *  Created by apple on 11-2-9.
 *  Copyright 2011 HuangSiwen. All rights reserved.
 *
 */

#ifndef __RGG2BMP_H__
#define __RGG2BMP_H__

#include "DebugLog.h" 

int ConvertRGBtoBMP(char* filename, unsigned char* BMPData, int width, int height, unsigned char* pRGBData);
unsigned char* FillBMPHeaderWithRGB(unsigned char* pBMPData, int width, int height);

#endif
