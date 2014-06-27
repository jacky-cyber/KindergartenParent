//
//  ZJCookBookTableViewCell.m
//  ybparents
//
//  Created by Define on 14-6-9.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJCookBookTableViewCell.h"
#import "CookBookModel.h"

#define kDinerW = 250

@implementation ZJCookBookTableViewCell


- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [[[NSBundle mainBundle]loadNibNamed:@"ZJCookBookTableViewCell" owner:self options:nil]objectAtIndex:0];
    if (self)
    {

        //设置子视图frame
        [self settingSubviewFrame];
    }
    return self;
}

-(void)setModel:(CookBookModel *)model
{
    _model = model;
    
    _breakFirstLabel.text = model.breakfast;
    
    _supperLabel.text = model.supper;
    
    _noonLabel.text = model.lunch;
    
    _jiacanLabel.text = model.jiacan;
    
    [self settingSubviewFrame];
}


- (void)settingSubviewFrame
{
    
    int dinerW = 230;
    
    //设置早餐高度
    CGFloat breakFirstH  = [_model.breakfast getHeightByWidth:dinerW font:kFont(17)];
    CGRect bFrame = _breakFirstLabel.frame;
    bFrame.size.height = breakFirstH;
    _breakFirstLabel.frame = bFrame;
    
    CGRect bVFrame = _breakfirstV.frame;
    bVFrame.size.height = H(_breakFirstLabel);
    _breakfirstV.frame = bVFrame;
    
    
    //设置午餐的高度
    CGFloat lunchH  = [_model.lunch getHeightByWidth:dinerW font:kFont(17)];
    CGRect lFrame = _noonLabel.frame;
    lFrame.size.height = lunchH;
    _noonLabel.frame = lFrame;


    CGRect nVFrame = _noonV.frame;
    nVFrame.size.height = H(_noonLabel);
    nVFrame.origin.y = YH(_breakfirstV)+5;
    _noonV.frame = nVFrame;
    
    //设置晚餐的高度
    CGFloat supperH  = [_model.supper getHeightByWidth:dinerW font:kFont(17)];
    CGRect sFrame = _supperLabel.frame;
    sFrame.size.height = supperH;
    _supperLabel.frame = sFrame;
    
    CGRect sVFrame = _supperV.frame;
    sVFrame.size.height = H(_supperLabel);
    sVFrame.origin.y = YH(_noonV)+5;
    _supperV.frame = sVFrame;
    
    
    //设置加餐的高度
    CGFloat jiacanH  = [_model.jiacan getHeightByWidth:dinerW font:kFont(17)];
    CGRect jFrame = _jiacanLabel.frame;
    jFrame.size.height = jiacanH;
    _jiacanLabel.frame = jFrame;
    
    CGRect jVFrame = _jiacanV.frame;
    jVFrame.size.height = H(_jiacanLabel);
    jVFrame.origin.y = YH(_supperV)+5;
    _jiacanV.frame = jVFrame;

    _cellH = YH(_jiacanV);
    
}
+(NSString *)ID
{
    return @"Cell";
}
+(CGFloat)cellHeight
{
    return 255;
}
@end
