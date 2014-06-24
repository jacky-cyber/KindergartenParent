//
//  LoginUser.h
//  Babytop
//
//  Created by classjing on 14-5-5.
//  Copyright (c) 2014年 babytop. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Singleton.h"
@interface LoginUser : NSObject
single_interface(LoginUser)

@property (strong, nonatomic) NSString *userName;
@property (strong, nonatomic) NSString *password;
@property (strong, nonatomic) NSString *userId;
@property (strong, nonatomic) NSString *nickname;
@property (strong, nonatomic) NSString *classes;
@property (strong, nonatomic) NSString *profilImg;
-(BOOL)isLogin;
-(void)loginWithUserName:(NSString*)userName passWord:(NSString*)pwd;
-(void)loginout;
@end
