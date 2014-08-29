//
//  ZJEditUserViewController.m
//  ybparents
//
//  Created by 郑敬 on 14-6-16.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJEditUserViewController.h"
@interface ZJEditUserViewController ()
{
    UITextField *_textField;
    UITextView  *_textView;
    
}
@end

@implementation ZJEditUserViewController



- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    self.title = self.contentLable;
    
    //导航按钮
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 4, 50, 25);
    [btnR addTarget:self action:@selector(updateAction) forControlEvents:UIControlEventTouchUpInside];
    
    [btnR setImage:[UIImage imageNamed:@"editpwd_save"] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;

    
    //修改
    if ([_contentLable isEqualToString:@"家庭住址"] || [_contentLable isEqualToString:@"过  敏  史"] || [_contentLable isEqualToString:@"备注信息"])
    {
        UIImageView *bgImageView = [[UIImageView alloc] initWithFrame:CGRectMake(20, 20, 280, 80)];
        bgImageView.image = [UIImage imageNamed:@"qiwang"];
        [self.view addSubview:bgImageView];
        _textView = [[UITextView alloc] initWithFrame:CGRectMake(22, 22, 276, 75)];
        _textView.layer.borderColor = [UIColor grayColor].CGColor;
        //_textView.layer.borderWidth = 1;
        _textView.text = self.contentTitle;
        [self.view addSubview:_textView];
        
    }else{
        _textField =[[UITextField alloc] initWithFrame:CGRectMake(20, 20, 280, 50)];
        _textField.layer.borderColor = [UIColor colorWithRed:0.110 green:0.502 blue:0.275 alpha:1.000].CGColor;
        _textField.layer.borderWidth = 1;
        _textField.text = self.contentTitle;
        [self.view addSubview:_textField];
    }
    
}

-(void)updateAction
{
    //设置回传的字符串
    NSString *contentStr = @"";
    if (_textView!=nil) {
        contentStr = _textView.text.trimString;
    }else if(_textField!=nil){
        contentStr = _textField.text.trimString;
    }
    
    //_contentLable.text = [_textView.text trimString];;
    
    //[_delegate editUserInfoViewControllerDidFinished];
    //相应代理
    [self.delegate performSelector:@selector(editUserInfoViewControllerDidFinished:withLabel:) withObject:contentStr withObject:_contentLable];
    //修改xmpp 昵称
    if ([_param isEqualToString:@"nickname"]) {
        [self savevCard:contentStr];
    }
    
    
    //[SVProgressHUD showWithStatus:@"修改用户信息" maskType:SVProgressHUDMaskTypeBlack];
    //kPBlack(@"正在修改用户信息");
    NSDictionary *params = @{@"role":@"0",
                             _param:contentStr,
                             @"username":[LoginUser sharedLoginUser].userName};
    
    [HttpTool updateFileWithPath:@"updateuserinfo" params:params withImag:nil success:^(id JSON) {
        if ([JSON[@"code"] intValue] == 0) {
             kPS(@"修改成功", 0);
            [self.navigationController popViewControllerAnimated:YES];
        }else{
            kPE(JSON[@"msg"], 0.5);
        }
    } failure:^(NSError *error) {
         kPE(@"系统异常", 1);
    }];
    
    //修改昵称
    if ([_param isEqual:@"nickname"]) {
        [LoginUser sharedLoginUser].nickname = contentStr;
    }
    

}

-(void)savevCard:(NSString*)str
{
      
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
