//
//  ZJADDViewController.m
//  ybparents
//
//  Created by xutao on 14-6-19.
//  Copyright (c) 2014年 xutao. All rights reserved.
//

#import "ZJADDViewController.h"

@interface ZJADDViewController ()
{
    UIImageView *_imageBg;
    UILabel *_label;
    UILabel *_titleName;
    UITextView *_contentView;
    UILabel *_timeLabel;
    UILabel *_YaoNameLabel;
    UILabel *_YaoNumLabel;
    UILabel *_YaoTimeLabel;
    UILabel *_YaoCareLabel;
    UILabel *_threeBtnLabel;
    UIImageView *_bghongxian;
    NSInteger k;
    BOOL zao;
    BOOL zhong;
    BOOL wan;
    
    NSString *_fuyaoTime;//服药时间
    NSString *_remark;//注意事项
    NSString *_jiliang;//剂量
    NSString *_yaowuname;//药物名称
    
}
@end

@implementation ZJADDViewController

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
    self.title = @"添加服药单";
    
    zao = NO;
    zhong = NO;
    wan = NO;
    
    MyLog(@"%@",[LoginUser sharedLoginUser].description);
    
    //修改数据
    NSString *stuName =[NSString stringWithFormat:@"学生姓名：%@",[LoginUser sharedLoginUser].name];
    NSString *className = [NSString stringWithFormat:@"所在班级：%@",[LoginUser sharedLoginUser].classes];
    NSString *parName = [NSString stringWithFormat:@"家长姓名：%@",[LoginUser sharedLoginUser].parentname];
    NSString *teleCall = [NSString stringWithFormat:@"联系方式：%@",[LoginUser sharedLoginUser].tel];
    NSArray *stuArray = [[NSArray alloc]initWithObjects:stuName,className,parName,teleCall, nil];
    
    NSString *YaoNameLabelText = @"药物名称: 请填写药物名称";
    NSString *YaoNumLabelText = @"服用剂量: 请填写服用剂量";
    NSString *YaoTimeLabelText = @"服药时间: 请选择服药时间";
    NSString *YaoCareLabelText = @"注意事项: 请填写注意事项";
    
    
    //发送
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 2, 50, 25);
    [btnR addTarget:self action:@selector(sendinfo) forControlEvents:UIControlEventTouchUpInside];
    [btnR setImage:[UIImage imageNamed:@"fasong_03"] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;
    
    
    
    
    //上层*************************
    
    UILabel *labelU = [[UILabel alloc]initWithFrame:CGRectMake(19, 18, 281, 1)];
    labelU.backgroundColor = [UIColor redColor];
    [self.view addSubview:labelU];
    
    UILabel *labelD = [[UILabel alloc]initWithFrame:CGRectMake(19, 305, 281, 1)];
    labelD.backgroundColor = [UIColor redColor];
    [self.view addSubview:labelD];
    
    UILabel *labelL = [[UILabel alloc]initWithFrame:CGRectMake(19, 18, 1, 287)];
    labelL.backgroundColor = [UIColor redColor];
    [self.view addSubview:labelL];
    
    UILabel *labelR = [[UILabel alloc]initWithFrame:CGRectMake(300, 18, 1, 287)];
    labelR.backgroundColor = [UIColor redColor];
    [self.view addSubview:labelR];
    
    
    _timeLabel = [[UILabel alloc]initWithFrame:CGRectMake(25, 20, 100, 40)];
    _timeLabel.font = kFont13;
    _timeLabel.text = [[NSString stringWithFormat:@"%@",[NSDate date]] substringToIndex:10];
    [self.view addSubview:_timeLabel];
    
    //学生信息
    UILabel *infobg = [[UILabel alloc]initWithFrame:CGRectMake(20, 50, 280, 73)];
    
    infobg.backgroundColor = [UIColor colorWithRed:245/255.0 green:245/255.0 blue:245/255.0 alpha:1];
    for (int i= 0; i<4; i++)
    {
        
        UILabel *label =[[UILabel alloc]initWithFrame:CGRectMake(10+i%2*110, 10+i/2*25, 160, 30)];
        label.font = kFont12;
        label.textColor = [UIColor grayColor];
        label.text = [stuArray objectAtIndex:i];
        label.backgroundColor = [UIColor clearColor];
        [infobg addSubview:label];
        
        
    }
    [self.view addSubview:infobg];
    
    
    
    
    UILabel *fuyaodan = [[UILabel alloc]initWithFrame:CGRectMake(25, 110, 100, 40)];
    fuyaodan.text = @"服药单";
    fuyaodan.font = kFont14;
    fuyaodan.backgroundColor = [UIColor clearColor];
    [self.view addSubview:fuyaodan];
    
    UILabel *labelD2 = [[UILabel alloc]initWithFrame:CGRectMake(25, 140, 270, 1)];
    labelD2.backgroundColor = [UIColor redColor];
    [self.view addSubview:labelD2];
    
    _YaoNameLabel = [[UILabel alloc]initWithFrame:CGRectMake(25, 141, 230, 40)];
    _YaoNameLabel.text = YaoNameLabelText;
    _YaoNameLabel.font = kFont13;
    [self.view addSubview:_YaoNameLabel];
    
    _YaoNumLabel = [[UILabel alloc]initWithFrame:CGRectMake(25, 181, 230, 40)];
    _YaoNumLabel.text = YaoNumLabelText;
    _YaoNumLabel.font = kFont13;
    [self.view addSubview:_YaoNumLabel];
    
    _YaoTimeLabel = [[UILabel alloc]initWithFrame:CGRectMake(25, 221, 230, 40)];
    _YaoTimeLabel.text = YaoTimeLabelText;
    _YaoTimeLabel.font = kFont13;
    [self.view addSubview:_YaoTimeLabel];
    
    _YaoCareLabel = [[UILabel alloc]initWithFrame:CGRectMake(25, 261, 230, 40)];
    _YaoCareLabel.text = YaoCareLabelText;
    _YaoCareLabel.font = kFont13;
    [self.view addSubview:_YaoCareLabel];
    
    for (int i = 0; i<4; i++)
    {
        UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
        btn.tag = i;
        btn.frame = CGRectMake(260, i*40+148, 25, 25);
        [btn addTarget:self action:@selector(btnClick:) forControlEvents:UIControlEventTouchUpInside];
        [btn setImage:[UIImage imageNamed:@"jiantou_03"] forState:UIControlStateNormal];
        [self.view addSubview:btn];
    }
    
    for (int i = 0; i<3; i++)
    {
        UIImageView *imageH= [[UIImageView alloc]initWithFrame:CGRectMake(25, 181+i*40, 270, 1)];
        imageH.image = [UIImage imageNamed:@"huitiao_10"];
        [self.view addSubview:imageH];
        
        
        
    }
    
    
    
    //*******添加
    
    _imageBg = [[UIImageView alloc]initWithFrame:CGRectMake(0, 0, 320, 0)];
    //_imageBg.backgroundColor = [[UIColor grayColor]colorWithAlphaComponent:0.5];
    _imageBg.backgroundColor = [[UIColor blackColor] colorWithAlphaComponent:0.3];
    
    _imageBg.userInteractionEnabled = YES;
    [self.view addSubview:_imageBg];
    
    _label  = [[UILabel alloc]initWithFrame:CGRectMake( 18, 10, 284, 190)];
    _label.backgroundColor = [UIColor whiteColor];
    _label.userInteractionEnabled = YES;
    _label.hidden = YES;
    _label.alpha = 1;
    [_imageBg  addSubview:_label];
    
    _titleName = [[UILabel alloc]initWithFrame:CGRectMake(10, 0, 100, 30)];
    _titleName.font = kFont13;
    [_label addSubview:_titleName];
    
    
    
    
    UIImageView *hongxian = [[UIImageView alloc]initWithFrame:CGRectMake(10, 30, 260, 1)];
    hongxian.image = [UIImage imageNamed:@"tiao_03"];
    [_label addSubview: hongxian];
    
    _bghongxian = [[UIImageView alloc]initWithFrame:CGRectMake(10, 40, 260, 100)];
    _bghongxian.userInteractionEnabled = YES;
    _bghongxian.image = [UIImage imageNamed:@"bj_27N"];
    [_label addSubview:_bghongxian];
    
    _contentView = [[UITextView alloc]initWithFrame:CGRectMake(1, 1, 258, 98)];
    _contentView.editable = YES;
    _contentView.delegate = self;
    [_bghongxian addSubview:_contentView];
    
    UIButton *btnBack = [UIButton buttonWithType:UIButtonTypeCustom];
    btnBack.frame = CGRectMake(60, 150, 65, 25);
    [btnBack addTarget:self action:@selector(btnBack) forControlEvents:UIControlEventTouchUpInside];
    [btnBack setImage:[UIImage imageNamed:@"fanhui_03"] forState:UIControlStateNormal];
    [_label addSubview:btnBack];
    
    UIButton *btnadd = [UIButton buttonWithType:UIButtonTypeCustom];
    btnadd.frame = CGRectMake(140, 150, 65, 25);
    [btnadd addTarget:self action:@selector(btnadd) forControlEvents:UIControlEventTouchUpInside];
    [btnadd setImage:[UIImage imageNamed:@"tianjia_05"] forState:UIControlStateNormal];
    [_label addSubview:btnadd];
    
    
    
    //早中晚按钮
    _threeBtnLabel = [[UILabel alloc]initWithFrame:CGRectMake(10, 40, 260, 100)];
    _threeBtnLabel.userInteractionEnabled = YES;
    _threeBtnLabel.hidden = YES;
    [_label addSubview:_threeBtnLabel];
    
    
    
    NSArray *Array = [[NSArray alloc]initWithObjects:@"zao_07",@"zhong_09",@"wan_11", nil];
    NSArray *SArray = [[NSArray alloc]initWithObjects:@"zao1_07",@"zhong1_09",@"wan1_11", nil];
    for (int i = 0; i<3; i++)
    {
        UIButton *btn = [UIButton buttonWithType:UIButtonTypeCustom];
        btn.frame = CGRectMake(40+i*65, 15, 48, 48);
        btn.tag = i+10;
        //btn.selected = NO;
        [btn setImage:[UIImage imageNamed:[NSString stringWithFormat:@"%@",[Array objectAtIndex:i]]] forState:UIControlStateNormal];
        [btn setImage:[UIImage imageNamed:[NSString stringWithFormat:@"%@",[SArray objectAtIndex:i]]] forState:UIControlStateSelected];
        [btn addTarget:self action:@selector(timeBtnClick:) forControlEvents:UIControlEventTouchUpInside];
        [_threeBtnLabel addSubview:btn];
        
    }
    
    
    
    
    
}

