//
//  ZJAppDelegate.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 ___FULLUSERNAME___. All rights reserved.
//

#import "ZJAppDelegate.h"
#import "ZJLeftSideDrawerViewController.h"
#import "ZJRightSideDrawerViewController.h"
#import "ZJHomeViewController.h"
#import "DDMenuController.h"
#import "LoginUser.h"
#import "ZJLoginViewController.h"
#import "ZJPhotWallViewController.h"
#import "ZJNotificationListCell.h"
#import "ZJDayReportViewController.h"
#import "ZJWeekReportsViewController.h"
#import "IQKeyboardManager.h"
#import "APService.h"
#import "BaseController.h"
#import "ZJWeekReportsViewController.h"
#import "ZJNotificationListViewController.h"
#import "ZJHomeDetialViewController.h"
#import "ZJHomeModel.h"
#import "ZJCookBooksViewController.h"
#import "ZJHonorViewController.h"
#import "ZJMonthCommentViewController.h"
#import "OCMobClientSDK.h"
#import "ffmpeg_h264.h"
#import "NewfeatureController.h"
#define kcurrentTIme @"currentTIme"
// 赋值语句不能够写在.h中，只能写在.m中
// 使用此种方式，可以保证常量字符串在内存中有且仅有一个地址
NSString * const kXMPPLoginUserNameKey = @"xmppUserName";
NSString * const kXMPPLoginPasswordKey = @"xmppPassword";
NSString * const kXMPPLoginHostNameKey = @"xmppHostName";

@interface ZJAppDelegate() <XMPPStreamDelegate>
{
    LoginFailedBlock        _failedBlock;
    
    XMPPReconnect           *_xmppReconnect;                // 重新连接的模块
    XMPPvCardCoreDataStorage    *_xmppvCardCoreDataStorage; // 电子名片数据存储扩展
    
    ZJHomeViewController *_viewController;
    BaseNavigationController * _navigationController;
}

/**
 *  设置XMPPStream
 */
- (void)setupXmppStream;

/**
 *  释放XMPPStream
 */
- (void)teardownXmppStream;

/**
 *  连接到服务器
 */
- (void)connect;

/**
 *  断开连接
 */
- (void)disconnect;

/**
 *  用户上线
 */
- (void)goOnline;

/**
 *  用户下线
 */
- (void)goOffline;

@end
@implementation ZJAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    [IQKeyboardManager sharedManager];
    
    
    
    
    NSString *key = (NSString *)kCFBundleVersionKey;
    
    // 1.从Info.plist中取出版本号
    NSString *version = [NSBundle mainBundle].infoDictionary[key];
    
    // 2.从沙盒中取出上次存储的版本号
    NSString *saveVersion = [[NSUserDefaults standardUserDefaults] objectForKey:key];
    
    
    ZJLeftSideDrawerViewController * leftSideDrawerViewController = [[ZJLeftSideDrawerViewController alloc] init];
    
    ZJRightSideDrawerViewController * rightSideDrawerViewController = [[ZJRightSideDrawerViewController alloc] init];
    
    ZJHomeViewController * centerViewController  = [[ZJHomeViewController alloc] init];
    _viewController = centerViewController;
    BaseNavigationController * navigationController = [[BaseNavigationController alloc] initWithRootViewController:centerViewController];
    _navigationController = navigationController;
    
    _menuController = [[DDMenuController alloc] initWithRootViewController:navigationController];
    _menuController.leftViewController = leftSideDrawerViewController;
    _menuController.rightViewController = rightSideDrawerViewController;
    
    if ([version isEqualToString:saveVersion]) { // 不是第一次使用这个版本
        // 显示状态栏
        application.statusBarHidden = NO;
        
        if ([LoginUser sharedLoginUser].isLogin) {
            self.window.rootViewController = _menuController;
        }else{
            ZJLoginViewController *login =  [[ZJLoginViewController alloc] init];
            self.window.rootViewController =login ;
        }
        
    } else { // 版本号不一样：第一次使用新版本
        // 将新版本号写入沙盒
        [[NSUserDefaults standardUserDefaults] setObject:version forKey:key];
        [[NSUserDefaults standardUserDefaults] synchronize];
        // 显示版本新特性界面
        self.window.rootViewController = [[NewfeatureController alloc] init];
    }


    

    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    application.statusBarStyle = UIStatusBarStyleLightContent;
  
    //[application setStatusBarHidden:NO];
    [application setStatusBarStyle:UIStatusBarStyleLightContent];
    
    
    
    // 设置应用程序能够接收APNS推送的消息
    // Required
    [APService registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge |
                                                   UIRemoteNotificationTypeSound |
                                                   UIRemoteNotificationTypeAlert)];
    // Required
    [APService setupWithOption:launchOptions];
    

    [APService setAlias:[LoginUser sharedLoginUser].userName callbackSelector:nil object:nil];
    
    
    //获取当前时间保存沙河
    [self setCurrentTime];
    
    
    //[VGMobClientSDK MobClientSDKInit];
    
    
    // 1. 实例化XMPPStream
    [self setupXmppStream];
    
    return YES;
}


