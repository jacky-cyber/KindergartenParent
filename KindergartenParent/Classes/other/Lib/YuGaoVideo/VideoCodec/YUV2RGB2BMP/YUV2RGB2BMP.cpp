/*
 *  YUV2RGB2BMP.cpp
 *  Camera
 *
 *  Created by apple on 11-1-21.
 *  Copyright 2011 HuangSiwen. All rights reserved.
 *
 */

#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "yuvrgb24.h"
#include "rgb2bmp.h"


int ConvertYUVtoRGBtoBMP(unsigned char *pBMPData, unsigned char *pYUVData, int nYUVLen, int nWidth, int nHeight)
{
    debugPrintf("--------------------ConvertYUVtoRGBtoBMP start--------------------\n");

    int nFrameCount  = 0;
    int nYUVFrameLen = 0;
    int nRGBFrameLen = 0;
    
    nYUVFrameLen = nHeight * nWidth + ((nHeight * nWidth) >> 1);
    nRGBFrameLen = nHeight * nWidth * 3;
    
    assert(nYUVFrameLen != 0);
    if (nYUVFrameLen == 0) {
        return 0;
    }
    nFrameCount  = nYUVLen / nYUVFrameLen;
    debugPrintf("ConvertYUVtoRGBtoBMP: nFrameCount = %d\n", nFrameCount);
    
    unsigned char *pRGBData = pBMPData+54;
    if (pRGBData == NULL) {
        debugPrintf("ConvertYUVtoRGBtoBMP: pRGBData == NULL!\n");
        return 0;
    }

    unsigned char *pY = NULL;
    unsigned char *pU = NULL;
    unsigned char *pV = NULL; 
    
    for (int frameNO = 0; frameNO < nFrameCount; frameNO++)
    {
        memset(pRGBData, 0, nRGBFrameLen);
        
        pY = pYUVData + nYUVFrameLen * frameNO;
        pU = pY +  (nHeight * nWidth);
        pV = pU + ((nHeight * nWidth) >> 2);
        
        // Convert YUV to RGB
        ConvertYUVtoRGB(pY, pU, pV, pRGBData, nWidth, nHeight);
        
        // Convert RGB to BMP
        // ConvertRGBtoBMP("", pBMPData, nWidth, nHeight, pRGBData);
        FillBMPHeaderWithRGB(pBMPData, nWidth, nHeight);
        
        debugPrintf("ConvertYUVtoRGBtoBMP: frameNO = %d\n", frameNO);
    }
    
    pY = NULL;
    pU = NULL;
    pV = NULL;
    
    debugPrintf("-----------------------ConvertYUVtoRGBtoBMP End-------------------\n");
    
	return 0;
}
