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
    
    
    //早中晚显示 全部显示出来  根据数据 去选择图片实心空心 没有服药 隐藏图片
    
    
    NSArray *imageArr = @[@"zao",@"zhong",@"wan"];
    NSArray *imageArrH = @[@"zao_h",@"zhong_h",@"wan_h"];
    for (int i = 0; i<3; i++)
    {
        UIButton *statusBtn = [[UIButton alloc]initWithFrame:CGRectMake(120+i*60, 10, 25, 25)];
        [statusBtn setImage:[UIImage imageNamed:imageArr[i]] forState:UIControlStateNormal];
        [statusBtn setImage:[UIImage imageNamed:imageArrH[i]] forState:UIControlStateSelected];
        [statusBtn setImage:[UIImage imageNamed:imageArrH[i]] forState:UIControlStateHighlighted];
        //imageV1.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@",[imageArr objectAtIndex:0]]];
        [self addSubview:statusBtn];
        if (i==0) {//早
            _mornImg = statusBtn;
        }else if (i==1){//中
            _noonImg = statusBtn;
        }else if (i==2){//晚
            _eveImg = statusBtn;
        }
    }
}


-(void)setFydmodel:(ZJFuYaoDanModel *)fydmodel
{
    
    _fydmodel = fydmodel;
    
    
    //设置时间
    _timeLabel.text = [_fydmodel.time substringToIndex:10];
    
    if (![self isRang:@"早" withModel:fydmodel]) {
        _mornImg.alpha = kAlpha;
        _mornImg.enabled = NO;
    }else{
        _mornImg.alpha = 1;
    }
    
    if (![self isRang:@"中" withModel:fydmodel]) {
        _noonImg.alpha = kAlpha;
        _noonImg.enabled = NO;
    }else{
        _noonImg.alpha = 1;
    }
    
    if (![self isRang:@"晚" withModel:fydmodel]) {
        _eveImg.alpha = kAlpha;
        _eveImg.enabled = NO;
    }else{
        _eveImg.alpha = 1;
    }
    
    
    //设置是否被已经服药
    
    

    //0 未服药 1 已服药
    if ([fydmodel.moringstatus isEqual:@(1)]) {
        _mornImg.selected = YES;
    }
    if ([fydmodel.noonstatus isEqual:@(1)]) {
        _noonImg.selected = YES;
    }
    if ([fydmodel.evestatus isEqual:@(1)]) {
        _eveImg.selected = YES;
    }
    
    
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


- (void)setSelected:(BOOL)selected animated:(BOOL)animated
{
    [super setSelected:selected animated:animated];
    
    // Configure the view for the selected state
}

@end
