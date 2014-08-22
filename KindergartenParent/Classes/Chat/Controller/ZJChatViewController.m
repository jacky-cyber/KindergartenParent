//
//  ChatViewController.m
//  XmppChart
//
//  Created by classjing on 14-4-3.
//  Copyright (c) 2014年 define. All rights reserved.
//

#import "ZJChatViewController.h"
#import "ChatMessageCell.h"
#import "AFNetworking.h"
#import "SoundTool.h"
#import <AVFoundation/AVFoundation.h>
#import "MBProgressHUD.h"
#import "UIImageView+MJWebCache.h"
#import "IQKeyboardManager.h"
@interface ZJChatViewController ()<UITableViewDataSource, NSFetchedResultsControllerDelegate, UITextFieldDelegate,UINavigationControllerDelegate,UIImagePickerControllerDelegate,UIActionSheetDelegate>
{
    // 查询结果控制器
    NSFetchedResultsController *_fetchResultsController;
    
    
    // 查询结果控制器
    NSFetchedResultsController *_fetchedResultsController;
    SoundTool *_soundTool;
    
    AVAudioPlayer *_player;
    NSString *_currentMusicName;//当前点击的语音的文件
    
    BOOL _isKeyBoardBecome;//判断是否键盘弹起
    
}


// 输入视图的底部约束
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *inputViewConstraint;

@property (weak, nonatomic) IBOutlet UITableView *tableView;

@property (weak, nonatomic) IBOutlet UIButton *sendMsgButton;
@property (weak, nonatomic) IBOutlet UITextField *inputTextField;


@end




@implementation ZJChatViewController

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    [IQKeyboardManager sharedManager].enable = NO;
}
-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [IQKeyboardManager sharedManager].enable = YES;
}
- (void)viewDidLoad
{
    [super viewDidLoad];
	
    
    
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardStateChanged:) name:UIKeyboardWillChangeFrameNotification object:nil];
    // 绑定数据
    [self dataBinding];
    
    [self scrollToTableBottom];
    
    //加入电话按钮
    [self callBtn];
    
    [self loadData];
    
}


#pragma mark 打电话
-(void)callBtn
{
    //报名
    UIButton *btnR = [UIButton buttonWithType:UIButtonTypeCustom];
    btnR.frame = CGRectMake(0, 0, 30, 30);
    [btnR addTarget:self action:@selector(callAction) forControlEvents:UIControlEventTouchUpInside];
    UIImage *backgroundImg= [UIImage imageNamed:@"icon_call_"];
    [btnR setBackgroundImage:backgroundImg forState:UIControlStateNormal];
    UIBarButtonItem *ItemR = [[UIBarButtonItem alloc]initWithCustomView:btnR];
    self.navigationItem.rightBarButtonItem = ItemR;
    
}
#pragma mark 打电话
-(void)callAction
{
    if (_tel.isEmptyString || _tel ==nil) {
        kPE(@"老师没有设置电话号码", 0.5);
        return;
    }
    UIActionSheet *sheet = [[UIActionSheet alloc] initWithTitle:nil delegate:self cancelButtonTitle:@"取消" destructiveButtonTitle:_tel otherButtonTitles:@"拨打", nil];
    [sheet showInView:self.view];
}
-(void)actionSheet:(UIActionSheet *)actionSheet clickedButtonAtIndex:(NSInteger)buttonIndex
{
     MyLog(@"%d", buttonIndex);
    if (buttonIndex==1) {
    NSString *telUrl = [NSString stringWithFormat:@"telprompt:%@",_tel];
    NSURL *url = [[NSURL alloc] initWithString:telUrl];
    [[UIApplication sharedApplication] openURL:url];
    }
}
#pragma mark - 文本框代理方法
- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    // 回车发送消息
    // 1. 检查是否有内容
    NSString *str = [textField.text trimString];
    
    if (str) {
        [self chat:str];
    }
    
    return YES;
}


-(void)textFieldDidBeginEditing:(UITextField *)textField
{
    _isKeyBoardBecome = YES;
}

#pragma mark 判断文本框是否有文字，然后显示影藏发送按钮
-(BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    // 1. 检查是否有内容
    if (range.location > 0 || range.length !=1) {
        _sendMsgButton.hidden = NO;
       
    }else{
        _sendMsgButton.hidden = YES;
    
    }
    
    return YES;
}

