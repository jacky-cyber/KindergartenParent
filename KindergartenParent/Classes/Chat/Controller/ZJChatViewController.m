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

@interface ZJChatViewController ()<UITableViewDataSource, NSFetchedResultsControllerDelegate, UITextFieldDelegate,UINavigationControllerDelegate,UIImagePickerControllerDelegate>
{
    // 查询结果控制器
    NSFetchedResultsController *_fetchResultsController;
    SoundTool *_soundTool;
    
    MBProgressHUD *_HUD;
    
}


// 输入视图的底部约束
@property (weak, nonatomic) IBOutlet NSLayoutConstraint *inputViewConstraint;

@property (weak, nonatomic) IBOutlet UITableView *tableView;

@property (weak, nonatomic) IBOutlet UIButton *sendMsgButton;
@property (weak, nonatomic) IBOutlet UITextField *inputTextField;


@end




@implementation ZJChatViewController


- (void)viewDidLoad
{
    [super viewDidLoad];
	
    [IQKeyboardManager sharedManager].enable = NO;
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardStateChanged:) name:UIKeyboardWillChangeFrameNotification object:nil];
    
    
    //将字符串切割成数组
    //    NSArray *bareArr = [_bareJID.user componentsSeparatedByString:@"@"];
    //    cell.textLabel.text = bareArr[0];
    self.title = _bareJID.user;
    
    // 绑定数据
    [self dataBinding];
    
    [self scrollToTableBottom];
    
    
    
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
#pragma mark 判断文本框是否有文字，然后显示影藏发送按钮
-(BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    // 1. 检查是否有内容
    if (string.length > 0) {
        _sendMsgButton.hidden = NO;
        //[_sendMsgButton setTitle:@"xianshi" forState:UIControlStateNormal];
    }else{
        _sendMsgButton.hidden = YES;
        //s [_sendMsgButton setTitle:@"ccc" forState:UIControlStateNormal];
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
    
//    NSLog(@"%@", [LoginUser sharedLoginUser].myJIDName);
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
            NSLog(@"%@",error);
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
        //NSLog(@"---%@",xmppDelegate.xmppStream.myJID.domain);
        flag = message.isOutgoing;
        if (flag) {
            CellIdentifier = FromID;
            if ([message.body hasPrefix:@"img:"]) {
                CellIdentifier = @"ImageChatFromCell";
                //MusicChatFromCell
            }
            if ([message.body hasPrefix:@"mp3:"]) {
                CellIdentifier = @"MusicChatFromCell";
                //MusicChatFromCell
            }
            
        } else {
            CellIdentifier = ToID;
            if ([message.body hasPrefix:@"img:"]) {
                CellIdentifier = @"ImageChatToCell";
                //MusicChatFromCell
            }
            if ([message.body hasPrefix:@"mp3:"]) {
                CellIdentifier = @"MusicChatToCell";
                //MusicChatFromCell
            }
        }
        
        cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    if (cell == nil) {
        cell = [[ChatMessageCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
     [cell setMessage:message isOutgoing:flag isImageTag:indexPath.row];
    

    if (flag) {
        [cell.headImageView setImageURLStr:[LoginUser sharedLoginUser].profilImg placeholder:[UIImage imageNamed:@"profile"]];
    } else {
        [cell.headImageView setImageURLStr:self.bareImageStr placeholder:[UIImage imageNamed:@"profile"]];
        
    }
    
    //MyLog(@"%@",message.body);
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
    //NSLog(@"----->%@",message.body);
    if ([message.body hasPrefix:@"img:"]) {
        return 120;
    }
    if ([message.body hasPrefix:@"mp3:"]) {
        return 60;
    }
    if (message.body.length>1000) {
        return 140;
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
}

#pragma mark UIImagePickerController代理方法
- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    // 提示：UIImage不能为空
    // NSData *data = UIImagePNGRepresentation(self.imageView.image);
    UIImage *image = info[UIImagePickerControllerOriginalImage];
    NSData *data = UIImageJPEGRepresentation(image,0.5);
    
    
    
 
    NSString *_encodedImageStr = [data base64Encoding];
    
    
     [self chat:_encodedImageStr];
    
    NSLog(@"===Encoded image:\n%@", _encodedImageStr);

    
    
    [self dismissViewControllerAnimated:YES completion:nil];
}
#pragma mark - 语音聊天操作
#pragma mark 开始录影
- (IBAction)startRecord:(UIButton*)sender {
    [sender setTitle:@"松开结束" forState:UIControlStateHighlighted];
    _soundTool = [[SoundTool alloc] init];
    [_soundTool startRecord];
    
}

- (IBAction)stopRecord {
    [_soundTool stopRecord];
    //判断录音时间是否太短
    if (_soundTool.currentTime < 1.0 ) {
        _HUD = [[MBProgressHUD alloc] initWithView:self.view];
        [self.navigationController.view addSubview:_HUD];
        _HUD.labelText = @"录音时间太短！";
        [_HUD show:YES];
        [_HUD hide:YES afterDelay:0.3];
        return;
    }
    NSString *mp3Path =  [_soundTool mp3Path];
    
    
    NSURL *fileUrl = [NSURL fileURLWithPath:mp3Path isDirectory:YES];
    
    NSURL *url = [NSURL URLWithString:kFileServerURL];
    AFHTTPClient *httpClient = [AFHTTPClient clientWithBaseURL:url];
    NSMutableURLRequest *request = [httpClient multipartFormRequestWithMethod:@"POST" path:@"/upload.php" parameters:nil constructingBodyWithBlock:^(id<AFMultipartFormData> formData) {
        [formData appendPartWithFileURL:fileUrl name:@"file" error:nil];
        
    }];
    // 准备做上传的工作！
    // 3. operation
    AFHTTPRequestOperation *op = [[AFHTTPRequestOperation alloc]initWithRequest:request];
    
    [op setCompletionBlockWithSuccess:^(AFHTTPRequestOperation *operation, id responseObject) {
        //时长
        NSString *soundTime = [NSString stringWithFormat:@"%.0f",_soundTool.currentTime];
        NSString *uploadPath = [NSString stringWithFormat:@"%@/upload/%@?%@",kFileServerURL,[[SoundTool sharedSoundTool] fileName],soundTime];
        //        // 发送消息给好友，通知发送了图像
        // 设置消息的正文
        NSString *msg = [NSString stringWithFormat:@"mp3:%@", uploadPath];
        [self chat:msg];
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        NSLog(@"1上传文件失败 %@", error);
    }];
    // 4. operation start
    [op start];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



@end
