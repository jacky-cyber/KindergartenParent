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


/**
 *  为指定文件名添加沙盒文档路径
 *
 *  @return 添加沙盒文档路径的完整路径字符串
 */
- (NSString *)appendToDocumentDir;

/**
 *  为指定文件名添加沙盒文档路径
 *
 *  @return 添加沙盒文档路径的完整路径字符串
 */
- (NSURL *)appendToDocumentURL;

/**
 *  对指定字符串进行BASE64编码
 *
 *  @return BASE64编码后的字符串
 */
- (NSString *)base64EncodedString;

/**
 *  对指定BASE64编码的字符串进行解码
 *
 *  @return 解码后的字符串
 */
- (NSString *)base64DecodeString;

/**
 *  在字符串末尾添加日期及时间
 *
 *  @return 添加日期及时间之后的字符串
 */
- (NSString *)appendDateTime;

/**
 *  在字符串末尾添加字符
 *
 *  @param str 要添加的字符
 *
 *  @return 添加后的字符串
 */
-(NSString*)appendStr:(NSString*)str;

//MD5加密
+ (NSString*)md5:(NSString *)str;

@end
