//
//  ZJWeekReportView.m
//  ybparents
//
//  Created by 郑敬 on 14-6-18.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJWeekReportView.h"
#define kMarginL 10
@implementation ZJWeekReportView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        self.layer.borderColor = [UIColor colorWithRed:0.945 green:0.333 blue:0.533 alpha:1.000].CGColor;
        self.layer.borderWidth = 1.0;
        self.layer.cornerRadius = 2;
        self.layer.masksToBounds = YES;
        [self addSubViews];
        
        //重新设置
       // [self setupFrame];
        
    }
    return self;
}
#pragma mark 添加子视图
-(void)addSubViews
{
    
    
    //1。标题
    _titleLabel = [[UILabel alloc] init];
    _titleLabel.frame = CGRectMake(10, 10, 200, 20);
    _titleLabel.textColor = [UIColor colorWithRed:0.945 green:0.333 blue:0.533 alpha:1.000];
    [self addSubview:_titleLabel];
    //2.分割线
    UIView *lineView = [[UIView alloc] init];
    lineView.frame = CGRectMake(kMarginL, 33, 260, 1);
    lineView.backgroundColor = [UIColor colorWithRed:0.945 green:0.333 blue:0.533 alpha:1.000];
    [self addSubview:lineView];
    
    
    //3.1 底部第一个title
    _ftitleLabel = [[UILabel alloc] init];
    _ftitleLabel.frame = CGRectMake(kMarginL, 40, 40, 21);
    [self addSubview:_ftitleLabel];
    //3.2 底部第一个内容
    _fcontentLabel = [[UILabel alloc] init];
    _fcontentLabel.numberOfLines = 0;
    _fcontentLabel.lineBreakMode = NSLineBreakByWordWrapping;
    _fcontentLabel.frame = CGRectMake(XW(_ftitleLabel)+5, 40, 215, 21);
    _fcontentLabel.textColor = [UIColor grayColor];
    //_fcontentLabel.backgroundColor = [UIColor redColor];
    [self addSubview:_fcontentLabel];
    
    //4.1 底部第二个title
    _stitleLabel = [[UILabel alloc] init];
    _stitleLabel.frame = CGRectMake(kMarginL, YH(_fcontentLabel)+5, 40, 21);
    [self addSubview:_stitleLabel];
    //4.2 底部第二个内容
    _scontentLabel = [[UILabel alloc] init];
    _scontentLabel.numberOfLines = 0;
    _scontentLabel.frame = CGRectMake(XW(_ftitleLabel)+5, 40, 215, 21);
    _scontentLabel.textColor = [UIColor grayColor];
    _scontentLabel.lineBreakMode = NSLineBreakByWordWrapping;
    [self addSubview:_scontentLabel];
 
    
}

-(void)setupFrame
{

    if (_isOne) {
        
        [_ftitleLabel removeFromSuperview];
        [_stitleLabel removeFromSuperview];
        [_scontentLabel removeFromSuperview];
        CGFloat height1 = [_fcontentLabel.text getHeightByWidth:260 font:kFont(17)];
        _fcontentLabel.frame = CGRectMake(kMarginL, 40, 260, height1);
        
        CGRect frame = self.frame;
        frame.size.height = YH(_fcontentLabel)+10;
        self.frame = frame;
        
    }else{

        CGFloat height1 = [_fcontentLabel.text getHeightByWidth:210 font:kFont(17)];
        CGRect fcFrame = _fcontentLabel.frame;
        fcFrame.size.height = height1;
        _fcontentLabel.frame = fcFrame;

        //设置第二个title Y
        CGRect stFrame = _stitleLabel.frame;
        stFrame.origin.y = YH(_fcontentLabel)+5;
        _stitleLabel.frame = stFrame;


        //设置第二个高度和Y
        CGFloat height2 = [_scontentLabel.text getHeightByWidth:210 font:kFont(17)];
        CGRect scFrame = _scontentLabel.frame;
        scFrame.origin.y = Y(_stitleLabel);
        scFrame.size.height = height2;
        _scontentLabel.frame = scFrame;
        
        CGRect frame = self.frame;
        frame.size.height = YH(_scontentLabel)+10;
        self.frame = frame;
    }
    
    
    
    
    
}
@end
