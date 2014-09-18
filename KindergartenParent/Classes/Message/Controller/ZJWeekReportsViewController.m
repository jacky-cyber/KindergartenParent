//
//  ZJWeekReportViewController.m
//  ybparents
//
//  Created by 郑敬 on 14-6-18.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJWeekReportsViewController.h"
#import "ZJWeekReportView.h"
#import "ZJWeekReportModel.h"

#define kMarginH 10

@interface ZJWeekReportsViewController ()
{
    ZJWeekReportModel *_model;
    UIScrollView *_scrollView;
}

@end

@implementation ZJWeekReportsViewController



- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
    }
    _scrollView = [[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, W(self.view), height)];
    [self.view addSubview:_scrollView];
    
    
    //初始化模型数据
    [self addModelData];
}

-(void)ldView
{
    
    
    //添加标题背景
    
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(20, kMarginH, 280, 70)];
    imageView.image = [UIImage imageNamed:@"weekReport_bg"];
    [_scrollView addSubview:imageView];
    
    //添加标题
    
    NSString *titleStr = [NSString stringWithFormat:@"%@周报",[LoginUser sharedLoginUser].kindergarten];
    
    //UILabel *titleLabel = [[UILabel alloc] initWithFrame:CGRectMake(0, 0, W(imageView), H(imageView)/2)];
    UILabel *titleLabel = [ZJUIMethods creatLabel:titleStr frame:CGRectMake(0, 0, W(imageView), H(imageView)/2) font:nil textColor:[UIColor whiteColor]];
    titleLabel.text = [NSString stringWithFormat:@"%@周报",[LoginUser sharedLoginUser].kindergarten];
    titleLabel.textAlignment = NSTextAlignmentCenter;
    titleLabel.backgroundColor = [UIColor clearColor];
    [imageView addSubview:titleLabel];
    
    //添加标题
     UILabel *timeLabel = [ZJUIMethods creatLabel:_model.createtime frame:CGRectMake(0,H(imageView)/2, W(imageView), H(imageView)/2) font:nil textColor:[UIColor whiteColor]];
    timeLabel.backgroundColor = [UIColor clearColor];
    timeLabel.textAlignment = NSTextAlignmentCenter;
    [imageView addSubview:timeLabel];

    
    ZJWeekReportView *jiankan = [[ZJWeekReportView alloc] init];
    jiankan.frame = CGRectMake(20, 110, 280, 50);
    jiankan.titleLabel.text = @"健康领域";
    jiankan.ftitleLabel.text = @"健康:";
    jiankan.stitleLabel.text = @"体育:";
    jiankan.fcontentLabel.text = _model.health;
    jiankan.scontentLabel.text = _model.sport;
    [jiankan setupFrame];
    
    [_scrollView addSubview:jiankan];
    
    
    ZJWeekReportView *language = [[ZJWeekReportView alloc] init];
    language.frame = CGRectMake(20, YH(jiankan)+kMarginH, 280, 50);
    language.titleLabel.text = @"语言领域";
    language.ftitleLabel.text = @"英语:";
    language.stitleLabel.text = @"汉语:";
    language.fcontentLabel.text = _model.english;
    language.scontentLabel.text = _model.yuwen;
    [language setupFrame];
    [_scrollView addSubview:language];
    
    
    ZJWeekReportView *kexue = [[ZJWeekReportView alloc] init];
    kexue.frame = CGRectMake(20, YH(language)+kMarginH, 280, 50);
    kexue.titleLabel.text = @"科学领域";
    kexue.ftitleLabel.text = @"数学:";
    kexue.stitleLabel.text = @"科学:";
    kexue.fcontentLabel.text = _model.math;
    kexue.scontentLabel.text = _model.kexue;
    [kexue setupFrame];
    [_scrollView addSubview:kexue];
    
    
    
    ZJWeekReportView *yishu = [[ZJWeekReportView alloc] init];
    yishu.frame = CGRectMake(20, YH(kexue)+kMarginH, 280, 50);
    yishu.titleLabel.text = @"艺术领域";
    yishu.ftitleLabel.text = @"音乐:";
    yishu.stitleLabel.text = @"美工:";
    yishu.fcontentLabel.text = _model.music;
    yishu.scontentLabel.text = _model.paint;
    [yishu setupFrame];
    [_scrollView addSubview:yishu];
    
    //社会领域
    ZJWeekReportView *shehui = [[ZJWeekReportView alloc] init];
    shehui.frame = CGRectMake(20, YH(yishu)+kMarginH, 280, 50);
    shehui.titleLabel.text = @"社会领域";
    shehui.isOne = YES;
    shehui.fcontentLabel.text = _model.shehui;
    [shehui setupFrame];
    [_scrollView addSubview:shehui];
    
    
    
    ZJWeekReportView *chuqin = [[ZJWeekReportView alloc] init];
    chuqin.frame = CGRectMake(20, YH(shehui)+kMarginH, 280, 50);
    chuqin.titleLabel.text = @"出勤情况";
    chuqin.isOne = YES;
    chuqin.fcontentLabel.text = _model.kaoqin;
    [chuqin setupFrame];
    [_scrollView addSubview:chuqin];
    
    
    
    ZJWeekReportView *biaoxian = [[ZJWeekReportView alloc] init];
    biaoxian.frame = CGRectMake(20, YH(chuqin)+kMarginH, 280, 50);
    biaoxian.titleLabel.text = @"在园表现";
    biaoxian.isOne = YES;
    biaoxian.fcontentLabel.text = _model.biaoxian;
    [biaoxian setupFrame];
    [_scrollView addSubview:biaoxian];
    
    
    ZJWeekReportView *jiayuangongyu = [[ZJWeekReportView alloc] init];
    jiayuangongyu.frame = CGRectMake(20, YH(biaoxian)+kMarginH, 280, 50);
    jiayuangongyu.titleLabel.text = @"家园共育";
    jiayuangongyu.isOne = YES;
    jiayuangongyu.fcontentLabel.text = _model.jiayuangongyu;
    [jiayuangongyu setupFrame];
    [_scrollView addSubview:jiayuangongyu];
    
    
    //计算scrollView的高度
    NSArray *subArr = _scrollView.subviews;
    UIView *view  = subArr.lastObject;
    NSLog(@"%@--%f----%f",view,YH(view),YH(language));
    //_scrollView.backgroundColor = [UIColor grayColor];
    _scrollView.contentSize = CGSizeMake(320, YH(view)+10);

}

#pragma mark 添加模型数据
-(void)addModelData
{
    _model = [[ZJWeekReportModel alloc] init];
    [SVProgressHUD showWithStatus:@"加载数据中" maskType:SVProgressHUDMaskTypeBlack];
    [HttpTool getWithPath:@"weekreport" params:@{@"id":self.userInfo} success:^(id JSON) {
        ///NSLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] ==0) {
            [_model setKeyValues:JSON[@"data"]];
            if ([_model.biaoxian isEmptyString] || _model.biaoxian == nil) {
                _model.biaoxian = @"未填写";
            }
            //加载界面
            [self ldView];
            kPdismiss;
        }else{
            [SVProgressHUD showErrorWithStatus:@"请求失败" duration:1];
        }
    } failure:^(NSError *error) {
        [SVProgressHUD showErrorWithStatus:@"网络连接错误" duration:1];
    }];

    
}




- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
