//
//  NSString+Helper.h
//  Qikewang
//
//  Created by classjing on 14-1-24.
//  Copyright (c) 2014年 qike. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (Helper)
#pragma mark 根据宽度和字体计算出字符串的高度
- (CGFloat)getHeightByWidth:(NSInteger)_width font:(UIFont *)_font;
/**
 *  清空字符串中的空白字符
 *
 *  @return 清空空白字符串之后的字符串
 */
- (NSString *)trimString;

/**
 *  是否空字符串
 *
 *  @return 如果字符串为nil或者长度为0返回YES
 */
- (BOOL)isEmptyString;

/**
 *  返回沙盒中的文件路径
 *
 *  @return 返回当前字符串对应在沙盒中的完整文件路径
 */
- (NSString *)documentsPath;

/**
 *  写入系统偏好
 *
 *  @param key 写入键值
 */
- (void)saveToNSDefaultsWithKey:(NSString *)key;

@end