#pragma mark - 获取DeviceToken
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    // Required
    [APService registerDeviceToken:deviceToken];

}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
        NSLog(@"Failed to get token, error:%@", error.description);
}
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    //在此处理接收到的消息。
    // Required
    application.applicationIconBadgeNumber = 0;
    [APService handleRemoteNotification:userInfo];
    
    int  type  =  [userInfo[@"type"] intValue];
    
//    [self pushController:[ZJDayReportViewController class] withInfo:userInfo[@"id"] withTitle:@"每日一报"];
    NSLog(@"Receive remote notification : %@",userInfo);

//    
//    
//      [alert show];

//    1	系统消息
//    2	幼儿园通知  ---
//    3	本周食谱 ----
//    4	每日一报 ----
//    5	每周一报 ---
//    6	医务室通知 ------
//    7	荣誉榜 -----
//    8	老师通知 ----
//    9	生日提醒 ----
//    10	活动通知 ---
//    11	签到/签退 ----
//    12	月评 -----
//    13	回复通知
    
//    全园通知（您有一条新的全园通知），点击进入详情页
//    活动通知（您有一条新的活动通知），点击进入详情页
//    本班通知（您有一条新的本班通知），点击进入详情页
    
    ZJHomeModel *model = [[ZJHomeModel alloc] init];
    model.type = userInfo[@"type"];
    model.id = userInfo[@"id"];
    model.content = userInfo[@"content"];
    
    
    if(type == 2){
        
        [_viewController pushController:[ZJHomeDetialViewController class] withInfo:model withTitle:@"幼儿园通知"];
        
    }else if(type == 3){//本周食谱
        [_viewController pushController:[ZJCookBooksViewController class] withInfo:model withTitle:@"本周食谱"];
    }else if(type == 4 ) {//每日一报
        [_viewController pushController:[ZJDayReportViewController class] withInfo:userInfo[@"id"] withTitle:@"每日一报"];
        
    }else if(type == 5){//每周一报
        [_viewController pushController:[ZJWeekReportsViewController class] withInfo:userInfo[@"id"] withTitle:@"每周一报"];
    }else if(type == 6){//医务通知
        
        [_viewController pushController:[ZJNotificationListViewController class] withInfo:@"6" withTitle:@"服药提醒"];
    }else if(type == 7){//荣誉榜
         [_viewController pushController:[ZJHonorViewController class] withInfo:nil withTitle:@"荣誉榜"];
    }else if(type == 8){//本班通知
        [_viewController pushController:[ZJHomeDetialViewController class] withInfo:model withTitle:@"本班通知"];
    }else if(type == 9){//通知公告
        [_viewController pushController:[ZJNotificationListViewController class] withInfo:@"2,8,9,10" withTitle:@"通知公告"];
    }else if(type == 10){//活动通知
        [_viewController pushController:[ZJHomeDetialViewController class] withInfo:model withTitle:@"活动通知"];
    }else if(type == 11){//签到/签退
        [_viewController pushController:[ZJNotificationListViewController class] withInfo:@"11" withTitle:@"签到/签退"];
    }else if(type == 12){//签到/签退
        [_viewController pushController:[ZJMonthCommentViewController class] withInfo:nil withTitle:@"家长月评"];
    }
    
}



