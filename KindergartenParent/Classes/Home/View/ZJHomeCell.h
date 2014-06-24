//
//  ZJHomeCell.h
//  ybparents
//
//  Created by Define on 14-6-6.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ZJHomeCell : UITableViewCell


@property(nonatomic,strong) UILabel *titleLb;//标题
@property(nonatomic,strong) UILabel *contentLb;//内容
@property(nonatomic,strong) UILabel *timeLb;//发布时间
@property(nonatomic,strong) UIImageView *typeImg;//类型图片
@property(nonatomic,strong) UIView *statuView;//状态视图

+ (NSString *)ID;
+ (CGFloat)cellHeight;
@end
