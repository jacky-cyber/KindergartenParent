/************************************************************
  *  * EaseMob CONFIDENTIAL 
  * __________________ 
  * Copyright (C) 2013-2014 EaseMob Technologies. All rights reserved. 
  *  
  * NOTICE: All information contained herein is, and remains 
  * the property of EaseMob Technologies.
  * Dissemination of this information or reproduction of this material 
  * is strictly forbidden unless prior written permission is obtained
  * from EaseMob Technologies.
  */

#import <UIKit/UIKit.h>
#import "BaseController.h"
@interface ChatViewController : BaseController

- (instancetype)initWithChatter:(NSString *)chatter;

- (instancetype)initWithGroup:(EMGroup *)chatGroup;
// 电话
@property (strong, nonatomic) NSString *tel;
// 电话
@property (strong, nonatomic) NSString *profileimngurl;

@end
