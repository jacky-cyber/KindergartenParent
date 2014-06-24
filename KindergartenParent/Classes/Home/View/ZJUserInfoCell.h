//
//  ZJUserInfoCell.h
//  ybparents
//
//  Created by 郑敬 on 14-6-13.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import <UIKit/UIKit.h>
@class ZJUserInfoModel;
@interface ZJUserInfoCell : UITableViewCell
@property(nonatomic,strong) UILabel *title;
@property(nonatomic,strong) UILabel *property;
@property(nonatomic,strong) ZJUserInfoModel *model;

@end
