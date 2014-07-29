//
//  ZJMonthCommentViewController.m
//  ybparents
//
//  Created by 郑敬 on 14-6-17.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJMonthCommentViewController.h"

@interface ZJMonthCommentViewController ()<UITextViewDelegate>
{
    NSMutableArray *_starArr;//存放星星数目
}
@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UITextView *commentTextView;
@property (weak, nonatomic) IBOutlet UITextView *expectTextView;
@property (weak, nonatomic) IBOutlet UIButton *sendBtn;

@end

@implementation ZJMonthCommentViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    
    _sendBtn.layer.cornerRadius = 5.0;
    _sendBtn.layer.masksToBounds = YES;
    // Do any additional setup after loading the view from its nib.
    _scrollView.contentSize = CGSizeMake(320, 480);
    
    //存放星星
    _starArr = [NSMutableArray array];
    for (int i =1; i<=3; i++) {
        [_starArr addObject:@(i)];
    }
}
#pragma mark 计算月评的星星数目
- (IBAction)starAction:(UIButton*)sender {
    
    UIImage *garyImg = [UIImage imageNamed:@"star"];
    UIImage *hImg = [UIImage imageNamed:@"star_h"];
    
    NSLog(@"%d",sender.tag);
    
    
    //是否包含星星
    if (![_starArr containsObject:@(sender.tag)]) {
        for (int i = 1 ; i <=5; i++) {
            if (sender.tag >=i && ![_starArr containsObject:@(i)]) {
                [_starArr addObject:@(i)];
            }
        }
        
    }else{
        for (int i = 1 ; i <=5; i++) {
            if (sender.tag <i) {
                [_starArr removeObject:@(i)];
            }
        }
    }
    
    
    //UIButton *btn = (UIButton*)[self.scrollView viewWithTag:5];
    
    for (int i = 1; i<=5; i++) {
        UIButton *btn = (UIButton*)[self.scrollView viewWithTag:i];
        if ([_starArr containsObject:@(i)]) {
             [btn setImage:hImg forState:UIControlStateNormal];
        }else{
             [btn setImage:garyImg forState:UIControlStateNormal];
        }
       
       
    }
    
   // MyLog(@"%@",_starArr);
    
    
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(void)textViewDidBeginEditing:(UITextView *)textView{
    [self setupTextView:textView];
}
-(void)textViewDidEndEditing:(UITextView *)textView
{
    [self setupTextView:textView];
}

-(void)setupTextView:(UITextView*)textView
{
    if (textView == _commentTextView) {
        if ([textView.text isEqual:@"对教师的评语"]) {
            textView.text = @"";
        }else if (textView.text.isEmptyString){
            textView.text = @"对教师的评语";
        }
    }else{
        if ([textView.text isEqual:@"对教师的期望"]) {
            textView.text = @"";
        }else if (textView.text.isEmptyString){
            textView.text = @"对教师的期望";
        }
    }

}

- (IBAction)sendAction:(id)sender {
    NSString *commentStr = _commentTextView.text.trimString;
    if (commentStr.isEmptyString || [commentStr isEqualToString:@"对教师的评语"]) {
        [SVProgressHUD showErrorWithStatus:@"请填写对教师的评语" duration:1.0];
        return;
    }
    
    NSString *expectStr = _expectTextView.text.trimString;
    if (expectStr.isEmptyString || [expectStr isEqualToString:@"对教师的期望"]) {
        [SVProgressHUD showErrorWithStatus:@"请填写对教师的期望" duration:1.0];
        return;
    }
    
    [SVProgressHUD showWithStatus:@"正在评论" maskType:SVProgressHUDMaskTypeBlack];
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,
                             @"comment":commentStr,
                             @"expect":expectStr,
                             @"teacherid":[LoginUser sharedLoginUser].teacherid,
                             @"starnum":@(_starArr.count)};
    [HttpTool getWithPath:@"addappraise" params:params success:^(id JSON) {
        MyLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] == 0) {
            
            [SVProgressHUD showSuccessWithStatus:@"评论成功" duration:1];
            
                 }
    } failure:^(NSError *error) {
        kPE(@"月评失败", 1);
    }];

    
    
}
- (IBAction)noSendAction:(id)sender {
}

@end
