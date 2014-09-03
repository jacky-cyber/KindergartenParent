//
//  ZJRightSideDrawerViewController.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJRightSideDrawerViewController.h"
#import "UIImageView+MJWebCache.h"
#import "SRRefreshView.h"
#import "DDMenuController.h"
#import "ZJAppDelegate.h"
#import "ChatViewController.h"
#import "EMSearchDisplayController.h"
#import "EMSearchBar.h"
#import "NSDate+Category.h"
#import "ConvertToCommonEmoticonsHelper.h"
@interface ZJRightSideDrawerViewController ()<UITableViewDataSource,UITableViewDelegate,DDMenuControllerDelegate,SRRefreshDelegate, UISearchBarDelegate,IChatManagerDelegate>
{
    UITableView *_tableView;

    
    NSMutableArray *_contactsArr;//通讯录数组
}
@property (strong, nonatomic) NSMutableArray        *dataSource;
@property (nonatomic, strong) EMSearchBar           *searchBar;
@property (nonatomic, strong) UIView                *networkStateView;

@property (strong, nonatomic) EMSearchDisplayController *searchController;
@end

@implementation ZJRightSideDrawerViewController
-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    //加载通讯录
    [self initContacts];
    
}

-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
}



- (void)viewDidLoad
{
    
    _contactsArr = [NSMutableArray arrayWithCapacity:2];
    
    [super viewDidLoad];
    
    _tableView = [[UITableView alloc] init];
    _tableView.frame = CGRectMake(240,0,self.view.frame.size.width,self.view.frame.size.height);
    _tableView.delegate = self;
    _tableView.dataSource = self;
    [self.view addSubview:_tableView];
    _tableView.bounces = NO;
    _tableView.rowHeight = 60;
 
    
    _tableView.backgroundColor = [UIColor colorWithRed:0.400 green:0.800 blue:1.000 alpha:1.000];
    UIView *bgView = [[UIView alloc] init];
    UIImageView *bgImag = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"beijing02"]];
    [bgView addSubview:bgImag];
    _tableView.backgroundView = bgView;
    
    _tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    if ([_tableView respondsToSelector:@selector(setSeparatorInset:)]) {
        [_tableView setSeparatorInset:UIEdgeInsetsMake(0, 50, 0, 0)];
    }
    
    
//    UILabel *kindergarten = [[UILabel alloc] initWithFrame:CGRectMake(0, kScreenHeight*0.9, 200, 21)];
//    
//    kindergarten.backgroundColor = [UIColor clearColor];
//    kindergarten.textColor = [UIColor whiteColor];
//    kindergarten.textAlignment = NSTextAlignmentCenter;
//    kindergarten.text = @"美蒙国际双语幼儿园";
//    [self.view addSubview:kindergarten];
    
    
    
    
}


#pragma mark 初始化通讯录
-(void)initContacts{
    
    //182.18.23.244:8080/kindergarten/service/app!addressbook.action?username=LS13436871757&role=1
    [_contactsArr removeAllObjects];
    NSDictionary *d = [NSDictionary dictionary];
    [_contactsArr addObject:d];
    
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,@"role":[LoginUser sharedLoginUser].role};
    
    
    [HttpTool getWithPath:@"addressbook" params:params success:^(id JSON) {
        MyLog(@"%@",JSON);
        if (JSON[@"data"]) {
            for (NSDictionary *dict in JSON[@"data"][0][@"userList"]) {
                [_contactsArr addObject:dict];
            
                
            }
            //[_contactsArr writeToFile:@"/Users/define/Desktop/a.plist" atomically:YES];
            [_tableView reloadData];
        }
        MyLog(@"-------%@",_contactsArr);
    } failure:^(NSError *error) {
        
    }];
    
    //NSString *path  = [[NSBundle mainBundle] pathForResource:@"a" ofType:@"plist"];
    //_contactsArr = [[NSMutableArray alloc] initWithContentsOfFile:path];
    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - Table view data source



- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{


    return _contactsArr.count;
}


 - (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
 {
     static NSString *ID = @"Cell";
 UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:ID];
     if (!cell) {
         cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:ID];
     }
     cell.selectionStyle = UITableViewCellSelectionStyleNone;
     cell.backgroundColor = [UIColor clearColor];
     if (indexPath.row > 0) {
         NSDictionary *dict = _contactsArr[indexPath.row];
         
         UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(15, 5, 50, 50)];
         [cell addSubview:imageView];
         
         [imageView setImageURLStr:dict[@"profileimngurl"] placeholder:[UIImage imageNamed:@"profile"]];
         imageView.layer.cornerRadius = 5;
         imageView.layer.masksToBounds = YES;
         imageView.layer.borderColor = [UIColor whiteColor].CGColor;
         imageView.layer.borderWidth = 2.0;
         imageView.tag = 1;

     }
    
     
 
 // Configure the cell...
 
 return cell;
 }


-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    //[self.navigationController pushViewController:chat animated:YES];
    if (indexPath.row == 0) {
        return;
    }
    NSDictionary *dict = _contactsArr[indexPath.row];
    
    ChatViewController *chatVC = [[ChatViewController alloc] initWithChatter:[dict[@"username"] lowercaseString]];
    chatVC.title = dict[@"name"];
    chatVC.tel = dict[@"tel"];
    chatVC.profileimngurl = dict[@"profileimngurl"];
    [self.navigationController pushViewController:chatVC animated:YES];
    
    DDMenuController *menuController = ((ZJAppDelegate*)[[UIApplication sharedApplication] delegate]).menuController;
    menuController.delegate = self;
    [menuController pushViewController:chatVC animated:YES];
    
//    [menuController setRootController:courseNav animated:YES];
//    
//    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    
    
    
}







-(void)menuController:(DDMenuController *)controller willShowViewController:(UIViewController *)controllers
{
    
    
    
}

@end
