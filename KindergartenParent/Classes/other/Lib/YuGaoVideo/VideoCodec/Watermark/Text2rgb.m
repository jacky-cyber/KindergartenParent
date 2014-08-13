//
//  Text2rgb.m
//  Camera
//
//  Created by apple on 12-4-19.
//  Copyright 2012 HuangSiwen. All rights reserved.
//

#import "Text2rgb.h"

const int bitsPerComponent = 8;
const int bytesPerPixel    = 4;


@implementation Text2rgb

- (id)init {
    if (self = [super init]) {
        m_pRgba = NULL;
        m_nRgbaLen = 0;
    }
    
    colorspace = CGColorSpaceCreateDeviceRGB();
    
    return self;
}

- (void)dealloc {
    
    if (m_pRgba != NULL) {
        free(m_pRgba);
        m_pRgba = NULL;
        m_nRgbaLen = 0;
    }
    
    CGColorSpaceRelease(colorspace);
    
    [super dealloc];
}

- (unsigned char *)rgbFromText:(const NSString *)text width:(int *)width height:(int *)height {
    
    CGFloat factor = [self adjustFontSizeFactorByImageWidth:*width];
    UIFont *font = [UIFont boldSystemFontOfSize:[UIFont labelFontSize]*factor];
    CGSize size = [text sizeWithFont:font constrainedToSize:CGSizeMake(*width, *height)];
    
    int w = ((int)size.width  + 1) & ~1;
    int h = ((int)size.height + 1) & ~1;
    
    unsigned char *data = [self adjustBuffer:w height:h];
    
    CGContextRef context = CGBitmapContextCreate(data,
                                                 w,
                                                 h,
                                                 bitsPerComponent,
                                                 bytesPerPixel*w,
                                                 colorspace,
                                                 kCGImageAlphaPremultipliedLast
                                                 );
    
    UIGraphicsPushContext(context);
    
    CGContextTranslateCTM(context, 0, h);
    CGContextScaleCTM(context, 1.0, -1.0);
    
    CGFloat lineWidth = 0.1*factor*factor;
    lineWidth = (factor <= 1) ? (0.3*factor) : lineWidth;
    CGContextSetLineWidth(context, lineWidth);
    CGContextSetRGBFillColor(context, 1.0, 1.0, 1.0, 1.0);
	CGContextSetRGBStrokeColor(context, 0.0, 0.0, 0.0, 1.0);
    CGContextSetTextDrawingMode(context, kCGTextFillStroke);
    
    [text drawInRect:CGRectMake(0, 0, w, h) withFont:font];
    
    UIGraphicsPopContext();
    
    CGContextRelease(context);    
    
    [self rgba2rgb:data width:w height:h];
    
    *width  = w;
    *height = h;
    
    return data;
}

- (unsigned char *)adjustBuffer:(int)width height:(int)height {
    
    int length = width * height * bytesPerPixel;
    
    if (m_pRgba == NULL || m_nRgbaLen == 0) {
        m_nRgbaLen = length;
        m_pRgba = malloc(m_nRgbaLen);
    } else if (length > m_nRgbaLen) {
        m_nRgbaLen = length;
        m_pRgba = realloc(m_pRgba, m_nRgbaLen);
    }
    memset(m_pRgba, 0, m_nRgbaLen);
    
    for (int i = 0; i < m_nRgbaLen; i += 4) {
        memset(m_pRgba + i+0, (BACKGROUND_RGBA >>  0) & 0xff, 1);
        memset(m_pRgba + i+1, (BACKGROUND_RGBA >>  8) & 0xff, 1);
        memset(m_pRgba + i+2, (BACKGROUND_RGBA >> 16) & 0xff, 1);
        memset(m_pRgba + i+3, (BACKGROUND_RGBA >> 24) & 0xff, 1);
    }    
    
    return m_pRgba;
}

- (unsigned char *)rgba2rgb:(unsigned char*)rgba width:(int)width height:(int)height {
    
    unsigned char* src = rgba;
    unsigned char* dst = rgba;
    
    for(int i = 0; i < width*height; i++, dst+=3, src+=4) {
        *(dst + 0) = *(src + 0);
        *(dst + 1) = *(src + 1);
        *(dst + 2) = *(src + 2);
    }
    
    return rgba;
}

- (CGFloat)adjustFontSizeFactorByImageWidth:(CGFloat)width {
    CGFloat factor = 1.0f;
 
    if (width > 1800) {         // 1080P
        factor = 6.0f;
    } else if (width > 1000) {  // 720P
        factor = 4.0f;
    } else if (width > 600) {   // D1, VGA
        factor = 2.0f;
    } else if (width > 300) {   // CIF, QVGA
        factor = 1.0f;
    } else if (width > 0) {     // QCIF, QQVGA
        factor = 0.8f;
    }
    
    return factor;
}

@end
