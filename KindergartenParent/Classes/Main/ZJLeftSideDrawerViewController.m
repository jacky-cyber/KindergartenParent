//
//  ZJLeftSideDrawerViewController.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJLeftSideDrawerViewController.h"
#import "ZJHomeViewController.h"
#import "ZJPhotWallViewController.h"
#import "ZJVideoViewController.h"
#import "ZJSetingViewController.h"
#import "ZJFuyaodanViewController.h"
#import "ZJCourseViewController.h"
#import "DDMenuController.h"
#import "ZJAppDelegate.h"
@interface ZJLeftSideDrawerViewController ()<UITableViewDataSource,UITableViewDelegate>
{
    UITableView *_tableView;
    
    NSArray *_dataArr;
    NSArray *_imgArr;
}
@end

@implementation ZJLeftSideDrawerViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    
//    if (ISIOS7) {
//        self.view.bounds = CGRectMake(0, -20, self.view.frame.size.width, self.view.frame.size.height-20 );
//    }
    
    _tableView = [[UITableView alloc] init];
    _tableView.frame = CGRectMake(0,0,self.view.frame.size.width,self.view.frame.size.height);
    _tableView.delegate = self;
    _tableView.dataSource = self;
    [self.view addSubview:_tableView];
    _tableView.bounces = NO;
    _tableView.rowHeight = 50;
    //数据源
    _dataArr = @[@"",@"家长中心",@"成长记录",@"视频直播",@"医务互通",@"每周课程",@"设置中心"];
    
    
    _tableView.backgroundColor = [UIColor colorWithRed:0.400 green:0.800 blue:1.000 alpha:1.000];
    UIView *bgView = [[UIView alloc] init];
    UIImageView *bgImag = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"beijing02"]];
    [bgView addSubview:bgImag];
    _tableView.backgroundView = bgView;
    
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    if ([_tableView respondsToSelector:@selector(setSeparatorInset:)]) {
        [_tableView setSeparatorInset:UIEdgeInsetsMake(0, 50, 0, 0)];
    }
    
    UILabel *kindergarten = [[UILabel alloc] initWithFrame:CGRectMake(0, kScreenHeight*0.9, 200, 21)];
    
    kindergarten.backgroundColor = [UIColor clearColor];
    kindergarten.textColor = [UIColor whiteColor];
    kindergarten.textAlignment = NSTextAlignmentCenter;
    kindergarten.text = @"美蒙国际双语幼儿园";
    [self.view addSubview:kindergarten];
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (void)setExtraCellLineHidden: (UITableView *)tableView

{
    
    UIView *view =[ [UIView alloc]init];
    
    view.backgroundColor = [UIColor clearColor];
    
    [tableView setTableFooterView:view];
}
#pragma mark - Table view data source



- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _dataArr.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *identifier = @"cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identifier];
    
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identifier];
    }
    
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    cell.backgroundColor = [UIColor clearColor];
    if (indexPath.row>0) {
        
        NSString *imagName = [NSString stringWithFormat:@"img0%d",indexPath.row];
        UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(30, 10, 40, 40)];
        imageView.image = [UIImage imageNamed:imagName];
        [cell.contentView addSubview:imageView];
        
       
        
        UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(XW(imageView), 10, 100,30 )];
        label.textColor = [UIColor whiteColor];
        label.text = _dataArr[indexPath.row];
        label.backgroundColor = [UIColor clearColor];
        [cell.contentView addSubview:label];
        
        
        UIView *fengeline = [[UIView alloc] initWithFrame:CGRectMake(X(label), YH(label)+10, 120, 0.5)];
        fengeline.backgroundColor = [UIColor whiteColor];
        [cell.contentView addSubview:fengeline];
    }
    
    //cell.textLabel.text = _dataArr[indexPath.row];
    cell.textLabel.textColor = [UIColor whiteColor];
    return cell;
}

-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    ZJHomeViewController * home  = [[ZJHomeViewController alloc] init];
    ZJPhotWallViewController *photowall  = [[ZJPhotWallViewController alloc] init];
    ZJFuyaodanViewController *fuyaodan = [[ZJFuyaodanViewController alloc] init];
    ZJVideoViewController *video = [[ZJVideoViewController alloc] init];
    ZJSetingViewController *seting = [[ZJSetingViewController alloc] init];
    ZJCourseViewController *course = [[ZJCourseViewController alloc] init];
    BaseNavigationController *homenav = [[BaseNavigationController alloc] initWithRootViewController:home];
    BaseNavigationController *wallnav = [[BaseNavigationController alloc] initWithRootViewController:photowall];
    BaseNavigationController *videonav = [[BaseNavigationController alloc] initWithRootViewController:video];
    BaseNavigationController *fuyaodannav = [[BaseNavigationController alloc] initWithRootViewController:fuyaodan];
    BaseNavigationController *setingnav = [[BaseNavigationController alloc] initWithRootViewController:course];
    BaseNavigationController *courseNav = [[BaseNavigationController alloc] initWithRootViewController:seting];
    
    NSArray *arr = @[homenav,wallnav,videonav,fuyaodannav,setingnav,courseNav];
    

    // (DDMenuController*)((ZJAppDelegate*)[UIApplication sharedApplication].delegate).menuController;

    if (indexPath.row == 0) {
        return;
    }
    
    DDMenuController *menuController = ((ZJAppDelegate*)[[UIApplication sharedApplication] delegate]).menuController;
    
    [menuController setRootController:arr[indexPath.row-1] animated:YES];

    [tableView deselectRowAtIndexPath:indexPath animated:YES];

    
    
   
}

@end