- (void)applicationDidEnterBackground:(UIApplication *)application
{
    
    MyLog(@"进入后台");

    NSString *hour =  [self getTimedifference];

    NSLog(@"---applicationDidEnterBackground----");
    //182.18.23.244:8080/kindergarten/service/app!onlinetime.action?hour=2&username=xuesheng
    
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,@"hour":hour};
    
    [HttpTool getWithPath:@"onlinetime" params:params success:^(id JSON) {
       // MyLog(@"%@",JSON);
    } failure:^(NSError *error) {
        //MyLog(@"%@",error.description);
    }];


}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
 
    //进入前台把当前时间存入用户中心
    [self setCurrentTime];
    
    [application setApplicationIconBadgeNumber:0];
    
    
    // 截获本地通知，用户通过通知的横幅点击进入系统，字典中才能找到本地通知
       
    
    MyLog(@"进入前台");
    NSLog(@"---applicationWillEnterForeground----");

    
}
#pragma mark 为了统计家长的在线时长
-(void)setCurrentTime
{
    NSTimeInterval current = [[NSDate date] timeIntervalSince1970];
    [[NSUserDefaults standardUserDefaults] setDouble:current forKey:kcurrentTIme];
}
#pragma mark 获取当前时间和上次时间的差
-(NSString*)getTimedifference
{
    
    NSTimeInterval lastTime =  (NSTimeInterval)[[NSUserDefaults standardUserDefaults] doubleForKey:kcurrentTIme];
    NSTimeInterval current = [[NSDate date] timeIntervalSince1970];
    CGFloat differenceTime = (current - lastTime)/3600;
    NSString *hour = nil;
    if (differenceTime > 0.1) {
        hour = [NSString stringWithFormat:@"%.1f",differenceTime];
    }else{
        hour = @"0.1";
    }
    return hour;
}

//----------------------------------------------------------------------------------------

#pragma mark xmpp 相关设置


#pragma mark - 注销激活状态
- (void)applicationWillResignActive:(UIApplication *)application
{
    //[self disconnect];
    
}

#pragma mark - 应用程序被激活
- (void)applicationDidBecomeActive:(UIApplication *)application
{
    [self connect];
}


// 系统退出到后台，如果内存不足，系统会自动调用AppDelegate的dealloc方法
// 可以在此方法中，做内存清理工作
- (void)dealloc
{
    [self teardownXmppStream];
}

#pragma mark - 成员方法
#pragma mark 登录&注册
- (void)connectOnFailed:(LoginFailedBlock)faild
{
    // 1. 保存块代码
    _failedBlock = faild;
    
    // 如果已经存在到服务器的长连接，先断开到服务器的连接
    // 提示：在使用连接之前，需要首先判断连接是否存在，
    // 因为再次建立的连接有可能应为MyJID的不同，让服务器无法区分准确的客户端！
    if (!_xmppStream.isDisconnected) {
        [_xmppStream disconnect];
    }
    
    // 连接到服务器
    [self connect];
}

#pragma mark 用户注销
- (void)logout
{
    [self disconnect];
    
    // 切换到Login视图控制器
   // [self showStoryboardWithBoardName:kLoginStoryboardName];
}

#pragma mark - XMPP相关方法
- (void)setupXmppStream
{
    // 断言！再此程序断言：_xmppStream必须是nil，如果不是nil，程序强行中断！
    // 要求调用方，必须自觉自律，要求是唯一的，你就应该是唯一的。
    // 断言针对程序的核心代码区，有重要的保护作用！
    // 要求在开发过程中，就能够及时的发现错误，而不是去频繁的修改核心代码！
    NSAssert(_xmppStream == nil, @"XMPPStream被重复实例化！");
    
    // 1. 实例化
    _xmppStream = [[XMPPStream alloc] init];
    //容许后台运行
    _xmppStream.enableBackgroundingOnSocket = YES;
    // 2. 实例化要扩展的模块
    // 1> 重新连接
    _xmppReconnect = [[XMPPReconnect alloc] init];
    // 2> 电子名片
    _xmppvCardCoreDataStorage = [XMPPvCardCoreDataStorage sharedInstance];
    _xmppvCardTempModule = [[XMPPvCardTempModule alloc] initWithvCardStorage:_xmppvCardCoreDataStorage];
    _xmppvCardAvatarModule = [[XMPPvCardAvatarModule alloc] initWithvCardTempModule:_xmppvCardTempModule];
    
    // 3> 花名册
    _xmppRosterCoreDataStorage = [[XMPPRosterCoreDataStorage alloc] init];
    _xmppRoster = [[XMPPRoster alloc] initWithRosterStorage:_xmppRosterCoreDataStorage];
    // 自动接收好友订阅请求，当前版本已经无需设置
    // 提示后续可以探索，需要认证添加好友的功能！
    [_xmppRoster setAutoAcceptKnownPresenceSubscriptionRequests:YES];
    // 自动从服务器加载好友信息
    [_xmppRoster setAutoFetchRoster:YES];
    
    // 4> 消息归档
    // 提示：不要使用sharedInstance，否则不同账号的聊天记录会保存在同一个CoreData数据库中
    _xmppMessageArchivingCoreDataStorage = [[XMPPMessageArchivingCoreDataStorage alloc] init];
    _xmppMessageArchiving = [[XMPPMessageArchiving alloc] initWithMessageArchivingStorage:_xmppMessageArchivingCoreDataStorage];
    //5>聊天室
//    _xmppRoomCoreDataStorage = [[XMPPRoomCoreDataStorage alloc] init];
    //     _xmppStream.myJID = [XMPPJID jidWithUser:@"lisi" domain:@"zhengjing.local" resource:nil];
    //    _xmppRoom = [[XMPPRoom alloc] initWithRoomStorage:self jid:_xmppStream.myJID];
    
    //[_xmppRoom addDelegate:self delegateQueue:dispatch_get_main_queue()];
    
    // 3. 激活扩展模块
    [_xmppReconnect activate:_xmppStream];
    [_xmppvCardTempModule activate:_xmppStream];
    [_xmppvCardAvatarModule activate:_xmppStream];
    [_xmppRoster activate:_xmppStream];
    [_xmppMessageArchiving activate:_xmppStream];
    //[_xmppRoom activate:_xmppStream];
    //[_xmppRoom addDelegate:self delegateQueue:dispatch_get_main_queue()];
    
    // 4. 设置代理
    // 提示：使用类似框架时，包括看网络开源代码，大多数会使用dispatch_get_main_queue()
    // 使用主线程队列通常不容易出错，自己开发时，一定要用多线程。
    // 一旦出问题，通常是UI更新的问题，此时对于理解多线程的工作非常有帮助！
    [_xmppStream addDelegate:self delegateQueue:dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0)];
}

