//
//  Tools.m
//  Babytop
//
//  Created by classjing on 14-5-5.
//  Copyright (c) 2014年 babytop. All rights reserved.
//

#import "TimeFormatTools.h"

@implementation TimeFormatTools
#pragma mark TimeTravel
- (NSString*)timeToNow:(NSString*)theDate
{
    if (!theDate) {
        return nil;
    }
    NSDateFormatter *dateFormatter=[[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *d=[dateFormatter dateFromString:theDate];
    if (!d) {
        return theDate;
    }
    
    NSTimeInterval late=[d timeIntervalSince1970]*1;
    
    
    NSDate* dat = [NSDate dateWithTimeIntervalSinceNow:0];
    NSTimeInterval now=[dat timeIntervalSince1970]*1;
    NSString *timeString=@"";
    
    NSTimeInterval cha=(now-late)>0 ? (now-late) : 0;
    
    if (cha/3600<1) {
        timeString = [NSString stringWithFormat:@"%f", cha/60];
        timeString = [timeString substringToIndex:timeString.length-7];
        timeString=[NSString stringWithFormat:@"%@ 分前", timeString];
        
    }else if (cha/3600>1 && cha/3600<24) {
        timeString = [NSString stringWithFormat:@"%f", cha/3600];
        timeString = [timeString substringToIndex:timeString.length-7];
        timeString=[NSString stringWithFormat:@"%@ 小时前", timeString];
    }
    else
    {
        /* if (needYear) {
         [dateFormatter setDateFormat:@"yyyy-MM-dd"];
         }
         else
         {
         [dateFormatter setDateFormat:@"MM-dd"];
         }
         timeString=[dateFormatter stringFromDate:d];*/
        timeString = [NSString stringWithFormat:@"%.0f 天前",cha/3600/24];
        if (cha/3600/24 > 5) {
            timeString = theDate;
        }
    }
    
    return timeString;
}
+ (NSString*)timeAfterNow:(NSString*)theDate
{
    if (!theDate) {
        return nil;
    }
    NSDateFormatter *dateFormatter=[[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *d=[dateFormatter dateFromString:theDate];
    if (!d) {
        return theDate;
    }
    
    NSTimeInterval late=[d timeIntervalSince1970]*1;
    NSDate* dat = [NSDate dateWithTimeIntervalSinceNow:0];
    NSTimeInterval now=[dat timeIntervalSince1970]*1;
    NSString *timeString=@"";
    
    NSTimeInterval cha= late - now;
    if(cha<0){
        return @"已过期";
    }
    if (cha/3600<1) {
        timeString = [NSString stringWithFormat:@"%f", cha/60];
        timeString = [timeString substringToIndex:timeString.length-7];
        timeString=[NSString stringWithFormat:@"%@ 分", timeString];
        
    }else if (cha/3600>1 && cha/3600<24) {
        timeString = [NSString stringWithFormat:@"%f", cha/3600];
        timeString = [timeString substringToIndex:timeString.length-7];
        timeString=[NSString stringWithFormat:@"%@ 小时 %d 分", timeString,(int)cha%3600];
    }
    else
    {
        /* if (needYear) {
         [dateFormatter setDateFormat:@"yyyy-MM-dd"];
         }
         else
         {
         [dateFormatter setDateFormat:@"MM-dd"];
         }
         timeString=[dateFormatter stringFromDate:d];*/
        timeString = [NSString stringWithFormat:@"%.0f 天 %d 小时 %d 分",cha/3600/24,(int)(cha/3600)%24,(int)(cha/3600/24)%60];
    }
    
    return timeString;
}

+(NSString *)timeFormatToNow:(NSString*)format
{
    
    
    format = format?format:@"yyyy-MM-dd HH:mm:ss";
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    //设定时间格式,这里可以设置成自己需要的格式
    [dateFormatter setDateFormat:format];
   // NSDate *date = [NSDate dateWithTimeIntervalSince1970:timeInterval];
    return [dateFormatter stringFromDate:[NSDate date]];

}

@end
