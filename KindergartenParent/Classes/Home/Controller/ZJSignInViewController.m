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
    btnR.frame = CGRectMake(0, 0, 50, 25);
    [btnR addTarget:self action:@selector(sendAction) forControlEvents:UIControlEventTouchUpInside];
    UIImage *backgroundImg= [UIImage resizedImage:@"nav_rightbackbround_image"];
    [btnR setBackgroundImage:backgroundImg forState:UIControlStateNormal];
    [btnR setTitle:@"提交" forState:UIControlStateNormal];
    [btnR setTitleColor:[UIColor colorWithRed:0.129 green:0.714 blue:0.494 alpha:1.000] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;
    
}
#pragma  makr 提交未到原因
-(void)sendAction
{
    NSString *causeStr = self.causeTextView.text.trimString;
    
    causeStr = [NSString stringWithFormat:@"%@ - %@",causeStr,[LoginUser sharedLoginUser].name];
    
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
            kPE(JSON[@"msg"], 1);
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
