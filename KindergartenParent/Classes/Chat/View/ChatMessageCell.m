//
//  ChatMessageCell.m
//  企信通
//
//  Created by apple on 13-12-4.
//  Copyright (c) 2013年 itcast. All rights reserved.
//

#import "ChatMessageCell.h"
#import "UIImageView+WebCache.h"
#import "AFNetworking.h"
#import "NSString+Helper.h"
#import <AVFoundation/AVFoundation.h>
CGRect goBackRect;
float goBackDuration;
UIView* goBackgroundView;
UIImageView* goBackImageView;
UIImageView* goBackFakeImageView;
UIViewController* goBackViewController;

@interface ChatMessageCell()
{
    UIImage *_receiveImage;
    UIImage *_receiveImageHL;
    UIImage *_senderImage;
    UIImage *_senderImageHL;
    BOOL isChanged;
    CGRect defaultRect;
    UIImageView *Im;
    UIWindow *window;
    UIImage *fakeImage;
    UIViewController *clickViewController;
    UIView *snapView;
    AVAudioPlayer *_player;
    NSString *_currentMusicName;//当前点击的语音的文件
}

@end

@implementation ChatMessageCell

- (UIImage *)stretcheImage:(UIImage *)img
{
    return [img stretchableImageWithLeftCapWidth:img.size.width * 0.5 topCapHeight:img.size.height * 0.6];
}

- (void)awakeFromNib
{
    // 实例化表格行背景使用的图像
    _receiveImage = [UIImage imageNamed:@"ReceiverTextNodeBkg"];
    _receiveImageHL = [UIImage imageNamed:@"ReceiverTextNodeBkgHL"];
    _senderImage = [UIImage imageNamed:@"SenderTextNodeBkg"];
    _senderImageHL = [UIImage imageNamed:@"SenderTextNodeBkgHL"];
    
    // 处理图像拉伸（因为iOS 6不支持图像切片）
//    _receiveImage = [_receiveImage stretchableImageWithLeftCapWidth:_receiveImage.size.width * 0.5 topCapHeight:_receiveImage.size.height * 0.6];
    _receiveImage = [self stretcheImage:_receiveImage];
    _receiveImageHL = [self stretcheImage:_receiveImageHL];
    _senderImage = [self stretcheImage:_senderImage];
    _senderImageHL = [self stretcheImage:_senderImageHL];
}

