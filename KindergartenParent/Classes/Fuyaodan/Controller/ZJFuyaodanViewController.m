//
//  ZJFuyaodanViewController.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJFuyaodanViewController.h"
#import "ZJFuYaoDanCell.h"
#import "ZJFuyaodanDetailViewController.h"
#import "ZJADDViewController.h"
#import "ZJFuYaoDanModel.h"
@interface ZJFuyaodanViewController ()
{
    UITableView *_tableView;
    NSMutableArray *_dataArr;
}
@end

@implementation ZJFuyaodanViewController



- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.title = @"服药单";
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
    }
    
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 2, 50, 25);
    [btnR addTarget:self action:@selector(addFuyaodan) forControlEvents:UIControlEventTouchUpInside];
    [btnR setImage:[UIImage imageNamed:@"tianjia_03"] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;
    
    
    _tableView = [[UITableView alloc]initWithFrame:CGRectMake(0,0,self.view.frame.size.width,height) style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    [_tableView setSeparatorStyle:UITableViewCellSeparatorStyleNone];
    
    _dataArr = [NSMutableArray array];
    
    [self.view addSubview:_tableView];
    
    
    //加载数据
    [self initData:false];
    
}

-(void)initData:(BOOL)flag{
    
    
    [SVProgressHUD showWithStatus:@"正在加载服药单" maskType:SVProgressHUDMaskTypeBlack];
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"page":@"1"};
    [HttpTool getWithPath:@"fylist" params:params success:^(id JSON) {
        MyLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] == 0) {
            
            //这是添加服药单返回来的时候,先删除所有的
            if (flag) {
                [_dataArr removeAllObjects];
            }
            
            for (NSDictionary *dict in JSON[@"data"]) {
                //将数据模型化
                ZJFuYaoDanModel *model = [[ZJFuYaoDanModel alloc] init];
                [model setKeyValues:dict];
                
               
                [_dataArr addObject:model];
            }
            //重新刷新tableview
            [_tableView reloadData];
            
            [SVProgressHUD dismiss];        
        }
    } failure:^(NSError *error) {
        
    }];
}



#pragma mark----- UITableView
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _dataArr.count;
    //return 10;
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 45;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    ZJFuYaoDanCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil)
    {
        cell = [[ZJFuYaoDanCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    
    //cell 数据Model 补充
    cell.fydmodel = _dataArr[indexPath.row];
    /*
    
     
     
     */
    return cell;
}
//点击cell 跳转下级界面
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    //根据具体情况修改

    [self pushController:[ZJFuyaodanDetailViewController class] withInfo:_dataArr[indexPath.row] withTitle:@"服药单详情"];
    
}

-(void)addFuyaodan
{
    NSLog(@"添加服药单");
    
    [self pushController:[ZJADDViewController class] withInfo:nil];
    
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



@end
