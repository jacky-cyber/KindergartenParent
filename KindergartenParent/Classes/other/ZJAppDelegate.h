//
//  ZJAppDelegate.h
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 ___FULLUSERNAME___. All rights reserved.
//

#import <UIKit/UIKit.h>


#import "XMPPFramework.h"

// 全局AppDelegate代理的宏
#define xmppDelegate    ((ZJAppDelegate *)[[UIApplication sharedApplication] delegate])

typedef enum
{
    kLoginNotConnection = 0,    // 无法连接
    kLoginLogonError,           // 用户名或者密码错误
    kLoginRegisterError,        // 用户注册失败
    
} kLoginErrorType;

// 定义注册登录出错块代码
typedef void(^LoginFailedBlock)(kLoginErrorType type);

@class DDMenuController;
@interface ZJAppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) DDMenuController *menuController;
@property (strong, nonatomic) UIWindow *window;


@property(nonatomic,strong,readonly) XMPPStream *xmppStream;
@property(nonatomic,strong,readonly) XMPPRoster *xmppRoster;
@property(nonatomic,strong,readonly) XMPPvCardTempModule *xmppvCardTempModule;
@property(nonatomic, strong, readonly) XMPPvCardAvatarModule *xmppvCardAvatarModule;
@property(nonatomic,strong,readonly) XMPPvCardCoreDataStorage *xmppvCardCoreDataStorage;
@property (nonatomic, strong, readonly) XMPPMessageArchiving *xmppMessageArchiving;
@property (nonatomic, strong, readonly) XMPPMessageArchivingCoreDataStorage *xmppMessageArchivingCoreDataStorage;
@property(nonatomic,strong,readonly) XMPPRosterCoreDataStorage *xmppRosterCoreDataStorage;
/**
 *  注册用户标记
 */
@property (nonatomic, assign) BOOL isRegisterUser;

/**
 *  有登录界面调用，登录到服务器
 */
- (void)connectOnFailed:(LoginFailedBlock)faild;

@end
