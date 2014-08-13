//
//  ImageCategory.h
//  Camera
//
//  Created by apple on 12-4-19.
//  Copyright 2012 HuangSiwen. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface UIImage (Watermark)

- (UIImage*)addWatermarkWithTime:(NSString *)time name:(NSString *)name;
- (UIImage*)addTextToImage:(const char *)charS leftPoint:(CGPoint)leftPoint typeSize:(int)typeSize;

@end
