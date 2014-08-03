//
//  ZJSystemMsgViewController.m
//  ybparents
//
//  Created by define on 14-6-20.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJSystemMsgViewController.h"

@interface ZJSystemMsgViewController (){
    NSDictionary *_data;
}

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
    
    
    //加载数据
    [self loadData];
    
   
}

#pragma mark 加载数据
-(void)loadData
{
    
    //app!loginstatus.action?username=xuesheng
    //[LoginUser sharedLoginUser].userName
    [HttpTool getWithPath:@"loginstatus" params:@{@"username":[LoginUser sharedLoginUser].userName} success:^(id JSON) {
        //MyLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] == 0 ) {
            _data = JSON[@"data"];
            //设置页面
            [self setViews];
        }
        
    } failure:^(NSError *error) {
        kPE(kHttpErrorMsg, 0.5);
        MyLog(@"%@",error.description);
    }];
}

-(void)setViews{
    //设置登陆次数
    UIImageView *loginNumImg = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"sys_loginNum"]];
    loginNumImg.frame = CGRectMake(20, 20, 110, 90);
    [self.view addSubview:loginNumImg];
    
    //loging Label
     NSString *loginNumStr = [NSString stringWithFormat:@"%d 次",[_data[@"loginnum"] intValue]];
    UILabel *logingLabel = [ZJUIMethods creatLabel:loginNumStr frame:CGRectMake(XW(loginNumImg), Y(loginNumImg), 170, 90) font:kFont(25) textColor:[UIColor colorWithRed:0.314 green:0.510 blue:0.824 alpha:1.000]];
    logingLabel.layer.borderColor = [UIColor colorWithRed:0.314 green:0.510 blue:0.824 alpha:1.000].CGColor;
    logingLabel.layer.borderWidth = 1;
    logingLabel.textAlignment = NSTextAlignmentCenter;
    [self.view addSubview:logingLabel];
    
    //设置登陆时常
    UIImageView *loginTimeImg = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"sys_zaixianTIme"]];
    loginTimeImg.frame = CGRectMake(20, YH(loginNumImg)+10, 110, 90);
    [self.view addSubview:loginTimeImg];
    
    //loging Label
    NSString *loginTimeStr = [NSString stringWithFormat:@"%0.1f 小时",[_data[@"logintime"] floatValue]];
    UILabel *loginTimeLabel = [ZJUIMethods creatLabel:loginTimeStr frame:CGRectMake(XW(loginTimeImg), Y(loginTimeImg), 170, 90) font:kFont(25) textColor:[UIColor colorWithRed:0.471 green:0.643 blue:0.055 alpha:1.000]];
    loginTimeLabel.layer.borderColor = [UIColor colorWithRed:0.471 green:0.643 blue:0.055 alpha:1.000].CGColor;
    loginTimeLabel.layer.borderWidth = 1;
    loginTimeLabel.textAlignment = NSTextAlignmentCenter;
    [self.view addSubview:loginTimeLabel];
    
    
    //设置关心
    UIImageView *guanxImg = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"sys_guanxin"]];
    guanxImg.frame = CGRectMake(20,YH(loginTimeImg)+10, 110, 90);
    [self.view addSubview:guanxImg];
    
    //loging Label
    
    
    NSString *guanxinStr = nil;
    int gx = [_data[@"guanxin"] intValue];
    if (gx == 1) {
        guanxinStr = @"太少啦";
    }else if(gx == 2){
        guanxinStr = @"一般";
    }else if(gx == 3){
        guanxinStr = @"大增";
    }else if(gx == 4){
        guanxinStr = @"暴增";
    }
    
    UILabel *guanxLabel = [ZJUIMethods creatLabel:guanxinStr frame:CGRectMake(XW(guanxImg), Y(guanxImg), 170, 90) font:kFont(25) textColor:[UIColor colorWithRed:0.945 green:0.333 blue:0.533 alpha:1.000]];
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
