//
//  ZJHonorCell.h
//  ybparents
//
//  Created by 郑敬 on 14-6-17.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import <UIKit/UIKit.h>
@class ZJHonorModel;
@interface ZJHonorCell : UITableViewCell
@property (nonatomic,strong) ZJHonorModel *model;
@property (nonatomic,strong)  UIView *rightView;


+(NSString*)ID;

@end
