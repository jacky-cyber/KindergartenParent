//
//  BaseBarButtonItem.h
//  ybparents
//
//  Created by 郑敬 on 14-6-16.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface BaseBarButtonItem : UIBarButtonItem

-(void)setBackgroundImage:(UIImage *)backgroundImage forState:(UIControlState)state style:(UIBarButtonItemStyle)style barMetrics:(UIBarMetrics)barMetrics;

@end
