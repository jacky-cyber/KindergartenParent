//
//  ZJFuyaodanDetailViewController.m
//  ybparents
//
//  Created by xutao on 14-6-18.
//  Copyright (c) 2014年 xutao. All rights reserved.
//

#import "ZJFuyaodanDetailViewController.h"
#import "ZJFuYaoDanModel.h"
@interface ZJFuyaodanDetailViewController ()<UIAlertViewDelegate>
{
    UIScrollView *_scrollView;
    UILabel *_YaoName;
    UILabel *_YaoNum;
    UILabel *_YaoTime;
    UILabel *_YaoCare;
    
    ZJFuYaoDanModel *_model;
}
@end

@implementation ZJFuyaodanDetailViewController

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
    
    self.title = @"服用详情";
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
    }
    // 初始化数据
    _model = self.userInfo;
    
    //学生信息
    
    NSString *stuName = [NSString stringWithFormat:@"学生姓名：%@",_model.uname];
    NSString *className = [NSString stringWithFormat:@"所在班级：%@",_model.uclasses];
    NSString *parName = [NSString stringWithFormat:@"家长姓名：%@",_model.uparentname];
    NSString *teleCall = [NSString stringWithFormat:@"联系方式：%@",_model.utel];
    
    NSArray *stuArray = [[NSArray alloc]initWithObjects:stuName,className,parName,teleCall, nil];
    
    
    //药物数据 string
    NSString *YaoWuName = _model.yaowuname;
    NSString *YaoTime = _model.fuyaotime;
    NSString *YaoCare = _model.remark;

    
    
    //导航
    
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 4, 80, 25);
    [btnR addTarget:self action:@selector(sendinfoAgain) forControlEvents:UIControlEventTouchUpInside];
    
    UIImage *backgroundImg= [UIImage resizedImage:@"nav_rightbackbround_image"];
    
    [btnR setBackgroundImage:backgroundImg forState:UIControlStateNormal];
    [btnR setTitle:@"再加一天" forState:UIControlStateNormal];
    [btnR setTitleColor:[UIColor colorWithRed:0.129 green:0.714 blue:0.494 alpha:1.000] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;
    
    //服药单详情33,182,126
    
    _scrollView = [[UIScrollView alloc]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, height)];
    _scrollView.showsVerticalScrollIndicator = NO;
    _scrollView.contentSize = CGSizeMake(320, 700);
    [self.view addSubview:_scrollView];
    
    UILabel *label1 = [[UILabel alloc]initWithFrame:CGRectMake(20, 20, 280, 25)];
    [_scrollView addSubview:label1];
    
    //左边时间
    UILabel *timeLabel = [[UILabel alloc]initWithFrame:CGRectMake(20, 5, 80, 25)];
    timeLabel.text = [_model.time substringToIndex:10];
    timeLabel.font = kFont13;
    [label1 addSubview:timeLabel];
    
    NSArray *imageArr = @[@"zao",@"zhong",@"wan"];
    NSArray *imageArrH = @[@"zao_h",@"zhong_h",@"wan_h"];
    for (int i = 0; i<3; i++)
    {
        UIButton *statusBtn = [[UIButton alloc]initWithFrame:CGRectMake(120+i*60, 10, 25, 25)];
        [statusBtn setImage:[UIImage imageNamed:imageArr[i]] forState:UIControlStateNormal];
        if (i==0) {
            if (_model.moringstatus.intValue == 1) {
                [statusBtn setImage:[UIImage imageNamed:imageArrH[i]] forState:UIControlStateNormal];
                
            }
            if ([self isRang:@"早"]) {
                statusBtn.enabled = NO;
                statusBtn.alpha = 0.5;
            }
        }
        if (i==1) {
            if (_model.noonstatus.intValue == 1) {
                [statusBtn setImage:[UIImage imageNamed:imageArrH[i]] forState:UIControlStateNormal];
            }
            if ([self isRang:@"中"]) {
                statusBtn.enabled = NO;
                statusBtn.alpha = 0.5;
            }
        }
        if (i==2) {
            if (_model.evestatus.intValue == 1) {
                [statusBtn setImage:[UIImage imageNamed:imageArrH[i]] forState:UIControlStateNormal];
            }
            if ([self isRang:@"晚"]) {
                statusBtn.enabled = NO;
                statusBtn.alpha = 0.5;
            }
        }
        
//        [statusBtn setImage:[UIImage imageNamed:imageArrH[i]] forState:UIControlStateSelected];
//        [statusBtn setImage:[UIImage imageNamed:imageArrH[i]] forState:UIControlStateHighlighted];
        //imageV1.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@",[imageArr objectAtIndex:0]]];
        [label1 addSubview:statusBtn];
//        if (i==0) {//早
//            _mornImg = statusBtn;
//        }else if (i==1){//中
//            _noonImg = statusBtn;
//        }else if (i==2){//晚
//            _eveImg = statusBtn;
//        }
    }

    
    //学生信息
    UILabel *infobg = [[UILabel alloc]initWithFrame:CGRectMake(20, 63, 280, 75)];
    //修改颜色值
    infobg.backgroundColor = [UIColor colorWithRed:245/255.0 green:245/255.0 blue:245/255.0 alpha:1];
    for (int i= 0; i<4; i++)
    {
        
        UILabel *label =[[UILabel alloc]initWithFrame:CGRectMake(10+i%2*110, 10+i/2*25, 160, 30)];
        label.font = kFont12;
        label.backgroundColor = [UIColor clearColor];
        label.textColor = [UIColor grayColor];
        label.text = [stuArray objectAtIndex:i];
        [infobg addSubview:label];
        
        
    }
    
    
    [_scrollView addSubview:infobg];
    
    //服药单 标签label
    UILabel *fuyaodan = [[UILabel alloc]initWithFrame:CGRectMake(30, 130, 80, 40)];
    fuyaodan.text = @"服药单";
    fuyaodan.font = kFont13;
    fuyaodan.backgroundColor = [UIColor clearColor];
    [_scrollView addSubview:fuyaodan];
    
    UIImageView *fuyaodanDxian =[[UIImageView alloc]initWithFrame:CGRectMake(30, 165, 260, 1)];
    fuyaodanDxian.image = [UIImage imageNamed:@"tiao_03"];
    [_scrollView addSubview:fuyaodanDxian];
    
    //服药单 内容
    UIFont *font = kFont13;
    CGSize size = CGSizeMake(200, 1000);
    
    //药物名称
    UILabel *YaoNameLabel = [[UILabel alloc]initWithFrame:CGRectMake(30, 165, 60, 40)];
    YaoNameLabel.text = @"药物名称:";
    YaoNameLabel.font = kFont13;
    YaoNameLabel.backgroundColor = [UIColor clearColor];
    [_scrollView addSubview:YaoNameLabel];
    
    _YaoName = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, 0, 0)];
    [_YaoName setNumberOfLines:0];
    
    _YaoName.text = YaoWuName;
    _YaoName.font = kFont13;
    
    
    
    CGSize lablesize1 = [YaoWuName sizeWithFont:font constrainedToSize:size ];
    [_YaoName setFrame:CGRectMake(100, 175, lablesize1.width, lablesize1.height)];
    [_scrollView addSubview:_YaoName];
    
    //服用剂量
    UIImageView *huixian1 = [[UIImageView alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+176, 260, 1)];
    huixian1.image = [UIImage imageNamed:@"huitiao_10"];
    [_scrollView addSubview:huixian1];
    
    UILabel *YaoNumLabel = [[UILabel alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+177, 60, 40)];
    YaoNumLabel.text = @"服用剂量:";
    YaoNumLabel.font = kFont13;
    YaoNumLabel.backgroundColor = [UIColor clearColor];
    [_scrollView addSubview:YaoNumLabel];
    
    _YaoNum = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, 0, 0)];
    [_YaoNum setNumberOfLines:0];
    _YaoNum.text = _model.jiliang;
    _YaoNum.font = kFont13;
    _YaoNum.backgroundColor = [UIColor clearColor];
    CGSize lablesize2 = [_model.jiliang sizeWithFont:font constrainedToSize:size];
    [_YaoNum setFrame:CGRectMake(100, _YaoName.frame.size.height+187, lablesize2.width, lablesize2.height)];
    [_scrollView addSubview:_YaoNum];
    
    //服药时间
    
    UIImageView *huixian2 = [[UIImageView alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+_YaoNum.frame.size.height+189, 260, 1)];
    huixian2.image = [UIImage imageNamed:@"huitiao_10"];
    [_scrollView addSubview:huixian2];
    
    UILabel *YaoTimeLabel = [[UILabel alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+_YaoNum.frame.size.height+185, 60, 40)];
    YaoTimeLabel.backgroundColor = [UIColor clearColor];
    YaoTimeLabel.text = @"服药时间:";
    YaoTimeLabel.font = kFont13;
    [_scrollView addSubview:YaoTimeLabel];
    
    _YaoTime = [[UILabel alloc]initWithFrame:CGRectMake(100, _YaoName.frame.size.height+_YaoNum.frame.size.height+185, 200, 40)];
    _YaoTime.text = YaoTime;
    _YaoTime.font = kFont13;
    _YaoTime.backgroundColor = [UIColor clearColor];
    [_scrollView addSubview:_YaoTime];
    //注意事项
    UIImageView *huixian3 = [[UIImageView alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+180, 260, 1)];
    huixian3.image = [UIImage imageNamed:@"huitiao_10"];
    [_scrollView addSubview:huixian3];
    
    UILabel *YaoCareLabel = [[UILabel alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+182, 60, 40)];
    YaoCareLabel.text = @"注意事项:";
    YaoCareLabel.font = kFont13;
    YaoCareLabel.backgroundColor = [UIColor clearColor];
    [_scrollView addSubview:YaoCareLabel];
    
    _YaoCare = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, 0, 0)];
    [_YaoCare setNumberOfLines:0];
    _YaoCare.text = YaoCare;
    _YaoCare.font = kFont13;
    CGSize lablesize3 = [YaoCare sizeWithFont:font constrainedToSize:size];
    [_YaoCare setFrame:CGRectMake(100, _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+195, lablesize3.width, lablesize3.height)];
    [_scrollView addSubview:_YaoCare];
    
    //
    UIImageView *imageViewU = [[UIImageView alloc]initWithFrame:CGRectMake(20, 19, 280, 1)];
    imageViewU.image = [UIImage imageNamed:@"tiao_03"];
    [_scrollView addSubview:imageViewU];
    
    UIImageView *imageViewD = [[UIImageView alloc]initWithFrame:CGRectMake(20,  _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+_YaoCare.frame.size.height+200, 280, 1)];
    imageViewD.image = [UIImage imageNamed:@"tiao_03"];
    [_scrollView addSubview:imageViewD];
    
    UIImageView *imageViewL = [[UIImageView alloc]initWithFrame:CGRectMake(20, 19, 1, _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+_YaoCare.frame.size.height+181)];
    imageViewL.image = [UIImage imageNamed:@"tiao_03"];
    [_scrollView addSubview:imageViewL];
    
    UIImageView *imageViewR = [[UIImageView alloc]initWithFrame:CGRectMake(300, 19, 1, _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+_YaoCare.frame.size.height+181)];
    imageViewR.image = [UIImage imageNamed:@"tiao_03"];
    [_scrollView addSubview:imageViewR];
    
    CGFloat H = _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+_YaoCare.frame.size.height+175 +19;
    if (H < 300)
    {
        _scrollView.scrollEnabled = NO;
    }
    else
    {
        _scrollView.scrollEnabled = YES;
        _scrollView.contentSize = CGSizeMake(320, H+10);
    }
    
}

