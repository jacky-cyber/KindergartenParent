//
//  ZJActivityViewController.m
//  KindergartenParent
//
//  Created by define on 14-6-25.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJActivityViewController.h"
#import "ZJHomeModel.h"
@interface ZJActivityViewController (){
    ZJHomeModel *_model;
}
@property (weak, nonatomic) IBOutlet UILabel *publishTime;

@end

@implementation ZJActivityViewController

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
    
    _model = self.userInfo;
    
    
    TimeFormatTools *timetools  = [[TimeFormatTools alloc] init];
    NSString *timeStr = [timetools timeToNow:_model.createtime];
    
    if (timeStr.length >10) {
        timeStr = [timeStr substringToIndex:10];
    }

    
    _publishTime.text =timeStr;
    
    
    UILabel *label = [[UILabel alloc] init];
    label.numberOfLines = 0;
    NSString *drugStr = _model.content;
    
    CGFloat height = [drugStr getHeightByWidth:280 font:kFont(14)];
    label.font = kFont(14);
    label.frame = CGRectMake(20, 70, 280, height);
    label.text = drugStr;
    [self.view addSubview:label];
    
    
    //监测报名状态
    [self baomingStatus];
    
}

-(void)baomingBtn
{
    //报名
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 0, 50, 25);
    [btnR addTarget:self action:@selector(baomingAction) forControlEvents:UIControlEventTouchUpInside];
    
    UIImage *backgroundImg= [UIImage resizedImage:@"nav_rightbackbround_image"];
    
    [btnR setBackgroundImage:backgroundImg forState:UIControlStateNormal];
    [btnR setTitle:@"报名" forState:UIControlStateNormal];
    [btnR setTitleColor:[UIColor colorWithRed:0.129 green:0.714 blue:0.494 alpha:1.000] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;

}
#pragma mark 报名action
-(void)baomingAction{
    //182.18.23.244:8080/kindergarten/service/app!apply.action?username=xuesheng&id=1
    
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"id":_model.id};
    [HttpTool getWithPath:@"apply" params:params success:^(id JSON) {
        MyLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] == 0) {
            NSDictionary *dict = JSON[@"data"];
            if ([dict[@"status"] isEqual:@1]) {
                kPS(@"报名成功", 1);
            }else{
                kPE(@"报名失败", 1);
            }
        }
    } failure:^(NSError *error) {
        
    }];
}

#pragma mark 报名状态监测
-(void)baomingStatus
{
    /*
    182.18.23.244:8080/kindergarten/service/app!applystatus.action?username=xuesheng&id=1
     */
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"id":_model.id};
    [HttpTool getWithPath:@"applystatus" params:params success:^(id JSON) {
        MyLog(@"%@",JSON);
        if ([JSON[@"data"][@"status"] isEqual:@0]) {
            [self baomingBtn];
        }
        
    } failure:^(NSError *error) {
        
    }];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
