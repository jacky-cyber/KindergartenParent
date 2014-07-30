//
//  ZJChatViewController.h
//  KindergartenParent
//
//  Created by define on 14-7-18.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ZJAppDelegate.h"
@interface ZJChatViewController : UIViewController
// 聊天的好友JID
@property (nonatomic, strong) XMPPJID *bareJID;

// 对话方头像
@property (strong, nonatomic) NSString *bareImageStr;
// 我的头像
@property (strong, nonatomic) NSString *myImageStr;
@end
