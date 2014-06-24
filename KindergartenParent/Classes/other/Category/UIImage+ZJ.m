//
//  UIImage+ZJ.m
//  Babytop
//
//  Created by 郑敬 on 14-5-10.
//  Copyright (c) 2014年 babytop. All rights reserved.
//

#import "UIImage+ZJ.h"

@implementation UIImage (ZJ)

// new_feature_1.png
+ (UIImage *)fullscrennImage:(NSString *)imgName
{
    // 1.如果是iPhone5，对文件名特殊处理
    //    if (iPhone5) {
    //        imgName = [imgName fileAppend:@"-568h@2x"];
    //    }
    
    // 2.加载图片
    return [self imageNamed:imgName];
}

#pragma mark 可以自由拉伸的图片
+ (UIImage *)resizedImage:(NSString *)imgName
{
    return [self resizedImage:imgName xPos:0.5 yPos:0.5];
}

+ (UIImage *)resizedImage:(NSString *)imgName xPos:(CGFloat)xPos yPos:(CGFloat)yPos
{
    UIImage *image = [UIImage imageNamed:imgName];
    return [image stretchableImageWithLeftCapWidth:image.size.width * xPos topCapHeight:image.size.height * yPos];
}

@end