#pragma mark 发送消息
- (IBAction)sendMsgAction {
    
    NSString *str = [_inputTextField.text trimString];
    //NSString *str = @"xxxx";
    if (str) {
        [self chat:str];
    }
    
}


-(void)chat:(NSString*)str
{
    
    NSString *type = @"chat";
    
    if (str.length > 0) {
        // 2. 实例化一个XMPPMessage（XML一个节点），发送出去即可
        XMPPMessage *message = [XMPPMessage messageWithType:type to:_bareJID];
        
        [message addBody:str];

        [xmppDelegate.xmppStream sendElement:message];
        
    }
    _inputTextField.text = @"";
    
    _sendMsgButton.hidden = YES;
}
-(void)loadData
{
    //1.获取花名册上下文
    NSManagedObjectContext *context = xmppDelegate.xmppRosterCoreDataStorage.mainThreadManagedObjectContext;
    //2.查询请求
    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"XMPPUserCoreDataStorageObject"];
    
    //3.排序
    NSSortDescriptor *sort = [NSSortDescriptor sortDescriptorWithKey:@"displayName" ascending:YES];
    
    request.sortDescriptors = @[sort];
    
    //4.实例化查询控制器
    
    _fetchedResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:request managedObjectContext:context sectionNameKeyPath:@"sectionNum" cacheName:nil];
    //5.设置代理
    _fetchedResultsController.delegate = self;
    //6.控制器查询
    NSError *error = nil;
    if (![_fetchedResultsController performFetch:&error]) {
        NSLog(@"%@",error);
    }
    NSArray *frends = [_fetchedResultsController fetchedObjects];
    
    for (int i = 0; i < frends.count; i++) {
        XMPPUserCoreDataStorageObject *user = frends[i];
        MyLog(@"------%@--------",user.displayName);
    }
    
}

#pragma mark - 绑定数据
- (void)dataBinding
{
  
        // 1. 数据库的上下文
        NSManagedObjectContext *context = xmppDelegate.xmppMessageArchivingCoreDataStorage.mainThreadManagedObjectContext;
        // 2. 定义查询请求
        NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"XMPPMessageArchiving_Message_CoreDataObject"];
        // 3. 定义排序
        NSSortDescriptor *sort = [NSSortDescriptor sortDescriptorWithKey:@"timestamp" ascending:YES];
        [request setSortDescriptors:@[sort]];
        // 4. 需要过滤查询条件，谓词，过滤当前对话双发的聊天记录，将“lisi”的聊天内容取出来
    
//    MyLog(@"%@", [LoginUser sharedLoginUser].myJIDName);
//    request.predicate = [NSPredicate predicateWithFormat:@"bareJidStr CONTAINS[cd] %@ AND streamBareJidStr CONTAINS[cd] %@", _bareJidStr, [LoginUser sharedLoginUser].myJIDName];
//    [request setSortDescriptors:@[sort]];

    
        NSPredicate *predicate = [NSPredicate predicateWithFormat:@"bareJidStr = %@", _bareJID.bare];
        [request setPredicate:predicate];
        //4.实例化查询控制器
        _fetchResultsController = [[NSFetchedResultsController alloc] initWithFetchRequest:request managedObjectContext:context sectionNameKeyPath:nil cacheName:nil];
        //5.设置代理
        _fetchResultsController.delegate = self;
        //6.控制器查询
        NSError *error = nil;
        if (![_fetchResultsController performFetch:&error]) {
            MyLog(@"%@",error);
        }
    
}
#pragma mark - 键盘监听方法
- (void)keyboardStateChanged:(NSNotification *)notifcation
{
    // 通过跟踪发现使用UIKeyboardFrameEndUserInfoKey可以知道键盘的高度
    CGRect rect = [notifcation.userInfo[UIKeyboardFrameEndUserInfoKey] CGRectValue];
    
    // 2. 根据rect的orgion.y可以判断键盘是开启还是关闭
    if (rect.origin.y == [UIScreen mainScreen].bounds.size.height) {
        // 关闭键盘
        _inputViewConstraint.constant = 0.0;
    } else {
        // 打开键盘
        _inputViewConstraint.constant = rect.size.height;
    }
    // 取出动画时长
    NSTimeInterval duration = [notifcation.userInfo[UIKeyboardAnimationDurationUserInfoKey] floatValue];
    
    // 在使用自动布局后，所有的对象位置都是由自动布局系统控制的
    // 程序员，不在需要指定控件的frame，如果需要动画，设定完属性之后，调用layoutIfNeeded即可。
    [UIView animateWithDuration:duration animations:^{
        [self.view layoutIfNeeded];
    }];
    
    [self scrollToTableBottom];
}
#pragma mark - 查询结果控制器代理方法
- (void)controllerDidChangeContent:(NSFetchedResultsController *)controller
{
    [_tableView reloadData];
    
    [self scrollToTableBottom];
}

