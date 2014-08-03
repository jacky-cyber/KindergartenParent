//
//  ZJHonorViewController.m
//  ybparents
//
//  Created by 郑敬 on 14-6-17.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJHonorViewController.h"
#import "ZJHonorCell.h"
#import "ZJHonorModel.h"
@interface ZJHonorViewController ()<UITableViewDelegate,UITableViewDataSource>
{
     UITableView *_tableView;
    NSMutableArray *_dataArr;
}
@end

@implementation ZJHonorViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
    }
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0,0,self.view.frame.size.width,height)];
    
    _tableView.delegate = self;
    _tableView.dataSource = self;
    [_tableView setSeparatorStyle:UITableViewCellSeparatorStyleNone];
    //_tableView.rowHeight = 100;
    _dataArr = [NSMutableArray array];
    //加载界面
    [self.view addSubview:_tableView];
    
    //加载数据
    [self headerRereshing];
    
    // 集成刷新控件
    [self setupRefresh];
}


/**
 *  集成刷新控件
 */
- (void)setupRefresh
{
    // 1.下拉刷新(进入刷新状态就会调用self的headerRereshing)
    [_tableView addHeaderWithTarget:self action:@selector(headerRereshing)];
    //#warning 自动刷新(一进入程序就下拉刷新)
    //[self.tableView headerBeginRefreshing];
    
    // 2.上拉加载更多(进入刷新状态就会调用self的footerRereshing)
    [_tableView addFooterWithTarget:self action:@selector(footerRereshing)];
}

-(void)headerRereshing
{
    [self getDataForHeaderOrFooter:@"header"];
}

- (void)footerRereshing
{
    
    [self getDataForHeaderOrFooter:@"footer"];
}

-(void)getDataForHeaderOrFooter:(NSString*)str
{
    NSString *timeStr = [TimeFormatTools timeFormatToNow:@"yyyy-MM-dd"];
    if ([str isEqualToString:@"footer"]) {
        MyLog(@"%d",_dataArr.count);
        ZJHonorModel *model = _dataArr.lastObject;
        MyLog(@"%@",model.date);
        timeStr = model.date;
    }
    
    
    kPBlack(@"数据加载中");
    NSDictionary *params = @{@"date":timeStr,@"username":[LoginUser sharedLoginUser].userName};
    
    [HttpTool getWithPath:@"honor" params:params success:^(id JSON) {
        if ([str isEqualToString:@"header"]) {
            [_dataArr removeAllObjects];
        }
        if ([JSON[@"code"] intValue] ==0) {
            NSArray *arr = JSON[@"data"];
            for (int i = 0; i<arr.count; i++) {
                ZJHonorModel *model1  = [[ZJHonorModel alloc] init];
                [model1 setKeyValues:arr[i]];
                
                if ([str isEqualToString:@"footer"]) {
                    if (i==0) {//在加载跟多的时候会把当前的日期的数据再加载过来，所以要过滤掉
                        continue;
                    }
                }
                [_dataArr addObject:model1];
            }
            
            
            [_tableView reloadData];
            if ([str isEqualToString:@"header"]) {
                [_tableView headerEndRefreshing];
            }else{
                [_tableView footerEndRefreshing];
            }
            
            kPdismiss;
        }else{
            if ([str isEqualToString:@"header"]) {
                [_tableView headerEndRefreshing];
            }else{
                [_tableView footerEndRefreshing];
            }
            [SVProgressHUD showErrorWithStatus:@"请求失败" duration:1];
        }
    } failure:^(NSError *error) {
        kPE(kHttpErrorMsg, 0.5);
    }];
}




-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _dataArr.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
   // NSString *CellIdentifier = [NSString stringWithFormat:@"%d",indexPath.row];
     static NSString *CellIdentifier = @"Cell";
    ZJHonorCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell =  [[ZJHonorCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
        
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
//    NSLog(@"%p",cell);
    NSArray *subviews = [[NSArray alloc] initWithArray:cell.rightView.subviews];
    
    for (UIView *subview in subviews) {
        [subview removeFromSuperview];
    }
    cell.model = _dataArr[indexPath.row];
    return cell;
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    CGFloat height = 0;
    ZJHonorModel *model = _dataArr[indexPath.row];
    
  
    //所有表扬个数
    int sum  = model.flower.intValue+ model.sun.intValue + model.praise.intValue;
    int colNum = sum/4;
    if (sum%4 !=0) {
        colNum +=1;
    }
    //NSLog(@"%@---%@----%@",model.flower,model.sun,model.praise);
    
    height = 42 * colNum +10;
    
    return height;
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



@end
