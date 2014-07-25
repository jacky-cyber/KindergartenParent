//
//  ZJDayReportViewController.m
//  ybparents
//
//  Created by Define on 14-6-12.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJDayReportViewController.h"

@interface ZJDayReportViewController ()<UITableViewDelegate,UITableViewDataSource>
{
    UITableView *_tableView;
    
    NSDictionary *_dataDict;
}


@end

@implementation ZJDayReportViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    CGFloat height = H(self.view)-kNavH;
    if (ISIOS7) {
        height -=20;
    }
    
    _tableView = [[UITableView alloc] initWithFrame:CGRectMake(0, 0, W(self.view),height)];
    [_tableView setSeparatorStyle:UITableViewCellSeparatorStyleNone];
    _tableView.delegate = self;
    _tableView.dataSource = self;
    
    
    //加载数据
    _dataDict = [[NSDictionary alloc] init];
    [self loadData];
    
    
    //[self.view addSubview:_tableView];
}
-(void)loadData
{
    [SVProgressHUD showWithStatus:@"加载数据中" maskType:SVProgressHUDMaskTypeBlack];
    [HttpTool getWithPath:@"todayreport" params:@{@"id":self.userInfo} success:^(id JSON) {
        ///NSLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] ==0) {
            _dataDict = JSON[@"data"];
            [self.view addSubview:_tableView];
            kPdismiss;
        }else{
           [SVProgressHUD showErrorWithStatus:@"请求失败" duration:1];
        }
    } failure:^(NSError *error) {
        
    }];
}

-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 6;
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
//    NSString *otherStr = @"今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，";
    CGFloat height = 70;
    if (indexPath.row == 5) {
        height = [_dataDict[@"other"] getHeightByWidth:150 font:kFont(14)]+20;
        if (height<70) {
            height=70;
        }
    }
    return height;
}
-(UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath{
    static NSString *identifier = @"Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:identifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:identifier];
    }
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    if (indexPath.row<5) {
        CGRect frame =CGRectMake(20, 10, 280, 50);
        NSString *imgStr = [NSString stringWithFormat:@"bj%d",indexPath.row+1];
        UIImageView *bgImg = [[UIImageView alloc] initWithFrame:frame];
        bgImg.image = [UIImage imageNamed:imgStr];
        UIView *bgView = [[UIView alloc] initWithFrame:frame];
        cell.backgroundView = bgView;
        [bgView addSubview:bgImg];
        
        UILabel *textLabel = [[UILabel alloc] initWithFrame:CGRectMake(120, 15, 100, 20)];
        textLabel.backgroundColor = [UIColor clearColor];
        textLabel.font = kFont(14);
        //设置显示
        
        //MyLog(@"%-----@",_dataDict);
        
        if (indexPath.row == 0) {
            textLabel.text = _dataDict[@"sleeptime"];
        }else if(indexPath.row == 1){
            textLabel.text = [NSString stringWithFormat:@"%@",_dataDict[@"eatnum"]];
        }else if(indexPath.row == 2){
            textLabel.text = [NSString stringWithFormat:@"%@",_dataDict[@"drinknum"]];
        }else if(indexPath.row == 3){
            textLabel.text = [NSString stringWithFormat:@"%@",_dataDict[@"shitnum"]];

        }else if(indexPath.row == 4){
            textLabel.text = _dataDict[@"mood"];
        }
        
        //textLabel.text =textStr;
        [bgImg addSubview:textLabel];
        
        //其他
    }else if (indexPath.row ==5) {
//         NSString *otherStr = @"今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，今天天气很好很，";
        NSString *otherStr = _dataDict[@"other"];
        CGFloat heigh = [otherStr getHeightByWidth:150 font:kFont(14)];
        if (heigh<50) {
            heigh = 50;
        }
        
        UIView *otherView = [[UIView alloc] initWithFrame:CGRectMake(20, 10, 280, heigh)];
        otherView.layer.borderWidth = 0.5;
        otherView.layer.cornerRadius = 1;
        otherView.layer.masksToBounds = YES;
        otherView.layer.borderColor = [UIColor colorWithRed:0.945 green:0.333 blue:0.533 alpha:1.000].CGColor;
        [cell.contentView addSubview:otherView];
        
        //左边背景
        UIView *leftBg = [[UIView alloc] initWithFrame:CGRectMake(0, 0,105, heigh)];
         leftBg.backgroundColor = [UIColor colorWithRed:0.945 green:0.333 blue:0.533 alpha:1.000];
        [otherView addSubview:leftBg];
        UIImageView *otherImagView = [[UIImageView alloc] initWithFrame:CGRectMake(0, (H(otherView)-19)/2, 105, 19)];
        otherImagView.image = [UIImage imageNamed:@"other"];
        [otherView addSubview:otherImagView];
  
        //其他label
        UILabel *otherLabel = [[UILabel alloc] init];
        otherLabel.numberOfLines = 0;
        
       
        otherLabel.font = kFont(14);
        otherLabel.frame = CGRectMake(120, 0, 165, heigh);
        otherLabel.text = otherStr;
        [otherView addSubview:otherLabel];
        
        
    }

    
    return cell;
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



@end
