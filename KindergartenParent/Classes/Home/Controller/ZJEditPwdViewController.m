//
//  ZJEditPwdViewController.m
//  ybparents
//
//  Created by 郑敬 on 14-6-16.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJEditPwdViewController.h"

@interface ZJEditPwdViewController ()
@property (weak, nonatomic) IBOutlet UITextField *oldPwd;
@property (weak, nonatomic) IBOutlet UITextField *pwdTextField;
@property (weak, nonatomic) IBOutlet UITextField *repwdTextField;

@end

@implementation ZJEditPwdViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    //导航
    
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 0, 50, 25);
    [btnR addTarget:self action:@selector(sendAction) forControlEvents:UIControlEventTouchUpInside];
    UIImage *backgroundImg= [UIImage resizedImage:@"nav_rightbackbround_image"];
    [btnR setBackgroundImage:backgroundImg forState:UIControlStateNormal];
    [btnR setTitle:@"保存" forState:UIControlStateNormal];
    [btnR setTitleColor:[UIColor colorWithRed:0.129 green:0.714 blue:0.494 alpha:1.000] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;
    
    //重新设置textfield高度边框颜色
    [self setupTextFiel:_oldPwd];
    //设置密码
    [self setupTextFiel:_pwdTextField];
    //重置密码
    [self setupTextFiel:_repwdTextField];
   
}
//设置textfield
-(void)setupTextFiel:(UITextField*)textField
{
    textField.layer.borderColor = [UIColor colorWithRed:0.000 green:0.678 blue:0.424 alpha:1.000].CGColor;
    textField.layer.borderWidth = 1.0;
    textField.layer.cornerRadius = 2;
    textField.layer.masksToBounds = YES;
    CGRect frame = textField.frame;
    frame.size.height +=20;
    textField.frame = frame;
}
#pragma mark 修改密码
-(void)sendAction
{
    [_repwdTextField resignFirstResponder];
    [_oldPwd resignFirstResponder];
    [_pwdTextField resignFirstResponder];
    NSString *oldpwd = _oldPwd.text.trimString;
    NSString *newpwd = _pwdTextField.text.trimString;
    NSString *repwd  = _repwdTextField.text.trimString;
    
    if (oldpwd.isEmptyString) {
        [SVProgressHUD showErrorWithStatus:@"旧密码不能为空" duration:1];
        //[_oldPwd becomeFirstResponder];
        return;
    }
    if (newpwd.isEmptyString) {
        [SVProgressHUD showErrorWithStatus:@"新密码不能为空" duration:1];
        //[_pwdTextField becomeFirstResponder];
        return;
    }
    if (repwd.isEmptyString) {
        [SVProgressHUD showErrorWithStatus:@"确认密码不能为空" duration:1];
        //[_repwdTextField becomeFirstResponder];
        return;
    }
    if (![repwd isEqualToString:newpwd]) {
        [SVProgressHUD showErrorWithStatus:@"两次密码不一样" duration:1];
        return;

    }
    [SVProgressHUD showWithStatus:@"数据加载" maskType:SVProgressHUDMaskTypeBlack];
    
    NSDictionary *params = @{@"oldpwd":oldpwd,@"newpwd":newpwd,@"username":[LoginUser sharedLoginUser].userName};
    
    [HttpTool getWithPath:@"updatepwd" params:params success:^(id JSON) {
        if ([JSON[@"code"] intValue] == 0) {
            [SVProgressHUD showSuccessWithStatus:@"密码修改成功" duration:0.5];
            [self.navigationController popViewControllerAnimated:YES];
        }else{
            [SVProgressHUD showErrorWithStatus:@"密码修改错误，请检查您旧密码是否真确" duration:1];
        }
    } failure:^(NSError *error) {
      //  NSLog(@"%@",error.description);
    }];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
