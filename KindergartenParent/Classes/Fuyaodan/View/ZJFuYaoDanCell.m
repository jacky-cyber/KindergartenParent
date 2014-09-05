//
//  ZJFuYaoDanCell.m
//  ybparents
//
//  Created by xutao on 14-6-18.
//  Copyright (c) 2014年 xutao. All rights reserved.
//

#import "ZJFuYaoDanCell.h"
#import "ZJFuYaoDanModel.h"

#define kAlpha 0.5

@interface ZJFuYaoDanCell(){
    UILabel *_timeLabel;//时间标签
    
    
}

@end

@implementation ZJFuYaoDanCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code
        self.userInteractionEnabled = YES;
        [self addSubviews];
    }
    return self;
}

- (void)awakeFromNib
{
    // Initialization code
}
-(void)addSubviews
{
    
    
    
    UIView *bgView = [[UIView alloc] initWithFrame:CGRectMake(20, 5, 280, 35)];
    bgView.layer.borderColor = [UIColor redColor].CGColor;
    bgView.userInteractionEnabled = YES;
    bgView.layer.borderWidth = 0.5;
    [self addSubview:bgView];
    
    
    
    //左边时间
    UILabel *timeLabel = [[UILabel alloc]initWithFrame:CGRectMake(30, 10, 80, 25)];
    //timeLabel.text = @"2014-06-21";
    timeLabel.font = kFont13;
    _timeLabel = timeLabel;
    [self.contentView addSubview:timeLabel];
    
    
    
    
}


-(void)setFydmodel:(ZJFuYaoDanModel *)fydmodel
{
    
    _fydmodel = fydmodel;
    
    
    //设置时间
    _timeLabel.text = [_fydmodel.time substringToIndex:10];
    
    
    //早中晚显示 全部显示出来  根据数据 去选择图片实心空心 没有服药 隐藏图片
    
    if (!_mornImg) {
        _mornImg = [[UIImageView alloc] initWithFrame:CGRectMake(120+0*60, 10, 25, 25)];
    }
    
    if (![self isRang:@"早" withModel:fydmodel]) {
        //        _mornImg.alpha = kAlpha;
        _mornImg.image = [UIImage imageNamed:@"fuyao_morning"];
        
    }else{
       _mornImg.image = [UIImage imageNamed:@"zao"];
    }
    
    [self addSubview:_mornImg];
    
    
    if (!_noonImg) {
        _noonImg = [[UIImageView alloc] initWithFrame:CGRectMake(120+1*60, 10, 25, 25)];
    }
    if (![self isRang:@"中" withModel:fydmodel]) {
        //        _noonImg.alpha = kAlpha;
        _noonImg.image = [UIImage imageNamed:@"fuyao_noon"];
        
    }else{
        _noonImg.image = [UIImage imageNamed:@"zhong"];
    }
    
    [self addSubview:_noonImg];
    
    if (!_eveImg) {
        _eveImg = [[UIImageView alloc] initWithFrame:CGRectMake(120+2*60, 10, 25, 25)];
    }
    if (![self isRang:@"晚" withModel:fydmodel]) {
        //        _eveImg.alpha = kAlpha;
        _eveImg.image = [UIImage imageNamed:@"fuyao_afternoon"];
        
    }else{
        _eveImg.image = [UIImage imageNamed:@"wan"];
    }
    
    [self addSubview:_eveImg];

    
    
    
    
   
    
    
    
    
    
    

    
}
#pragma mark 看是否包含
-(BOOL)isRang:(NSString*)str withModel:(ZJFuYaoDanModel*)model
{
    BOOL flag = true;
    //
    NSRange rang = [model.fuyaotime rangeOfString:str];//判断字符串是否包含
    if (rang.length == 0)//不包含
    {
        flag = false;
    }
    return flag;
}

-(void)layoutSubviews
{
    [super layoutSubviews];
    
  
    //设置是否被已经服药
    if (_fydmodel.moringstatus.intValue == 1) {
        _mornImg.image = [UIImage imageNamed:@"zao_h"];
    }else{
        if (![self isRang:@"中" withModel:_fydmodel]) {
            //        _noonImg.alpha = kAlpha;
            _noonImg.image = [UIImage imageNamed:@"fuyao_noon"];
            
        }else{
            _noonImg.image = [UIImage imageNamed:@"zhong"];
        }
        //_mornImg.image = [UIImage imageNamed:@"zao"];
    }
    
    if (_fydmodel.noonstatus.intValue == 1) {
        _noonImg.image = [UIImage imageNamed:@"zhong_h"];
    }else{
        if (![self isRang:@"中" withModel:_fydmodel]) {
            //        _noonImg.alpha = kAlpha;
            _noonImg.image = [UIImage imageNamed:@"fuyao_noon"];
            
        }else{
            _noonImg.image = [UIImage imageNamed:@"zhong"];
        }
    }
    
    if (_fydmodel.evestatus.intValue == 1) {
        _eveImg.image = [UIImage imageNamed:@"wan_h"];
    }else{
        if (![self isRang:@"晚" withModel:_fydmodel]) {
            //        _eveImg.alpha = kAlpha;
            _eveImg.image = [UIImage imageNamed:@"fuyao_afternoon"];
            
        }else{
            _eveImg.image = [UIImage imageNamed:@"wan"];
        }

    }

    
}
@end
