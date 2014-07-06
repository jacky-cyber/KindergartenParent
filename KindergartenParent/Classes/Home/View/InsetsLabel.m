//
//  InsetsLabel.m
//  KindergartenParent
//
//  Created by 郑敬 on 14-7-6.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "InsetsLabel.h"

@implementation InsetsLabel

-(id) initWithFrame:(CGRect)frame andInsets:(UIEdgeInsets)insets {
    self = [super initWithFrame:frame];
    if(self){
        self.insets = insets;
    }
    return self;
}

-(id) initWithInsets:(UIEdgeInsets)insets {
    self = [super init];
    if(self){
        self.insets = insets;
    }
    return self;
}

-(void) drawTextInRect:(CGRect)rect {
    return [super drawTextInRect:UIEdgeInsetsInsetRect(rect, self.insets)];
}

@end