-(void)btnClick:(UIButton *)btn
{
    
    [UIView animateWithDuration:0.5 animations:^{
         _imageBg.frame = CGRectMake(0, 0, 320, H(self.view));
         _label.hidden = NO;
    }];
    if (btn.tag == 0)
    {

       
        _titleName.text = @"药物名称";
        _contentView.text = _yaowuname?_yaowuname:@"填写药物名称，多个用逗号隔开";
        k =0;
        
    }
    else if (btn.tag == 1)
    {

        _titleName.text = @"服用剂量";
        _contentView.text = _jiliang?_jiliang:@"填写各个药物的服用剂量，多个用逗号隔开";
        k = 1;
    }
    else if (btn.tag == 2)
    {

        _bghongxian.hidden = YES;
        _threeBtnLabel.hidden = NO;
        _titleName.text = @"服药时间";
        k = 2;
        
    }
    else if (btn.tag == 3)
    {
        _titleName.text = @"注意事项";
        _contentView.text = _remark?_remark:@"请在此处填写注意事项";
        k = 3;
    }
}
//早中晚 按钮  判断
-(void)timeBtnClick:(UIButton *)btn
{
    if (btn.tag == 10)
    {
        if (btn.selected == NO)
        {
            btn.selected = YES;
            zao = YES;
        }
        else
        {
            btn.selected = NO;
            zao = NO;
        }
        
    }
    if (btn.tag == 11)
    {
        if (btn.selected == NO)
        {
            btn.selected = YES;
            zhong = YES;
        }
        else
        {
            btn.selected = NO;
            zhong = NO;
        }
    }
    if (btn.tag == 12)
    {
        if (btn.selected == NO)
        {
            btn.selected = YES;
            wan = YES;
        }
        else
        {
            btn.selected = NO;
            wan = NO;
        }
    }
}
-(void)btnBack
{
    
    
    [UIView animateWithDuration:0.5 animations:^{
        _label.hidden = YES;
        _label.hidden = YES;
        _bghongxian.hidden = NO;
        _threeBtnLabel.hidden = YES;
        _imageBg.frame = CGRectMake(0, 0, 320, 0);
    }];
    
}
-(void)btnadd
{
    
    if (k == 0)
    {
        _YaoNameLabel.text = [NSString stringWithFormat:@"药物名称：%@",_contentView.text];
        _yaowuname = _contentView.text.trimString;
    }
    else if(k == 1)
    {
        _YaoNumLabel.text = [NSString stringWithFormat:@"服用剂量：%@",_contentView.text];
        _jiliang = _contentView.text.trimString;

    }
    else if(k == 2)
    {
        NSString *szao = [[NSString alloc]init];
        NSString *szhong = [[NSString alloc]init];
        NSString *swan = [[NSString alloc]init];
        if (zao == YES)
        {
            szao = @"早";
        }
        else
        {
            szao =@"";
        }
        if (zhong == YES)
        {
            szhong = @"中";
        }
        else
        {
            szhong = @"";
        }
        if (wan == YES)
        {
            swan = @"晚";
        }
        else
        {
            swan = @"";
        }
        
        _YaoTimeLabel.text = [NSString stringWithFormat:@"服药时间：%@ %@ %@",szao,szhong,swan];
        
        //药物名称
        _fuyaoTime =  [NSString stringWithFormat:@"%@%@%@",szao,szhong,swan];

    }
    
    else if(k == 3)
    {
        _YaoCareLabel.text = [NSString stringWithFormat:@"注意事项：%@",_contentView.text];
        _remark = _contentView.text.trimString;
    }
    [self btnBack];
}
#pragma mark-----UiTextViewDelegate

