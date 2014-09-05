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
#import "ChatViewController.h"
#define kcurrentTIme @"currentTIme"

//两次提示的默认间隔
static const CGFloat kDefaultPlaySoundInterval = 3.0;
@interface ZJAppDelegate()<IChatManagerDelegate,UIAlertViewDelegate>
{

    ZJHomeViewController *_viewController;
    BaseNavigationController * _navigationController;
}
@property (strong, nonatomic)NSDate *lastPlaySoundDate;
@end
@implementation ZJAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    [IQKeyboardManager sharedManager];
    
    //初始化体层库(域高视频)
    [VGMobClientSDK MobClientSDKInit];
    
    
    NSString *key = (NSString *)kCFBundleVersionKey;
    
    // 1.从Info.plist中取出版本号
    NSString *version = [NSBundle mainBundle].infoDictionary[key];
    
    // 2.从沙盒中取出上次存储的版本号
    NSString *saveVersion = [[NSUserDefaults standardUserDefaults] objectForKey:key];
    
    
    ZJLeftSideDrawerViewController * leftSideDrawerViewController = [[ZJLeftSideDrawerViewController alloc] init];
    
    ZJRightSideDrawerViewController * rightSideDrawerViewController = [[ZJRightSideDrawerViewController alloc] init];
    
    _viewController = [[ZJHomeViewController alloc] init];

    BaseNavigationController * navigationController = [[BaseNavigationController alloc] initWithRootViewController:_viewController];
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

    //[APService setAlias:[LoginUser sharedLoginUser].userName callbackSelector:nil object:nil];
    [APService setTags:[NSSet setWithObject:@"ios"] callbackSelector:nil object:nil];
    
    //获取当前时间保存沙河
    [self setCurrentTime];
    
    
    
    //#warning SDK注册 APNS文件的名字, 需要与后台上传证书时的名字一一对应
    NSString *apnsCertName = nil;
#if DEBUG
    apnsCertName = @"parentpush_dev";
#else
    apnsCertName = @"parentpush_pro";
#endif
    [[EaseMob sharedInstance] registerSDKWithAppKey:@"kindergarten#kindergartenparent" apnsCertName:apnsCertName];
    
#if DEBUG
    [[EaseMob sharedInstance] enableUncaughtExceptionHandler];
#endif
    //以下一行代码的方法里实现了自动登录，异步登录，需要监听[didLoginWithInfo: error:]
    //demo中此监听方法在MainViewController中
    [[EaseMob sharedInstance] application:application didFinishLaunchingWithOptions:launchOptions];
    
    //#warning 注册为SDK的ChatManager的delegate (及时监听到申请和通知)
    [[EaseMob sharedInstance].chatManager removeDelegate:self];
    [[EaseMob sharedInstance].chatManager addDelegate:self delegateQueue:nil];
    
    //#warning 如果使用MagicalRecord, 要加上这句初始化MagicalRecord
    //demo coredata, .pch中有相关头文件引用
    [MagicalRecord setupCoreDataStackWithStoreNamed:[NSString stringWithFormat:@"%@.sqlite", @"UIDemo"]];
    
    
    MyLog(@"loginInfo:%@",[[EaseMob sharedInstance].chatManager loginInfo]);

    return YES;
}


#pragma mark - 获取DeviceToken
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    // Required
    [APService registerDeviceToken:deviceToken];
    NSLog(@"deviceToken:%@",deviceToken.description);
    
    
    //#warning SDK方法调用
    [[EaseMob sharedInstance] application:application didRegisterForRemoteNotificationsWithDeviceToken:deviceToken];
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
    //#warning SDK方法调用
    [[EaseMob sharedInstance] application:application didFailToRegisterForRemoteNotificationsWithError:error];
    NSLog(@"did Fail To Register For Remote Notifications With Error: %@", error);
}

//avoid compile error for sdk under 7.0
#ifdef __IPHONE_7_0
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
    [APService handleRemoteNotification:userInfo];
    completionHandler(UIBackgroundFetchResultNoData);
}
#endif


- (void)application:(UIApplication *)application didReceiveLocalNotification:(UILocalNotification *)notification
{
    
//    if (notification.userInfo[@"username"]) {
//        [_viewController pushController:[ChatViewController class] withInfo:notification.userInfo[@"username"] withTitle:notification.userInfo[@"username"]];
//    }
    //#warning SDK方法调用
    [[EaseMob sharedInstance] application:application didReceiveLocalNotification:notification];
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    //#warning SDK方法调用
    [[EaseMob sharedInstance] applicationWillResignActive:application];
}



- (void)applicationDidBecomeActive:(UIApplication *)application
{
    
    
    //#warning SDK方法调用
    [[EaseMob sharedInstance] applicationDidBecomeActive:application];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    //#warning SDK方法调用
    [[EaseMob sharedInstance] applicationWillTerminate:application];
}
#pragma mark - push

