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
#import "ZJAppDelegate.h"
#import "ZJHomeViewController.h"
#import "APService.h"
@interface ZJLoginViewController ()<UITextFieldDelegate>
@property (weak, nonatomic) IBOutlet UITextField *userNameText;
@property (weak, nonatomic) IBOutlet UITextField *pwdText;
@property (weak, nonatomic) IBOutlet UIButton *loginBtn;
@property (weak, nonatomic) IBOutlet UIButton *forgetPwd;
@property (weak, nonatomic) IBOutlet UIView *superBgView;

@end

@implementation ZJLoginViewController



- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    [[EaseMob sharedInstance].chatManager asyncLogoff];
    
    // Do any additional setup after loading the view from its nib.
    UIImage *bgImage = [UIImage imageNamed:@"login_backgroundImage"];
    
    
    self.view.backgroundColor = [UIColor colorWithPatternImage:bgImage];
    
    UIImage *loginImg = [UIImage resizedImage:@"denglu_10"];
    [_loginBtn setBackgroundImage:loginImg forState:UIControlStateNormal];
    _loginBtn.layer.cornerRadius = 3;
    _loginBtn.layer.masksToBounds = YES;
    
    
    UIImage *forgetImg = [UIImage resizedImage:@"wangjipwd"];
    [_forgetPwd setBackgroundImage:forgetImg forState:UIControlStateNormal];
    _forgetPwd.backgroundColor = [UIColor clearColor];
    
    
    //冲洗设置文本框高度
    [self setupTextFiel:_userNameText];
    [self setupTextFiel:_pwdText];
    
    //设置背景的局地不的高度
    CGRect bgFrame = _superBgView.frame;
    CGFloat heihgt = kScreenHeight;
    if (!ISIOS7) {
        heihgt-=20;
    }
    bgFrame.origin.y = heihgt-H(_superBgView)-40;
    _superBgView.frame = bgFrame;

    
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
    
    [HttpTool getWithPath:@"login" params:@{@"username":username,@"pwd":pwd} success:^(id JSON) {
        if (JSON[@"data"]) {
            
           // MyLog(@"%@",JSON);
            
            [SVProgressHUD showSuccessWithStatus:@"登录成功" duration:0.5];
           
            //[LoginUser sharedLoginUser].userName = username;
            
            ZJUserInfoModel *user = [[ZJUserInfoModel alloc] init];
            if ([user.role intValue]!=0) {
                kPE(@"用户名/密码错误", 0.5);
                return ;
            }
            [user setKeyValues:JSON[@"data"]];
            [LoginUser sharedLoginUser].password = pwd;
            [[LoginUser sharedLoginUser] saveInfo:user];
            
           // MyLog(@"%@",[LoginUser sharedLoginUser].description);
            
            [APService setAlias:user.username callbackSelector:nil object:nil];
            DDMenuController *menuController = ((ZJAppDelegate*)[[UIApplication sharedApplication] delegate]).menuController;
            ZJHomeViewController * centerViewController  = [[ZJHomeViewController alloc] init];
            BaseNavigationController *navigationController = [[BaseNavigationController alloc] initWithRootViewController:centerViewController];
            menuController.rootViewController = navigationController;
            
            self.view.window.rootViewController = menuController;
            
            [[EaseMob sharedInstance].chatManager asyncLoginWithUsername:[user.username lowercaseString] password:@"123456" completion:^(NSDictionary *loginInfo, EMError *error) {
                if (!error) {
                    NSLog(@"登录成功:%@",loginInfo);
                    self.view.window.rootViewController = navigationController;
                }else{
                    NSLog(@"error:%@",error);
                }
                
                [[EaseMob sharedInstance].chatManager setIsAutoLoginEnabled:YES];
                
                
            } onQueue:nil];
            [[ZJUserInfoModel sharedZJUserInfoModel] setKeyValues:JSON[@"data"]];
            

        }else{
            [SVProgressHUD showErrorWithStatus:@"用户名/密码错误" duration:1];
        }
    } failure:^(NSError *error) {
        kPE(kHttpErrorMsg, 0.5);
        MyLog(@"%@",error);
    }];
    
    
}
-(BOOL)textFieldShouldReturn:(UITextField *)textField{
    [_userNameText resignFirstResponder];
    [_pwdText resignFirstResponder];
    [self loginAction:nil];
    return YES;
}

- (IBAction)wangjipwdAction:(id)sender {
    kPS(@"请联系管理员", 2);
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
