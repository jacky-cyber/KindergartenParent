//
//  ZJSetingCell.h
//  ybparents
//
//  Created by Define on 14-6-10.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface ZJSetingCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UILabel *titleLb;
@property (weak, nonatomic) IBOutlet UILabel *versionLabel;
@property (weak, nonatomic) IBOutlet UISwitch *notifiStatus;
@property (weak, nonatomic) IBOutlet UIView *bgView;

+(NSString*)ID;
+(CGFloat)cellH;
@end
