//
//  BaseController.m
//  Babytop
//
//  Created by classjing on 14-4-27.
//  Copyright (c) 2014年 babytop. All rights reserved.
//

#import "BaseController.h"
#import "ZJLeftSideDrawerViewController.h"
#import "ZJRightSideDrawerViewController.h"


@interface BaseController ()<IChatManagerDelegate>

@end

@implementation BaseController


- (void)viewDidLoad
{
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
//    if (ISIOS7) {
//        self.view.bounds = CGRectMake(0, -20, self.view.frame.size.width, self.view.frame.size.height-20 );
//    }
    if (ISIOS7) {
        self.edgesForExtendedLayout = UIRectEdgeNone;
    }
    
    self.page = 1;//默认为1
//    NSLog(@"bounds:%@",NSStringFromCGRect(self.view.bounds));
//     NSLog(@"frame:%@",NSStringFromCGRect(self.view.frame));
     [[EaseMob sharedInstance].chatManager addDelegate:self delegateQueue:nil];
}
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation {
    return YES;
}
#pragma mark - Methods
- (BaseController*)pushController:(Class)controller withInfo:(id)info
{
    return [self pushController:controller withInfo:info withTitle:nil withOther:nil];
}
- (BaseController*)pushController:(Class)controller withInfo:(id)info withTitle:(NSString*)title
{
    return [self pushController:controller withInfo:info withTitle:title withOther:nil];
}
- (BaseController*)pushController:(Class)controller withInfo:(id)info withTitle:(NSString*)title withOther:(id)other
{
    return [self pushController:controller withInfo:info withTitle:title withOther:other withModel:NO];
}
/**
 *	自动配置次级controller头部并跳转
 *  次级controller为base的基类的时候，参数生效，否则无效
 *
 *	@param	controller	次级controller
 *	@param	info	主参数
 *	@param	title	次级顶部title（次级设置优先）
 *	@param	other	附加参数
 *
 *	@return	次级controller实体
 */
- (BaseController*)pushController:(Class)controller withInfo:(id)info withTitle:(NSString*)title withOther:(id)other withModel:(BOOL)isModel
{
    NSLog(@"Base UserInfo:%@-》%@--->%@",info,other,controller);
    BaseController *base = [[controller alloc] init];
    if ([(NSObject*)base respondsToSelector:@selector(setUserInfo:)]) {
        base.userInfo = info;
        base.otherInfo = other;
        base.title = title;
        if (isModel) {
            //base.isModel  = YES;
            CATransition *transition = [CATransition animation];
            transition.duration = 0.3f;
            transition.timingFunction = [CAMediaTimingFunction functionWithName:kCAMediaTimingFunctionLinear];
            transition.type = kCATransitionMoveIn;
            transition.subtype = kCATransitionFromTop;
            transition.delegate = self;
            [self.navigationController.view.layer addAnimation:transition forKey:nil];
        }
    }
    self.hidesBottomBarWhenPushed = YES;
    [self.navigationController pushViewController:base animated:!isModel];
    self.hidesBottomBarWhenPushed = NO;
    return base;
}
//不需要Base来配置头部
- (BaseController*)pushController:(Class)controller withOnlyInfo:(id)info
{
    NSLog(@"Base UserInfo:%@",info);
    BaseController *base = [[controller alloc] init];
    if ([(NSObject*)base respondsToSelector:@selector(setUserInfo:)]) {
        base.userInfo = info;
    }
    [self.navigationController pushViewController:base animated:YES];
    return base;
}
- (BOOL)popController:(NSString*)controllerstr withSel:(SEL)sel withObj:(id)info
{
    BOOL pop = NO;
    for (id controller in self.navigationController.viewControllers) {
        if ([NSStringFromClass([controller class]) isEqualToString:controllerstr]) {
            if ([(NSObject*)controller respondsToSelector:sel]) {
                [controller performSelector:sel withObject:info afterDelay:0.01];
            }
            [self popController:controller];
            pop = YES;
            break;
        }
    }
    return pop;
}
- (void)popController:(id)controller
{
    //Class cls = NSClassFromString(controller);
    if ([controller isKindOfClass:[UIViewController class]]) {
        [self.navigationController popToViewController:controller animated:NO];
    }else{
        NSLog(@"popToController NOT FOUND.");
        [self.navigationController popViewControllerAnimated:YES];
    }
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
