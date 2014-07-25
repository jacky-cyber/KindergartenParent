//
//  ZJDayReportListViewController.m
//  KindergartenParent
//
//  Created by define on 14-7-25.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJDayAndWeekReportListViewController.h"
#import "ZJHomeModel.h"
#import "ZJTDayReportCell.h"
#import "ZJDayReportViewController.h"
#import "ZJWeekReportsViewController.h"
@interface ZJDayAndWeekReportListViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    UITableView *_tableView;
    NSMutableArray *_dataArr;
 
}

@end

@implementation ZJDayAndWeekReportListViewController



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
    [self initData];

}

#pragma mark 初始化数据
-(void)initData
{
 
    
    
    NSString *type = @"4";//日报
    if ([self.userInfo isEqualToString:@"week"]) {
        type = @"5";//周报
    }
    
    NSDictionary *parmas = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"type":type,
                             @"page":@(1)};
    kPBlack(@"数据加载中...");
    [HttpTool getWithPath:@"msglist" params:parmas success:^(id JSON) {
        if ([JSON[@"code"] intValue] == 0) {
           
            for (NSDictionary *dict in JSON[@"data"]) {
                [_dataArr addObject:dict];
            }
            [_tableView reloadData];
            kPdismiss;
        }
    } failure:^(NSError *error) {
        kPE(@"网络错误,请稍再试", 0.5);
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
    ZJTDayReportCell *cell = [tableView dequeueReusableCellWithIdentifier:ID];
    if (cell == nil) {
        cell = [[ZJTDayReportCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:ID];
    }
    
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    
    NSDictionary *dict = _dataArr[indexPath.row];
    
    
    if ([self.userInfo isEqualToString:@"week"]) {
        cell.bgImageView.image = [UIImage imageNamed:@"weekReport"];
    }
    
    cell.timeLabel.text = [dict[@"createtime"] substringToIndex:10];

    return cell;
}




-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
     NSDictionary *dict = _dataArr[indexPath.row];
    
    if ([self.userInfo isEqualToString:@"day"]) {//周报
        [self pushController:[ZJDayReportViewController class] withInfo:dict[@"id"] withTitle:@"每日一报"];
    }else{
        [self pushController:[ZJWeekReportsViewController class] withInfo:dict[@"id"] withTitle:@"每周一报"];
    }
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



@end
