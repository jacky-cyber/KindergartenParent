//
//  LoginUser.m
//  Babytop
//
//  Created by classjing on 14-5-5.
//  Copyright (c) 2014年 babytop. All rights reserved.
//

#import "LoginUser.h"
#import "NSString+Helper.h"


#define kUserName @"userName"
#define kPassWord @"passWord"
#define kUserId @"userId"
#define kNickName @"nickName"
#define kClasses @"classes"
#define kProfileImg @"profileImg"

@implementation LoginUser
single_implementation(LoginUser)

#pragma mark - 私有方法
- (NSString *)loadStringFromDefaultsWithKey:(NSString *)key
{
    NSString *str = [[NSUserDefaults standardUserDefaults] stringForKey:key];
    
    return (str) ? str : @"";
}

-(NSString *)userName{
    return [self loadStringFromDefaultsWithKey:kUserName];
}
-(NSString *)password
{
    return [self loadStringFromDefaultsWithKey:kPassWord];
}
-(NSString *)userId
{
    return [self loadStringFromDefaultsWithKey:kUserId];
}
-(NSString *)nickname
{
    return  [self loadStringFromDefaultsWithKey:kNickName];
}
-(NSString *)classes
{
    return  [self loadStringFromDefaultsWithKey:kClasses];
}
-(NSString *)profilImg
{
    return [self loadStringFromDefaultsWithKey:kProfileImg];
}

-(void)setUserName:(NSString *)userName
{
    [userName saveToNSDefaultsWithKey:kUserName];
}
-(void)setPassword:(NSString *)password
{
    [password saveToNSDefaultsWithKey:kPassWord];
}
-(void)setUserId:(NSString *)userId
{
    [userId saveToNSDefaultsWithKey:kUserId];
}
-(void)setNickname:(NSString *)nickname
{
    [nickname saveToNSDefaultsWithKey:kNickName];
}
-(void)setClasses:(NSString *)classes
{
    [classes saveToNSDefaultsWithKey:kClasses];
}
-(void)setProfilImg:(NSString *)profilImg
{
    [profilImg saveToNSDefaultsWithKey:kProfileImg];
}

-(BOOL)isLogin
{
    BOOL flag = YES;
 
    NSLog(@"username:%@,password:%@",self.userName,self.password);
    if(self.userName.isEmptyString || self.password.isEmptyString)
    {
        flag = false;
    }
    
    return flag;
}

-(void)loginWithUserName:(NSString *)userName passWord:(NSString *)pwd
{
    self.userName = userName;
    self.password = pwd;
}

-(void)loginout
{
    self.userName = @"";
    self.password = @"";
}
@end
