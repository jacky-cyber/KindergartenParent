//
//  ZJHomeDetialViewController.m
//  ybparents
//
//  Created by Define on 14-6-9.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJHomeDetialViewController.h"
#import "ZJHomeModel.h"
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
    ZJHomeModel *model = self.userInfo;
    TimeFormatTools *timeTool = [[TimeFormatTools alloc] init];
    _createTime.text = [timeTool timeToNow:model.createtime];
    
    
    
    //判断通知的类型
    if ([model.type intValue] == 2) {//全员通知
        _typeImage.image = [UIImage imageNamed:@"youeryuan_notif"];
    }else if ([model.type intValue] == 6) {//服药提醒
        _typeImage.image = [UIImage imageNamed:@"weiyaoNofifi"];
    }else if ([model.type intValue] == 8) {//本班通知
        _typeImage.image = [UIImage imageNamed:@"classDetialNotifi"];
    }
    
    
    _heightConstraint.constant = [model.content getHeightByWidth:W(_contentLabel) font:_contentLabel.font];

    _contentLabel.text = model.content;
    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
