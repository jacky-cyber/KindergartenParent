//
//  InsetsLabel.h
//  KindergartenParent
//
//  Created by 郑敬 on 14-7-6.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface InsetsLabel : UILabel
@property(nonatomic) UIEdgeInsets insets;
-(id) initWithFrame:(CGRect)frame andInsets: (UIEdgeInsets) insets;
-(id) initWithInsets: (UIEdgeInsets) insets;
@end
