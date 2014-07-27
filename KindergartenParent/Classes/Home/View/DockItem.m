//
//  DockItem.m
//  新浪微博
//
//  Created by apple on 13-10-27.
//  Copyright (c) 2013年 zhenjing. All rights reserved.
//

#import "DockItem.h"

#define kDockItemSelectedBG @"tabbar_slider.png"

// 文字的高度比例
#define kTitleRatio 0.3

@implementation DockItem

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // 1.文字居中
        self.titleLabel.textAlignment = NSTextAlignmentCenter;
        //self.titleLabel.backgroundColor = [UIColor blueColor];
        // 2.文字大小
        self.titleLabel.font = [UIFont systemFontOfSize:15];
        
        //字体颜色
        [self setTitleColor:[[UIColor blackColor] colorWithAlphaComponent:0.9] forState:UIControlStateNormal];
//        [self setTitleColor:[UIColor colorWithRed:0.114 green:0.690 blue:0.424 alpha:1.000] forState:UIControlStateSelected];
        
        // 3.图片的内容模式
        self.imageView.contentMode = UIViewContentModeTop;
       // self.imageView.backgroundColor = [UIColor yellowColor];
        
        // 4.设置选中时的背景
        [self setBackgroundImage:[UIImage imageNamed:kDockItemSelectedBG] forState:UIControlStateSelected];
    }
    return self;
}

#pragma mark 覆盖父类在highlighted时的所有操作
- (void)setHighlighted:(BOOL)highlighted {
//    [super setHighlighted:highlighted];
}

#pragma mark 调整内部ImageView的frame
- (CGRect)imageRectForContentRect:(CGRect)contentRect
{
    CGFloat imageX = 0;
    CGFloat imageY = 0;
    CGFloat imageWidth = contentRect.size.width;
    CGFloat imageHeight = contentRect.size.height * 0.5;
    return CGRectMake(imageX, imageY, imageWidth, imageHeight);
}

#pragma mark 调整内部UILabel的frame
- (CGRect)titleRectForContentRect:(CGRect)contentRect
{
    CGFloat titleX = 0;
    CGFloat titleHeight = 20;
    CGFloat titleY = (contentRect.size.height )/2;
    CGFloat titleWidth = contentRect.size.width;
    return CGRectMake(titleX, titleY, titleWidth, titleHeight);
}

@end
