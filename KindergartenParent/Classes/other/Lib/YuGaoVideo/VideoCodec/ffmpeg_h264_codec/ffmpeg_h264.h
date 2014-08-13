/*
 *  ffmpeg_h264.h
 *  Camera
 *
 *  Created by apple on 12-7-2.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */

#ifndef __FFMPEG_H264_H__
#define __FFMPEG_H264_H__

#include "vcodecvideo.h"
#include "ImageFlip.h"
#include "TextWatermarkYUV.h"
#include "SafeLock.h"

class CFFmpegH264Decoder {
    
public:
    CFFmpegH264Decoder();
    ~CFFmpegH264Decoder();
    
public:
    int Open();
    int Close();
    bool IsOpen();
    
public:
    int SetImageFlip(ImageFlipType flipType = IMG_FLIP_MIRRORV);
    int SetWatermarkText(const char *lefttop, const char *rightbottom);
    int Decode(const unsigned char *buf, const int bufLen, int *width, int *height);
    UIImage* GetCurrentImage();
    
private:
    void CacheYUVPicture();
    void ConvertYUV2RGB();
    UIImage* ImageFromAVPicture(AVPicture pict, int width, int height);
    
private:
    CriticalSection m_csDecoder;
    
    vdecodervideohandle *m_pDecoderHandle;
    vdecoder_video_param m_tDecoderParam;
    
    AVPicture m_tYUVPicture;
    AVPicture m_tRGBPicture;
	struct SwsContext *m_pSwsCtx;
    
    int m_nWidth, m_nHeight;
    PixelFormat m_tFormat;
    
    bool m_bSizeChanged;
    
private:
    ImageFlipType m_tFlipType;
    bool m_bWatermark;
    const char *m_pLefttopText;
    const char *m_pRightbottomText;
    CTextWatermarkYUV m_clsTextWatermark;
};


#endif
