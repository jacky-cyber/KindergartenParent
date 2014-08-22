//
//  ZJNotificationListViewController.m
//  KindergartenParent
//
//  Created by define on 14-7-25.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJNotificationListViewController.h"
#import "ZJNotificationListCell.h"
#import "ZJDayReportViewController.h"
#import "ZJWeekReportsViewController.h"
#import "ZJHomeDetialViewController.h"
#import "ZJSignInViewController.h"
#import "ZJFuyaodanDetailViewController.h"
#import "ZJHomeModel.h"

@interface ZJNotificationListViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    UITableView *_tableView;
    NSMutableArray *_dataArr;
    
    
}


@end

@implementation ZJNotificationListViewController

//-(void)viewWillAppear:(BOOL)animated
//{
//    [super viewWillAppear:animated];
//    page = 1;
//}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    // Do any additional setup after loading the view.
    //初始化数组
    _dataArr = [NSMutableArray array];
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
    }
    
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, W(self.view),height) style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.rowHeight = 55;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    //_tableView.sectionFooterHeight = 0;
    //_tableView.rowHeight = [ZJCookBookTableViewCell cellHeight];
    
    [self.view addSubview:_tableView];
    
    //重新加载数据
    [self headerRefreshing];
    // 集成刷新控件
    [self setupRefresh];
}


/**
 *  集成刷新控件
 */
- (void)setupRefresh
{
    // 1.下拉刷新(进入刷新状态就会调用self的headerRereshing)
    [_tableView addHeaderWithTarget:self action:@selector(headerRefreshing)];
    //#warning 自动刷新(一进入程序就下拉刷新)
    //[self.tableView headerBeginRefreshing];
    
    // 2.上拉加载更多(进入刷新状态就会调用self的footerRereshing)
    [_tableView addFooterWithTarget:self action:@selector(footerRereshing)];
}
#pragma mark 初始化数据
-(void)headerRefreshing
{
    self.page = 1;
    
    NSDictionary *parmas = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"type":self.userInfo,
                             @"page":@(self.page)};
    kPBlack(@"数据加载中...");
    [HttpTool getWithPath:@"msglist" params:parmas success:^(id JSON) {
        if ([JSON[@"code"] intValue] == 0) {
            [_dataArr removeAllObjects];
            for (NSDictionary *dict in JSON[@"data"]) {
                ZJHomeModel *model = [[ZJHomeModel alloc] init];
                [model setKeyValues:dict];
                [_dataArr addObject:model];
            }
            [_tableView reloadData];
            self.page++;
            kPdismiss;
            [_tableView headerEndRefreshing];
        }
    } failure:^(NSError *error) {
        [_tableView headerEndRefreshing];
        kPE(kHttpErrorMsg, 0.5);
       // MyLog(@"%@",error.localizedDescription);
    }];
}
#pragma mark 初始化数据
-(void)footerRereshing
{
    
    NSDictionary *parmas = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"type":self.userInfo,
                             @"page":@(self.page)};
    kPBlack(@"数据加载中...");
    [HttpTool getWithPath:@"msglist" params:parmas success:^(id JSON) {
        MyLog(@"%@",JSON[@"data"]);
        if ([JSON[@"code"] intValue] == 0) {
            if (((NSArray*)JSON[@"data"]).count) {
                self.page++;
            }
            for (NSDictionary *dict in JSON[@"data"]) {
                ZJHomeModel *model = [[ZJHomeModel alloc] init];
                [model setKeyValues:dict];
                [_dataArr addObject:model];
            }
            [_tableView reloadData];
            
            kPdismiss;
            [_tableView footerEndRefreshing];
        }
    } failure:^(NSError *error) {
        kPE(kHttpErrorMsg, 0.5);
         [_tableView footerEndRefreshing];
        MyLog(@"%@",error.localizedDescription);
    }];
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    
    // NSDictionary *dict = [_dataArr[section] keyValues];
    
    return _dataArr.count ;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString *ID = @"Cell";
    ZJNotificationListCell *cell = [tableView dequeueReusableCellWithIdentifier:ID];
    if (cell == nil) {
        cell = [[ZJNotificationListCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:ID];
    }
    
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
     ZJHomeModel *model = _dataArr[indexPath.row];
    //2,8,9,10
    
    int type = [model.type intValue];
    
    if ( type== 2) {
        cell.bgImage.image = [UIImage imageNamed:@"quanyuanNotifi"];
    }else if ( type== 6) {
        cell.bgImage.image = [UIImage imageNamed:@"fuyaoNotifi"];
    }else if ( type== 8) {
        cell.bgImage.image = [UIImage imageNamed:@"classNotfi"];
    }else if ( type== 9) {
        cell.bgImage.image = [UIImage imageNamed:@"birthdayNotifi"];
    }else if ( type== 10) {
        cell.bgImage.image = [UIImage imageNamed:@"ActivityNotifi"];
    }else if ( type== 11) {
        cell.bgImage.image = [UIImage imageNamed:@"qiandaoNotifi"];
    }
   // MyLog(@"%@----",model.createtime);
    cell.tiemeLabel.text = model.createtime.length>16?[model.createtime substringToIndex:16]:@"";
    
    if ([model.content isEqualToString:@"您的孩子还未入园，请填写未到原因！"]||
        [model.content isEqualToString:@"您的孩子未签退！"]
        )
    {
        cell.contentLabel.textColor = [UIColor colorWithRed:0.988 green:0.341 blue:0.000 alpha:1.000];
    }
    if ([model.content isEqualToString:@"您的孩子还未入园，请填写未到原因！"]) {
        cell.qiandaoBtn.alpha = 1;
    }
    
    cell.contentLabel.text = model.content;
    
        
    return cell;
}




-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{

    ZJHomeModel *model = _dataArr[indexPath.row];
    //2,8,9,10
    
    int type = [model.type intValue];
    
    if ( type== 2) {//全园通知
        [self pushController:[ZJHomeDetialViewController class] withInfo:model withTitle:model.title withOther:nil];
    }else if ( type== 6) {//服药单
        [self pushController:[ZJHomeDetialViewController class] withInfo:model withTitle:model.title withOther:nil];
    }else if ( type== 8) {//本班通知
        [self pushController:[ZJHomeDetialViewController class] withInfo:model withTitle:model.title withOther:nil];
    }else if ( type== 9) {
        
    }else if ( type== 10) {
         [self pushController:[ZJHomeDetialViewController class] withInfo:model withTitle:model.title withOther:@"通知详情"];
//        [self pushController:[ZJActivityViewController class] withInfo:model withTitle:@"通知详情"];
    }else if ( type== 11) {
        if ([model.content isEqualToString:@"您的孩子还未入园，请填写未到原因！"]) {
            [self pushController:[ZJSignInViewController class] withInfo:model.id withTitle:@"未到原因"];
        }
        
    }
    

    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



@end
