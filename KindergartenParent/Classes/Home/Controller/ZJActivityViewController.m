//
//  ZJActivityViewController.m
//  KindergartenParent
//
//  Created by define on 14-6-25.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJActivityViewController.h"

@interface ZJActivityViewController ()
@property (weak, nonatomic) IBOutlet UILabel *publishTime;

@end

@implementation ZJActivityViewController

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
    
    
    TimeFormatTools *timeTool = [[TimeFormatTools alloc] init];
    _publishTime.text = [timeTool timeToNow:@"2014-06-23 13:23:00"];
    
    
    UILabel *label = [[UILabel alloc] init];
    label.numberOfLines = 0;
    NSString *drugStr = @"今天有个活动，有哥哥活动，有哥哥活动，有哥哥活动，有哥哥活动";
    
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
