//
//  ZJCookBookViewController.m
//  ybparents
//
//  Created by Define on 14-6-9.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJCookBooksViewController.h"
#import "ZJCookBookTableViewCell.h"
#import "CookBookModel.h"
@interface ZJCookBooksViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    UITableView *_tableView;
    NSMutableArray *_dataArr;
    NSArray *_images;//图片
}
@end

@implementation ZJCookBooksViewController

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
    // Do any additional setup after loading the view from its nib.
    
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        //height -=20;
    }
    
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, W(self.view),height) style:UITableViewStylePlain];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    //_tableView.sectionFooterHeight = 0;
    //_tableView.rowHeight = [ZJCookBookTableViewCell cellHeight];
    
    [self.view addSubview:_tableView];
    
    [self initData];
}
#pragma mark 初始化数据
-(void)initData
{
    
    //初始化数组
    _dataArr = [NSMutableArray array];
    
    [HttpTool getWithPath:@"recipe" params:@{@"id":@1} success:^(id JSON) {
        if ([JSON[@"code"] intValue] == 0) {
            NSArray *data = JSON[@"data"][@"everyday"];
            for (NSDictionary *dict in data) {
                
                NSLog(@"%d-----",dict.count);
                 CookBookModel *model = [CookBookModel objectWithKeyValues:dict];
                [model setKeyValues:dict];
                [_dataArr addObject:model];
            }
            [_tableView reloadData];
            
            if (JSON[@"data"][@"images"]) {
                _images = JSON[@"data"][@"images"];
            
                UIButton *btnR = [UIButton buttonWithType:UIButtonTypeSystem];
                btnR.frame = CGRectMake(0, 2, 50, 25);
                [btnR setBackgroundImage:[UIImage imageNamed:@"nav_rightbackbround_image"] forState:UIControlStateNormal];
                [btnR setTitle:@"图片" forState:UIControlStateNormal];
                [btnR setTitleColor:[UIColor colorWithRed:0.129 green:0.714 blue:0.494 alpha:1.000] forState:UIControlStateNormal];
                UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
                self.navigationItem.rightBarButtonItem = ItemR;
                
            }
            
            
        }
        
//        CookBookModel *dict = _dataArr[1];
//        NSLog(@"%d",dict.count);
        
    } failure:^(NSError *error) {
        MyLog(@"%@",error.localizedDescription);
    }];
}

//-(NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
//{
//    return _dataArr.count;
//
//}

//-(UIView *)tableView:(UITableView *)tableView viewForHeaderInSection:(NSInteger)section{
//    UIView *bgView = [[UIView alloc] init];
//    bgView.backgroundColor = [UIColor whiteColor];
//    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(20, 0, 280, 30)];
//    NSString *imageName = [NSString stringWithFormat:@"zhou%d",section+1];
//    imageView.image = [UIImage imageNamed:imageName];
//    [bgView addSubview:imageView];
//    return bgView;
//}
//-(CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section{
//    return 30;
//}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    
   // NSDictionary *dict = [_dataArr[section] keyValues];
    
    return _dataArr.count ;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    ZJCookBookTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:[ZJCookBookTableViewCell ID]];
    if (cell == nil) {
        cell = [[ZJCookBookTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:[ZJCookBookTableViewCell ID]];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    NSString *imageName = [NSString stringWithFormat:@"zhou%d",indexPath.row+1];
    cell.weekImg.image = [UIImage imageNamed:imageName];
    CookBookModel *model = _dataArr[indexPath.row];
    
    [cell setModel:model];
    
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    CookBookModel *model = _dataArr[indexPath.row];
    
    CGFloat height = 60;
    
    height += [self getHeight:model.breakfast];
    height += [self getHeight:model.lunch];
    height += [self getHeight:model.supper];
    height += [self getHeight:model.jiacan];
    
    return height;
}
#pragma mark 计算没餐的高度
-(CGFloat)getHeight:(NSString*)str
{
    CGFloat h = 0;
     h = [str getHeightByWidth:230 font:kFont(17)];
     h = h>21?h:21;
    return h;
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
