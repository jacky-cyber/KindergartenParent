//
//  HttpTool.m
//  新浪微博
//
//  Created by apple on 13-10-30.
//  Copyright (c) 2013年 itcast. All rights reserved.
//

#import "HttpTool.h"
#import "AFNetworking.h"

@implementation HttpTool
+ (void)requestWithPath:(NSString *)path params:(NSDictionary *)params success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure method:(NSString *)method
{
    
    
    NSMutableString *urlString = [NSMutableString stringWithFormat:@"kindergarten/service/app!%@.action",path];
    if ([path isEqual:@"updateuserinfo"]) {
        [urlString appendString:@"?isParse=false"];
    }
    
    // 1.创建post请求
    AFHTTPClient *client = [AFHTTPClient clientWithBaseURL:[NSURL URLWithString:kBaseUrl]];
    
    NSMutableDictionary *allParams = [NSMutableDictionary dictionary];
    // 拼接传进来的参数
    if (params) {
        [allParams setDictionary:params];
    }
    // MyLog(@"%@",allParams);
    NSMutableURLRequest *request = [client requestWithMethod:method path:urlString parameters:allParams];
    
    
    MyLog(@"Request: %@ --- params : %@",request.URL,allParams);
    //设置超时
    [request setTimeoutInterval:10];
    // 2.创建AFJSONRequestOperation对象
    NSOperation *op = [AFJSONRequestOperation JSONRequestOperationWithRequest:request
                                                                      success:^(NSURLRequest *request, NSHTTPURLResponse *response, id JSON) {
                                                                          if (success == nil) return;
                                                                          success(JSON);
                                                                          //MyLog(@"%@",JSON);
                                                                      }
                                                                     failure : ^(NSURLRequest *request, NSHTTPURLResponse *response, NSError *error, id JSON) {
                                                                         if (failure == nil) return;
                                                                         //[SVProgressHUD showErrorWithStatus:@"网络请求错误" duration:1];
                                                                         //#warning 错误信息
                                                                         failure(error);
                                                                         MyLog(@"%@",error.description);
                                                                     }];
    
    // 3.发送请求
    [op start];
}

#pragma mark 上传图像
+ (void)updateFileWithPath:(NSString *)path params:(NSDictionary *)params withImag:(UIImage*)image success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure
{
    
    [self updateFileWithPath:path params:params withImag:image withURL:nil withArr:nil success:success failure:failure];
    
}

