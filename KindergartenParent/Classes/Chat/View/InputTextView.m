//
//  InputTextView.m
//  企信通
//
//  Created by apple on 13-12-6.
//  Copyright (c) 2013年 itcast. All rights reserved.
//

#import "InputTextView.h"

@interface InputTextView() <UITextFieldDelegate>



// 输入文本
@property (weak, nonatomic) IBOutlet UITextField *inputText;
// 录音按钮
@property (weak, nonatomic) IBOutlet UIButton *recorderButton;

@property (weak, nonatomic) IBOutlet UIButton *voiceButton;

// 点击声音切换按钮
- (IBAction)clickVoice:(UIButton *)button;
// 点击表情切换按钮
- (IBAction)clickEmote:(UIButton *)button;

@end

@implementation InputTextView

- (void)awakeFromNib
{
    // 设置录音按钮的背景图片拉伸效果
    UIImage *image = [UIImage imageNamed:@"VoiceBtn_Black"];
    image = [image stretchableImageWithLeftCapWidth:image.size.width * 0.5 topCapHeight:image.size.height * 0.5];
    UIImage *imageHL = [UIImage imageNamed:@"VoiceBtn_BlackHL"];
    imageHL = [imageHL stretchableImageWithLeftCapWidth:imageHL.size.width * 0.5 topCapHeight:imageHL.size.height * 0.5];
    
    [_recorderButton setBackgroundImage:image forState:UIControlStateNormal];
    [_recorderButton setBackgroundImage:imageHL forState:UIControlStateHighlighted];
    
}

#pragma mark 设置按钮的图像
- (void)setButton:(UIButton *)button imgName:(NSString *)imgName imgHLName:(NSString *)imgHLName
{
    UIImage *image = [UIImage imageNamed:imgName];
    UIImage *imageHL = [UIImage imageNamed:imgHLName];
    
    [button setBackgroundImage:image forState:UIControlStateNormal];
    [button setBackgroundImage:imageHL forState:UIControlStateHighlighted];
}

#pragma mark - Actions
#pragma mark 点击声音切换按钮
- (void)clickVoice:(UIButton *)button
{
    // 1 设置按钮的tag
    button.tag = !button.tag;
    
    // 2 显示录音按钮
    _recorderButton.hidden = !button.tag;
    // 3 隐藏输入文本框
    _inputText.hidden = button.tag;
    
    // 4. 判断当前输入状态，如果是文本输入，显示录音按钮，同时关闭键盘
    if (button.tag) {
        // 1) 关闭键盘
        [_inputText resignFirstResponder];
        
        // 2) 切换按钮图标，显示键盘图标
        [self setButton:button imgName:@"ToolViewInputText" imgHLName:@"ToolViewInputTextHL"];
    } else {
        // 打开文本录入
        // 1) 切换按钮图标，显示录音图标
        [self setButton:button imgName:@"chat_bottom_voice_nor" imgHLName:@"chat_bottom_voice_nor"];
        
        // 2) 打开键盘
       // [_inputText becomeFirstResponder];
    }
}

#pragma mark 点击表情切换按钮
- (void)clickEmote:(UIButton *)button
{
    // 1. 如果当前正在录音，需要切换到文本状态
    if (!_recorderButton.hidden) {
        [self clickVoice:_voiceButton];
    }
    
    // 2. 判断当前按钮的状态，如果是输入文本，替换输入视图(选择表情)
    // 1) 设置按钮的tag
    button.tag = !button.tag;
    
    // 2) 激活键盘
    [_inputText becomeFirstResponder];
    
//    if (button.tag) {
//        // 显示表情选择视图
//        //[_inputText setInputView:_emoteView];
//        
//        // 切换按钮图标，显示键盘选择图像
//        [self setButton:button imgName:@"ToolViewInputText" imgHLName:@"ToolViewInputTextHL"];
//    } else {
//        // 显示系统默认键盘
//        //[_inputText setInputView:nil];
//        
//        // 切换按钮图标，显示表情选择图像
//        [self setButton:button imgName:@"ToolViewEmotion" imgHLName:@"ToolViewEmotionHL"];
//    }
    
    // 3. 刷新键盘的输入视图
    [_inputText reloadInputViews];
}

#pragma mark - 表情选择视图代理方法
// 拼接表情字符串
- (void)emoteSelectorViewSelectEmoteString:(NSString *)emote
{
    // 拼接现有文本
    // 1. 取出文本
    NSMutableString *strM = [NSMutableString stringWithString:_inputText.text];

    // 2. 拼接字符串
    [strM appendString:emote];
    
    // 3. 设置文本
    _inputText.text = strM;
}

// 删除字符串
- (void)emoteSelectorViewRemoveChar
{
    // 1. 取出文本
    NSString *str = _inputText.text;
    
    // 2. 删除最末尾的字符，并设置文本
    _inputText.text =  [str substringToIndex:(str.length - 1)];
}



@end
