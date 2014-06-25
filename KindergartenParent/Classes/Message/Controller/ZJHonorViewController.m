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
    
    
    //加载数据
    [self loadData];
}

-(void)ldView
{

   
}

-(void)loadData
{
    [SVProgressHUD showWithStatus:@"加载数据中" maskType:SVProgressHUDMaskTypeBlack];
    
    NSDictionary *params = @{@"date":@"2014-06-02 00:00:00",@"username":[LoginUser sharedLoginUser].userName};
    
    [HttpTool getWithPath:@"honor" params:params success:^(id JSON) {
        if ([JSON[@"code"] intValue] ==0) {
            NSArray *arr = JSON[@"data"];
            for (int i = 0; i<arr.count; i++) {
                ZJHonorModel *model1  = [[ZJHonorModel alloc] init];
                
                 [model1 setKeyValues:arr[i]];
                
                [_dataArr addObject:model1];
            }
           
            //加载界面
             [self.view addSubview:_tableView];
            [SVProgressHUD showSuccessWithStatus:@"请求成功" duration:1];
        }else{
            [SVProgressHUD showErrorWithStatus:@"请求失败" duration:1];
        }
    } failure:^(NSError *error) {
        [SVProgressHUD showErrorWithStatus:@"网络连接错误" duration:1];
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
    NSLog(@"%@---%@----%@",model.flower,model.sun,model.praise);
    
    height = 42 * colNum +10;
    
    return height;
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



@end
