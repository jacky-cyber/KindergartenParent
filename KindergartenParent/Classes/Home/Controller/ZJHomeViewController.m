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
#import "ZJHomeModel.h"
#define kFoodtH 0

@interface ZJHomeViewController ()<UITableViewDelegate,UITableViewDataSource,UINavigationControllerDelegate,UIImagePickerControllerDelegate,UIActionSheetDelegate>
{
    UITableView *_tableView;
    NSDictionary *_userinfo;//用户信息
    
    ZJUserInfoModel *_uinfo;
    UIView *headView;//
    
    NSMutableArray *_oneDayData;//一天的数据
}
@end

@implementation ZJHomeViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    self.title = @"家长中心";
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
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
    
   
    //load加载用户信息
    //[self loadUserInfo];
    
    //加载通知信息
    [self loadNotifi];
    
    [self loadHeadView];
    
    //加载数据
    [self loadData];
}

-(void)loadData
{
    _oneDayData = [NSMutableArray arrayWithCapacity:12];
    
    
    
    for (int i = 1; i<12; i++) {
        ZJHomeModel *model1 = [[ZJHomeModel alloc] init];
        model1.nid = @"110";
        model1.type = [NSString stringWithFormat:@"%d",i];
        model1.content = @"系统消息";
        model1.createtime = @"2014-06-01 18:18:00";
        model1.createuid = @"123";
        [_oneDayData addObject:model1];

    }
    
}



-(void)loadHeadView
{
    //设置头像
    UIButton *profileImgbtn = [[UIButton alloc] init];
    profileImgbtn.tag = 1;
    profileImgbtn.frame = CGRectMake(20, H(headView)*0.8-45, 60, 60);
    
    
    [profileImgbtn setImage:[UIImage imageNamed:@"201277441619594489.jpg"] forState:UIControlStateNormal];
    [profileImgbtn addTarget:self action:@selector(updateProfileAction) forControlEvents:UIControlEventTouchUpInside];
    profileImgbtn.layer.cornerRadius = 10;
    profileImgbtn.layer.masksToBounds = YES;
    [headView addSubview:profileImgbtn];
    //设置家长名字
    //NSString *nickname = [LoginUser sharedLoginUser].nickname;
    UILabel *parentName = [[UILabel alloc] init];
    parentName.frame = CGRectMake(XW(profileImgbtn),H(headView)*0.8-25,100,25);
    parentName.backgroundColor = [UIColor colorWithWhite:0.000 alpha:0.200];
    
    
    
    parentName.font = kFont(16);
    parentName.text = [LoginUser sharedLoginUser].nickname;
    [headView addSubview:parentName];
    //设置学生班级
    //NSString *classes = [LoginUser sharedLoginUser].nickname;
    UILabel *userNameLabel = [[UILabel alloc] init];
    userNameLabel.frame = CGRectMake(XW(parentName),Y(parentName),50,H(parentName));
    
    userNameLabel.text =  [LoginUser sharedLoginUser].classes;;
    userNameLabel.font = kFont(13);
    userNameLabel.backgroundColor = [UIColor colorWithWhite:0.000 alpha:0.200];
    [headView addSubview:userNameLabel];
    //荣誉榜
    UIButton *btn = [[UIButton alloc] initWithFrame:CGRectMake(270,H(headView)*0.8-22, 16, 22)];
    [btn setImage:[UIImage imageNamed:@"honor"] forState:UIControlStateNormal];
    // btn.backgroundColor = [UIColor colorWithRed:0.400 green:1.000 blue:1.000 alpha:1.000];
    [btn addTarget:self action:@selector(honorAction) forControlEvents:UIControlEventTouchUpInside];
    [headView addSubview:btn];

}
-(void)loadUserInfo{
    [HttpTool getWithPath:@"getuserinfo" params:@{@"username":[LoginUser sharedLoginUser].userName} success:^(id JSON) {
        //MyLog(@"%@",JSON);
        _uinfo = [[ZJUserInfoModel alloc] init];
        [_uinfo setKeyValues:JSON[@"data"]];
        NSLog(@"%@",_uinfo.name);
        
    } failure:^(NSError *error) {
        
    }];
}
#pragma mark 获取通知信息
-(void)loadNotifi
{
    
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,@"page":@"1"};
    
    [HttpTool getWithPath:@"getnotifi" params:params success:^(id JSON) {
        NSLog(@"%@",JSON);
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
    return _oneDayData.count;
}
-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 2;
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    

    ZJHomeCell *cell = [tableView dequeueReusableCellWithIdentifier:[ZJHomeCell ID]];
    if (cell == nil) {
        
        cell = [[ZJHomeCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:[ZJHomeCell ID]];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    
    
    cell.typeImg.image = [UIImage imageNamed:@"cookbook"];
    cell.titleLb.text = @"本周食谱已更新";
    cell.timeLb.text = @"一天前";
    cell.contentLb.text = @"您好，你的孩子已经服药了，早上没有忘记吃药，请您放心，吃药了，吃了";
    return cell;
}

-(NSInteger)tableView:(UITableView *)tableView indentationLevelForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 10;
}


-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    //[self.navigationController pushViewController:[[ZJHomeDetialViewController alloc] init] animated:YES];
    if (indexPath.row == 0) {
        [self pushController:[ZJWeekReportsViewController class] withInfo:nil withTitle:@"每周一报"];
    }else
    if (indexPath.row == 1) {
        [self pushController:[ZJCookBooksViewController class] withInfo:nil withTitle:@"详情"];
    }else if (indexPath.row == 2) {
        [self pushController:[ZJSignInViewController class] withInfo:nil withTitle:@"详情"];
    }else if(indexPath.row == 3){
       [self pushController:[ZJMonthCommentViewController class] withInfo:nil withTitle:@"月评"];
    }else if(indexPath.row == 4){
        [self pushController:[ZJDrugStatusController class] withInfo:nil withTitle:@"服药通知"];
    }else if(indexPath.row == 5){
        [self pushController:[ZJDayReportViewController class] withInfo:nil withTitle:@"每日一报"];
    }
    
    else{
        [self pushController:[ZJHomeDetialViewController class] withInfo:nil withTitle:@"详情"];
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
        if ([deviceModel isEqualToString:@"iPhone Simulator"]) {
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
