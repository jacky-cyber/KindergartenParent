//
//  ZJHomeViewController.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJHomeViewController.h"
#import "ZJHomeCell.h"
#import "ZJHomeDetialViewController.h"
#import "ZJCookBooksViewController.h"
#import "ZJSignInViewController.h"
#import "ZJDayReportViewController.h"
#import "ZJUserInfoViewController.h"
#import "ZJUserInfoModel.h"
#import "ZJHonorViewController.h"
#import "ZJDrugStatusController.h"
#import "ZJMonthCommentViewController.h"
#import "ZJWeekReportsViewController.h"
#import "ZJActivityViewController.h"
#import "ZJSystemMsgViewController.h"
#import "ZJHomeModel.h"
#import "InsetsLabel.h"
#define kFoodtH 0

@interface ZJHomeViewController ()<UITableViewDelegate,UITableViewDataSource,UINavigationControllerDelegate,UIImagePickerControllerDelegate,UIActionSheetDelegate>
{
    UITableView *_tableView;
    NSDictionary *_userinfo;//用户信息
    
    ZJUserInfoModel *_uinfo;
    UIView *headView;//
    
    NSMutableArray *_allDays;//按天分组
    NSMutableArray *_oneDayData;//一天的数据
    
}
@end

@implementation ZJHomeViewController
-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    // page = 1;
}
-(void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:YES];
}
-(void)viewDidAppear:(BOOL)animated
{
    [super viewDidAppear:YES];
}
-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    page = 2;
    
}
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title = @"家长中心";
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        //height -=20;
    }
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0,0,self.view.frame.size.width,height) style:UITableViewStyleGrouped];

    _tableView.delegate = self;
    _tableView.dataSource = self;
    [self.view addSubview:_tableView];
    
    _tableView.rowHeight = [ZJHomeCell cellHeight];
    // 1.设置背景
    // backgroundView的优先级 > backgroundColor
    _tableView.backgroundView = nil;
    //    // 0~1
    //_tableView.backgroundColor = kGlobalBg;
    
    // 2.设置tableView每组头部的高度
    _tableView.sectionHeaderHeight = kFoodtH;
    _tableView.sectionFooterHeight = kFoodtH;
    
    [_tableView setSeparatorStyle:UITableViewCellSeparatorStyleNone];
    //[_tableView setSeparatorColor:[UIColor redColor]];
    //设置tableViewheadView
    headView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 320, 180)];
    headView.backgroundColor = [UIColor whiteColor];
    _tableView.tableHeaderView = headView;
    
    
    UIImageView *backImageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"beijing"]];
    backImageView.frame = CGRectMake(0, 0, 320, H(headView)*0.8);
    [headView addSubview:backImageView];
    

    
    //初始化分组数组
    _allDays = [NSMutableArray array];
    
    //初始化每天的数据
    _oneDayData = [NSMutableArray array];
    
    //加载通知信息
    [self loadNotifi];
    
    [self loadHeadView];
    
    // 集成刷新控件
    [self setupRefresh];
    
}



#pragma mark 开始进入刷新状态
static int page = 2;
- (void)footerRereshing
{
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,@"page":@(page)};
    
    [HttpTool getWithPath:@"getnotifi" params:params success:^(id JSON) {
        //NSLog(@"%@",JSON[@"data"]);
        
        //第一层循环所有的天数
        for (NSDictionary *dict in JSON[@"data"]) {
            NSMutableArray*oneDayDaya = [NSMutableArray array];
            //循环每天的数据
            for (NSDictionary *dayDatas in dict[@"list"]) {
                // NSLog(@"%@",dayDatas);
                ZJHomeModel *model = [[ZJHomeModel alloc] init];
                [model setKeyValues:dayDatas];
                [model setNid:dayDatas[@"id"]];
                [oneDayDaya addObject:model];
            }
            
            [_allDays addObject:oneDayDaya];
            
        }

        //加载成功
        page ++;
        [_tableView reloadData];
        // (最好在刷新表格后调用)调用endRefreshing可以结束刷新状态
        [_tableView footerEndRefreshing];

    } failure:^(NSError *error) {
        [_tableView footerEndRefreshing];
        NSLog(@"%@",error.localizedDescription);
    }];
    
}

