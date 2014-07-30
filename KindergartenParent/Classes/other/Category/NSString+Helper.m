//
//  NSString+Helper.m
//  Qikewang
//
//  Created by classjing on 14-1-24.
//  Copyright (c) 2014年 qike. All rights reserved.
//

#import "NSString+Helper.h"

@implementation NSString (Helper)

#pragma mark 根据宽度和字体计算出字符串的高度
- (CGFloat)getHeightByWidth:(NSInteger)_width font:(UIFont *)_font
{
    
    
    return [self sizeWithFont:_font constrainedToSize:CGSizeMake(_width, MAXFLOAT)].height;
    
    return [self sizeWithFont:_font forWidth:_width lineBreakMode:NSLineBreakByWordWrapping].height;
}

#pragma mark 清空字符串中的空白字符
- (NSString *)trimString
{
    return [self stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
}

#pragma mark 是否空字符串
- (BOOL)isEmptyString
{
    return (self.length == 0);
}

#pragma mark 返回沙盒中的文件路径
- (NSString *)documentsPath
{
    NSString *path = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)[0];
    return [path stringByAppendingString:self];
}

#pragma mark 写入系统偏好
- (void)saveToNSDefaultsWithKey:(NSString *)key
{
    [[NSUserDefaults standardUserDefaults] setObject:self forKey:key];
    [[NSUserDefaults standardUserDefaults] synchronize];
}
- (NSString *)appendToDocumentDir
{
    NSString *docDir = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)[0];
    
    return [docDir stringByAppendingPathComponent:self];
}
- (NSURL *)appendToDocumentURL
{
    return [NSURL fileURLWithPath:[self appendToDocumentDir]];
}

- (NSString *)base64EncodedString
{
    NSData *data = [self dataUsingEncoding:NSUTF8StringEncoding];
    
    return [data base64EncodedStringWithOptions:0];
}

- (NSString *)base64DecodeString
{
    NSData *data = [[NSData alloc] initWithBase64EncodedString:self options:0];
    
    return [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
}

- (NSString *)appendDateTime
{
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    formatter.dateFormat = @"yyyyMMddHHmmss";
    NSString *str = [formatter stringFromDate:[NSDate date]];
    
    return [NSString stringWithFormat:@"%@%@", self, str];
}


-(NSString*)appendStr:(NSString*)str
{
    return [NSString stringWithFormat:@"%@%@",self,str];
}
@end
