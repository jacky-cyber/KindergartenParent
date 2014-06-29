//
//  ZJSignInViewController.m
//  ybparents
//
//  Created by Define on 14-6-11.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJSignInViewController.h"

@interface ZJSignInViewController ()
@property (weak, nonatomic) IBOutlet UITextView *causeTextView;

@end

@implementation ZJSignInViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    [self.causeTextView becomeFirstResponder];
    
    
    
    
    //发送
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 2, 50, 25);
    [btnR addTarget:self action:@selector(sendAction) forControlEvents:UIControlEventTouchUpInside];
    [btnR setImage:[UIImage imageNamed:@"signin_submit"] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;
    
}
#pragma  makr 提交未到原因
-(void)sendAction
{
    NSString *causeStr = self.causeTextView.text.trimString;
    [_causeTextView resignFirstResponder];
    
   
    kPBlack(@"正在提交未到原因");
    NSDictionary *params = @{@"content":causeStr,
                             @"username":[LoginUser sharedLoginUser].userName,
                             @"notifid":self.userInfo};
    
    [HttpTool getWithPath:@"reply" params:params success:^(id JSON) {
        if ([JSON[@"code"] intValue] == 0) {
            kPS(@"未到原因提交成功", 1);
            
            
            [self.navigationController popViewControllerAnimated:YES];
        }else{
            kPE(@"系统异常", 1)
        }
    } failure:^(NSError *error) {
      //  NSLog(@"%@",error.description);
    }];

    
    MyLog(@"%@",causeStr);
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
