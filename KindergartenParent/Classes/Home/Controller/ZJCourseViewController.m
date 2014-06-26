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
    
    self.title = @"每周课程";
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
    
    [self ladData];
}
-(void)ladData
{
//    [HttpTool getWithPath:@"recipe" params:@{@"id":@1} success:^(id JSON) {
//        MyLog(@"%@",JSON);
//    } failure:^(NSError *error) {
//        MyLog(@"%@",error.localizedDescription);
//    }];
    _dataArr = [NSMutableArray array];
    for (int i = 0; i<7; i++) {
        ZJCourseModel *cModel = [[ZJCourseModel alloc] init];
        cModel.timestamp = @"11:00-12:00";
        cModel.content = @"吃饭睡觉打豆豆";
        [_dataArr addObject:cModel];
        ZJCourseModel *cModel1 = [[ZJCourseModel alloc] init];
        cModel1.timestamp = @"9:00-9:30";
        cModel1.content = @"还不知道干点啥呢，哈哈哈";
        
        [_dataArr addObject:cModel1];
    }
    
}
-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    return 7;
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
    return _dataArr.count;
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

    
    ZJCourseModel *cModel = _dataArr[indexPath.row];
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
