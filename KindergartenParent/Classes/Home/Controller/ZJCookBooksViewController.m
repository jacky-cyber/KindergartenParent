//
//  ZJCookBookViewController.m
//  ybparents
//
//  Created by Define on 14-6-9.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJCookBooksViewController.h"
#import "ZJCookBookTableViewCell.h"
@interface ZJCookBooksViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    UITableView *_tableView;
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
        height -=20;
    }
    
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, W(self.view),height)];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    _tableView.rowHeight = [ZJCookBookTableViewCell cellHeight];
    
    [self.view addSubview:_tableView];
    
    [self ladData];
}
-(void)ladData
{
    [HttpTool getWithPath:@"recipe" params:@{@"id":@1} success:^(id JSON) {
        MyLog(@"%@",JSON);
    } failure:^(NSError *error) {
        MyLog(@"%@",error.localizedDescription);
    }];
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 7;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    
    ZJCookBookTableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:[ZJCookBookTableViewCell ID]];
    if (cell == nil) {
        cell = [[ZJCookBookTableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:[ZJCookBookTableViewCell ID]];
    }
    
    NSString *imgStr = [NSString stringWithFormat:@"zhou%d",indexPath.row+1];
    cell.weekImg.image = [UIImage imageNamed:imgStr];
    
    return cell;
}

-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return [ZJCookBookTableViewCell cellHeight];
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
