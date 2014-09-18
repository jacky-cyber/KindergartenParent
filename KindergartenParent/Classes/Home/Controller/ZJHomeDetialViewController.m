//
//  ZJHomeDetialViewController.m
//  ybparents
//
//  Created by Define on 14-6-9.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJHomeDetialViewController.h"
#import "ZJHomeModel.h"
@interface ZJHomeDetialViewController (){
    ZJHomeModel *_model;
}
@property (strong, nonatomic)  UILabel *createTime;
@property (strong, nonatomic)  UILabel *contentLabel;
@property (strong, nonatomic)  UIImageView *typeImage;

@property (strong, nonatomic)  UIScrollView *scrollView;

@end

@implementation ZJHomeDetialViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    _scrollView = [[UIScrollView alloc] init];
    
    CGFloat scrollH =kScreenHeight-kNavH-20;
    
    _scrollView.frame = CGRectMake(0, 0, W(self.view),scrollH);
    [self.view addSubview:_scrollView];
    
    if (self.userInfo) {
        _model = self.userInfo;
        [self addSubVies];
    }else{
        _model = self.otherInfo;
        [self loadData];
    }
   
    
    
  
}

-(void)loadData
{
    
    NSDictionary *params = @{@"type":_model.type,@"msgid":_model.id};
    
    [HttpTool getWithPath:@"msgdetial" params:params success:^(id JSON) {
        if ([JSON[@"code"] intValue] ==0) {
            _model.content = JSON[@"data"][@"content"];
            _model.createtime = JSON[@"data"][@"createtime"];
            [self addSubVies];
        }
    } failure:^(NSError *error) {
        
    }];
}

-(void)addSubVies
{
    _typeImage = [[UIImageView alloc] initWithFrame:CGRectMake(20, 20, 280, 30)];
    //判断通知的类型
    if ([_model.type intValue] == 2) {//全员通知
        _typeImage.image = [UIImage imageNamed:@"youeryuan_notif"];
    }else if ([_model.type intValue] == 6) {//服药提醒
        _typeImage.image = [UIImage imageNamed:@"weiyaoNofifi"];
    }else if ([_model.type intValue] == 8) {//本班通知
        _typeImage.image = [UIImage imageNamed:@"classDetialNotifi"];
    }else if ([_model.type intValue] == 10) {//活动通知
        _typeImage.image = [UIImage imageNamed:@"huodongtongzhi"];
    }
    [_scrollView addSubview:_typeImage];
    
    _createTime = [[UILabel alloc] initWithFrame:CGRectMake(195, 25, 105, 21)];
    _createTime.font = kFont(11);
    _createTime.textAlignment = NSTextAlignmentRight;
    _createTime.textColor = [UIColor lightGrayColor];
    [_scrollView addSubview:_createTime];
    TimeFormatTools *timeTool = [[TimeFormatTools alloc] init];
    NSString *timeStr = [timeTool timeToNow:_model.createtime];
    
    if (timeStr.length >10) {
        timeStr = [timeStr substringToIndex:10];
    }
    _createTime.text = timeStr;
    
    
    CGFloat height = [_model.content getHeightByWidth:280 font:kFont(14)];
    _contentLabel = [ZJUIMethods creatLabel:_model.content frame:CGRectMake(20, 70, 280, height) font:kFont(14) textColor:nil];
    [_scrollView addSubview:_contentLabel];
    
    //重新计算scrollview contentsize
    _scrollView.contentSize = CGSizeMake(W(self.view), YH(_contentLabel)+5);
    
    
    
    if ([_model.type intValue] == 10) {
        //监测报名状态
        [self baomingStatus];
    }
    
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
                [self setBaoming];
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
        if ([JSON[@"data"][@"status"] isEqual:@2]) {
            [self baomingBtn];
        }else{
            [self setBaoming];
        }
        
    } failure:^(NSError *error) {
        
    }];
}
-(void)setBaoming
{
    //报名
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 0, 60, 25);
    UIImage *backgroundImg= [UIImage resizedImage:@"nav_rightbackbround_image"];
    btnR.enabled = NO;
    [btnR setBackgroundImage:backgroundImg forState:UIControlStateNormal];
    [btnR setTitle:@"已报名" forState:UIControlStateNormal];
    [btnR setTitleColor:[UIColor colorWithRed:0.129 green:0.714 blue:0.494 alpha:1.000] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;
    self.navigationItem.rightBarButtonItem.enabled = NO;
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