- (void)teardownXmppStream
{
    // 在做内存清理工作的步骤，与实例化XMPPStream的工作刚好相反
    // 1. 删除XMPPStream的代理
    [_xmppStream removeDelegate:self];
    
    // 2. 断开XMPPStream的连接
    [_xmppStream disconnect];
    
    // 3. 停止模块
    [_xmppReconnect deactivate];
    [_xmppvCardTempModule deactivate];
    [_xmppvCardAvatarModule deactivate];
    [_xmppRoster deactivate];
    [_xmppMessageArchiving deactivate];
    
    // 4. 清理内存
    _xmppReconnect = nil;
    
    _xmppvCardAvatarModule = nil;
    _xmppvCardCoreDataStorage = nil;
    _xmppvCardTempModule = nil;
    
    _xmppRosterCoreDataStorage = nil;
    _xmppRoster = nil;
    
    _xmppMessageArchivingCoreDataStorage = nil;
    _xmppMessageArchiving = nil;
    
    _xmppStream = nil;
}


- (void)connect
{
  
    NSString *hostName = kHostName;
    NSString *userName = [LoginUser sharedLoginUser].userName;
    
    // 如果用户名或者主机为空，不再继续
    if (hostName.length == 0 || userName.length == 0) {
        // 用户名和主机都为空说明用户没有登录，通常是第一次运行程序
        // 直接显示登录窗口
       // [self showStoryboardWithBoardName:kLoginStoryboardName];
        
        return;
    }
    
    // 设置XMPPStream的hostName&JID
    _xmppStream.hostName = hostName;
    _xmppStream.myJID = [XMPPJID jidWithUser:userName domain:hostName resource:nil];
    
    // 连接
    // GCDAsnycSocket框架中，所有的网络通讯都是异步的
    NSError *error = nil;
    if (![_xmppStream connectWithTimeout:XMPPStreamTimeoutNone error:&error]) {
        NSLog(@"%@", error.localizedDescription);
    } else {
        NSLog(@"发送连接请求成功");
    }
}

- (void)disconnect
{
    // 通知服务器，我下线了
    [self goOffline];
    
    // 真正的断开连接
    [_xmppStream disconnect];
}

- (void)goOnline
{
    NSLog(@"用户上线");
    
    // 通知服务器用户上线，服务器知道用户上线后，可以根据服务器记录的好友关系，
    // 通知该用户的其他好友，当前用户上线
    XMPPPresence *presence = [XMPPPresence presence];
    NSLog(@"%@", presence);
    
    // 将展现状态发送给服务器
    [_xmppStream sendElement:presence];
}

- (void)goOffline
{
    NSLog(@"用户下线");
    
    // 通知服务器用户下线
    XMPPPresence *presence = [XMPPPresence presenceWithType:@"unavailable"];
    NSLog(@"%@", presence);
    
    [_xmppStream sendElement:presence];
}

