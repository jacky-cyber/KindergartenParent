//
//  ZJNotificationListCell.h
//  KindergartenParent
//
//  Created by define on 14-7-25.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ZJNotificationListCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UIImageView *bgImage;
@property (weak, nonatomic) IBOutlet UILabel *contentLabel;
@property (weak, nonatomic) IBOutlet UILabel *tiemeLabel;
@property (weak, nonatomic) IBOutlet UIButton *qiandaoBtn;
@end
