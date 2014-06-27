//
//  ZJCookBookTableViewCell.h
//  ybparents
//
//  Created by Define on 14-6-9.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import <UIKit/UIKit.h>
@class CookBookModel;
@interface ZJCookBookTableViewCell : UITableViewCell
@property (weak, nonatomic) IBOutlet UIView *breakfirstV;
@property (weak, nonatomic) IBOutlet UIView *noonV;
@property (weak, nonatomic) IBOutlet UIView *supperV;
@property (weak, nonatomic) IBOutlet UIView *jiacanV;
@property (weak, nonatomic) IBOutlet UILabel *breakFirstLabel;
@property (weak, nonatomic) IBOutlet UILabel *noonLabel;
@property (weak, nonatomic) IBOutlet UILabel *supperLabel;

@property (weak, nonatomic) IBOutlet UILabel *jiacanLabel;
@property (weak, nonatomic) IBOutlet UIImageView *weekImg;
@property (weak, nonatomic) IBOutlet UILabel *bTitle;
@property (weak, nonatomic) IBOutlet UILabel *lTitle;
@property (weak, nonatomic) IBOutlet UILabel *sTitle;
@property (weak, nonatomic) IBOutlet UILabel *jTItle;
@property (assign, nonatomic)  CGFloat cellH;
@property(nonatomic,strong) CookBookModel *model;
+ (NSString *)ID;
+ (CGFloat)cellHeight;
@end
