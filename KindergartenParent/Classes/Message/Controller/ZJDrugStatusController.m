//
//  ZJDrueStatusController.m
//  ybparents
//
//  Created by 郑敬 on 14-6-17.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJDrugStatusController.h"

@interface ZJDrugStatusController ()
@property (weak, nonatomic) IBOutlet UILabel *detialLabel;
@property (weak, nonatomic) IBOutlet UILabel *publishTimeLabel;

@end

@implementation ZJDrugStatusController



- (void)viewDidLoad
{
    [super viewDidLoad];
    TimeFormatTools *timeTool = [[TimeFormatTools alloc] init];
    _publishTimeLabel.text = [timeTool timeToNow:@"2014-06-13 13:23:00"];
    
    
    UILabel *label = [[UILabel alloc] init];
    label.numberOfLines = 0;
    NSString *drugStr = @"您的孩子已经服药了，哈哈，请放心吧，真的，放心吧，我是不会骗你的，骗你也是可能的";

    CGFloat height = [drugStr getHeightByWidth:280 font:kFont(14)];
    label.font = kFont(14);
    label.frame = CGRectMake(20, 70, 280, height);
    label.text = drugStr;
    [self.view addSubview:label];
    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
