//
//  ZJEditUserViewController.h
//  ybparents
//
//  Created by 郑敬 on 14-6-16.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "BaseController.h"

@protocol EditUserInfoViewControllerDelegate <NSObject>

- (void)editUserInfoViewControllerDidFinished:(NSString*)contentStr withLabel:(NSString*)labelStr;

@end
@interface ZJEditUserViewController : BaseController


@property (weak, nonatomic) id<EditUserInfoViewControllerDelegate> delegate;



// 内容标题
@property (strong, nonatomic) NSString *contentTitle;
// 内容标签
@property (copy, nonatomic) NSString *contentLable;

//修改的参数
@property(copy,nonatomic) NSString *param;

@end
