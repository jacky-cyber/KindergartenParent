/*
 *  ffmpeg_h264.cpp
 *  Camera
 *
 *  Created by apple on 12-7-2.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */

#include <unistd.h>
#include "ffmpeg_h264.h"


CFFmpegH264Decoder::CFFmpegH264Decoder()
{
    m_pDecoderHandle = NULL;
    
    m_pSwsCtx = NULL;
    
    m_nWidth  = 0;
    m_nHeight = 0;
    m_tFormat = PIX_FMT_NONE;
    m_bSizeChanged = false;
    
    m_tFlipType = IMG_FLIP_NONE;
    m_bWatermark = false;
}
CFFmpegH264Decoder::~CFFmpegH264Decoder()
{
    avpicture_free(&m_tYUVPicture);
    avpicture_free(&m_tRGBPicture);
    sws_freeContext(m_pSwsCtx);
}

UIImage* CFFmpegH264Decoder::GetCurrentImage()
{
    CSafeLock lock(&m_csDecoder);
    
    if (m_pDecoderHandle == NULL) {
        return nil;
    }
    if (!m_tDecoderParam.data[0]) {
        return nil;   
    }
    
    CacheYUVPicture();
    
    if (m_tFlipType != IMG_FLIP_NONE) {
        YUVImage yuv = {
            m_tYUVPicture.data[0],
            m_tYUVPicture.data[1],
            m_tYUVPicture.data[2],
        };
        CImageFlip::ImageFlip(m_tFlipType, &yuv, m_tYUVPicture.linesize[0], m_nWidth, m_nHeight);
    }
    m_tFlipType = IMG_FLIP_NONE;
   
    if (m_bWatermark) {
        unsigned char *yuv[3] = {
            m_tYUVPicture.data[0],
            m_tYUVPicture.data[1],
            m_tYUVPicture.data[2],
        };
        //TODO添加水印功能
//        m_clsTextWatermark.AddWatermark(yuv, m_nWidth, m_nHeight, m_tYUVPicture.linesize[0], m_pLefttopText, m_pRightbottomText);
    }
    m_bWatermark = false;
    
    ConvertYUV2RGB();
    
    return ImageFromAVPicture(m_tRGBPicture, m_nWidth, m_nHeight);
}

bool CFFmpegH264Decoder::IsOpen()
{
    return (m_pDecoderHandle != NULL);
}

int CFFmpegH264Decoder::Open()
{
    CSafeLock lock(&m_csDecoder);
    
    m_pDecoderHandle = vdecoder_video_open(vcodec_video_h264);
    if (m_pDecoderHandle == NULL) {
        return -1;
    }
    memset(&m_tDecoderParam, 0, sizeof(vdecoder_video_param));
    
    return 0;
}

int CFFmpegH264Decoder::Close()
{
    CSafeLock lock(&m_csDecoder);
    
    if (m_pDecoderHandle != NULL) {
        vdecoder_video_close(m_pDecoderHandle);
        m_pDecoderHandle = NULL;
    }
    memset(&m_tDecoderParam, 0, sizeof(vdecoder_video_param));
    
    return 0;
}

int CFFmpegH264Decoder::Decode(const unsigned char *buf, const int bufLen, int *width, int *height)
{
    CSafeLock lock(&m_csDecoder);
    
    if (m_pDecoderHandle == NULL) {
        return -1;
    }
    
    memset(&m_tDecoderParam, 0, sizeof(vdecoder_video_param));
    m_tDecoderParam.buf    = buf;
    m_tDecoderParam.bufLen = bufLen;
    
    if (vdecoder_video_decode(m_pDecoderHandle, &m_tDecoderParam) < 0) {
        printf("CFFmpegH264Decoder::Decode: vdecoder_video_decode < 0.\n"); 
        return -1;
    }
    
    if ((m_nWidth != m_tDecoderParam.width) || (m_nHeight != m_tDecoderParam.height)) {
        m_nWidth  = m_tDecoderParam.width;
        m_nHeight = m_tDecoderParam.height;
        m_tFormat = VFormat2PFormat(m_tDecoderParam.format);
        
        m_bSizeChanged = true;
    } else {
        m_bSizeChanged = false;
    }
    
    *width  = m_nWidth;
    *height = m_nHeight;
    
    // printf("CFFmpegH264Decoder::Decode: width = %d; height = %d; stride = %d.\n", m_tDecoderParam.width, m_tDecoderParam.height, m_tDecoderParam.linesize[0]);
    
    return 0;
}

