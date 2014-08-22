

#import "ChatMessageCell.h"
#import "UIImageView+WebCache.h"
#import "AFNetworking.h"
#import "NSString+Helper.h"
#import <AVFoundation/AVFoundation.h>
#import "MJPhotoBrowser.h"
#import "MJPhoto.h"
#import "XMPPMessageArchiving_Message_CoreDataObject.h"
#import "MLEmojiLabel.h"


@interface ChatMessageCell()
{
    UIImage *_receiveImage;
    UIImage *_receiveImageHL;
    UIImage *_senderImage;
    UIImage *_senderImageHL;
    // UIImageView *Im;
    AVAudioPlayer *_player;
    NSString *_currentMusicName;//当前点击的语音的文件
    
    XMPPMessageArchiving_Message_CoreDataObject *_messageCoreData;
    
    UIView *_msgView;
}
@property (nonatomic, strong) MLEmojiLabel *emojiLabel;
@end

@implementation ChatMessageCell

- (UIImage *)stretcheImage:(UIImage *)img
{
    return [img stretchableImageWithLeftCapWidth:img.size.width * 0.5 topCapHeight:img.size.height * 0.65];
}
- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier
{
    self = [[[NSBundle mainBundle]loadNibNamed:reuseIdentifier owner:self options:nil]objectAtIndex:0];
    if (self)
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
        _headImageView.layer.cornerRadius = 5.0;
        _headImageView.layer.masksToBounds = YES;
    }
    return self;
}


- (void)setMessage:(XMPPMessageArchiving_Message_CoreDataObject *)messageCoreData isOutgoing:(BOOL)isOutgoing isImageTag:(NSInteger)tag
{
    _messageCoreData = messageCoreData;
    NSString *message = messageCoreData.body;
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
    CGSize size = [message sizeWithFont:[UIFont systemFontOfSize:14.0] constrainedToSize:CGSizeMake(180, MAXFLOAT)];
    
    // 2.2 使用文本占用空间设置按钮的约束
    // 提示：需要考虑到在Stroyboard中设置的间距
    
    //如果文件是语音文件
    if ([message hasSuffix:@".mp3"]) {
        //_messageHeightConstraint.constant -=5;
        
        UIImageView *imageView = [[UIImageView alloc] init];
        
        if (isOutgoing) {
            imageView.image =[UIImage imageNamed:@"chat_bottom_voice_press_right"];
            imageView.frame = CGRectMake(W(_messageButton)-40, 6, 25, 25);
        }else{
            
            [imageView setImage:[UIImage imageNamed:@"chat_bottom_voice_press_left"]];
            imageView.frame = CGRectMake(10,6, 25, 25);
        }
        
        //截取出filename
        NSString *fileName  = [[message substringFromIndex:message.length-20] substringFromIndex:2];
        _soundTimeLabel.text = @"xxx";
        [_messageButton addSubview:imageView];
        
//        [_messageButton addTarget:self action:@selector(bofangSound:) forControlEvents:UIControlEventTouchUpInside];
        
        //判断图片是否有有img 如果有，就显示图片
    }else
        if ([message hasSuffix:@".png"]) {
            _messageHeightConstraint.constant = 80+10;
            _messageWeightConstraint.constant = 80+20;
            UIView *view = [[UIView alloc] init];
            view.layer.cornerRadius = 5;
            view.layer.masksToBounds = YES;
            UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 80, 80)];
            view.tag = tag+1;
            UIImage *placeHolderImage = [UIImage imageNamed:@"timeline_image_loading"];
            
            // 加载图像
            [imageView setImageURLStr:message placeholder:placeHolderImage];
            imageView.userInteractionEnabled = YES;
            if (isOutgoing) {
                view.frame = CGRectMake(5, 5, 80, 80);
            }else{
                view.frame = CGRectMake(15, 5, 80, 80);
            }
            [imageView addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapImage:)]];
            [view addSubview:imageView];
            [_messageButton addSubview:view];
        }else{
            
            
            [_messageButton setTitleEdgeInsets:UIEdgeInsetsMake(10, 0, 0, 0)];
            
            
            MyLog(@"%@", NSStringFromCGSize(size));
            // 2.3 设置按钮文字
            //[self.view addSubview:self.emojiLabel];
            
            if (isOutgoing) {
                self.emojiLabel.frame= CGRectMake(5, 5, size.width, size.height);
            }else{
                self.emojiLabel.frame= CGRectMake(15, 5, size.width, size.height);
            }
            
            
            [self.emojiLabel setEmojiText:message];
            [self.emojiLabel sizeToFit];
            
            _messageHeightConstraint.constant = H(self.emojiLabel) + 10.0;
            _messageWeightConstraint.constant = W(self.emojiLabel) + 20.0;
            
            
            [_messageButton addSubview:self.emojiLabel];
            
            //[_messageButton setTitle:message forState:UIControlStateNormal];
        }
    
    //MyLog(@"%@",fileName);
    //设置消息时间
    _timestamp.text = [TimeFormatTools timeFormatToDate:messageCoreData.timestamp];
    // 2.4 重新调整布局
    //[self layoutIfNeeded];
}


