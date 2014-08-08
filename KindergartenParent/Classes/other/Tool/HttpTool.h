//
//  HttpTool.h
//  新浪微博
//
//  Created by apple on 13-10-30.
//  Copyright (c) 2013年 itcast. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MediaPlayer/MediaPlayer.h>
typedef void (^HttpSuccessBlock)(id JSON);
typedef void (^HttpFailureBlock)(NSError *error);

@interface HttpTool : NSObject
+ (void)getWithPath:(NSString *)path params:(NSDictionary *)params success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure;
+ (void)postWithPath:(NSString *)path params:(NSDictionary *)params success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure;

#pragma mark 上传图像
+ (void)updateFileWithPath:(NSString *)path params:(NSDictionary *)params withImag:(UIImage*)image success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure;
//+ (void)downloadImage:(NSString *)url place:(UIImage *)place imageView:(UIImageView *)imageView;

#pragma mark 上传图像
+ (void)updateFileWithPath:(NSString *)path params:(NSDictionary *)params withImag:(UIImage*)image withURL:(NSURL*)withurl withArr:(NSArray*)array success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure;
+ (void)updateFileWithPath:(NSString *)path withMp3URL:(NSURL*)withurl success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure;
//+ (void)downloadImage:(NSString *)url place:(UIImage *)place imageView:(UIImageView *)imageView;
+(UIImage *)fFirstVideoFrame:(NSString *)path;
@end