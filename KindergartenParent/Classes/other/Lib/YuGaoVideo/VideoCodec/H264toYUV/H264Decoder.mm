/*
 *  H264Decoder.cpp
 *  Camera
 *
 *  Created by apple on 12-4-9.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "H264Decoder.h"

CH264Decoder::CH264Decoder()
{
    m_tHandle = NULL;
    m_bIsOpen = false;
    m_bIsDecing = false;
    
    m_nYUVFrameLen = MAX_YUV_LEN;
    m_pYUVFrame = (unsigned char*)malloc(MAX_YUV_LEN);
    m_nBMPFrameLen = 0;
    m_pBMPFrame = NULL;
    
    m_bWatermark = false;
    m_pLefttopText = NULL;
    m_pRightbottomText = NULL;
}
CH264Decoder::~CH264Decoder()
{
    m_bIsOpen = false;
    if (m_tHandle != NULL) {
        h264_dec_close(m_tHandle);
        m_tHandle = NULL;
    }
    
    if (m_pYUVFrame != NULL) {
        free(m_pYUVFrame);
        m_pYUVFrame = NULL;
        m_nYUVFrameLen = 0;
    }
    
    if (m_pBMPFrame != NULL) {
        free(m_pBMPFrame);
        m_pBMPFrame = NULL;
        m_nBMPFrameLen = 0;
    }
}

int CH264Decoder::DecoderOpen()
{
    printf("CH264Decoder::DecoderOpen: h264_dec_open.\n");
    if (m_tHandle != NULL) {
        h264_dec_close(m_tHandle);
        m_tHandle = NULL;
    }
    m_tHandle = h264_dec_open();
    
    m_bIsOpen = true;
    m_bIsDecing = false;
    
    return 0;
}

int CH264Decoder::DecoderClose()
{
    printf("CH264Decoder::DecoderClose: h264_dec_close.\n");
    
    static int count = 0;
    while (m_bIsDecing) {
        printf("CH264Decoder::DecoderClose: (%u) sleep while decing (count=%d)....\n", time(NULL), count);
        usleep(1000); // 0.001s
        
        if (count++ >= 1000) {
            count = 0;
            break;
        }
    }
    
    m_bIsDecing = false;
    m_bIsOpen = false;
    if (m_tHandle != NULL) {
        h264_dec_close(m_tHandle);
        m_tHandle = NULL;
        printf("CH264Decoder::DecoderClose: h264_dec_close done.\n");
    }
    
    return 0;
}

bool CH264Decoder::DecoderIsOpen()
{
    return m_bIsOpen;
}

int CH264Decoder::SetWatermakrText(const char *lefttop, const char *rightbottom)
{
    m_bWatermark = true;
    m_pLefttopText = lefttop;
    m_pRightbottomText = rightbottom;
    
    return 0;
}

unsigned char* CH264Decoder::DecH264toBMP(const unsigned char *pH264Frame, const int nH264FrameLen, int *pWidth, int *pHeight, int *pBMPFrameLen)
{
    int nYUVFrameLen  = 0;
    int nFrameTotalY  = 0;
    int nFrameTotalUV = 0;
    
    if (this->DecH264toYUV(pH264Frame, nH264FrameLen, 
                           m_pYUVFrame, &nYUVFrameLen, 
                           pWidth, pHeight, &nFrameTotalY, &nFrameTotalUV) < 0) {
        return NULL;
    }
    
    if (m_bWatermark) {
        m_clsTextWatermarkYUV.AddWatermark(m_pYUVFrame, *pWidth, *pHeight, *pWidth, m_pLefttopText, m_pRightbottomText);
    }
    m_bWatermark = false;
    
    if (nYUVFrameLen != (*pWidth) * (*pHeight) * 3/2) {
         debugPrintf("CH264Decoder::DecH264toBMP: nYUVFrameLen = %d; width = %d; height = %d.(w*h*1.5 = %d).\n", nYUVFrameLen, (*pWidth), (*pHeight), (*pWidth) * (*pHeight) * 3/2);
    }
    
    //assert(nYUVFrameLen == (*pWidth) * (*pHeight) * 3/2);
    *pBMPFrameLen = nYUVFrameLen * 2 + 54; //// #define BMPHEADER_LEN    54
    
    if (m_nBMPFrameLen <= 0) {
        m_nBMPFrameLen = *pBMPFrameLen;
        m_pBMPFrame = (unsigned char*)malloc(m_nBMPFrameLen);
    } else if (*pBMPFrameLen != m_nBMPFrameLen) {
        m_nBMPFrameLen = *pBMPFrameLen;
        m_pBMPFrame = (unsigned char*)realloc(m_pBMPFrame, m_nBMPFrameLen);
    }
    
    this->ConvertYUVtoRGBtoBMP(m_pBMPFrame, m_pYUVFrame, nYUVFrameLen, *pWidth, *pHeight);
    
    return m_pBMPFrame;
}

int CH264Decoder::DecH264toYUV(const unsigned char *pH264Data, const int nH264Len,
                     unsigned char *pYUVData,  int *pYUVLen,
                     int *pWidth, int *pHeight, int *pTotalY, int *pTotalUV)
{
    if (!m_bIsOpen) {
        printf(" CH264Decoder::ConvertH264toYUV: dec handle doesn't open!!!\n");
        return -1;
    }

    m_bIsDecing = true;
    
    int ret = 0;
    try {
        ret = ::ConvertH264toYUV(m_tHandle, (unsigned char *)pH264Data, nH264Len,
                           pYUVData, pYUVLen,
                           pWidth, pHeight, pTotalY, pTotalUV);
    } catch (...) {
        printf("CH264Decoder::ConvertH264toYUV: catch some exception.\n");
        return -1;
    }
    
    m_bIsDecing = false;
    
    return ret;
}

int CH264Decoder::ConvertYUVtoRGBtoBMP(unsigned char *pBMPData, const unsigned char *pYUVData, const int nYUVLen, const int nWidth, const int nHeight)
{
    return  ::ConvertYUVtoRGBtoBMP(pBMPData, (unsigned char*)pYUVData, nYUVLen, nWidth, nHeight);
}