- (void)setMessage:(NSString *)message isOutgoing:(BOOL)isOutgoing isImageTag:(NSInteger)tag
{
    // 1. 根据isOutgoing判断消息是发送还是接受，依次来设置按钮的背景图片
    if (isOutgoing) {
        [_messageButton setBackgroundImage:_senderImage forState:UIControlStateNormal];
        [_messageButton setBackgroundImage:_senderImageHL forState:UIControlStateHighlighted];
    } else {
        [_messageButton setBackgroundImage:_receiveImage forState:UIControlStateNormal];
        [_messageButton setBackgroundImage:_receiveImageHL forState:UIControlStateHighlighted];
    }
    
    // 2. 设置按钮文字
    // 2.1 计算文字占用的区间
    CGSize size = [message sizeWithFont:[UIFont systemFontOfSize:14.0] constrainedToSize:CGSizeMake(180, 10000.0)];
    
    // 2.2 使用文本占用空间设置按钮的约束
    // 提示：需要考虑到在Stroyboard中设置的间距
   
    //如果文件是语音文件
    if ([message hasPrefix:@"mp3:"]) {
       // [self download:message];
        
        //[_messageButton setBackgroundImage:[UIImage imageNamed:@"sound"] forState:UIControlStateNormal];
        //如果是10秒内截取的长度是20个长度，大于就是21个长度
        NSInteger length = 20;
        if ([message substringFromIndex:(message.length -20)].length >20) {
            length = 21;
        }
        
        NSArray *arr = [[message substringFromIndex:(message.length -length)] componentsSeparatedByString:@"?"];
        //2014041716012
        _currentMusicName = arr[0];
        if (arr[1]) {
            //_soundTimeLabel.text = [arr[1] appendStr:@"\""];
        }
        //设置时常
       
   
         UIImageView *imageView = [[UIImageView alloc] init];
        
        if (isOutgoing) {
            imageView.image =[UIImage imageNamed:@"chat_bottom_voice_press_right"];
            imageView.frame = CGRectMake(20, 0, 30, 30);
        }else{
            
            [imageView setImage:[UIImage imageNamed:@"chat_bottom_voice_press_left"]];
            imageView.frame = CGRectMake(20, 0, 30, 30);
        }
        [_messageButton addSubview:imageView];
        
        [_messageButton addTarget:self action:@selector(bofangSound:) forControlEvents:UIControlEventTouchUpInside];
   
      //判断图片是否有有img 如果有，就显示图片
    }else
        if ([message hasPrefix:@"img:"]) {
         _messageHeightConstraint.constant = 80+30;
         _messageWeightConstraint.constant = 80+30;
        
         UIImageView *imageView = [[UIImageView alloc] init];
         imageView.tag = tag+1;
         NSString *urlStr = [message substringFromIndex:4];

         NSURL *url = [NSURL URLWithString:urlStr];
         UIImage *placeHolderImage = [UIImage imageNamed:@"DefaultProfileHead"];
            
         // 加载图像
         [imageView setImageWithURL:url placeholderImage:placeHolderImage];
         if (isOutgoing) {
             imageView.frame = CGRectMake(13, 10, 80, 80);
         }else{
             imageView.frame = CGRectMake(17, 10, 80, 80);
         }
   
        [_messageButton addSubview:imageView];
            
        _messageButton.tag = tag+1;

        [_messageButton addTarget:self action:@selector(imageAction:) forControlEvents:UIControlEventTouchUpInside];
            
     }else{
         _messageHeightConstraint.constant = size.height + 30.0;
         _messageWeightConstraint.constant = size.width + 30.0;
        // 2.3 设置按钮文字
         [_messageButton setTitle:message forState:UIControlStateNormal];
    }
    // 2.4 重新调整布局
    [self layoutIfNeeded];
}
-(void)imageAction:(UIButton*)btn{
    NSArray *subViews = btn.subviews;
    for (NSObject *obj in subViews) {
        if ([obj isKindOfClass:[UIImageView class]] ) {
            UIImageView *imageView = (UIImageView*)obj;
            if (imageView.tag == btn.tag) {
                UIImageView *img = imageView;
                [self showImage:img];
            }
        }
    }

   
}
- (void)showImage:(UIImageView *)defaultImageView{
    goBackDuration = 0.5;
    UIImage *image = defaultImageView.image;
    window = [UIApplication sharedApplication].keyWindow;
    goBackgroundView = [[UIView alloc]initWithFrame:CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height)];
    defaultRect = [defaultImageView convertRect:defaultImageView.bounds toView:window];//关键代码，坐标系转换
    goBackgroundView.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0];
    
    UIImageView *imageView = [[UIImageView alloc]initWithFrame:defaultRect];
    imageView.image = image;
    imageView.tag = 1;
    UIImageView *fakeImageView = [[UIImageView alloc]initWithFrame:defaultRect];
    [goBackgroundView addSubview:fakeImageView];
    [goBackgroundView addSubview:imageView];
    [window addSubview:goBackgroundView];
    if (clickViewController!=nil) {
        //截图
        if (!snapView) {
            snapView = clickViewController.view;
        }
        if(UIGraphicsBeginImageContextWithOptions != NULL)
        {
            UIGraphicsBeginImageContextWithOptions(snapView.frame.size, NO, 0.0);
        } else {
            UIGraphicsBeginImageContext(snapView.frame.size);
        }
        [snapView.layer renderInContext:UIGraphicsGetCurrentContext()];
        fakeImage = UIGraphicsGetImageFromCurrentImageContext();
        UIGraphicsEndImageContext();
        
        fakeImageView.image = fakeImage;
        [UIView animateWithDuration:0.5 animations:^{
            imageView.alpha = 0;
            imageView.frame = CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height);
            fakeImageView.frame = CGRectMake(0, 0, [UIScreen mainScreen].bounds.size.width, [UIScreen mainScreen].bounds.size.height);
        } completion:^(BOOL finished) {
            [UIView animateWithDuration:(0.5/2) animations:^{
                goBackImageView = imageView;
                goBackFakeImageView = fakeImageView;
                goBackRect = defaultRect;
                
            } completion:^(BOOL finished) {
                goBackgroundView.alpha = 0;
                for (UIView* next = [self superview]; next; next = next.superview) {
                    UIResponder *nextResponder = [next nextResponder];
                    if ([nextResponder isKindOfClass:[UIViewController class]]) {
                        [((UIViewController*)nextResponder) presentViewController:clickViewController animated:NO completion:^{
                            ;
                        }];//可更改为UINavigation推入
                    }
                }
                goBackViewController = clickViewController;
            }];
        }];
    } else {
        UITapGestureRecognizer *tap=[[UITapGestureRecognizer alloc]initWithTarget:self action:@selector(hideImage:)];
        [goBackgroundView addGestureRecognizer:tap];
        defaultImageView.alpha = 1;
        [UIView animateWithDuration:0.5 animations:^{
            imageView.frame=CGRectMake(0,([UIScreen mainScreen].bounds.size.height-image.size.height*[UIScreen mainScreen].bounds.size.width/image.size.width)/2, [UIScreen mainScreen].bounds.size.width, image.size.height*[UIScreen mainScreen].bounds.size.width/image.size.width);
            goBackgroundView.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.5];
            
        } completion:^(BOOL finished) {
        }];
    }
}
- (void)hideImage:(UITapGestureRecognizer*)tap{
    UIImageView *imageView = (UIImageView*)[tap.view viewWithTag:1];
    [UIView animateWithDuration:0.5 animations:^{
        imageView.frame = defaultRect;
        goBackgroundView.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0];
    } completion:^(BOOL finished) {
        self.alpha = 1;
        [goBackgroundView removeFromSuperview];
    }];
}

