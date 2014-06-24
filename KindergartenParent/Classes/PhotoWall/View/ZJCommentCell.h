//
//  ZJCommentCell.h
//  ybparents
//
//  Created by define on 14-6-20.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import <UIKit/UIKit.h>
@class ZJCommentModel;
@interface ZJCommentCell : UITableViewCell


@property(nonatomic,strong) ZJCommentModel *cmmodel;
@property(nonatomic,strong) UIView *bgView;
+(NSString*)ID;

-(CGFloat)cellH;
@end
