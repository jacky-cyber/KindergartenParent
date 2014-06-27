//
//  ZJYijianViewController.m
//  ybparents
//
//  Created by define on 14-6-20.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJYijianViewController.h"

@interface ZJYijianViewController ()<UITextViewDelegate>
@property (weak, nonatomic) IBOutlet UITextView *suggestTextView;

@end

@implementation ZJYijianViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    
    //发送
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 2, 50, 25);
    [btnR addTarget:self action:@selector(sendYijianAction) forControlEvents:UIControlEventTouchUpInside];
    [btnR setImage:[UIImage imageNamed:@"fasong_03"] forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;

}

-(void)textViewDidBeginEditing:(UITextView *)textView
{
    if ([textView.text isEqual:@"期待您的反馈！"]) {
        textView.text = @"";
    }
}

-(void)sendYijianAction{
    
    [_suggestTextView resignFirstResponder];
    NSString *suggest = _suggestTextView.text.trimString;

    if (suggest.isEmptyString || [suggest isEqualToString:@"期待您的反馈！"]) {
        [SVProgressHUD showErrorWithStatus:@"请填写您的反馈内容" duration:1.0];
        return;
    }
    
    [SVProgressHUD showWithStatus:@"正在提交数据" maskType:SVProgressHUDMaskTypeBlack];
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"role":@"0",
                             @"suggest":suggest};
    [HttpTool getWithPath:@"suggest" params:params success:^(id JSON) {
        MyLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] == 0) {
            
            [SVProgressHUD showSuccessWithStatus:@"反馈成功" duration:1];
            [self.navigationController popViewControllerAnimated:YES];
        }
    } failure:^(NSError *error) {
        
    }];
    

    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

@end
