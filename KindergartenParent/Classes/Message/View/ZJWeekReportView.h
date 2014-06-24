//
//  ZJWeekReportView.h
//  ybparents
//
//  Created by 郑敬 on 14-6-18.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ZJWeekReportView : UIView

@property(nonatomic,strong) UILabel *titleLabel;//标题
@property(nonatomic,strong) UILabel *ftitleLabel;//标签1
@property(nonatomic,strong) UILabel *fcontentLabel;//内容1
@property(nonatomic,strong) UILabel *scontentLabel;//内容2
@property(nonatomic,strong) UILabel *stitleLabel;//标签2

@property(nonatomic,assign) BOOL isOne;//判断是否两个还是一个内容

-(void)setupFrame;

@end
