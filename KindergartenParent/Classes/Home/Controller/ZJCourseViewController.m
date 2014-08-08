//
//  ZJCourseViewController.m
//  ybparents
//
//  Created by define on 14-6-23.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJCourseViewController.h"
#import "ZJCookBookTableViewCell.h"
#import "ZJCourseModel.h"
@interface ZJCourseViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    UITableView *_tableView;
    NSMutableArray *_dataArr;
}
@end

@implementation ZJCourseViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
       // height -=20;
    }
    
    self.title = @"课程安排";
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, W(self.view),height) style:UITableViewStyleGrouped];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    //_tableView.sectionHeaderHeight = 30;
    _tableView.sectionFooterHeight = 0;
    
    //设置背景，ios6里面不然就很乱
    UIView *backView = [[UIView alloc] init];
    backView.backgroundColor = [UIColor whiteColor];
    [_tableView setBackgroundView:backView];
    
    _tableView.backgroundColor = [UIColor clearColor];
    _tableView.rowHeight = [ZJCookBookTableViewCell cellHeight];
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    [self.view addSubview:_tableView];
    
    
    //初始化数据
    _dataArr = [NSMutableArray array];
    [self ladData];
}
-(void)ladData
{
    
    kPBlack(@"正在加载课程表");
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName};
    [HttpTool getWithPath:@"course" params:params success:^(id JSON) {
       // MyLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] == 1) {
            kPE(kHttpErrorMsg, 0.5);
            return ;
        }
        kPdismiss;
        NSDictionary *data = JSON[@"data"];
        
        
        if (data[@"monday"]) {//周1
            
            [_dataArr addObject:data[@"monday"]];
        }
        if (data[@"tuesday"]) {//周2
            [_dataArr addObject:data[@"tuesday"]];
        }
        if (data[@"thursday"]) {
            [_dataArr addObject:data[@"thursday"]];
        }
        if (data[@"wednesday"]) {
            [_dataArr addObject:data[@"wednesday"]];
        }
        if (data[@"friday"]) {
            [_dataArr addObject:data[@"friday"]];
        }
        if (data[@"saturday"]) {
            [_dataArr addObject:data[@"saturday"]];
        }
        if (data[@"sunday"]) {
            [_dataArr addObject:data[@"sunday"]];
        }
        
        [_tableView reloadData];
        
       // MyLog(@"%@",_dataArr);
        
    } failure:^(NSError *error) {
         kPE(kHttpErrorMsg, 0.5);
        MyLog(@"%@",error.localizedDescription);
    }];
   
    
    
    
    
}
-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    return _dataArr.count;
}

-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
    UIView *bgView = [[UIView alloc] init];
    
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(20, 0, 280, 30)];
    NSString *imageName = [NSString stringWithFormat:@"caurse0%d",section+1];
    imageView.image = [UIImage imageNamed:imageName];
    [bgView addSubview:imageView];
    return bgView;
}
-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
    return 40;
}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    NSArray *daycourse = [NSArray array];
    //MyLog(@"%d",((NSArray*)_dataArr[section]).count);
    if (((NSArray*)_dataArr[section]).count>0) {
        daycourse =  _dataArr[section];
    }
    return daycourse.count;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    
    static NSString *ID = @"Cell";
    
    UITableViewCell *cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:ID];
//    if (cell == nil) {
//        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:ID];
//    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    //设置背景，ios6里面不然就很乱
    UIView *backView = [[UIView alloc] init];
    [cell setBackgroundView:backView];

    
    NSDictionary *dict = _dataArr[indexPath.section][indexPath.row];
    ZJCourseModel *cModel = [[ZJCourseModel alloc] init];
    [cModel setKeyValues:dict];
    //时间段
    
    UILabel *timeLabel = [ZJUIMethods creatLabel:cModel.timestamp frame:CGRectMake(20, 0, 80, 20) font:kFont(14) textColor:nil];
    
    timeLabel.textAlignment = NSTextAlignmentRight;
    //timeLabel.backgroundColor = [UIColor brownColor];
    [cell.contentView addSubview:timeLabel];
    
    //课程内容
    UILabel *contentLabel =[ZJUIMethods creatLabel:cModel.content frame:CGRectMake(XW(timeLabel)+10, Y(timeLabel), 280-W(timeLabel)-10, H(timeLabel)) font:kFont(14) textColor:nil];
    
    contentLabel.textAlignment = NSTextAlignmentLeft;
    //contentLabel.backgroundColor = [UIColor brownColor];
    [cell.contentView addSubview:contentLabel];
    
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 30;
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
