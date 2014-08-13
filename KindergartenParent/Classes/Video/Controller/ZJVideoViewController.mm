//
//  ZJVideoViewController.m
//  ybparents
//
//  Created by Define on 14-6-5.
//  Copyright (c) 2014年 Define. All rights reserved.
//

#import "ZJVideoViewController.h"
#import "OCMobClientSDK.h"
#import "ffmpeg_h264.h"

@interface ZJVideoViewController ()<VGUserDelegate, VGViewDelegate, VGSetupDelegate>
{
    CFFmpegH264Decoder ffmpegDecoder;
    UIImageView *_cameraImageView;
}
@property (strong , nonatomic) VGUser * user;
@property (strong , nonatomic) VGView * vgView;
@property (strong , nonatomic) VGSetup * vgSetup;
@property (strong , nonatomic) NSMutableArray * cameraListArray;

@end

@implementation ZJVideoViewController

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
    // Do any additional setup after loading the view.
    self.title = @"视频";
    
    _cameraImageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, 0, 320, 300)];
    _cameraImageView.backgroundColor = [UIColor redColor];
    [self.view addSubview:_cameraImageView];
    _cameraImageView.contentMode = UIViewContentModeScaleAspectFit;
    //初始化体层库
    [VGMobClientSDK MobClientSDKInit];
    //开启解码库
    ffmpegDecoder.Open();

    self.user = [[VGUser alloc] initWithDelegate:self andUserType:0];
    
    
    //这里写死了用户名密码和服务器
    [self.user Login:@"202.75.218.139" username:@"vigo" password:@"jama"];
    
    
}

- (void) cameraList
{
    if (self.cameraListArray != nil) {
        self.cameraListArray = nil;
    }
    self.cameraListArray = [NSMutableArray array];
    VGListCache * listCache = [self.user GetListCache];
    [listCache GetCameraList:self.cameraListArray cameraSrc:0 mediaSrc:0 platformID:0 parentID:0 onlyChild:NO onlyValid:YES];
    NSLog(@"self.cameraListArray:%@",self.cameraListArray);
    
    
    
    [self.vgSetup Stop];
    if (self.vgView != nil) {
        self.vgView = nil;
    }
    
    if (self.vgSetup != nil) {
        self.vgSetup = nil;
    }
    
    self.vgView = [[VGView alloc] initWithDelegate:self user:self.user];
    self.vgSetup = [[VGSetup alloc] initWithDelegate:self view:self.vgView];
    if (self.cameraListArray.count) {
        CameraInfo * camera = [self.cameraListArray objectAtIndex:5];
        [self.vgView ViewCamera:camera.szCameraSerialNO platformID:0 viewType:0x00 viewSubType:0];
    }
    
    
    //[self.cameraListTableView reloadData];
}



#pragma mark -
#pragma mark VGUserDelegate
- (int)OnConnect:           (VGUser *)user
{
    NSLog(@"user连接成功");
    return 1;
}

- (int)OnAuthenticate:      (VGUser *)user userInfo:(UserInfo *)userInfo
{
    NSLog(@"user认证成功");
    
    [self.user performSelectorOnMainThread:@selector(GetCameraList) withObject:nil waitUntilDone:NO];
    
    return 1;
}

- (int)OnNewListInfo:       (VGUser* )user type:(int)nListType platformID:(DWORD)dwPlatformID
          thisFinish:(BOOL)bThisFinish allFinish:(BOOL)bAllFinish
{
    NSLog(@"user有新的列表数据");
    [self performSelectorOnMainThread:@selector(cameraList) withObject:nil waitUntilDone:YES];
    return 1;
}

- (int)OnUserError:         (VGUser *)user eno:(int)nErrNo
{
    NSLog(@"user有错误");
    return 1;
}

#pragma mark -
#pragma mark VGViewDelegate
- (int)OnConnectInfo:		(VGView *)view cameraCon:(DevConInfo *)tCameraCon relayCon:(DevConInfo *)tRelayCon
{
    return 1;
}

- (int)OnConnect:           (VGView *)view devConSucc:(DevConSuccInfo *)devConSucc
{
    return 1;
}

- (int)OnAuthenticate:		(VGView *)view
{
    return 1;
}

- (int)OnVideoData:			(VGView *)view data:(MediaData *)mdVideo
{
    NSLog(@"视频数据");
    
    int nFrameLen = mdVideo.nRawLen;
    unsigned char *pFrameData = mdVideo.pRawData;
    int nFrameWidth  = 0;
    int nFrameHeight = 0;
    
    UIImage * cameraImage = nil;
    
    if (ffmpegDecoder.IsOpen())
    {
        if (ffmpegDecoder.Decode(pFrameData, nFrameLen, &nFrameWidth, &nFrameHeight) >= 0) {
            cameraImage = ffmpegDecoder.GetCurrentImage();
        }
    }
    
    if (cameraImage != nil) {
        [_cameraImageView performSelectorOnMainThread:@selector(setImage:) withObject:cameraImage waitUntilDone:YES];
    }
    
    return 1;
}

- (int)OnAudioData:			(VGView *)view data:(MediaData *)mdAudio
{
    return 1;
}

- (int)OnPlaybackFinished:	(VGView *)view
{
    return 1;
}

- (int)OnViewError:			(VGView *)view eno:(int)nErrNo
{
    return 1;
}




- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
