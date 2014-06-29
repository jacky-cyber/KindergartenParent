//
//  ZJFuYaoDanCell.m
//  ybparents
//
//  Created by xutao on 14-6-18.
//  Copyright (c) 2014年 xutao. All rights reserved.
//

#import "ZJFuYaoDanCell.h"
#import "ZJFuYaoDanModel.h"

@interface ZJFuYaoDanCell(){
    UILabel *_timeLabel;//时间标签
    UIImageView *_mornImg;//
    UIImageView *_noonImg;//中午
    UIImageView *_eveImg;//晚上
    
}

@end

@implementation ZJFuYaoDanCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code
        
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
  
    //四边红线
    UIImageView *imageViewL = [[UIImageView alloc]initWithFrame:CGRectMake(20, 5, 1, 35)];
    UIImageView *imageViewR = [[UIImageView alloc]initWithFrame:CGRectMake(300, 5, 1, 35)];
    UIImageView *imageViewU = [[UIImageView alloc]initWithFrame:CGRectMake(20, 5, 280, 1)];
    UIImageView *imageViewD = [[UIImageView alloc]initWithFrame:CGRectMake(20, 40, 280, 1)];
    imageViewL.image = [UIImage imageNamed:@"xian_03"];
    imageViewR.image = [UIImage imageNamed:@"xian_03"];
    imageViewU.image = [UIImage imageNamed:@"xian_03"];
    imageViewD.image = [UIImage imageNamed:@"xian_03"];
    [self.contentView addSubview:imageViewD];
    [self.contentView addSubview:imageViewU];
    [self.contentView addSubview:imageViewL];
    [self.contentView addSubview:imageViewR];
    
    //左边时间
    UILabel *timeLabel = [[UILabel alloc]initWithFrame:CGRectMake(30, 10, 80, 25)];
    timeLabel.text = @"2014-06-21";
    timeLabel.font = kFont13;
    _timeLabel = timeLabel;
    [self.contentView addSubview:timeLabel];
    
   
    //早中晚显示 全部显示出来  根据数据 去选择图片实心空心 没有服药 隐藏图片
    
    
    /*
    for (int i = 0; i<3; i++)
    {
        UIImageView *imageV = [[UIImageView alloc]initWithFrame:CGRectMake(120+i*60, 10, 25, 25)];
        imageV.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@",[imageArr objectAtIndex:i]]];
        
        [self.contentView addSubview:imageV];
        
    }
     */
    
    NSArray *imageArr = [[NSArray alloc]initWithObjects:@"zao_07",@"zhong_09",@"wan_11", nil];
    //早
    UIImageView *imageV1 = [[UIImageView alloc]initWithFrame:CGRectMake(120+0*60, 10, 25, 25)];
    imageV1.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@",[imageArr objectAtIndex:0]]];
    [self.contentView addSubview:imageV1];
    _mornImg = imageV1;
    //中
    UIImageView *imageV2 = [[UIImageView alloc]initWithFrame:CGRectMake(120+1*60, 10, 25, 25)];
    imageV2.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@",[imageArr objectAtIndex:1]]];
    [self.contentView addSubview:imageV2];
    _noonImg = imageV2;
    //晚
    UIImageView *imageV3 = [[UIImageView alloc]initWithFrame:CGRectMake(120+2*60, 10, 25, 25)];
    imageV3.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@",[imageArr objectAtIndex:2]]];
    [self.contentView addSubview:imageV3];
    _eveImg = imageV3;
    
    
    
    
}


-(void)setFydmodel:(ZJFuYaoDanModel *)fydmodel
{
    
    
    
    _fydmodel = fydmodel;
    
    //设置时间
    _timeLabel.text = [_fydmodel.time substringToIndex:10];
      if (![self isRang:@"早" withModel:fydmodel]) {
        _mornImg.alpha = 0;
    }else{
        _mornImg.alpha = 1;
    }
    
    if (![self isRang:@"中" withModel:fydmodel]) {
        _noonImg.alpha = 0;
    }else{
        _noonImg.alpha = 1;
    }
    
    if (![self isRang:@"晚" withModel:fydmodel]) {
        _eveImg.alpha = 0;
    }else{
        _eveImg.alpha = 1;
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
