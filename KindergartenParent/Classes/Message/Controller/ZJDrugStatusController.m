//
//  ZJDrueStatusController.m
//  ybparents
//
//  Created by 郑敬 on 14-6-17.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJDrugStatusController.h"
#import "ZJHomeModel.h"
@interface ZJDrugStatusController ()
@property (weak, nonatomic) IBOutlet UILabel *detialLabel;
@property (weak, nonatomic) IBOutlet UILabel *publishTimeLabel;

@end

@implementation ZJDrugStatusController



- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    ZJHomeModel *model = self.userInfo;
    
    TimeFormatTools *timeTool = [[TimeFormatTools alloc] init];
    _publishTimeLabel.text = [timeTool timeToNow:model.createtime];
    
    
    UILabel *label = [[UILabel alloc] init];
    label.numberOfLines = 0;
    NSString *drugStr = model.content;

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
