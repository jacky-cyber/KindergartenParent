//
//  ZJNavigationController.m
//  framework搭建
//
//  Created by Define on 14-1-9.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "BaseNavigationController.h"
#import "Commons.h"
@interface BaseNavigationController ()

@end

@implementation BaseNavigationController

+(void)initialize{


}

- (void)viewDidLoad
{
    [super viewDidLoad];
#ifdef __IPHONE_7_0
    if ([[UIDevice currentDevice].systemVersion floatValue] >= 7.0) {
        self.edgesForExtendedLayout = UIRectEdgeNone;
    }
#endif
    // 1.取出设置主题的对象
    UINavigationBar *navBar = [UINavigationBar appearance];
   // UIBarButtonItem *barItem = [UIBarButtonItem appearance];
    
    
    
//    // 2.设置导航栏的背景图片
    NSString *navBarBg = nil;
    if (ISIOS7) { // iOS7
        navBarBg = @"128";
//        
//        // 设置导航栏的渐变色为白色（iOS7中返回箭头的颜色变为这个颜色：白色）
        navBar.tintColor = [UIColor whiteColor];
    } else { // 非iOS7
        navBarBg = @"88";
        [[UINavigationBar appearance] setTintColor:[UIColor colorWithRed:0.000 green:0.678 blue:0.424 alpha:1.000]];
//        [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleBlackOpaque;
//        
//        // 设置导航栏按钮的背景图片
//        [barItem setBackgroundImage:[UIImage imageNamed:@"NavButton"] forState:UIControlStateNormal barMetrics:UIBarMetricsDefault];
//        [barItem setBackgroundImage:[UIImage imageNamed:@"NavButtonPressed"] forState:UIControlStateHighlighted barMetrics:UIBarMetricsDefault];
//        
//        // 设置导航栏返回按钮的背景图片
//        [barItem setBackButtonBackgroundImage:[UIImage imageNamed:@"navigationbar_back"] forState:UIControlStateNormal barMetrics:UIBarMetricsDefault];
//        [barItem setBackButtonBackgroundImage:[UIImage imageNamed:@"NavBackButtonPressed"] forState:UIControlStateHighlighted barMetrics:UIBarMetricsDefault];
    }
//    
    [navBar setBackgroundImage:[UIImage imageNamed:navBarBg] forBarMetrics:UIBarMetricsDefault];
//    
    // 3.设置导航栏标题颜色为白色
//    [navBar setTitleTextAttributes:@{
//                                     UITextAttributeTextColor : [UIColor whiteColor]
//                                     }];
//    
//    // 4.设置导航栏按钮文字颜色为白色
//    [barItem setTitleTextAttributes:@{
//                                      UITextAttributeTextColor : [UIColor whiteColor],
//                                      UITextAttributeFont : [UIFont systemFontOfSize:13]
//                                      } forState:UIControlStateNormal];
    
//    if (ISIOS7) {
//        [[UINavigationBar appearance] setBarTintColor:[UIColor colorWithRed:0.000 green:0.678 blue:0.424 alpha:1.000]];
//        [[UINavigationBar appearance] setTintColor:[UIColor whiteColor]];
//    }else{
//        [[UINavigationBar appearance] setTintColor:[UIColor colorWithRed:0.000 green:0.678 blue:0.424 alpha:1.000]];
//    }
    [[UINavigationBar appearance] setTitleTextAttributes:[NSDictionary dictionaryWithObjectsAndKeys:
                                                          [UIColor colorWithRed:255.0/255.0 green:255.0/255.0 blue:255.0/255.0 alpha:1.0],
                                                          UITextAttributeTextColor,
                                                          [UIColor whiteColor],
                                                          UITextAttributeTextShadowColor,
//                                                          [NSValue valueWithUIOffset:UIOffsetMake(0, -1)],
//                                                          UITextAttributeTextShadowOffset,
                                                          nil]];

}
//- (UIStatusBarStyle)preferredStatusBarStyle
//{
//    // 白色样式
//    return UIStatusBarStyleLightContent;
//}


@end
