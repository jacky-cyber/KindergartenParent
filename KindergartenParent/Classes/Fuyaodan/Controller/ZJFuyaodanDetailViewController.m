//
//  ZJFuyaodanDetailViewController.m
//  ybparents
//
//  Created by xutao on 14-6-18.
//  Copyright (c) 2014年 xutao. All rights reserved.
//

#import "ZJFuyaodanDetailViewController.h"

@interface ZJFuyaodanDetailViewController ()
{
    UIScrollView *_scrollView;
    UILabel *_YaoName;
    UILabel *_YaoNum;
    UILabel *_YaoTime;
    UILabel *_YaoCare;
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
    //学生信息
    
    NSString *stuName = @"学生姓名：小果果";
    NSString *className = @"所在班级：海盗班";
    NSString *parName = @"家长姓名：果断断";
    NSString *teleCall = @"联系方式：121212121212";
    NSArray *stuArray = [[NSArray alloc]initWithObjects:stuName,className,parName,teleCall, nil];
    
    
    //药物数据 string
    NSString *YaoWuName = @"九胃泰葵药物名称: 九胃泰葵花葵花药物名称: 九胃泰葵花葵花药物名称: 九胃泰葵花葵花药物名称: 九胃泰葵花葵花药物名称: 九胃泰葵花葵花药物名称: 九胃泰葵花葵花花葵花胃康灵，小儿感冒灵，九九胃泰葵花胃康灵，小儿感冒灵，九九胃泰葵花";
    NSString *YaoWuNum = @"葵花胃康灵，小儿感冒灵，九九胃泰葵花胃康灵，小儿感冒灵，九九胃泰葵花胃康灵，小儿感冒灵，九九胃";
    NSString *YaoTime = @"早，中，晚";
    NSString *YaoCare = @"生态制剂（整肠生、金双歧、妈咪爱、培菲康等）与抗生素合用时，注意应间隔两小时服用，避免药效丧失。";
    
    
    //导航
    
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 4, 80, 25);
    [btnR addTarget:self action:@selector(sendinfoAgain) forControlEvents:UIControlEventTouchUpInside];
    
    [btnR setTitle:@"再加一天" forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;

    //服药单详情
    
    _scrollView = [[UIScrollView alloc]initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, height)];
    _scrollView.showsVerticalScrollIndicator = NO;
    _scrollView.contentSize = CGSizeMake(320, 700);
    [self.view addSubview:_scrollView];
   
    UILabel *label1 = [[UILabel alloc]initWithFrame:CGRectMake(20, 20, 280, 25)];
    [_scrollView addSubview:label1];
    
    //左边时间
    UILabel *timeLabel = [[UILabel alloc]initWithFrame:CGRectMake(20, 10, 80, 25)];
    timeLabel.text = @"2014-06-21";
    timeLabel.font = kFont13;
    [label1 addSubview:timeLabel];
    
    NSArray *imageArr = [[NSArray alloc]initWithObjects:@"zao_03",@"zhong_05",@"wan_07", nil];
    //早
    UIImageView *imageV1 = [[UIImageView alloc]initWithFrame:CGRectMake(120+0*60, 10, 25, 25)];
    imageV1.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@",[imageArr objectAtIndex:0]]];
    [label1 addSubview:imageV1];
    //中
    UIImageView *imageV2 = [[UIImageView alloc]initWithFrame:CGRectMake(120+1*60, 10, 25, 25)];
    imageV2.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@",[imageArr objectAtIndex:1]]];
    [label1 addSubview:imageV2];
    //晚
    UIImageView *imageV3 = [[UIImageView alloc]initWithFrame:CGRectMake(120+2*60, 10, 25, 25)];
    imageV3.image = [UIImage imageNamed:[NSString stringWithFormat:@"%@",[imageArr objectAtIndex:2]]];
    [label1 addSubview:imageV3];
    
    //学生信息
    UILabel *infobg = [[UILabel alloc]initWithFrame:CGRectMake(20, 60, 280, 75)];
    //修改颜色值
    infobg.backgroundColor = [UIColor colorWithRed:245/255.0 green:245/255.0 blue:245/255.0 alpha:1];
    for (int i= 0; i<4; i++)
    {
       
            UILabel *label =[[UILabel alloc]initWithFrame:CGRectMake(10+i%2*110, 10+i/2*25, 160, 30)];
            label.font = kFont12;
            label.textColor = [UIColor grayColor];
            label.text = [stuArray objectAtIndex:i];
            [infobg addSubview:label];
       
       
    }
    
   
    [_scrollView addSubview:infobg];
    
    //服药单 标签label
    UILabel *fuyaodan = [[UILabel alloc]initWithFrame:CGRectMake(30, 130, 80, 40)];
    fuyaodan.text = @"服药单";
    fuyaodan.font = kFont13;
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
    [_scrollView addSubview:YaoNameLabel];
    
    _YaoName = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, 0, 0)];
    [_YaoName setNumberOfLines:0];
   
    _YaoName.text = YaoWuName;
    _YaoName.font = kFont13;
    
    
    
    CGSize lablesize1 = [YaoWuName sizeWithFont:font constrainedToSize:size ];
    [_YaoName setFrame:CGRectMake(100, 175, lablesize1.width, lablesize1.height)];
    [_scrollView addSubview:_YaoName];
    
    //服用剂量
    UIImageView *huixian1 = [[UIImageView alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+175, 260, 1)];
    huixian1.image = [UIImage imageNamed:@"huitiao_10"];
    [_scrollView addSubview:huixian1];
    
    UILabel *YaoNumLabel = [[UILabel alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+175, 60, 40)];
    YaoNumLabel.text = @"服用剂量:";
    YaoNumLabel.font = kFont13;
    [_scrollView addSubview:YaoNumLabel];
    
    _YaoNum = [[UILabel alloc]initWithFrame:CGRectMake(0, 0, 0, 0)];
    [_YaoNum setNumberOfLines:0];
    _YaoNum.text = YaoWuNum;
    _YaoNum.font = kFont13;
    CGSize lablesize2 = [YaoWuNum sizeWithFont:font constrainedToSize:size];
    [_YaoNum setFrame:CGRectMake(100, _YaoName.frame.size.height+185, lablesize2.width, lablesize2.height)];
    [_scrollView addSubview:_YaoNum];
    
    //服药时间
    
    UIImageView *huixian2 = [[UIImageView alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+_YaoNum.frame.size.height+185, 260, 1)];
    huixian2.image = [UIImage imageNamed:@"huitiao_10"];
    [_scrollView addSubview:huixian2];
    
    UILabel *YaoTimeLabel = [[UILabel alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+_YaoNum.frame.size.height+185, 60, 40)];
    YaoTimeLabel.text = @"服药时间:";
    YaoTimeLabel.font = kFont13;
    [_scrollView addSubview:YaoTimeLabel];

    _YaoTime = [[UILabel alloc]initWithFrame:CGRectMake(100, _YaoName.frame.size.height+_YaoNum.frame.size.height+185, 200, 40)];
    _YaoTime.text = YaoTime;
    _YaoTime.font = kFont13;
    [_scrollView addSubview:_YaoTime];
    //注意事项
    UIImageView *huixian3 = [[UIImageView alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+185, 260, 1)];
    huixian3.image = [UIImage imageNamed:@"huitiao_10"];
    [_scrollView addSubview:huixian3];
    
    UILabel *YaoCareLabel = [[UILabel alloc]initWithFrame:CGRectMake(30, _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+185, 60, 40)];
    YaoCareLabel.text = @"注意事项:";
    YaoCareLabel.font = kFont13;
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
    
    UIImageView *imageViewD = [[UIImageView alloc]initWithFrame:CGRectMake(20,  _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+_YaoCare.frame.size.height+195, 280, 1)];
    imageViewD.image = [UIImage imageNamed:@"tiao_03"];
    [_scrollView addSubview:imageViewD];
    
    UIImageView *imageViewL = [[UIImageView alloc]initWithFrame:CGRectMake(20, 19, 1, _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+_YaoCare.frame.size.height+175)];
    imageViewL.image = [UIImage imageNamed:@"tiao_03"];
    [_scrollView addSubview:imageViewL];
    
    UIImageView *imageViewR = [[UIImageView alloc]initWithFrame:CGRectMake(300, 19, 1, _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+_YaoCare.frame.size.height+175)];
    imageViewR.image = [UIImage imageNamed:@"tiao_03"];
    [_scrollView addSubview:imageViewR];
    
    CGFloat H = _YaoName.frame.size.height+_YaoNum.frame.size.height+_YaoTime.frame.size.height+_YaoCare.frame.size.height+175 +19;
    if (H < 460)
    {
        _scrollView.scrollEnabled = NO;
    }
    else
    {
        _scrollView.scrollEnabled = YES;
        _scrollView.contentSize = CGSizeMake(320, H+10);
    }
    
}
#pragma mark-----发送数据
-(void)sendinfoAgain
{
    NSLog(@"发送信息");
    
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
