//
//  ZJSystemMsgViewController.m
//  ybparents
//
//  Created by define on 14-6-20.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJSystemMsgViewController.h"

@interface ZJSystemMsgViewController ()

@end

@implementation ZJSystemMsgViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    //设置登陆次数
    UIImageView *loginNumImg = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"sys_loginNum"]];
    loginNumImg.frame = CGRectMake(20, 20, 110, 90);
    [self.view addSubview:loginNumImg];
    
    //loging Label
    UILabel *logingLabel = [ZJUIMethods creatLabel:@"41243" frame:CGRectMake(XW(loginNumImg), Y(loginNumImg), 170, 90) font:kFont(25) textColor:[UIColor colorWithRed:0.314 green:0.510 blue:0.824 alpha:1.000]];
    logingLabel.layer.borderColor = [UIColor colorWithRed:0.314 green:0.510 blue:0.824 alpha:1.000].CGColor;
    logingLabel.layer.borderWidth = 1;
    logingLabel.textAlignment = NSTextAlignmentCenter;
    [self.view addSubview:logingLabel];
    
    //设置登陆时常
    UIImageView *loginTimeImg = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"sys_zaixianTIme"]];
    loginTimeImg.frame = CGRectMake(20, YH(loginNumImg)+10, 110, 90);
    [self.view addSubview:loginTimeImg];
    
    //loging Label
    
    UILabel *loginTimeLabel = [ZJUIMethods creatLabel:@"40小时" frame:CGRectMake(XW(loginTimeImg), Y(loginTimeImg), 170, 90) font:kFont(25) textColor:[UIColor colorWithRed:0.471 green:0.643 blue:0.055 alpha:1.000]];
    loginTimeLabel.layer.borderColor = [UIColor colorWithRed:0.471 green:0.643 blue:0.055 alpha:1.000].CGColor;
    loginTimeLabel.layer.borderWidth = 1;
    loginTimeLabel.textAlignment = NSTextAlignmentCenter;
    [self.view addSubview:loginTimeLabel];
    
    
    //设置关心
    UIImageView *guanxImg = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"sys_guanxin"]];
    guanxImg.frame = CGRectMake(20,YH(loginTimeImg)+10, 110, 90);
    [self.view addSubview:guanxImg];
    
    //loging Label
    
    
     UILabel *guanxLabel = [ZJUIMethods creatLabel:@"暴增" frame:CGRectMake(XW(guanxImg), Y(guanxImg), 170, 90) font:kFont(25) textColor:[UIColor colorWithRed:0.945 green:0.333 blue:0.533 alpha:1.000]];
    guanxLabel.layer.borderColor = [UIColor colorWithRed:0.945 green:0.333 blue:0.533 alpha:1.000].CGColor;
    guanxLabel.textAlignment = NSTextAlignmentCenter;
    guanxLabel.layer.borderWidth = 1;
    [self.view addSubview:guanxLabel];
    
    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
