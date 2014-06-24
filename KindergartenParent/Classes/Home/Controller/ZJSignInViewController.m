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
-(void)sendAction
{
    NSString *causeStr = self.causeTextView.text.trimString;
    
    MyLog(@"%@",causeStr);
}
- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
