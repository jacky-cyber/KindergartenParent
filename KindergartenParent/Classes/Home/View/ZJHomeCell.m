//
//  ZJHomeCell.m
//  ybparents
//
//  Created by Define on 14-6-6.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJHomeCell.h"

#define kLableH 25

@interface ZJHomeCell()
{
   

}
@end

@implementation ZJHomeCell

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self)
    {
        UIView *backView = [[UIView alloc] init];
        [self setBackgroundView:backView];
       //添加子视图
        [self addSubviews];
        //设置子视图frame
        [self settingSubviewFrame];
    }
    return self;
}
-(void)addSubviews
{
    //0.状态图
    _typeImg = [[UIImageView alloc] init];
    [self.contentView addSubview:_typeImg];
    
    //1.标题
    _titleLb = [[UILabel alloc] init];
    _titleLb.font = kFont(18);
    //_titleLb.backgroundColor = [UIColor colorWithRed:0.800 green:1.000 blue:0.400 alpha:1.000];
    [self.contentView addSubview:_titleLb];
    //2.创建时间
    _timeLb = [[UILabel alloc] init];
    _timeLb.font = kFont(11);
    _timeLb.textAlignment = NSTextAlignmentRight;
    //_timeLb.backgroundColor = [UIColor colorWithRed:0.400 green:1.000 blue:0.800 alpha:1.000];
    [self.contentView addSubview:_timeLb];
    //3.内容
    _contentLb = [[UILabel alloc] init];
    _contentLb.font = kFont(13);
    [self.contentView addSubview:_contentLb];

    //4.是否阅读
    _statuView = [[UIView alloc] init];
    _statuView.layer.cornerRadius = 5;
    _statuView.layer.masksToBounds = YES;
    //[self.contentView addSubview:_statuView];
}
- (void)settingSubviewFrame
{
    _typeImg.frame = CGRectMake(20, 0, 40, 70);

    _titleLb.frame = CGRectMake(XW(_typeImg)+10, 10, 150, kLableH);
    
    _timeLb.frame = CGRectMake(190, 10, 110, kLableH);
    

    _contentLb.frame = CGRectMake(X(_titleLb), YH(_titleLb), 230, kLableH);
    
    _statuView.frame = CGRectMake(5, (H(self.contentView))/2, 10, 10);
    _statuView.backgroundColor = [UIColor redColor];
   
    UIView *lineView = [[UIView alloc] initWithFrame:CGRectMake(X(_titleLb), YH(_contentLb), W(_contentLb), 0.5)];
    lineView.backgroundColor = [UIColor lightGrayColor];
    [self.contentView addSubview:lineView];
}

+(NSString *)ID
{
    return @"Cell";
}
+(CGFloat)cellHeight
{
    return 70;
}
@end