- (void)didBindDeviceWithError:(EMError *)error
{
    if (error) {
        //TTAlertNoTitle(@"消息推送与设备绑定失败");
    }
}


- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    //在此处理接收到的消息。
    // Required
    [APService handleRemoteNotification:userInfo];
    
    //#warning SDK方法调用
    [[EaseMob sharedInstance] application:application didReceiveRemoteNotification:userInfo];

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
        [_navigationController pushViewController:[[ZJHonorViewController alloc] init] animated:YES];
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
    
    [[NSNotificationCenter defaultCenter] postNotificationName:@"applicationDidEnterBackground" object:nil];
    //#warning SDK方法调用
    [[EaseMob sharedInstance] applicationDidEnterBackground:application];
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
    //#warning SDK方法调用
    [[EaseMob sharedInstance] applicationWillEnterForeground:application];
 
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




#pragma mark - private


// 收到消息回调
-(void)didReceiveMessage:(EMMessage *)message{
    
#if !TARGET_IPHONE_SIMULATOR
    [self playSoundAndVibration];
    
    BOOL isAppActivity = [[UIApplication sharedApplication] applicationState] == UIApplicationStateActive;
    if (!isAppActivity) {
        [self showNotificationWithMessage:message];
    }
#endif
}

- (void)playSoundAndVibration{
    
    //如果距离上次响铃和震动时间太短, 则跳过响铃
    NSLog(@"%@, %@", [NSDate date], self.lastPlaySoundDate);
    NSTimeInterval timeInterval = [[NSDate date]
                                   timeIntervalSinceDate:self.lastPlaySoundDate];
    if (timeInterval < kDefaultPlaySoundInterval) {
        return;
    }
    //保存最后一次响铃时间
    self.lastPlaySoundDate = [NSDate date];
    
    // 收到消息时，播放音频
    [[EaseMob sharedInstance].deviceManager asyncPlayNewMessageSound];
    // 收到消息时，震动
    [[EaseMob sharedInstance].deviceManager asyncPlayVibration];
}

- (void)showNotificationWithMessage:(EMMessage *)message
{
    EMPushNotificationOptions *options = [[EaseMob sharedInstance].chatManager pushNotificationOptions];
    //发送本地推送
    UILocalNotification *notification = [[UILocalNotification alloc] init];
    notification.fireDate = [NSDate date]; //触发通知的时间
    
    if (options.displayStyle == ePushNotificationDisplayStyle_messageSummary) {
        id<IEMMessageBody> messageBody = [message.messageBodies firstObject];
        NSString *messageStr = nil;
        switch (messageBody.messageBodyType) {
            case eMessageBodyType_Text:
            {
                messageStr = ((EMTextMessageBody *)messageBody).text;
            }
                break;
            case eMessageBodyType_Image:
            {
                messageStr = @"[图片]";
            }
                break;
            case eMessageBodyType_Location:
            {
                messageStr = @"[位置]";
            }
                break;
            case eMessageBodyType_Voice:
            {
                messageStr = @"[音频]";
            }
                break;
            case eMessageBodyType_Video:{
                messageStr = @"[视频]";
            }
                break;
            default:
                break;
        }
        NSDictionary *userInfo = @{@"username":message.from};
        notification.userInfo = userInfo;
        NSString *title = message.from;
        if (message.isGroup) {
            NSArray *groupArray = [[EaseMob sharedInstance].chatManager groupList];
            for (EMGroup *group in groupArray) {
                if ([group.groupId isEqualToString:message.conversation.chatter]) {
                    title = [NSString stringWithFormat:@"%@(%@)", message.groupSenderName, group.groupSubject];
                    break;
                }
            }
        }
       
        notification.alertBody = [NSString stringWithFormat:@"%@:%@", title, messageStr];
    }
    else{
        notification.alertBody = @"您有一条新消息";
    }
    //#warning 去掉注释会显示[本地]开头, 方便在开发中区分是否为本地推送
    //notification.alertBody = [[NSString alloc] initWithFormat:@"[本地]%@", notification.alertBody];
    NSDictionary *userInfo = @{@"username":message.from};
    notification.userInfo = userInfo;
    notification.alertAction = @"打开";
    notification.timeZone = [NSTimeZone defaultTimeZone];
    //发送通知
    [[UIApplication sharedApplication] scheduleLocalNotification:notification];
    UIApplication *application = [UIApplication sharedApplication];
    application.applicationIconBadgeNumber += 1;
}



#pragma mark - IChatManagerDelegate 登录状态变化

- (void)didLoginFromOtherDevice
{
    [[EaseMob sharedInstance].chatManager asyncLogoffWithCompletion:^(NSDictionary *info, EMError *error) {
        UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"提示" message:@"您的账号已在其他地方登录" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil, nil];
        alertView.tag = 100;
        [alertView show];
        
    } onQueue:nil];
}
-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    
    ZJLoginViewController *login =  [[ZJLoginViewController alloc] init];
    self.window.rootViewController =login ;
    
}

@end
