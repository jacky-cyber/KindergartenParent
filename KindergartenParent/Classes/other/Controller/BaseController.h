//
//  BaseController.h
//  Babytop
//
//  Created by classjing on 14-4-27.
//  Copyright (c) 2014年 babytop. All rights reserved.
//

#import <UIKit/UIKit.h>



@interface BaseController : UIViewController

@property (nonatomic,strong) id  userInfo;
@property (nonatomic,strong) id  otherInfo;
@property (nonatomic,assign) int  page;



- (BaseController*)pushController:(Class)controller withOnlyInfo:(id)info;
- (BaseController*)pushController:(Class)controller withInfo:(id)info;
- (BaseController*)pushController:(Class)controller withInfo:(id)info withTitle:(NSString*)title;
- (BaseController*)pushController:(Class)controller withInfo:(id)info withTitle:(NSString*)title withOther:(id)other;
- (BaseController*)pushController:(Class)controller withInfo:(id)info withTitle:(NSString*)title withOther:(id)other withModel:(BOOL)isModel;
- (BOOL)popController:(NSString*)controller withSel:(SEL)sel withObj:(id)info;

@end