/**
 *  集成刷新控件
 */
- (void)setupRefresh
{
    // 1.下拉刷新(进入刷新状态就会调用self的headerRereshing)
    [_tableView addHeaderWithTarget:self action:@selector(loadNotifi)];
//#warning 自动刷新(一进入程序就下拉刷新)
    //[self.tableView headerBeginRefreshing];
    
    // 2.上拉加载更多(进入刷新状态就会调用self的footerRereshing)
    [_tableView addFooterWithTarget:self action:@selector(footerRereshing)];
}


-(void)loadHeadView
{
        //设置家长名字
  
    InsetsLabel * parentName=[[InsetsLabel alloc] initWithFrame:CGRectMake(70,H(headView)*0.8-25,180,25)];
    parentName.insets = UIEdgeInsetsMake(0, 10, 0, 0);

    parentName.backgroundColor =  [UIColor colorWithWhite:0.000 alpha:0.200];
    parentName.textColor = [UIColor colorWithWhite:0.996 alpha:1.000];
    parentName.layer.cornerRadius = 5;
    parentName.layer.masksToBounds = YES;
    
    NSString *str  = [NSString stringWithFormat:@"%@    %@",[LoginUser sharedLoginUser].nickname,[LoginUser sharedLoginUser].classes];
    
    //班级的长度
    int classesLeng  = [LoginUser sharedLoginUser].classes.length;
    
    // 如果想要改变部份文本内容的风格，我们就需要用到NSAttributedString NSMutableAttributedString
    NSMutableAttributedString *attributeString = [[NSMutableAttributedString alloc] initWithString:str];
    //设置属性
    [attributeString addAttribute:NSFontAttributeName value:[UIFont systemFontOfSize:11.0f] range:NSMakeRange(str.length-classesLeng, classesLeng)];
    parentName.font = kFont(16);
    [parentName setAttributedText:attributeString];
    
    //parentName.text = str;
    [headView addSubview:parentName];
    
    //设置头像
    UIImageView *prifileImg = [[UIImageView alloc] init];
    prifileImg.tag = 1;
    prifileImg.frame = CGRectMake(20, H(headView)*0.8-45, 60, 60);
    //设置头像
    [prifileImg setImageWithURL:[NSURL URLWithString:[LoginUser sharedLoginUser].profilImg] placeholderImage:nil];
    prifileImg.layer.cornerRadius = 5;
    prifileImg.layer.masksToBounds = YES;
    prifileImg.userInteractionEnabled = YES;
    //单击头像
    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc]
                                   initWithTarget:self action:@selector(updateProfileAction)];
    [prifileImg addGestureRecognizer:tap];
    [headView addSubview:prifileImg];


    //荣誉榜
    UIButton *btn = [[UIButton alloc] initWithFrame:CGRectMake(290,H(headView)*0.8-22, 16, 22)];
    [btn setImage:[UIImage imageNamed:@"honor"] forState:UIControlStateNormal];
    // btn.backgroundColor = [UIColor colorWithRed:0.400 green:1.000 blue:1.000 alpha:1.000];
    [btn addTarget:self action:@selector(honorAction) forControlEvents:UIControlEventTouchUpInside];
    [headView addSubview:btn];
    
//    bringSubviewToFront
    [self.view bringSubviewToFront:prifileImg];
}

