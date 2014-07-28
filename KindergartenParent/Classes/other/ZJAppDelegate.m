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
#import "IQKeyboardManager.h"
#import "APService.h"

#define kcurrentTIme @"currentTIme"

@implementation ZJAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [IQKeyboardManager sharedManager];
    ZJLeftSideDrawerViewController * leftSideDrawerViewController = [[ZJLeftSideDrawerViewController alloc] init];
   
    ZJRightSideDrawerViewController * rightSideDrawerViewController = [[ZJRightSideDrawerViewController alloc] init];
    
    ZJHomeViewController * centerViewController  = [[ZJHomeViewController alloc] init];
    
    BaseNavigationController * navigationController = [[BaseNavigationController alloc] initWithRootViewController:centerViewController];

    
    _menuController = [[DDMenuController alloc] initWithRootViewController:navigationController];
    _menuController.leftViewController = leftSideDrawerViewController;
    _menuController.rightViewController = rightSideDrawerViewController;
    

    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    
    if ([LoginUser sharedLoginUser].isLogin) {
        self.window.rootViewController = _menuController;
    }else{
        BaseNavigationController * navigationController = [[BaseNavigationController alloc] initWithRootViewController:[[ZJLoginViewController alloc] init]];
        self.window.rootViewController =navigationController ;
    }
    
    
    self.window.backgroundColor = [UIColor whiteColor];
    [self.window makeKeyAndVisible];
    application.statusBarStyle = UIStatusBarStyleLightContent;
  
    [application setStatusBarHidden:NO];
    [application setStatusBarStyle:UIStatusBarStyleLightContent];
    
    
    
    // 设置应用程序能够接收APNS推送的消息
    // Required
    [APService registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge |
                                                   UIRemoteNotificationTypeSound |
                                                   UIRemoteNotificationTypeAlert)];
    // Required
    [APService setupWithOption:launchOptions];
    

    [APService setAlias:@"xuesheng" callbackSelector:nil object:nil];
    
    
    //获取当前时间保存沙河
    [self setCurrentTime];
    return YES;
}


#pragma mark - 获取DeviceToken
- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken
{
    // Required
    [APService registerDeviceToken:deviceToken];
    NSLog(@"deviceToken : %@", deviceToken);
    // 1. 从系统偏好取之前的token
//    NSData *oldToken = [[NSUserDefaults standardUserDefaults]objectForKey:@"deviceToken"];
//    // 2. 新旧token进行比较
//    if (![oldToken isEqualToData:deviceToken]) {
//        // 3. 如果不一致，保存token到系统偏好
//        [[NSUserDefaults standardUserDefaults]setObject:deviceToken forKey:@"deviceToken"];
//        
//        // 4. 使用post请求传输新旧token至服务器
//        // 1) url
//        // 具体的URL地址以及POST请求中的参数和格式，是由公司的后端程序员提供的
//        // 2) request POST body（包含新旧token的数据）
//        // 3) connection 的异步
//    }
}

- (void)application:(UIApplication *)application didFailToRegisterForRemoteNotificationsWithError:(NSError *)error {
        NSLog(@"Failed to get token, error:%@", error.description);
}
- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo
{
    //在此处理接收到的消息。
    // Required
    [APService handleRemoteNotification:userInfo];

    MyLog(@"Receive remote notification : %@",userInfo);
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"订阅消息" message:userInfo.description delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"查看", nil];
    
    
      [alert show];

   
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
    NSLog(@"---applicationWillResignActive----");

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
        hour = [NSString stringWithFormat:@".1%f",differenceTime];
    }else{
        hour = @"0.1";
    }
    return hour;
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
     NSLog(@"---applicationDidBecomeActive----");
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    NSLog(@"---applicationWillTerminate----");

}

@end