#pragma mark - 数据源方法
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    id <NSFetchedResultsSectionInfo> info = _fetchResultsController.sections[section];
    return [info numberOfObjects];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *FromID = @"ChatFromCell";
    static NSString *ToID = @"ChatToCell";
    
    NSString *CellIdentifier;
    
    ChatMessageCell *cell = nil;
    
    BOOL flag;
    

        XMPPMessageArchiving_Message_CoreDataObject *message = [_fetchResultsController objectAtIndexPath:indexPath];
        //MyLog(@"---%@",xmppDelegate.xmppStream.myJID.domain);
        flag = message.isOutgoing;
        if (flag) {
            CellIdentifier = FromID;
            if ([message.body hasSuffix:@".png"]) {
                CellIdentifier = @"ImageChatFromCell";
                //MusicChatFromCell
            }
            if ([message.body hasSuffix:@".mp3"]) {
                CellIdentifier = @"MusicChatFromCell";
                //MusicChatFromCell
            }
            
        } else {
            CellIdentifier = ToID;
            if ([message.body hasSuffix:@".png"]) {
                CellIdentifier = @"ImageChatToCell";
                //MusicChatFromCell
            }
            if ([message.body hasSuffix:@".mp3"]) {
                CellIdentifier = @"MusicChatToCell";
                //MusicChatFromCell
            }
        }
        
        cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    if (cell == nil) {
        cell = [[ChatMessageCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    cell.messageButton.tag = indexPath.row;
     [cell setMessage:message isOutgoing:flag isImageTag:indexPath.row];
    
    
        if (flag) {
            [cell.headImageView setImageURLStr:[LoginUser sharedLoginUser].profilImg placeholder:[UIImage imageNamed:@"profile"]];
        } else {
            [cell.headImageView setImageURLStr:self.bareImageStr placeholder:[UIImage imageNamed:@"profile"]];
            
        }
    
        //如果是语音，点击的时候开始下载并且播放
        if ([message.body hasSuffix:@".mp3"]) {
            [cell.messageButton addTarget:self action:@selector(playSoundAction:) forControlEvents:UIControlEventTouchUpInside];
        }

   
    return cell;
}
#pragma mark - 滚动到表格的末尾
- (void)scrollToTableBottom
{
    // 让表格滚动到末尾
    // 计算出所有的数据行数，直接定位到最末一行即可
    id <NSFetchedResultsSectionInfo> info = _fetchResultsController.sections[0];
    // 所有记录行数
    NSInteger count = [info numberOfObjects];
    // 判断是否有数据
    if (count <= 0) {
        return;
    }
    
    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:(count - 1) inSection:0];
    
    [_tableView selectRowAtIndexPath:indexPath animated:YES scrollPosition:UITableViewScrollPositionBottom];
}
#pragma mark 表格行高
- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    // 提示，在此不能直接调用表格行控件的高度，否则会死循环
    // 1. 取出显示行的文本
    XMPPMessageArchiving_Message_CoreDataObject *message = [_fetchResultsController objectAtIndexPath:indexPath];
    NSString *str = message.body;
    
    // 2. 计算文本的占用空间
    CGSize size = [str sizeWithFont:[UIFont systemFontOfSize:14.0] constrainedToSize:CGSizeMake(180, 10000.0)];
    
    //如果是图片的话，就显示图片高度，固定120
    //MyLog(@"----->%@",message.body);
    if ([message.body hasSuffix:@".png"]) {
        return 130;
    }
    if ([message.body hasSuffix:@".mp3"]) {
        return 70;
    }
    
    
    // 3. 根据文本空间计算行高
    if (size.height + 50.0 > 80.0) {
        return size.height + 50.0;
    }
    
    return 80;
}