- (MLEmojiLabel *)emojiLabel
{
	if (!_emojiLabel) {
        _emojiLabel = [[MLEmojiLabel alloc]init];
        //_emojiLabel.backgroundColor = [UIColor brownColor];
        _emojiLabel.numberOfLines = 0;
        _emojiLabel.font = [UIFont systemFontOfSize:14.0f];
        MyLog(@"%f",_emojiLabel.font.lineHeight);
        //_emojiLabel.emojiDelegate = self;
        //        _emojiLabel.textAlignment = NSTextAlignmentCenter;
        _emojiLabel.backgroundColor = [UIColor clearColor];
        _emojiLabel.lineBreakMode = NSLineBreakByCharWrapping;
        _emojiLabel.isNeedAtAndPoundSign = YES;
        _emojiLabel.customEmojiRegex = @"\\[[a-zA-Z0-9\\u4e00-\\u9fa5]+\\]";
        _emojiLabel.customEmojiPlistName = @"expressionImage_custom.plist";
    }
	return _emojiLabel;
}


#pragma mark 跳转到图片页面
- (void)tapImage:(UITapGestureRecognizer *)tap
{
    //计算出父视图tag值
    int tag = tap.view.superview.tag;
    
    //MyLog(@"%@",_messageCoreData.body);
    
    NSArray *images = @[_messageCoreData.body];
    // 1.封装图片数据
    NSMutableArray *photos = [NSMutableArray arrayWithCapacity:images.count];
    for (int i = 0; i<images.count; i++) {
        // 替换为中等尺寸图片
        //NSString *url = [model.images[i] stringByReplacingOccurrencesOfString:@"thumbnail" withString:@"bmiddle"];
        MJPhoto *photo = [[MJPhoto alloc] init];
        photo.url = [NSURL URLWithString:images[i]]; // 图片路径
        UIView *imageBgView  = [self viewWithTag:tag];
        //只放两张图片，所以i<2
        //if (i<2) {
        photo.srcImageView = imageBgView.subviews[i]; // 来源于哪个UIImageView
        //}
        
        [photos addObject:photo];
    }
    // 2.显示相册
    MJPhotoBrowser *browser = [[MJPhotoBrowser alloc] init];
    browser.currentPhotoIndex = tap.view.tag; // 弹出相册时显示的第一张图片是？
    browser.photos = photos; // 设置所有的图片
    [browser show];
    
}


- (void)download:(NSString*)urlStr
{
    // 1. NSURL
    // NSString *urlStr = [[kFileServerURL appendStr:@"/upload/"] appendStr:_currentMusicName];
    // 如果有中文或者空格，需要加百分号
    MyLog(@"%@",urlStr);
    
    _currentMusicName  = [urlStr substringFromIndex:urlStr.length-18];
    NSString *downloadPath = [[NSString stringWithFormat:@"/%@",_currentMusicName] documentsPath];
    
    //判断文件是否存在，不存在就系在，存在就算了
    NSFileManager *fileManger = [NSFileManager defaultManager];
    if ([fileManger fileExistsAtPath:downloadPath isDirectory:nil]) {
        return;
    }else{
        MyLog(@"去下载");
    }
    
    NSURL *url = [NSURL URLWithString:[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    
    // 2. NSURLRequest
    NSURLRequest *request = [NSURLRequest requestWithURL:url];
    
    // 3. 定义Operation
    AFHTTPRequestOperation *op = [[AFHTTPRequestOperation alloc]initWithRequest:request];
    
    // 下载文件—》要告诉op下载到哪里？
    // 输出流（数据在网络上都是以流的方式传输的）
    // 所谓输出流，就是让数据流流到哪里-》保存到沙箱
    // 指定下载路径
    [op setOutputStream:[NSOutputStream outputStreamToFileAtPath:downloadPath append:NO]];
    
    // 设置下载进度代码
    /**
     bytesRead      此次下载的字节数(5k)
     totalBytesRead 已经下载完成的字节数(80k)
     totalBytesExpectedToRead 文件总字节数(100k)
     */
    [op setDownloadProgressBlock:^(NSUInteger bytesRead, long long totalBytesRead, long long totalBytesExpectedToRead) {
        
        float percent = (float)totalBytesRead / totalBytesExpectedToRead;
        MyLog(@"%f", percent);
    }];
    
    // 设置下载完成块代码
    [op setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        
        //MyLog(@"下载完成");
        //下载完成后开始播放
        // [self soundIng];
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        MyLog(@"下载失败");
    }];
    [op start];
    
    
    
}

#pragma mark 播放语音
-(void)bofangSound:(UIButton*)btn
{
    
    
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
        MyLog(@"%@", error.localizedDescription);
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

//



@end