#pragma mark - XMPPStream协议代理方法
#pragma mark 完成连接
- (void)xmppStreamDidConnect:(XMPPStream *)sender
{
    NSLog(@"连接成功");
    
    // 登录到服务器，将用户密码发送到服务器验证身份
    NSString *password = [LoginUser sharedLoginUser].password;
    
    if (_isRegisterUser) {
        // 注册
        [_xmppStream registerWithPassword:password error:nil];
    } else {
        // 用户登录
        [_xmppStream authenticateWithPassword:password error:nil];
    }
}

#pragma mark 断开连接
- (void)xmppStreamDidDisconnect:(XMPPStream *)sender withError:(NSError *)error
{
    NSLog(@"断开连接 %@", error);
    if (_failedBlock && error) {
        dispatch_async(dispatch_get_main_queue(), ^{
            _failedBlock(kLoginNotConnection);
        });
    }
}


#pragma mark 身份验证成功
- (void)xmppStreamDidAuthenticate:(XMPPStream *)sender
{
    NSLog(@"身份验证成功!");
    
    // 通知服务器用户上线，QQ上面自己头像“亮”是客户端干的，只需要通知服务器，上线即可
    // 后续操作都是客户端针对状态，自行调整的
    [self goOnline];
    
    // 显示Main.storyboard
    //[self showStoryboardWithBoardName:kMainStroyboardName];
}

#pragma mark 用户名或者密码错误
- (void)xmppStream:(XMPPStream *)sender didNotAuthenticate:(DDXMLElement *)error
{
    NSLog(@"用户名或者密码错误");
    
    // 判断出错处理块代码是否定义
    if (_failedBlock) {
        dispatch_async(dispatch_get_main_queue(), ^{
            _failedBlock(kLoginLogonError);
        });
    }
    
    // 如果用户名或者密码错误，将系统偏好中的内容清除
    NSUserDefaults *defaults = [NSUserDefaults standardUserDefaults];
    
    [defaults removeObjectForKey:kXMPPLoginHostNameKey];
    [defaults removeObjectForKey:kXMPPLoginPasswordKey];
    [defaults removeObjectForKey:kXMPPLoginUserNameKey];
    
    [defaults synchronize];
}

#pragma mark 接收到其他用户的订阅
- (void)xmppStream:(XMPPStream *)sender didReceivePresence:(XMPPPresence *)presence
{
    // 通过跟踪发现，如果展现的类型是subscribe，说明是由其他客户端发起的添加好友请求
    // 如果展现的类型是subscribe，直接接受用户请求
    if ([presence.type isEqualToString:@"subscribe"]) {
        // 订阅我的人是presence.from
        // 接受from的请求即可
        [_xmppRoster acceptPresenceSubscriptionRequestFrom:presence.from andAddToRoster:YES];
    }
    
    
    
   // _navigationController.navigationItem.rightBarButtonItem = button;
     MyLog(@"<><><>%@",presence.description);
}

-(void)xmppStream:(XMPPStream*)sender didReceiveMessage:(XMPPMessage *)message
{
    UIBarButtonItem *button = _navigationController.navigationItem.rightBarButtonItem;
    //  topController.navigationItem.leftBarButtonItem = button;
    [button setBackgroundImage:[UIImage imageNamed:@"chat_bottom_voice_press_left"] forState:UIControlStateNormal barMetrics:UIBarMetricsDefault];
//    //程序运行在前台，消息正常显示
//    if ([[UIApplication sharedApplication] applicationState] == UIApplicationStateActive)
//    {
//        UILocalNotification *localNotification = [[UILocalNotification alloc] init];
//        localNotification.alertAction = @"Ok";
//        localNotification.alertBody = [NSString stringWithFormat:@"From: %@\n\n%@",@"test",@"This is a test message"];//通知主体
//        localNotification.soundName = @"crunch.wav";//通知声音
//        localNotification.applicationIconBadgeNumber = 1;//标记数
//        [[UIApplication sharedApplication] presentLocalNotificationNow:localNotification];//发送通知
//    }else{//如果程序在后台运行，收到消息以通知类型来显示
//        UILocalNotification *localNotification = [[UILocalNotification alloc] init];
//        localNotification.alertAction = @"Ok";
//        localNotification.alertBody = [NSString stringWithFormat:@"From: %@\n\n%@",@"test",@"This is a test message"];//通知主体
//        localNotification.soundName = @"crunch.wav";//通知声音
//        localNotification.applicationIconBadgeNumber = 1;//标记数
//        [[UIApplication sharedApplication] presentLocalNotificationNow:localNotification];//发送通知
//    }
}



@end
