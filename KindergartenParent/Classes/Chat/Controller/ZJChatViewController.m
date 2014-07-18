//
//  ZJChatViewController.m
//  KindergartenParent
//
//  Created by define on 14-7-18.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJChatViewController.h"
#import "ChatMessageCell.h"
@interface ZJChatViewController ()
@property (weak, nonatomic) IBOutlet UITableView *tableView;

@end

@implementation ZJChatViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}



#pragma mark - 数据源方法
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
//    id <NSFetchedResultsSectionInfo> info = _fetchResultsController.sections[section];
//    return [info numberOfObjects];
    return 1;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *FromID = @"ChatFromCell";
    static NSString *ToID = @"ChatToCell";
    
    NSString *CellIdentifier;
    
    ChatMessageCell *cell = nil;
    
    BOOL flag;
    
  
    
    return cell;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
