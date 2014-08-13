/*
 *  TextWatermarkYUV.h
 *  Camera
 *
 *  Created by apple on 12-4-20.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */

#ifndef __TEXT_WATERMARK_YUV_H__
#define __TEXT_WATERMARK_YUV_H__

#include "BlendText.h"
#include "Text2rgb.h"

typedef enum {
    MARK_POS_LEFTTOP,
    MAEK_POS_RIGHTBOTTOM,
} mark_pos_t;


class CTextWatermarkYUV {

public:
    CTextWatermarkYUV();
    virtual ~CTextWatermarkYUV();
    
public:
    int AddWatermark(unsigned char *yuv, const int width, const int height, const int stride,
                     const char *lefttop, const char *rightbottom);
    int AddWatermark(unsigned char *y, unsigned char *u, unsigned char *v, 
                     const int width, const int height, const int stride,
                     const char *lefttop, const char *rightbottom);
    int AddWatermark(unsigned char *yuv[3], const int width, const int height, const int stride,
                     const char *lefttop, const char *rightbottom);
    
private:
    int AddWatermark(unsigned char *yuv[3], const int width, const int height, const int stride,
                     const char *text, mark_pos_t pos);

private:
    Text2rgb *m_objcText2rgb;
    unsigned char *m_pTextyuv;
    int m_nTextyuvLen;
};


#endif