- (void)download
{
    // 1. NSURL
   // NSString *urlStr = [[kFileServerURL appendStr:@"/upload/"] appendStr:_currentMusicName];
    NSString *urlStr  = nil;
    // 如果有中文或者空格，需要加百分号
    MyLog(@"%@",urlStr);
    NSURL *url = [NSURL URLWithString:[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    
    // 2. NSURLRequest
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    
    // 3. 定义Operation
    AFHTTPRequestOperation *op = [[AFHTTPRequestOperation alloc]initWithRequest:request];
    
    // 下载文件—》要告诉op下载到哪里？
    // 输出流（数据在网络上都是以流的方式传输的）
    // 所谓输出流，就是让数据流流到哪里-》保存到沙箱
    
    // 指定下载路径
    
    NSString *downloadPath = [_currentMusicName documentsPath];
    
    [op setOutputStream:[NSOutputStream outputStreamToFileAtPath:downloadPath append:NO]];
    
    // 设置下载进度代码
    /**
     bytesRead      此次下载的字节数(5k)
     totalBytesRead 已经下载完成的字节数(80k)
     totalBytesExpectedToRead 文件总字节数(100k)
     */
    [op setDownloadProgressBlock:^(NSUInteger bytesRead, long long totalBytesRead, long long totalBytesExpectedToRead) {
        
        float percent = (float)totalBytesRead / totalBytesExpectedToRead;
        NSLog(@"%f", percent);
    }];
    
    // 设置下载完成块代码
    [op setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        //NSLog(@"下载完成");
       //下载完成后开始播放
        [self soundIng];
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"下载失败");
    }];
    [op start];
    
   
    
}

#pragma mark 播放语音
-(void)bofangSound:(UIButton*)btn
{

//    NSString *filePath = [_currentMusicName appendToDocumentDir];
//    
//    //判断文件是否存在，不存在就系在，存在就算了
//    NSFileManager *fileManger = [NSFileManager defaultManager];
//    if (![fileManger fileExistsAtPath:filePath isDirectory:nil]) {
//        [self download];
//    }else{
//        [self soundIng];
//    }
    
}

-(void)soundIng
{

    if (_player.playing) {
        [_player stop];
        return;
    }

    NSString *filePath = [_currentMusicName documentsPath];
    NSURL *url = [NSURL fileURLWithPath:filePath isDirectory:YES];
    /**
     AVAudioPlayer只能播放本地的音频,而不能播放网络音频!
     */
    NSError *error = nil;
    _player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&error];
      if (error != nil) {
        NSLog(@"%@", error.localizedDescription);
        return;
    }
    // -1 表示无限循环播放
    // 0 表示1次
    // 1 表示2次
    _player.numberOfLoops = 0;
    
    // 准备播放器,将URL指定的文件做一个预加载,从而可以在需要播放的时候,立即播出声音
    [_player prepareToPlay];
    
    [_player play];
    
    
}

@end
