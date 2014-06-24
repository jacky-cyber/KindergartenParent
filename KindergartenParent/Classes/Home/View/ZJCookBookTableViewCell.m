//
//  ZJCookBookTableViewCell.m
//  ybparents
//
//  Created by Define on 14-6-9.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJCookBookTableViewCell.h"

@implementation ZJCookBookTableViewCell


- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [[[NSBundle mainBundle]loadNibNamed:@"ZJCookBookTableViewCell" owner:self options:nil]objectAtIndex:0];
    if (self)
    {

        //设置子视图frame
        [self settingSubviewFrame];
    }
    return self;
}

- (void)settingSubviewFrame
{
    
}
+(NSString *)ID
{
    return @"Cell";
}
+(CGFloat)cellHeight
{
    return 155;
}
@end
