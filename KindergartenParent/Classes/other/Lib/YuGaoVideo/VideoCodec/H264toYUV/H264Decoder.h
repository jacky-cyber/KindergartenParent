/*
 *  H264Decoder.h
 *  Camera
 *
 *  Created by apple on 12-4-9.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */

#ifndef __H264DECODER_H__
#define __H264DECODER_H__

#include "h264dec.h"
#include "ConvertH264toYUV.h"
#include "YUV2RGB2BMP.h"
#include "TextWatermarkYUV.h"

#define MAX_YUV_LEN         (1920 * 1620) // 1080P: 1920*1080; yuvdata: *1.5


class CH264Decoder {
  
public:
    CH264Decoder();
    virtual ~CH264Decoder();
    
public:
    int DecoderOpen();
    int DecoderClose();
    bool DecoderIsOpen();
    
    int SetWatermakrText(const char *lefttop, const char *rightbottom);
    unsigned char* DecH264toBMP(const unsigned char *pH264Frame, const int nH264FrameLen, int *pWidth, int *pHeight, int *pBMPFrameLen);

private:
    int DecH264toYUV(const unsigned char *pH264Data, const int nH264Len,
                         unsigned char *pYUVData,  int *pYUVLen,
                         int *pWidth, int *pHeight, int *pTotalY, int *pTotalUV);
    
    static int ConvertYUVtoRGBtoBMP(unsigned char *pBMPData,
                                    const unsigned char *pYUVData, const int nYUVLen,
                                    const int nWidth, const int nHeight);
    
private:
    h264_dec_handle_t m_tHandle;
    volatile bool m_bIsOpen;
    volatile bool m_bIsDecing;
    
    unsigned char *m_pYUVFrame;
    int m_nYUVFrameLen;
    unsigned char *m_pBMPFrame;
    int m_nBMPFrameLen;

private:
    bool m_bWatermark;
    const char *m_pLefttopText;
    const char *m_pRightbottomText;
    CTextWatermarkYUV m_clsTextWatermarkYUV;
};


#endif
