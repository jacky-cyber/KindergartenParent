//
//  Text2rgb.h
//  Camera
//
//  Created by apple on 12-4-19.
//  Copyright 2012 HuangSiwen. All rights reserved.
//

#import <Foundation/Foundation.h>

#define BACKGROUND_RGBA     0x77777777

@interface Text2rgb : NSObject {
    
    unsigned char *m_pRgba;
    int m_nRgbaLen;
    
    CGColorSpaceRef colorspace;
}

- (unsigned char *)rgbFromText:(const NSString *)text width:(int *)width height:(int *)height;
- (unsigned char *)adjustBuffer:(int)width height:(int)height;
- (unsigned char *)rgba2rgb:(unsigned char*)rgba width:(int)width height:(int)height;
- (CGFloat)adjustFontSizeFactorByImageWidth:(CGFloat)width;

@end