#pragma mark - 添加照片
- (IBAction)addPhoto {
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    
    picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    picker.delegate = self;
    
    [self presentViewController:picker animated:YES completion:nil];
     _inputViewConstraint.constant = 0.0;
}

#pragma mark UIImagePickerController代理方法
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    UIImage *image = info[UIImagePickerControllerOriginalImage];
    [HttpTool updateFileWithPath:@"xmppuploadfile" params:nil withImag:image success:^(id JSON) {
        MyLog(@"%@",JSON);
        if ([JSON[@"code"] intValue] == 0) {
            [self chat:JSON[@"data"][@"url"]];
        }
    } failure:^(NSError *error) {
        
    }];
    
    [self dismissViewControllerAnimated:YES completion:nil];
}
#pragma mark - 语音聊天操作
#pragma mark 开始录影

- (IBAction)startRecord:(id)sender {
    [sender setTitle:@"松开结束" forState:UIControlStateHighlighted];
    _soundTool = [[SoundTool alloc] init];
    [_soundTool startRecord];
}
- (IBAction)stopRecord:(id)sender {


    [_soundTool stopRecord];
    //判断录音时间是否太短
    if (_soundTool.currentTime < 1.0 ) {
        //[SVProgressHUD showWithStatus:@"录音时间太短！"];
        kPE(@"录音时间太短！", 0.5);
        return;
    }
    NSString *mp3Path =  [_soundTool mp3Path];
    
    
    NSURL *fileUrl = [NSURL fileURLWithPath:mp3Path isDirectory:YES];
    
    [HttpTool updateFileWithPath:@"xmppuploadfile" withMp3URL:fileUrl success:^(id JSON) {
        if ([JSON[@"code"] intValue] == 1) {
            kPE(@"发送语音失败", 0.5);
            return ;
        }
        MyLog(@"%@",JSON);
        if (JSON[@"data"]) {
            [self chat:JSON[@"data"][@"url"]];
        }
        kPdismiss;
    } failure:^(NSError *error) {
        kPE(kHttpErrorMsg, 0.5);
    }];
}

#pragma mark 下载并播放音乐
-(void)playSoundAction:(UIButton*)sender
{

    NSIndexPath *indexPath = [NSIndexPath indexPathForRow:(sender.tag) inSection:0];
    
    XMPPMessageArchiving_Message_CoreDataObject *message = [_fetchResultsController objectAtIndexPath:indexPath];
    NSString *str = message.body;
    MyLog(@"%@",str);
    _currentMusicName  = [str substringFromIndex:str.length-20];
    NSString *downloadPath = [[NSString stringWithFormat:@"/%@",_currentMusicName] documentsPath];
    
    //判断文件是否存在，不存在就系在，存在就算了
    NSFileManager *fileManger = [NSFileManager defaultManager];
    if (![fileManger fileExistsAtPath:downloadPath isDirectory:nil]) {
        [self download:str];
    }else{
        [self soundIng];
    }

    
    
}
- (void)download:(NSString*)urlStr
{
    // 1. NSURL
    // NSString *urlStr = [[kFileServerURL appendStr:@"/upload/"] appendStr:_currentMusicName];
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
    
    NSString *downloadPath = [[NSString stringWithFormat:@"/%@",_currentMusicName] documentsPath];

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
        [self soundIng];
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        MyLog(@"下载失败");
    }];
    [op start];
    
    
    
}
-(void)soundIng
{
    
    if (_player.playing) {
        [_player stop];
        return;
    }
    
    NSString *filePath = [[NSString stringWithFormat:@"/%@",_currentMusicName] documentsPath];
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
    _player.volume = 1.0;
    // 准备播放器,将URL指定的文件做一个预加载,从而可以在需要播放的时候,立即播出声音
    [_player prepareToPlay];
    
    [_player play];
    
    
}
CGFloat lastContentOffset;
-(void)scrollViewWillBeginDragging:(UIScrollView*)scrollView{
    lastContentOffset = scrollView.contentOffset.y;
    _isKeyBoardBecome = NO;
}
-( void )scrollViewDidScroll:( UIScrollView *)scrollView {
    if (scrollView. contentOffset.y <lastContentOffset )
    {
        if (_inputViewConstraint.constant >0 && !_isKeyBoardBecome) {
            _inputViewConstraint.constant = 0.0;
            [_inputTextField resignFirstResponder];
        }
    }
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
    
}



@end
