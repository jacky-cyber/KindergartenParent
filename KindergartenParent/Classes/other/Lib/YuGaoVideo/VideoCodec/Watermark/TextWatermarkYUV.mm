/*
 *  TextWatermarkYUV.cpp
 *  Camera
 *
 *  Created by apple on 12-4-20.
 *  Copyright 2012 HuangSiwen. All rights reserved.
 *
 */

#include "TextWatermarkYUV.h"


CTextWatermarkYUV::CTextWatermarkYUV()
{
    m_objcText2rgb = [Text2rgb new];
    
    m_pTextyuv = NULL;
    m_nTextyuvLen = 0;
}

CTextWatermarkYUV::~CTextWatermarkYUV()
{
    [m_objcText2rgb release];
    
    if (m_pTextyuv != NULL) {
        free(m_pTextyuv);
        m_pTextyuv = NULL;
        m_nTextyuvLen = 0;
    }
}

int CTextWatermarkYUV::AddWatermark(unsigned char *yuv,
                                    const int width, const int height, const int stride,
                                    const char *lefttop, const char *rightbottom)
{
    unsigned char *yuv3[3] = {
        yuv,
        yuv + width*height,
        yuv + width*height*5/4,
    };
    
    AddWatermark(yuv3, width, height, stride, lefttop, rightbottom);
    
    return 0;
}

int CTextWatermarkYUV::AddWatermark(unsigned char *y, unsigned char *u, unsigned char *v, 
                 const int width, const int height, const int stride,
                 const char *lefttop, const char *rightbottom)
{
    unsigned char *yuv3[3] = {
        y,
        u,
        v,
    };
    
    AddWatermark(yuv3, width, height, stride, lefttop, rightbottom);
    
    return 0;
}

int CTextWatermarkYUV::AddWatermark(unsigned char *yuv[3],
                                    const int width, const int height, const int stride,
                                    const char *lefttop, const char *rightbottom)
{
    AddWatermark(yuv, width, height, stride, lefttop, MARK_POS_LEFTTOP);
    AddWatermark(yuv, width, height, stride, rightbottom, MAEK_POS_RIGHTBOTTOM);
    
    return 0;
}

int CTextWatermarkYUV::AddWatermark(unsigned char *yuv[3],
                                    const int width, const int height, const int stride,
                                    const char *text, mark_pos_t pos)
{    
    if (!yuv[0] || !yuv[1] || !yuv[2]) {
        return -1;
    }
    
    if (!text || strlen(text) <= 0) {
        return -1;
    }
    
    int textWidth  = width;
    int textHeight = height;
    unsigned char *ltrgb = [m_objcText2rgb rgbFromText:[NSString stringWithUTF8String:text] width:&textWidth height:&textHeight];
    
    int left = 0;
    int top  = 0;
    if (pos == MAEK_POS_RIGHTBOTTOM) {
        left = width  - textWidth;
        top  = height - textHeight;
        
        (left < 0) ? 0 : left;
        (top  < 0) ? 0 : top;
    }
    
    int newLen = textWidth * textHeight * 3/2;
    if (m_pTextyuv == NULL || m_nTextyuvLen == 0) {
        m_nTextyuvLen = newLen;
        m_pTextyuv = (unsigned char *)malloc(m_nTextyuvLen);
    } else if (newLen > m_nTextyuvLen) {
        m_nTextyuvLen = newLen;
        m_pTextyuv = (unsigned char *)realloc(m_pTextyuv, m_nTextyuvLen);
    }
    memset(m_pTextyuv, 0, m_nTextyuvLen);
    
    RGB_2_YUV420(ltrgb, textWidth, textHeight, m_pTextyuv, m_pTextyuv + textWidth*textHeight, m_pTextyuv + textWidth*textHeight*5/4);
    
    struct BLENDIMAGE textImage;
    textImage.pTextBits = m_pTextyuv;
    textImage.nTextLeft = left;
    textImage.nTextTop  = top;
    textImage.nTextWidth = textWidth;
    textImage.nTextHeight = textHeight;
    textImage.nTransColor = BACKGROUND_RGBA;
 
    struct IMAGE dstImage;
    dstImage.y = yuv[0];
    dstImage.u = yuv[1];
    dstImage.v = yuv[2];
    dstImage.edged_width = stride;
    dstImage.width  = width;
    dstImage.height = height;
    
    OutputImageToStream(&dstImage, &textImage);
    
    return 0;
}
