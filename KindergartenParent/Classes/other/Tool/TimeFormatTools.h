//
//  Tools.h
//  Babytop
//
//  Created by classjing on 14-5-5.
//  Copyright (c) 2014年 babytop. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface TimeFormatTools : NSObject
- (NSString*)timeToNow:(NSString*)theDate;
+ (NSString*)timeAfterNow:(NSString*)theDate;
+(NSString *)timeFormatToDate:(NSDate*)date;
+(NSString *)timeFormatToNow:(NSString*)format;
@end
