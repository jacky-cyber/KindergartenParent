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

#define kMarginH 20

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
    
    
    //添加标题背景
    
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(20, kMarginH, 280, 70)];
    imageView.image = [UIImage imageNamed:@"weekReport_bg"];
    [_scrollView addSubview:imageView];
    
    
    
    //初始化模型数据
    [self addModelData];
    
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
    biaoxian.titleLabel.text = @"在园表演";
    biaoxian.ftitleLabel.text = @"进餐:";
    biaoxian.stitleLabel.text = @"饮水:";
    biaoxian.fcontentLabel.text = _model.eat;
    biaoxian.scontentLabel.text = _model.drink;
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
    
    _model.health = @"自己能平稳的端饭端菜，不会洒出或者掉在地上";
    _model.sport = @"我爱体育，我爱体育，我爱体育，我爱体育，我爱体育，";
    _model.english = @"my name is han mei mei ,i'm 18, i'm come frome beijing";
    _model.yuwen = @"一直小鸭子，洗呀洗澡澡，哈哈，嘎嘎";
    
    _model.math = @"1+1=2,2+2=3,3+3=4";
    _model.music = @"一闪一闪亮晶晶，漫天都是小信息";
    _model.kexue = @"奥特曼大战怪兽，保卫地球";
    _model.paint = @"梵高的映像派不错，呵呵";
    _model.kaoqin = @"签到";
    _model.shehui = @"我们今天去父老来来过马路了，不错";
    _model.eat = @"吃了三个馒头";
    _model.drink = @"喝了五瓶水";
    _model.jiayuangongyu = @"老师和家长要共同养育，一定要，老师和家长要共同养育，一定要老师和家长要共同养育，一定要老师和家长要共同养育，一定要";
    
    //    @interface ZJWeekReportModel : NSObject
    //    @property(nonatomic,strong) NSString *title;
    //    @property(nonatomic,strong) NSString *health;
    //    @property(nonatomic,strong) NSString *sport;
    //    @property(nonatomic,strong) NSString *english;
    //    @property(nonatomic,strong) NSString *yuwen;
    //    @property(nonatomic,strong) NSString *math;
    //    @property(nonatomic,strong) NSString *kexue;
    //    @property(nonatomic,strong) NSString *music;
    //    @property(nonatomic,strong) NSString *paint;
    //    @property(nonatomic,strong) NSString *shehui;
    //    @property(nonatomic,strong) NSString *kaoqin;
    //    @property(nonatomic,strong) NSString *biaoxian;
    //    @property(nonatomic,strong) NSString *jiayuangongyu;
    //    @property(nonatomic,strong) NSString *userid;
    
}




- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
