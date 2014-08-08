//
//  ZJRightSideDrawerViewController.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJRightSideDrawerViewController.h"
#import "UIImageView+MJWebCache.h"
#import "ZJChatViewController.h"
#import "DDMenuController.h"
#import "ZJAppDelegate.h"
@interface ZJRightSideDrawerViewController ()<UITableViewDataSource,UITableViewDelegate>
{
    UITableView *_tableView;

    
    NSMutableArray *_contactsArr;//通讯录数组
}

@end

@implementation ZJRightSideDrawerViewController


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
    
    
    //加载通讯录
    [self initContacts];
    
}


#pragma mark 初始化通讯录
-(void)initContacts{
    
    //182.18.23.244:8080/kindergarten/service/app!addressbook.action?username=LS13436871757&role=1
    
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
    ZJChatViewController *chat  = [[ZJChatViewController alloc] init];
       BaseNavigationController *courseNav = [[BaseNavigationController alloc] initWithRootViewController:chat];
    
       // (DDMenuController*)((ZJAppDelegate*)[UIApplication sharedApplication].delegate).menuController;
     NSDictionary *dict = _contactsArr[indexPath.row];
    if (indexPath.row == 0) {
        return;
    }
    
    
    NSString *domain = [xmppDelegate.xmppStream.myJID domain];
    NSString *friendText = [NSString stringWithFormat:@"%@@%@", dict[@"username"], domain];
    
    
    
    // 1. 如果已经是好友，则无需添加
    // 如果已经添加成好友，好友的信息会记录在本地数据库中
    // 在本地数据库中直接查找该好友是否存在即可
    XMPPJID *jid = [XMPPJID jidWithString:friendText];
    
    //如果加好友了，就不需要再添加好友了
    if (![xmppDelegate.xmppRosterCoreDataStorage userExistsWithJID:jid xmppStream:xmppDelegate.xmppStream]) {
        // 在XMPP中添加好友的方法，叫做：“订阅”，类似于微博中的关注
        // 发送订阅请求给指定的用户
        // 2. 添加好友操作
        [xmppDelegate.xmppRoster subscribePresenceToUser:jid];
    }
    
    
    chat.bareImageStr = dict[@"profileimngurl"];
    
    chat.bareJID = jid;
    chat.title = dict[@"name"];



   
    
    
    
    //[self.navigationController pushViewController:chat animated:YES];
    
    
    DDMenuController *menuController = ((ZJAppDelegate*)[[UIApplication sharedApplication] delegate]).menuController;
    
    [menuController setRootController:courseNav animated:YES];
    
    [tableView deselectRowAtIndexPath:indexPath animated:YES];
    
    
    
    
}
/*
 // Override to support conditional editing of the table view.
 - (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
 {
 // Return NO if you do not want the specified item to be editable.
 return YES;
 }
 */

/*
 // Override to support editing the table view.
 - (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
 {
 if (editingStyle == UITableViewCellEditingStyleDelete) {
 // Delete the row from the data source
 [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
 } else if (editingStyle == UITableViewCellEditingStyleInsert) {
 // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
 }
 }
 */

/*
 // Override to support rearranging the table view.
 - (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
 {
 }
 */

/*
 // Override to support conditional rearranging of the table view.
 - (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
 {
 // Return NO if you do not want the item to be re-orderable.
 return YES;
 }
 */

/*
 #pragma mark - Navigation
 
 // In a storyboard-based application, you will often want to do a little preparation before navigation
 - (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
 {
 // Get the new view controller using [segue destinationViewController].
 // Pass the selected object to the new view controller.
 }
 */

@end
