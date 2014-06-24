//
//  ZJUIMethods.m
//  ybparents
//
//  Created by 郑敬 on 14-6-19.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJUIMethods.h"

@implementation ZJUIMethods
+(UIButton *)creatButton:(NSString *)name frame:(CGRect)frame  delegate:(id)delegate selector:(SEL)selector tag:(NSInteger)tag {
    
    
    UIButton *button=[UIButton buttonWithType:UIButtonTypeCustom];
    
    [button addTarget:delegate action:selector forControlEvents:UIControlEventTouchUpInside];
    
    button.tag=tag;
    
    button.frame=frame;
    [button setTitle:name forState:UIControlStateNormal];
    
    return button;
    
}

+(UILabel *)creatLabel:(NSString *)name frame:(CGRect)frame font:(UIFont *)font textColor:(UIColor *)textColor{
    
    UILabel *label=[[UILabel alloc]initWithFrame:frame];
    label.text=name;
    label.textColor=textColor;
    label.font=font;
    return label;
}

+(UITextField *)creatTextFiled:(NSString *)placeHolder frame:(CGRect)frame delegate:(id)delegate  tag:(NSInteger )tag{
    
    UITextField *textFiled=[[UITextField alloc]initWithFrame:frame];
    textFiled.delegate=delegate;
    textFiled.placeholder=placeHolder;
    textFiled.borderStyle=UITextBorderStyleRoundedRect;
    textFiled.tag=tag;
    return textFiled;
}
+(UIView *)creatView:(CGRect)frame bgColor:(UIColor *)bgColor{
    UIView *view=[[UIView alloc]initWithFrame:frame];
    view.backgroundColor=bgColor;
    return view;
}
+(UIImageView *)creatImageView:(CGRect)frame imageName:(NSString *)imageName {
    
    UIImageView *imageView=[[UIImageView alloc]initWithImage:[UIImage imageNamed:imageName]];
    imageView.frame=frame;
    imageView.userInteractionEnabled=YES;
    return imageView;
    
}
@end
