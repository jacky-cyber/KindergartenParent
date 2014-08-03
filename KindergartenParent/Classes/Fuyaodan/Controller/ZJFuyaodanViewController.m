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
    
    
    [self getDataForHeaderOrFooter:@"header"];
    
    //加载数据
    //[self headerRefreshing];
    
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

-(void)headerRefreshing
{
    [self getDataForHeaderOrFooter:@"header"];
}

- (void)footerRereshing
{
    [self getDataForHeaderOrFooter:@"footer"];
}
-(void)getDataForHeaderOrFooter:(NSString*)str
{
//    username	String	服药人id（老是查看就传老师username）
//    type	String	(0,个人 1全班，2全员)
//    page	int	分页（默认 1）
//    classid	String	班级ID
//    kid	String	幼儿园id（医务室可以查看全员的服药单）
    if ([str isEqualToString:@"header"]) {
        self.page = 1;
    }
    [SVProgressHUD showWithStatus:@"正在加载服药单" maskType:SVProgressHUDMaskTypeBlack];
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"type":@"0",
                             @"classid":[LoginUser sharedLoginUser].classid,
                             @"page":@(self.page)};
    [HttpTool getWithPath:@"fylist" params:params success:^(id JSON) {
        MyLog(@"%@",JSON);
        if ([str isEqualToString:@"header"]) {
            [_dataArr removeAllObjects];
        }
        if (((NSArray*)JSON[@"data"]).count>0) {
            self.page ++;
        }
        if ([JSON[@"code"] intValue] == 0) {
            
            //这是添加服药单返回来的时候,先删除所有的
           
            
            for (NSDictionary *dict in JSON[@"data"]) {
                //将数据模型化
                ZJFuYaoDanModel *model = [[ZJFuYaoDanModel alloc] init];
                [model setKeyValues:dict];
                
               
                [_dataArr addObject:model];
            }
            //重新刷新tableview
            [_tableView reloadData];
            
            //重新刷新tableview
            [_tableView reloadData];
            if ([str isEqualToString:@"header"]) {
                [_tableView headerEndRefreshing];
            }else{
                [_tableView footerEndRefreshing];
            }
            kPdismiss;
        }
    } failure:^(NSError *error) {
        if ([str isEqualToString:@"header"]) {
            [_tableView headerEndRefreshing];
        }else{
            [_tableView footerEndRefreshing];
        }
        kPdismiss;
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
