//
//  ZJFuYaoDanCell.h
//  ybparents
//
//  Created by xutao on 14-6-18.
//  Copyright (c) 2014年 xutao. All rights reserved.
//

#import <UIKit/UIKit.h>

@class ZJFuYaoDanModel;
@interface ZJFuYaoDanCell : UITableViewCell
@property (nonatomic,strong) ZJFuYaoDanModel *fydmodel;
@property (nonatomic,strong) UIImageView *mornImg;
@property (nonatomic,strong) UIImageView *noonImg;
@property (nonatomic,strong) UIImageView *eveImg;
@end
