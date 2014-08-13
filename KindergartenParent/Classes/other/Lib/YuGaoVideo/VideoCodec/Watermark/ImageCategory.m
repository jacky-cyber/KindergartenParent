//
//  ImageCategory.m
//  Camera
//
//  Created by apple on 12-4-19.
//  Copyright 2012 HuangSiwen. All rights reserved.
//

#import "ImageCategory.h"


@implementation UIImage (Watermark)

- (UIImage*)addWatermarkWithTime:(NSString *)time name:(NSString *)name {
    CGFloat fontSize = [UIFont labelFontSize];
    UIFont  *font = [UIFont systemFontOfSize:fontSize];
    NSString *fontName = font.fontName;
    
    UIGraphicsBeginImageContext(self.size);

    [self drawInRect:CGRectMake(0, 0, self.size.width, self.size.height)];
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextSetLineWidth(context, 1.0);
    CGContextSetRGBFillColor(context, 0.0, 0.0, 0.0, 1.0);
    CGContextSetRGBStrokeColor(context, 1.0, 1.0, 1.0, 1.0);
    CGContextSelectFont(context, [fontName UTF8String], fontSize, kCGEncodingMacRoman);
    CGContextSetTextMatrix(context, CGAffineTransformMakeScale(1.0, -1.0));
    CGContextSetTextDrawingMode(context, kCGTextStroke);
    
    CGContextShowTextAtPoint(context, 10, 10, [time UTF8String], [time length]);
    
    CGSize nameSize = [name sizeWithFont:font];
    CGFloat nameX = self.size.width  - nameSize.width  - 10;
    CGFloat nameY = self.size.height - nameSize.height - 10;
    nameX = (nameX < 0) ? 0 : nameX;
    nameY = (nameY < 0) ? 0 : nameY;
    CGContextShowTextAtPoint(context, nameX, nameY, [name UTF8String], [name length]);
    
    UIImage *resultingImage = UIGraphicsGetImageFromCurrentImageContext();
    
    CGContextRelease(context);
    
    return resultingImage;
}

-(UIImage*)addTextToImage:(const char *)charS leftPoint:(CGPoint)leftPoint typeSize:(int)typeSize {
    UIGraphicsBeginImageContext(self.size);
    [self drawInRect:CGRectMake(0, 0, self.size.width, self.size.height)];
    CGContextRef context = UIGraphicsGetCurrentContext();
    CGContextSetRGBFillColor(context, 1.0, 1.0, 1.0, 1.0);
    CGContextSetRGBStrokeColor(context, 1.0, 0.0, 0.0, 1.0);
    
    CGContextSelectFont(context, "Arial", typeSize, kCGEncodingMacRoman);
    CGContextSetTextMatrix(context, CGAffineTransformMakeScale(1.0, -1.0));
    
    
    CGContextSetTextDrawingMode(context, kCGTextFill);
    CGContextShowTextAtPoint(context, leftPoint.x, leftPoint.y, charS, strlen(charS));
    
    UIImage *resultingImage = UIGraphicsGetImageFromCurrentImageContext();
    
    CGContextRelease(context);
    return resultingImage;
}

@end
