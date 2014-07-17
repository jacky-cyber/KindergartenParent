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
#define kClassid @"classid"

#define kProfileImg @"profileImg"
#define kParentName @"parentName"
#define kTel @"tel"
#define kName @"name"
#define kTeacherId @"teacherid"
#define kKindergarten @"kindergarten"
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
-(NSString *)parentname{
    return [self loadStringFromDefaultsWithKey:kParentName];
}
-(NSString *)tel{
    return [self loadStringFromDefaultsWithKey:kTel];
}
-(NSString *)name{
    return [self loadStringFromDefaultsWithKey:kName];
}
-(NSString *)teacherid{
    return [self loadStringFromDefaultsWithKey:kTeacherId];
}
-(NSString *)kindergarten{
    return  [self loadStringFromDefaultsWithKey:kKindergarten];
}
-(NSString *)classid{
    return [self loadStringFromDefaultsWithKey:kClassid];
}
-(void)setClassid:(NSString *)classid
{
    [classid saveToNSDefaultsWithKey:kClassid];
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
-(void)setTel:(NSString *)tel
{
    [tel saveToNSDefaultsWithKey:kTel];
}
-(void)setParentname:(NSString *)parentname
{
    [parentname saveToNSDefaultsWithKey:kParentName];
}
-(void)setName:(NSString *)name
{
    [name saveToNSDefaultsWithKey:kName];
}
-(void)setTeacherid:(NSString *)teacherid
{
    [teacherid saveToNSDefaultsWithKey:kTeacherId];
}
-(void)setKindergarten:(NSString *)kindergarten
{
    [kindergarten saveToNSDefaultsWithKey:kKindergarten];
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

-(NSString *)description{
    return [NSString stringWithFormat:@"用户姓名:%@,名称:%@,班级:%@,电话：%@，家长姓名：%@,幼儿园%@",self.userId,self.name,self.classes,self.tel,self.parentname,self.kindergarten];
}

-(void)loginout
{
    self.userName = @"";
    self.password = @"";
}
@end
