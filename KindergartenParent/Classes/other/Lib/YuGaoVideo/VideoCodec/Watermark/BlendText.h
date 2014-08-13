/*
 *  BlendText.h
 *  Camera
 *
 *  Created by apple on 12-4-20.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */

#ifndef __BLEND_TEXT_H__
#define __BLEND_TEXT_H__

struct IMAGE
{
	unsigned char *y;
	unsigned char *u;
	unsigned char *v;
	unsigned int edged_width;
	unsigned int width;
	unsigned int height;
};

struct BLENDIMAGE
{
	void *pTextBits;
	int   nTextLeft;
	int   nTextTop;
	int   nTextWidth;
	int   nTextHeight;
	int   nTransColor;
};

void OutputImageToStream(IMAGE* img, BLENDIMAGE *pBlendImage);
int  RGB_2_YUV420(const unsigned char *pRGB, int nWidth, int nHeight, unsigned char *pY, unsigned char *pU, unsigned char *pV);

#endif
