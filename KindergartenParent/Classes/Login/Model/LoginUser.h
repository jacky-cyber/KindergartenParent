//
//  LoginUser.h
//  Babytop
//
//  Created by classjing on 14-5-5.
//  Copyright (c) 2014年 babytop. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Singleton.h"

@class ZJUserInfoModel;

@interface LoginUser : NSObject
single_interface(LoginUser)


//address = "\U5317\U4eac\U5e02\U6d77\U6dc0\U533a\U4e0a\U5730\U6865\U4e1c\U96c5\U7f8e\U79d1\U6280\U56ed2030";
//classes = "\U667a\U6167\U73ed";
//classid = 1;
//guominshi = "\U5bf9\U82b1\U7c89\U8fc7\U654f\Uff0c\U53ef\U80fd\U5bf9\U7c89\U5c18\U7c7b\U7684\U4e1c\U897f\U8fc7\U654f\U3002\U5176\U4ed6\U7684\U6ca1\U6709\U4e86\U5bf9\U82b1\U7c89\U8fc7\U654f\Uff0c\U53ef\U80fd\U5bf9\U7c89\U5c18\U7c7b\U7684\U4e1c\U897f\U8fc7\U654f\U3002\U5176\U4ed6\U7684\U6ca1\U6709\U4e86\U5bf9\U82b1\U7c89\U8fc7\U654f\Uff0c\U53ef\U80fd\U5bf9\U7c89\U5c18\U7c7b\U7684\U4e1c\U897f\U8fc7\U654f\U3002\U5176\U4ed6\U7684\U6ca1\U6709\U4e86";
//kindergarten = "\U5e38\U9752\U56ed\U5e7c\U513f\U56ed";
//kindergartenid = 1;
//name = "\U5b66\U751f";
//nickname = "\U54c8\U54c8";
//parentname = "\U4e0d\U77e5";
//profileimg = "http://static.oschina.net/uploads/user/98/196012_100.jpg?t=1378866193000";
//remark = "";
//role = 0;
//teacherid = LS13436871757;
//tel = 12332324;
//totalflower = 6;
//totalpraise = 5;
//totalsun = 2;
//userid = 1;
//username = xuesheng;

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
