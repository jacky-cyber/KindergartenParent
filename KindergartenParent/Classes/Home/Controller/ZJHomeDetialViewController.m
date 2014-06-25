//
//  ZJHomeDetialViewController.m
//  ybparents
//
//  Created by Define on 14-6-9.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJHomeDetialViewController.h"

@interface ZJHomeDetialViewController ()
@property (weak, nonatomic) IBOutlet UILabel *createTime;
@property (weak, nonatomic) IBOutlet UILabel *contentLabel;
@property (weak, nonatomic) IBOutlet UIImageView *typeImage;
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *heightConstraint;

@end

@implementation ZJHomeDetialViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    TimeFormatTools *timeTool = [[TimeFormatTools alloc] init];
    _createTime.text = [timeTool timeToNow:@"2014-06-23 13:23:00"];
    
    
    //判断通知的类型
    if (1<2) {
       // _typeImage.image = [UIImage imageNamed:@"youeryuan_notif"];
    }
    
    NSString *contentStr = @"2014年5月13日，万邦部中班将会来到美丽的滨江森林公园。里面环境优美，漫地都是五颜六色绽开的花朵，有蔷薇、杜鹃、木兰……小朋友们在草坪上放风筝、踢足球、野餐还展开了有趣的集体游戏。家长带了很多好吃的在草地上席地而坐，相互分享自己带来的好吃的，聊天谈话，孩子们吃着美味的食物闻着大自然的味道，家长和小朋友们玩的都非常的开心。里面还有海盗船、碰碰车、过山车……大人带着小孩一起勇敢挑战，乐此不疲。公园里面有三江交汇的壮观场面、在湿地行走有一种冒险的感觉。";
    
    _heightConstraint.constant = [contentStr getHeightByWidth:W(_contentLabel) font:_contentLabel.font];

    _contentLabel.text = contentStr;
    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