int CFFmpegH264Decoder::SetImageFlip(ImageFlipType flipType /*= IMG_FLIP_MIRRORV*/)
{
    m_tFlipType = flipType;
    
    return 0;
}

int CFFmpegH264Decoder::SetWatermarkText(const char *lefttop, const char *rightbottom)
{
    m_bWatermark = true;
    m_pLefttopText = lefttop;
    m_pRightbottomText = rightbottom;
    
    return 0;
}

void CFFmpegH264Decoder::CacheYUVPicture()
{
    if (!m_tDecoderParam.data[0]) {
        return;   
    }
    
    AVPicture tmp;
    tmp.data[0] = m_tDecoderParam.data[0];
    tmp.data[1] = m_tDecoderParam.data[1];
    tmp.data[2] = m_tDecoderParam.data[2];
    tmp.linesize[0] = m_tDecoderParam.linesize[0];
    tmp.linesize[1] = m_tDecoderParam.linesize[1];
    tmp.linesize[2] = m_tDecoderParam.linesize[2];
    
    if (m_bSizeChanged) {
        avpicture_free(&m_tYUVPicture);
        avpicture_alloc(&m_tYUVPicture, m_tFormat, m_nWidth, m_nHeight);
    }
    av_picture_copy(&m_tYUVPicture, &tmp, m_tFormat, m_nWidth, m_nHeight);    
}

void CFFmpegH264Decoder::ConvertYUV2RGB()
{
    if (m_bSizeChanged) {
        avpicture_free(&m_tRGBPicture);
        sws_freeContext(m_pSwsCtx);
        
        avpicture_alloc(&m_tRGBPicture, PIX_FMT_RGB24, m_nWidth, m_nHeight);
        m_pSwsCtx = sws_getContext(m_nWidth, 
                                   m_nHeight,
                                   m_tFormat,
                                   m_nWidth, 
                                   m_nHeight,
                                   PIX_FMT_RGB24,
                                   SWS_FAST_BILINEAR,
                                   NULL, NULL, NULL);
    }
    
    if (!m_tYUVPicture.data[0]) return;
    if (!m_tRGBPicture.data[0]) return;
    
	sws_scale(m_pSwsCtx,
              m_tYUVPicture.data, m_tYUVPicture.linesize, 0, m_nHeight,
              m_tRGBPicture.data, m_tRGBPicture.linesize);
}

UIImage* CFFmpegH264Decoder::ImageFromAVPicture(AVPicture pict, int width, int height)
{
	CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CGColorRenderingIntent intent = kCGRenderingIntentDefault;
	CFDataRef data = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, pict.data[0], pict.linesize[0]*height,kCFAllocatorNull);
	CGDataProviderRef provider = CGDataProviderCreateWithCFData(data);
	CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
	CGImageRef cgImage = CGImageCreate(width, 
									   height, 
									   8, 
									   24, 
									   pict.linesize[0], 
									   colorSpace, 
									   bitmapInfo, 
									   provider, 
									   NULL, 
									   NO, 
									   intent);

	UIImage *image = [UIImage imageWithCGImage:cgImage];

	CGImageRelease(cgImage);
    CGColorSpaceRelease(colorSpace);
	CGDataProviderRelease(provider);
	CFRelease(data);
	
	return image;
}
