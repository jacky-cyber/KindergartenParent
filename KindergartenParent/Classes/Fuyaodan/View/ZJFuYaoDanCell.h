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
@property (nonatomic,strong) UIButton *mornImg;
@property (nonatomic,strong) UIButton *noonImg;
@property (nonatomic,strong) UIButton *eveImg;
@end
