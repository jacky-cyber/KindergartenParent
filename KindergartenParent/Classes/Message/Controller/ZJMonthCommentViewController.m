//
//  ZJMonthCommentViewController.m
//  ybparents
//
//  Created by 郑敬 on 14-6-17.
//  Copyright (c) 2014年 郑敬. All rights reserved.
//

#import "ZJMonthCommentViewController.h"

@interface ZJMonthCommentViewController ()<UITextViewDelegate>
@property (weak, nonatomic) IBOutlet UIScrollView *scrollView;
@property (weak, nonatomic) IBOutlet UITextView *commentTextView;
@property (weak, nonatomic) IBOutlet UITextView *expectTextView;

@end

@implementation ZJMonthCommentViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    _scrollView.contentSize = CGSizeMake(320, 480);
}
- (IBAction)starAction:(id)sender {
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
}
- (IBAction)noSendAction:(id)sender {
}

@end