#pragma mark 看是否包含
-(BOOL)isRang:(NSString*)str
{
    BOOL flag = false;
    //
    NSRange rang = [_model.fuyaotime rangeOfString:str];//判断字符串是否包含
    if (rang.length == 0)//不包含
    {
        flag = true;
    }
    return flag;
}

#pragma mark-----发送数据
-(void)sendinfoAgain
{
    
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"确定再添加一天" message:nil delegate:self cancelButtonTitle:@"取消" otherButtonTitles:@"确定",nil];
    [alert show];
    return;
    
    NSLog(@"发送信息");
    
}

-(void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex
{
    MyLog(@"%d",buttonIndex);
    if (buttonIndex == 1) {
        NSString *yaoname = _YaoName.text;//名称
        NSString *jiliang = _YaoNum.text.trimString;//剂量
        NSString *time    = _YaoTime.text.trimString;//服药时间
        NSString *remark  = _YaoCare.text.trimString;//注意事项
        
        
        //发送数据
        [SVProgressHUD showWithStatus:@"正在添加服药单" maskType:SVProgressHUDMaskTypeBlack];
        NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,
                                 @"yaowuname":yaoname,
                                 @"jiliang":jiliang,
                                 @"fuyaotime":time,
                                 @"remark":remark};
        [HttpTool getWithPath:@"addfuyao" params:params success:^(id JSON) {
            MyLog(@"%@",JSON);
            if ([JSON[@"code"] intValue] == 0) {
                
                [SVProgressHUD dismiss];
                [self popController:@"ZJFuyaodanViewController" withSel:@selector(initData:) withObj:@(true)];
                
                [self.navigationController popViewControllerAnimated:YES];
            }
        } failure:^(NSError *error) {
            
        }];
    }
}



- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
 {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

@end
