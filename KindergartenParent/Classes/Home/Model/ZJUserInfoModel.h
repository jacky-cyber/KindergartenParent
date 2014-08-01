//
//  ZJUserInfo.h
//  ybparents
//
//  Created by 郑敬 on 14-6-13.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Singleton.h"
@interface ZJUserInfoModel : NSObject
single_interface(ZJUserInfoModel)
@property(nonatomic,copy) NSString *address;
@property(nonatomic,copy) NSString *classid;
@property(nonatomic,copy) NSString *classes;
@property(nonatomic,copy) NSString *guominshi;
@property(nonatomic,copy) NSString *username;
@property(nonatomic,copy) NSString *nickname;
@property(nonatomic,copy) NSString *name;
@property(nonatomic,copy) NSString *parentname;
@property(nonatomic,copy) NSString *profileimg;
@property(nonatomic,copy) NSString *remark;
@property(nonatomic,copy) NSString *tel;
@property(nonatomic,copy) NSString *totalflower;
@property(nonatomic,copy) NSString *totalpraise;
@property(nonatomic,copy) NSString *totalsun;
@property(nonatomic,copy) NSString *userid;
@property(nonatomic,copy) NSString *teacherid;
@property(nonatomic,copy) NSString *kindergarten;
@property(nonatomic,copy) NSString *kindergartenid;
@property(nonatomic,copy) NSString *role;

@end

