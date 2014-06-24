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
@interface ZJFuyaodanViewController ()
{
    UITableView *_tableView;
    NSMutableArray *_dataArr;
}
@end

@implementation ZJFuyaodanViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

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
    
    
    
}

#pragma mark----- UITableView
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    //return _dataArr.count;
    return 10;
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
    /*
     
     
     
     */
    return cell;
}
//点击cell 跳转下级界面
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    //根据具体情况修改
    
    ZJFuyaodanDetailViewController *detailVC = [[ZJFuyaodanDetailViewController alloc]init];

    [self.navigationController pushViewController:detailVC animated:YES];
    
}

-(void)addFuyaodan
{
    NSLog(@"添加服药单");
    ZJADDViewController *add = [[ZJADDViewController alloc]init];
    [self.navigationController pushViewController:add animated:YES];
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



@end
