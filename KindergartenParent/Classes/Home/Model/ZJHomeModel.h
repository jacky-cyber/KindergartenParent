//
//  ZJHomeModel.h
//  KindergartenParent
//
//  Created by define on 14-6-24.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface ZJHomeModel : NSObject
@property(nonatomic,copy) NSString *id;
@property(nonatomic,copy) NSString *type;
@property(nonatomic,copy) NSString *content;
@property(nonatomic,copy) NSString *createtime;
@property(nonatomic,copy) NSString *createuid;
@property(nonatomic,copy) NSString *title;

//"status" : 0,
//"content" : "测试园长信息",
//"id" : 8,
//"title" : "活动通知",
//"createuid" : "胡园长",
//"type" : 10,
//"createtime" : "2014-07-17 23:25:57"

@end
