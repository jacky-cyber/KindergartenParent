//
//  ZJCommentModel.h
//  ybparents
//
//  Created by define on 14-6-20.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ZJCommentModel : NSObject

//"content": "内容",
//"cmtime": "2015-05-13 10:00:00",
//"cmuid": 1,
//"cmuname": "张三",
//"cmuimgurl": "http://ww.baidu.com/a.jpg"
@property(nonatomic,copy) NSString *content;
@property(nonatomic,copy) NSString *cmtime;
@property(nonatomic,copy) NSString *cmuid;
@property(nonatomic,copy) NSString *cmnickname;
@property(nonatomic,copy) NSString *cmuimgurl;
@end
