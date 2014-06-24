//
//  ZJSetingViewController.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJSetingViewController.h"
#import "ZJSetingCell.h"
#import "ZJYijianViewController.h"
#import "ZJAboutMeViewController.h"
@interface ZJSetingViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    NSArray *_dataArr;//数据源
    UITableView *_talbeView;
}
@end

@implementation ZJSetingViewController

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
    self.title = @"设置";
    
    
    _talbeView = [[UITableView alloc] init];
    _talbeView.frame = CGRectMake(0,20,self.view.frame.size.width,self.view.frame.size.height);
    _talbeView.delegate = self;
    _talbeView.dataSource = self;
    _talbeView.rowHeight = [ZJSetingCell cellH];
    [self.view addSubview:_talbeView];
    _talbeView.separatorStyle = UITableViewCellSeparatorStyleNone;
    
    _dataArr = @[@"消息推送",@"意见反馈",@"关于我们",@"版本信息"];
    
//     [_talbeView registerNib:[UINib nibWithNibName:@"ZJSetingCell" bundle:nil] forCellReuseIdentifier:[ZJSetingCell ID]];
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return _dataArr.count;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    
   // static NSString *CellIdentifier = @"ZJHomeCell";
    
    ZJSetingCell *cell = [tableView dequeueReusableCellWithIdentifier:[ZJSetingCell ID]];
    if (cell == nil) {
        cell = [[ZJSetingCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:[ZJSetingCell ID]];
    }
    cell.titleLb.text = _dataArr[indexPath.row];
  
    if (indexPath.row ==0) {
        cell.notifiStatus.alpha = 1;
        [cell.notifiStatus addTarget:self action:@selector(upStatusAction:) forControlEvents:UIControlEventValueChanged];
    }else if (indexPath.row == 3){
        cell.versionLabel.alpha = 1;
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    return cell;
}
-(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (indexPath.row == 1) {
        [self pushController:[ZJYijianViewController class] withInfo:nil withTitle:@"意见反馈"];
    }else if(indexPath.row ==2){
         [self pushController:[ZJAboutMeViewController class] withInfo:nil withTitle:@"意见反馈"];
    }
}

-(void)upStatusAction:(UISwitch*)sw{
    BOOL isButtonOn = [sw isOn];
    if (isButtonOn) {
        MyLog(@"YES");
    }else {
        MyLog(@"NO");
    }
}

@end