#pragma mark 上传图像
+ (void)updateFileWithPath:(NSString *)path params:(NSDictionary *)params withImag:(UIImage*)image withURL:(NSURL*)withurl withArr:(NSArray*)array success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure
{
    
    // 1. 定义httpClient
    // 所谓baseURL就是此后所有的请求都基于此地址
    NSURL *url = [NSURL URLWithString:kBaseUrl];
    AFHTTPClient *httpClient = [AFHTTPClient clientWithBaseURL:url];
    NSString *urlString = [NSString stringWithFormat:@"kindergarten/service/app!%@.action?isParse=false",path];
    // 2. 根据httpClient生成post请求
    NSMutableURLRequest *request = [httpClient multipartFormRequestWithMethod:@"POST" path:urlString parameters:params constructingBodyWithBlock:^(id<AFMultipartFormData> formData) {
        
        // 提示：UIImage不能为空
        //NSData *data = UIImagePNGRepresentation(image);
        NSData *data = UIImageJPEGRepresentation(image , 0.3);
        
        // 1) 取当前系统时间
        NSDate *date = [NSDate date];
        // 2) 使用日期格式化工具
        NSDateFormatter *formatter = [[NSDateFormatter alloc]init];
        // 3) 指定日期格式
        [formatter setDateFormat:@"yyyyMMddHHmmss"];
        NSString *dateStr = [formatter stringFromDate:date];
        // 4) 使用系统时间生成一个文件名
        NSString *fileName = [NSString stringWithFormat:@"%@.png", dateStr];
        if (image) {
            MyLog(@"%d",data.length);
            [formData appendPartWithFileData:data name:@"filename" fileName:fileName mimeType:@"image/png"];
            
        }
        
        if (withurl) {
            NSData *data = [NSData dataWithContentsOfURL:withurl];
            MyLog(@"bytelength:%d",data.length);
            NSError *error = nil;
            
            
            //截取出filename
            NSString *fileName  = [withurl.path substringFromIndex:withurl.path.length-19];
            
            MyLog(@"path :%@",path);
            
            //[formData appendPartWithFormData:data name:@"video"];
            //[formData appendPartWithFileURL:withurl name:@"video" error:&error];
            [formData appendPartWithFileURL:withurl name:@"video" fileName:fileName mimeType:@"video/mp4" error:&error];
            
        }
        if (array) {
            for (UIImage *image in array) {
                NSData *data = UIImageJPEGRepresentation(image , 0.75);
                
                NSString *fileName = [NSString stringWithFormat:@"%d.png", arc4random() % 100];
                MyLog(@"%@",fileName);
                [formData appendPartWithFileData:data name:@"filename" fileName:fileName mimeType:@"image/png"];
                
                
            }
        }
    }];
    
    // 准备做上传的工作！
    // 3. operation
    AFHTTPRequestOperation *op = [[AFHTTPRequestOperation alloc]initWithRequest:request];
    
    MyLog(@"%@--->%@",request,params);
    
    // 显示上传进度
    /*
     bytesWritten   本次上传的字节数(本次上传了5k)
     totalBytesWritten  已经上传的字节数(已经上传了80k)
     totalBytesExpectedToWrite  文件总字节数（100k）
     */
    [op setUploadProgressBlock:^(NSUInteger bytesWritten, long long totalBytesWritten, long long totalBytesExpectedToWrite) {
        
        
        // NSLog(@"上传 %f", (float)(totalBytesWritten / totalBytesExpectedToWrite));
    }];
    
    [op setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        NSDictionary *weatherDic = [NSJSONSerialization JSONObjectWithData:responseObject options:NSJSONReadingMutableLeaves error:nil];
        
        success(weatherDic);
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        failure(error);
        // NSLog(@"上传文件失败 %@", error);
    }];
    
    // 4. operation start
    [op start];
    
}

+ (void)updateFileWithPath:(NSString *)path withMp3URL:(NSURL*)withurl success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure
{
    // 1. 定义httpClient
    // 所谓baseURL就是此后所有的请求都基于此地址
    NSURL *url = [NSURL URLWithString:kBaseUrl];
    AFHTTPClient *httpClient = [AFHTTPClient clientWithBaseURL:url];
    NSString *urlString = [NSString stringWithFormat:@"kindergarten/service/app!%@.action?isParse=false",path];
    // 2. 根据httpClient生成post请求
    NSMutableURLRequest *request = [httpClient multipartFormRequestWithMethod:@"POST" path:urlString parameters:nil constructingBodyWithBlock:^(id<AFMultipartFormData> formData) {
        
        if (withurl) {
            NSData *data = [NSData dataWithContentsOfURL:withurl];
            MyLog(@"bytelength:%d",data.length);
            NSError *error = nil;
            
            //截取出filename
            NSString *fileName  = [withurl.path substringFromIndex:withurl.path.length-20];
            [formData appendPartWithFileURL:withurl name:@"audio" fileName:fileName mimeType:@"audio/mp3" error:&error];
            
        }
        
    }];
    
    // 准备做上传的工作！
    // 3. operation
    AFHTTPRequestOperation *op = [[AFHTTPRequestOperation alloc]initWithRequest:request];
    
    [op setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        NSDictionary *weatherDic = [NSJSONSerialization JSONObjectWithData:responseObject options:NSJSONReadingMutableLeaves error:nil];
        
        success(weatherDic);
        
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        failure(error);
        // NSLog(@"上传文件失败 %@", error);
    }];
    // 4. operation start
    [op start];
}
+ (void)postWithPath:(NSString *)path params:(NSDictionary *)params success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure
{
    [self requestWithPath:path params:params success:success failure:failure method:@"POST"];
}

+ (void)getWithPath:(NSString *)path params:(NSDictionary *)params success:(HttpSuccessBlock)success failure:(HttpFailureBlock)failure
{
    [self requestWithPath:path params:params success:success failure:failure method:@"GET"];
}

@end