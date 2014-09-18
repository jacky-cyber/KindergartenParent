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
#import "ZJCourseCell.h"
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
    _tableView.sectionFooterHeight = 1;
    
    //设置背景，ios6里面不然就很乱
    UIView *backView = [[UIView alloc] init];
     backView.backgroundColor = [UIColor whiteColor];
    _tableView.backgroundView = backView;
   
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
            kPE(JSON[@"msg"], 0.5);
            return ;
        }
        kPdismiss;
        NSDictionary *data = JSON[@"data"];
        
        
        if (data[@"monday"]) {//周1
            NSArray *arr = data[@"monday"];
            [self addDataSource:arr];
        }
        if (data[@"tuesday"]) {//周2
            
            NSArray *arr = data[@"tuesday"];
            [self addDataSource:arr];

        }
        if (data[@"thursday"]) {
            NSArray *arr = data[@"thursday"];
            [self addDataSource:arr];

        }
        if (data[@"wednesday"]) {
            NSArray *arr = data[@"wednesday"];
            [self addDataSource:arr];
        }
        if (data[@"friday"]) {
            NSArray *arr = data[@"friday"];
            [self addDataSource:arr];        }
        if (data[@"saturday"]) {
            NSArray *arr = data[@"saturday"];
            [self addDataSource:arr];        }
        if (data[@"sunday"]) {
            NSArray *arr = data[@"sunday"];
            [self addDataSource:arr];
        }
        
        [_tableView reloadData];
        
       // MyLog(@"%@",_dataArr);
        
    } failure:^(NSError *error) {
         kPE(kHttpErrorMsg, 0.5);
        MyLog(@"%@",error.localizedDescription);
    }];
   
    
    
    
    
}

-(void)addDataSource:(NSArray*)arr
{
    NSMutableArray *data = [NSMutableArray array];
    for (int i=0; i<arr.count; i++) {
        NSDictionary *dict = arr[i];
        if (![dict[@"content"] isEqualToString:@""]) {
            [data addObject:dict];
        }
    }
    if (arr.count>0) {
        [_dataArr addObject:data];
    }
}

-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView{
    return _dataArr.count;
}

-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
    UIView *bgView = [[UIView alloc] init];
    
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(20, 20, 280, 30)];
    NSString *imageName = [NSString stringWithFormat:@"caurse0%d",section+1];
    imageView.image = [UIImage imageNamed:imageName];
    [bgView addSubview:imageView];
    return bgView;
}
-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
    return 50;
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
    
    ZJCourseCell *cell = [tableView dequeueReusableCellWithIdentifier:ID];
    if (cell == nil) {
        cell = [[ZJCourseCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:ID];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    //设置背景，ios6里面不然就很乱
//    UIView *backView = [[UIView alloc] init];
//    [cell setBackgroundView:backView];

    
    NSDictionary *dict = _dataArr[indexPath.section][indexPath.row];
    ZJCourseModel *cModel = [[ZJCourseModel alloc] init];
    [cModel setKeyValues:dict];
    
    cell.model = cModel;

    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    NSDictionary *dict = _dataArr[indexPath.section][indexPath.row];
    ZJCourseModel *cModel = [[ZJCourseModel alloc] init];
    [cModel setKeyValues:dict];
     CGFloat contentH = [cModel.content getHeightByWidth:200 font:kFont(14)];
    contentH = contentH<20?20:contentH;
    return contentH+10;
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
