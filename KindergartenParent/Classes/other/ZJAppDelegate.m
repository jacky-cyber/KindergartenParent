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
@implementation ZJAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    [IQKeyboardManager sharedManager];
    ZJLeftSideDrawerViewController * leftSideDrawerViewController = [[ZJLeftSideDrawerViewController alloc] init];
   
    ZJRightSideDrawerViewController * rightSideDrawerViewController = [[ZJRightSideDrawerViewController alloc] init];
    
    ZJPhotWallViewController * centerViewController  = [[ZJPhotWallViewController alloc] init];
    
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
    return YES;
}
				
- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

@end
