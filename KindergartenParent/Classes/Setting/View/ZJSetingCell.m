//
//  ZJSetingCell.m
//  ybparents
//
//  Created by Define on 14-6-10.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJSetingCell.h"

@implementation ZJSetingCell

- (void)awakeFromNib
{
    // Initialization code
}

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    //self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    
    self = [[[NSBundle mainBundle]loadNibNamed:@"ZJSetingCell" owner:self options:nil]objectAtIndex:0];
    if (self)
    {
        

        _bgView.layer.borderWidth = 1;
        _bgView.layer.borderColor = [UIColor colorWithWhite:0.675 alpha:1.000].CGColor;
    }
    return self;
}
+(NSString *)ID
{
    return @"Cell";
}
+(CGFloat)cellH
{
    return 60;
}
@end
