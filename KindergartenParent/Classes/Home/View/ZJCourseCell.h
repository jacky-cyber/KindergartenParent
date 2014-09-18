//
//  ZJCourseCell.h
//  KindergartenParent
//
//  Created by define on 14-9-18.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ZJCourseModel.h"
@interface ZJCourseCell : UITableViewCell

@property(nonatomic,strong) UILabel *timeLabel;
@property(nonatomic,strong) UILabel *contentLabel;
@property(nonatomic,strong) ZJCourseModel *model;

@end
