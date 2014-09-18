//
//  ZJSetingViewController.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJSetingViewController.h"
#import "ZJSetingCell.h"
#import "ZJYijianViewController.h"
#import "ZJAboutMeViewController.h"
#import "ZJLoginViewController.h"
@interface ZJSetingViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    NSArray *_dataArr;//数据源
    UITableView *_talbeView;
    
    BOOL isPush;//判断是否打开推送开关
}
@end

@implementation ZJSetingViewController

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
    // Do any additional setup after loading the view.
    self.title = @"设置";
    
    
    _talbeView = [[UITableView alloc] init];
    _talbeView.frame = CGRectMake(0,20,self.view.frame.size.width,self.view.frame.size.height);
    _talbeView.delegate = self;
    _talbeView.dataSource = self;
    _talbeView.rowHeight = [ZJSetingCell cellH];
    [self.view addSubview:_talbeView];
    _talbeView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    _dataArr = @[@"消息推送",@"意见反馈",@"关于我们",@"版本信息",@"注销"];
    
//     [_talbeView registerNib:[UINib nibWithNibName:@"ZJSetingCell" bundle:nil] forCellReuseIdentifier:[ZJSetingCell ID]];
    
    
    //判断是否打开推送开关
    [self pushmsgstatus];
    
    
}
#pragma mark 登出
-(void)loginoutAction
{
    ZJLoginViewController *login =  [[ZJLoginViewController alloc] init];
    self.view.window.rootViewController =login ;
    [[EaseMob sharedInstance].chatManager asyncLogoff];
    [[LoginUser sharedLoginUser] loginout];
    
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _dataArr.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
   // static NSString *CellIdentifier = @"ZJHomeCell";
    
    ZJSetingCell *cell = [tableView dequeueReusableCellWithIdentifier:[ZJSetingCell ID]];
    if (cell == nil) {
        cell = [[ZJSetingCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:[ZJSetingCell ID]];
    }
    cell.titleLb.text = _dataArr[indexPath.row];
  
    if (indexPath.row ==0) {
        UISwitch *swh = [[UISwitch alloc] init];
        swh.frame = CGRectMake(W(cell.bgView)-W(swh)-10, 10, 50, H(cell.bgView));
        [swh setOn:isPush];
        [cell.bgView addSubview:swh];
        [swh addTarget:self action:@selector(upStatusAction:) forControlEvents:UIControlEventValueChanged];
    }else if (indexPath.row == 3){
        NSString *key = (NSString *)kCFBundleVersionKey;
        
        // 1.从Info.plist中取出版本号
        NSString *version = [NSBundle mainBundle].infoDictionary[key];
        cell.versionLabel.text = [@"V" appendStr:version];
        cell.versionLabel.alpha = 1;
    }else if (indexPath.row == 4){
        cell.titleLb.alpha = 0;
        cell.bgView.backgroundColor = [UIColor redColor];
        cell.bgView.layer.backgroundColor = [UIColor redColor].CGColor;
        cell.bgView.layer.cornerRadius = 5;
        cell.bgView.layer.masksToBounds = YES;
        cell.bgView.layer.borderWidth = 0;
        UILabel *lael = [[UILabel alloc] initWithFrame:CGRectMake(100, 20, 120, 21)];
        lael.text = _dataArr[indexPath.row];
        [lael setTextColor:[UIColor whiteColor]];
        lael.backgroundColor = [UIColor redColor];
        lael.textAlignment = NSTextAlignmentCenter;
        [cell addSubview:lael];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    return cell;
}
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 1) {
        [self pushController:[ZJYijianViewController class] withInfo:nil withTitle:@"意见反馈"];
    }else if(indexPath.row ==2){
         [self pushController:[ZJAboutMeViewController class] withInfo:nil withTitle:@"关于我们"];
    }else if (indexPath.row == 4){
        [self loginoutAction];
    }
}

//判断是否推送状态
-(void)pushmsgstatus{
    
    //182.18.23.244:8080/kindergarten/service/app!pushmsgstatus.action?username=xuesheng
    [HttpTool getWithPath:@"pushmsgstatus" params:@{@"username":[LoginUser sharedLoginUser].userName} success:^(id JSON) {
        MyLog(@"%@",JSON);
        
//        code = 0;
//        data =     {
//            status = 2;
//        };
//        msg = success;
        //1,开通，2关闭 默认关闭
        if ([JSON[@"data"][@"status"] isEqual:@2]) {
            isPush = NO;
        }else{
            isPush = YES;
        }
        [_talbeView reloadData];
    } failure:^(NSError *error) {
        kPE(kHttpErrorMsg, 0.5);
    }];
    
}

#pragma mark 修改

-(void)upStatusAction:(UISwitch*)sw{
    BOOL isButtonOn = [sw isOn];
    if (isButtonOn) {
        [self pushOnOff];
        MyLog(@"YES");
    }else {
        MyLog(@"NO");
        [self pushOnOff];
    }
}

-(void)pushOnOff{
    
    //182.18.23.244:8080/kindergarten/service/app!openpushmsgstatus.action?username=xuesheng&notifistatus=1

    //1打开
    int  notifistatus = 2;
     //消息推送状态 1 推送，2 不推送，
    
    //isPush == NO 就是推送关闭转状态，那个就打开
    if (isPush == NO) {
        //2.关闭
        notifistatus = 1;
    }
    
    [HttpTool getWithPath:@"openpushmsgstatus" params:@{@"username":[LoginUser sharedLoginUser].userName,@"notifistatus":@(notifistatus)} success:^(id JSON) {
        MyLog(@"%@",JSON);
        
        //消息推送状态 1 推送，2 不推送，默认推送
        if ([JSON[@"code"] intValue] == 0) {
            isPush = !isPush;
            MyLog(@"success");
        }else{
            MyLog(@"failure");
        }
    } failure:^(NSError *error) {
        
    }];

}

@end
