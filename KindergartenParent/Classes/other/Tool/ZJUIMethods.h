//
//  ZJUIMethods.h
//  ybparents
//
//  Created by 郑敬 on 14-6-19.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ZJUIMethods : NSObject
//创建按钮
+(UIButton *)creatButton:(NSString *)name frame:(CGRect)frame  delegate:(id)delegate selector:(SEL)selector tag:(NSInteger)tag ;

//创建标签
+(UILabel *)creatLabel:(NSString *)name frame:(CGRect)frame font:(UIFont *)font textColor:(UIColor *)textColor;
//创建输入框
+(UITextField *)creatTextFiled:(NSString *)placeHolder frame:(CGRect)frame delegate:(id)delegate  tag:(NSInteger )tag;

//创建视图
+(UIView *)creatView:(CGRect)frame bgColor:(UIColor *)bgColor ;

//创建图片视图
+(UIImageView *)creatImageView:(CGRect)frame imageName:(NSString *)imageName ;
@end
