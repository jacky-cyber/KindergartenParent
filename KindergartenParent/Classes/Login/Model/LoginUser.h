//
//  LoginUser.h
//  Babytop
//
//  Created by classjing on 14-5-5.
//  Copyright (c) 2014年 babytop. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Singletons.h"

@class ZJUserInfoModel;

@interface LoginUser : NSObject
single_interface(LoginUser)

@property (strong, nonatomic) NSString *userName;
@property (strong, nonatomic) NSString *password;
@property (strong, nonatomic) NSString *userId;
@property (strong, nonatomic) NSString *nickname;
@property (strong, nonatomic) NSString *classes;
@property (strong, nonatomic) NSString *profilImg;
@property (strong, nonatomic) NSString *tel;
@property (strong, nonatomic) NSString *parentname;
@property (strong, nonatomic) NSString *name;
@property (strong, nonatomic) NSString *teacherid;
@property (strong, nonatomic) NSString *kindergarten;
@property (strong, nonatomic) NSString *kindergartenid;
@property (strong, nonatomic) NSString *classid;
@property (strong, nonatomic) NSString *role;
-(BOOL)isLogin;
-(void)loginWithUserName:(NSString*)userName passWord:(NSString*)pwd;
-(void)loginout;
-(void)saveInfo:(ZJUserInfoModel*)user;
@end