-(BOOL)textViewShouldBeginEditing:(UITextView *)textView
{
   
    if ([_contentView.text isEqualToString:@"请在此处填写注意事项"] ||
        [_contentView.text isEqualToString:@"填写各个药物的服用剂量，多个用逗号隔开"] ||
        [_contentView.text isEqualToString:@"填写药物名称，多个用逗号隔开"]) {
         _contentView.text = @"";
    }
      return YES;
}
#pragma mark-----发送数据
-(void)sendinfo
{
    
    MyLog(@"%@-%@---%@-----%@",_yaowuname,_jiliang,_fuyaoTime,_remark);
    
    NSString *msg = nil;
    if (_yaowuname.isEmptyString || _yaowuname==nil) {
        msg = @"药物名称不能为空";
        [SVProgressHUD showErrorWithStatus:msg duration:1];
        return;
    }
    if (_jiliang.isEmptyString || _jiliang==nil) {
        msg = @"药物剂量不能为空";
        [SVProgressHUD showErrorWithStatus:msg duration:1];
        return;
    }
    if (_fuyaoTime.isEmptyString || _fuyaoTime==nil) {
        msg = @"服药时间不能为空";
        [SVProgressHUD showErrorWithStatus:msg duration:1];
        return;
    }
    if (_remark.isEmptyString || _remark==nil) {
        msg = @"注意事项不能为空";
        [SVProgressHUD showErrorWithStatus:msg duration:1];
        return;
    }
   
    
    //发送数据
    [SVProgressHUD showWithStatus:@"正在添加服药单" maskType:SVProgressHUDMaskTypeBlack];
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"yaowuname":_yaowuname,
                             @"jiliang":_jiliang,
                             @"fuyaotime":_fuyaoTime,
                             @"remark":_remark};
    [HttpTool getWithPath:@"addfuyao" params:params success:^(id JSON) {
        MyLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] == 0) {
            
            [SVProgressHUD dismiss];
            [self popController:@"ZJFuyaodanViewController" withSel:@selector(getDataForHeaderOrFooter:) withObj:@"header"];
            
            //[self.navigationController popViewControllerAnimated:YES];
        }
    } failure:^(NSError *error) {
        kPdismiss;
    }];
    
    
    NSLog(@"发送信息");
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



@end
  