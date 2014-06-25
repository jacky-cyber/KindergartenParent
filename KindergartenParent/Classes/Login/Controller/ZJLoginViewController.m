//
//  ZJLoginViewController.m
//  ybparents
//
//  Created by Define on 14-6-11.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJLoginViewController.h"
#import "ZJAppDelegate.h"
#import "LoginUser.h"
#import "DDMenuController.h"
#import "UIImage+ZJ.h"
#import "ZJUserInfoModel.h"
@interface ZJLoginViewController ()<UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UITextField *userNameText;
@property (weak, nonatomic) IBOutlet UITextField *pwdText;
@property (weak, nonatomic) IBOutlet UIButton *loginBtn;
@property (weak, nonatomic) IBOutlet UIButton *forgetPwd;

@end

@implementation ZJLoginViewController



- (void)viewDidLoad
{
    self.title = @"登陆";
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    UIImage *bgImage = [UIImage imageNamed:@"login_beijing"];
    self.view.backgroundColor = [UIColor colorWithPatternImage:bgImage];
    
    UIImage *loginImg = [UIImage resizedImage:@"denglu_10"];
    [_loginBtn setBackgroundImage:loginImg forState:UIControlStateNormal];
    _loginBtn.layer.cornerRadius = 3;
    _loginBtn.layer.masksToBounds = YES;
    
    
    UIImage *forgetImg = [UIImage resizedImage:@"wangjipwd"];
    [_forgetPwd setBackgroundImage:forgetImg forState:UIControlStateNormal];
    _forgetPwd.backgroundColor = [UIColor clearColor];
    
    
    [self setupTextFiel:_userNameText];
    [self setupTextFiel:_pwdText];
}

//设置textfield
-(void)setupTextFiel:(UITextField*)textField
{
//    textField.layer.borderColor = [UIColor colorWithRed:0.109 green:0.502 blue:0.273 alpha:1.000].CGColor;
    textField.layer.borderWidth = 0;
    CGRect frame = textField.frame;
    frame.size.height +=10;
    textField.frame = frame;
}
- (IBAction)loginAction:(id)sender {
    
    NSString *username = _userNameText.text.trimString;
    NSString *pwd = _pwdText.text.trimString;
    
    if (username.isEmptyString) {
        [SVProgressHUD showErrorWithStatus:@"用户名不能为空" duration:0.5];
        return;
    }else if (pwd.isEmptyString){
        [SVProgressHUD showErrorWithStatus:@"密码不能为空" duration:0.5];
        return;
    }
   
    [SVProgressHUD showWithStatus:@"正在登录，请稍后" maskType:SVProgressHUDMaskTypeBlack];
    
    [HttpTool getWithPath:@"login" params:@{@"username":username,@"pwd":pwd,@"role":@"0"} success:^(id JSON) {
        if (JSON[@"data"]) {
            
            [SVProgressHUD showSuccessWithStatus:@"登录成功" duration:1];
           
            [LoginUser sharedLoginUser].userName = username;
            
            ZJUserInfoModel *user = [[ZJUserInfoModel alloc] init];
            [user setKeyValues:JSON[@"data"]];
            
            [LoginUser sharedLoginUser].password = pwd;
            [LoginUser sharedLoginUser].userId =user.userid;
            [LoginUser sharedLoginUser].classes = user.classes;
            [LoginUser sharedLoginUser].profilImg = user.profileimg;
            [LoginUser sharedLoginUser].nickname = user.nickname;
            
            DDMenuController *menuController = ((ZJAppDelegate*)[[UIApplication sharedApplication] delegate]).menuController;
            self.view.window.rootViewController = menuController;
            
            //[[ZJUserInfoModel sharedZJUserInfoModel] setKeyValues:JSON[@"data"]];
            

        }else{
            [SVProgressHUD showErrorWithStatus:@"登录失败，请检查您用户名密码是否正确" duration:1];
        }
        NSLog(@"%@",JSON);
    } failure:^(NSError *error) {
        MyLog(@"%@",error);
    }];
    
    
}
-(BOOL)textFieldShouldReturn:(UITextField *)textField{
    [_userNameText resignFirstResponder];
    [_pwdText resignFirstResponder];
    [self loginAction:nil];
    return YES;
}

- (IBAction)jizhupwdAction:(id)sender {
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