#pragma mark 获取通知信息
-(void)loadNotifi
{
    
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,@"page":@"1"};
    
    [HttpTool getWithPath:@"getnotifi" params:params success:^(id JSON) {
        //NSLog(@"%@",JSON[@"data"]);
        
        //删除所有的数据先
        [_allDays removeAllObjects];
        //第一层循环所有的天数
        for (NSDictionary *dict in JSON[@"data"]) {
            NSMutableArray*oneDayDaya = [NSMutableArray array];
            //循环每天的数据
            for (NSDictionary *dayDatas in dict[@"list"]) {
               // NSLog(@"%@",dayDatas);
                ZJHomeModel *model = [[ZJHomeModel alloc] init];
                [model setKeyValues:dayDatas];
                [model setNid:dayDatas[@"id"]];
                [oneDayDaya addObject:model];
            }
            
            [_allDays addObject:oneDayDaya];
           
        }
        
         MyLog(@"----------------%d",_allDays.count);
        [_tableView reloadData];
        // (最好在刷新表格后调用)调用endRefreshing可以结束刷新状态
        [_tableView headerEndRefreshing];
    } failure:^(NSError *error) {
        NSLog(@"%@",error.localizedDescription);
    }];
}


#pragma mark 荣誉版
-(void)honorAction
{
     [self pushController:[ZJHonorViewController class] withInfo:nil withTitle:@"荣誉榜"];
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(UIView*)tableView:(UITableView *)tableView viewForFooterInSection:(NSInteger)section{
    
    UIView *view = [[UIView alloc] init];
    view.frame = CGRectMake(0, 0, 320, kFoodtH);
    view.backgroundColor = [UIColor redColor];
    return view;
}

-(CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section
{
    return kFoodtH;
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    NSArray *oneDatData = _allDays[section];
    //MyLog(@"-----------%@------%d",oneDatData,oneDatData.count);
    
    return oneDatData.count;
}
-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
     //MyLog(@"---------_allDays------%d",_allDays.count);
    return _allDays.count;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    

    ZJHomeCell *cell = [tableView dequeueReusableCellWithIdentifier:[ZJHomeCell ID]];
    if (cell == nil) {
        
        cell = [[ZJHomeCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:[ZJHomeCell ID]];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    
    
    ZJHomeModel *model  = _allDays[indexPath.section][indexPath.row];
    //    通知类型
    //    1	系统消息
    //    2	幼儿园通知
    //    3	本周食谱
    //    4	每日一报
    //    5	每周一报
    //    6	医务室通知
    //    7	荣誉榜
    //    8	老师通知
    //    9	生日提醒
    //    10	活动通知
    //    11	签到/签退
    //    12	月评
    
    NSString *title = nil;
    
    if([model.type isEqual:@1]){
        title = @"系统消息";
    }else if([model.type isEqual:@2]){
        title = @"幼儿园通知";
    }else if([model.type isEqual:@3]){
        title = @"本周食谱";
    }else if([model.type isEqual:@4]){
        title = @"每日一报";
    }else if ([model.type isEqual:@5]) {
        title = @"每周一报";
    }else if([model.type isEqual:@6]){
        title = @"医务室通知";
    }else if([model.type isEqual:@7]){
        title = @"荣誉榜";
    }else if([model.type isEqual:@8]){
        title = @"老师通知";
    }else if([model.type isEqual:@9]){
        title = @"生日提醒";
    }else if([model.type isEqual:@10]){
        title = @"活动信息";
    }else if ([model.type isEqual:@11]) {
        title = @"签到，签退";
    }else if([model.type isEqual:@12]){
        title = @"老师月评";
    }
    
    
    
    NSString *typeImageName = [NSString stringWithFormat:@"type_%@",model.type];
    
    cell.typeImg.image = [UIImage imageNamed:typeImageName];
    cell.titleLb.text = title;
    
    //格式化时间
    TimeFormatTools *timetools  = [[TimeFormatTools alloc] init];
    NSString *timeStr = [timetools timeToNow:model.createtime];
    
    if (timeStr.length >10) {
        timeStr = [timeStr substringToIndex:10];
    }
    
    cell.timeLb.text = timeStr;
    cell.contentLb.text = model.content;
    return cell;
}

-(NSInteger)tableView:(UITableView *)tableView indentationLevelForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 10;
}


-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    //[self.navigationController pushViewController:[[ZJHomeDetialViewController alloc] init] animated:YES];
    
    ZJHomeModel *model  = _allDays[indexPath.section][indexPath.row];
    
    
//    通知类型
//    1	系统消息
//    2	幼儿园通知
//    3	本周食谱
//    4	每日一报
//    5	每周一报
//    6	医务室通知
//    7	荣誉榜
//    8	老师通知
//    9	生日提醒
//    10	活动通知
//    11	签到/签退
//    12	月评
    if ([model.type isEqual:@5]) {
        [self pushController:[ZJWeekReportsViewController class] withInfo:model.nid withTitle:@"每周一报"];
    }else if ([model.type isEqual:@3]) {
        [self pushController:[ZJCookBooksViewController class] withInfo:model.nid withTitle:@"本周食谱"];
    }else if ([model.type isEqual:@11]) {
        [self pushController:[ZJSignInViewController class] withInfo:model.nid withTitle:@"未到原因"];
    }else if([model.type isEqual:@12]){
       [self pushController:[ZJMonthCommentViewController class] withInfo:model withTitle:@"月评"];
    }else if([model.type isEqual:@6]){
        [self pushController:[ZJDrugStatusController class] withInfo:model withTitle:@"服药通知"];
    }else if([model.type isEqual:@4]){
        [self pushController:[ZJDayReportViewController class] withInfo:model.nid withTitle:@"每日一报"];
    }else if([model.type isEqual:@7]){
        [self pushController:[ZJHonorViewController class] withInfo:model.nid withTitle:@"荣誉榜"];
    }else if([model.type isEqual:@1]){
        [self pushController:[ZJSystemMsgViewController class] withInfo:model withTitle:@"系统通知"];
    }else if([model.type isEqual:@10]){
        [self pushController:[ZJActivityViewController class] withInfo:model withTitle:@"通知详情"];
    }else if([model.type isEqual:@2]){
        [self pushController:[ZJHomeDetialViewController class] withInfo:model withTitle:@"通知详情"];
    }

}

#pragma mark 修改头像
-(void)updateProfileAction
{
    NSLog(@"%@",_uinfo);
    [self pushController:[ZJUserInfoViewController class] withInfo:_uinfo withTitle:@"详情"];
    return;
    UIActionSheet *sheet = [[UIActionSheet alloc] initWithTitle:nil delegate:self cancelButtonTitle:@"取消" destructiveButtonTitle:@"拍照" otherButtonTitles:@"选择照片", nil];
    
    [sheet showInView:self.view];
    
}
-(void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
    
    NSLog(@"%d", buttonIndex);
    if (buttonIndex == 2) {
        return;
    }
    
    UIImagePickerController *controller = [[UIImagePickerController alloc] init];
   
    if (buttonIndex == 0) {
         NSString *deviceModel=[UIDevice currentDevice].model;
        if ([deviceModel isEqual:@"iPhone Simulator"]) {
            [SVProgressHUD showErrorWithStatus:@"模拟器不支持拍照" duration:2];
            return;
        }
        controller.sourceType = UIImagePickerControllerSourceTypeCamera;
    }else{
        controller.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    }
    
    controller.allowsEditing = YES;
    controller.delegate = self;
    
    [self presentViewController:controller animated:YES completion:nil];
    
    
}
#pragma mark UIImagePickerController代理方法
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    // 提示：UIImage不能为空
    // NSData *data = UIImagePNGRepresentation(self.imageView.image);
    UIImage *image = info[UIImagePickerControllerOriginalImage];
    
    //UIImage *img = [UIImage imageNamed:@"some.png"];
    //_imageData = UIImageJPEGRepresentation(image, 1.0);
    
    UIButton *btn = (UIButton*)[self.view viewWithTag:1];
    [btn setImage:image forState:UIControlStateNormal];
    
    
    [self dismissViewControllerAnimated:YES completion:nil];
}

@end
