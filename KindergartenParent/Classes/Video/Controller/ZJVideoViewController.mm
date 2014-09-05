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
    NSString *_camerano;//摄像头序列号
}
@property (strong , nonatomic) VGUser * user;
@property (strong , nonatomic) VGView * vgView;
@property (strong , nonatomic) VGSetup * vgSetup;
@property (strong , nonatomic) NSMutableArray * cameraListArray;
@property (strong , nonatomic) NSDictionary * userData;
@property (retain, nonatomic)  UIImageView *cameraImageView;
@end

@implementation ZJVideoViewController

-(void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    
}
-(void)viewDidDisappear:(BOOL)animated
{
    [super viewDidDisappear:animated];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    
    
    
    // Do any additional setup after loading the view.
    self.title = @"视频";
     if (!_cameraImageView) {
    
         CGFloat marginT = (kScreenHeight - kNavH -20 -250)/2;
         
    self.cameraImageView = [[UIImageView alloc] initWithFrame:CGRectMake(0,marginT, 320,250)];
     //_cameraImageView.contentMode = UIViewContentModeScaleAspectFit;
    //_cameraImageView.backgroundColor = [UIColor redColor];
         MyLog(@"------");
     }
    if (!self.userData) {
         self.userData = [NSDictionary dictionary];
    }
   
    [self.view addSubview:_cameraImageView];
   
    //开启解码库
    ffmpegDecoder.Open();

    self.user = [[VGUser alloc] initWithDelegate:self andUserType:0];
//        //这里写死了用户名密码和服务器
//        [self.user Login:@"202.75.218.139" username:@"vigo" password:@"jama"];

    [self getAddress];
    

}

#pragma mark 获取视频地址
-(void)getAddress
{
    
    kPBlack(@"请求视频数据");
    NSDictionary *params = @{@"username":[LoginUser sharedLoginUser].userName,@"classid":[LoginUser sharedLoginUser].classid};
    
    [HttpTool getWithPath:@"videoinfo" params:params success:^(id JSON) {
        if ([JSON[@"code"] intValue] ==1) {
            kPE(JSON[@"msg"], 1);
            UIImage *backImg = [UIImage imageNamed:@"video_960"];
            if (iPhone5) {
                backImg = [UIImage imageNamed:@"video_1136"];
            }
            self.view.backgroundColor = [UIColor colorWithPatternImage:backImg];
            [backImg release];
            return ;
        }else{
            
            self.view.backgroundColor = [UIColor blackColor];
            //这里写死了用户名密码和服务器
            self.userData = JSON[@"data"];
            MyLog(@"%@",JSON[@"data"]);
            NSString *urladdress = JSON[@"data"][@"address"];
            NSString *username = JSON[@"data"][@"username"];
            NSString *password = JSON[@"data"][@"password"];
            
            [self.user Login:urladdress username:username password:password];
            //[self.user Login:@"202.75.218.139" username:@"vigo" password:@"jama"];
        }
       
    } failure:^(NSError *error) {
        UIImage *backImg = [UIImage imageNamed:@"video_960"];
        if (iPhone5) {
            backImg = [UIImage imageNamed:@"video_1136"];
        }
        self.view.backgroundColor = [UIColor colorWithPatternImage:backImg];
        [backImg release];
        kPE(kHttpErrorMsg, 0.5);
    }];
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
       // CameraInfo * camera = [self.cameraListArray objectAtIndex:1];
//        [self.vgView ViewCamera:camera.szCameraSerialNO platformID:0 viewType:0x00 viewSubType:0];
        //MyLog(@"%@",self.userData);
        for (CameraInfo *camera in self.cameraListArray) {
            
            //MyLog(@"szCameraSerialNO：%@---%@",camera.szCameraSerialNO,[self.userData objectForKey:@"camerano"]);
            if ([camera.szCameraSerialNO isEqualToString:[_userData objectForKey:@"camerano"]]) {
//                MyLog(@"szCameraSerialNO：%@---%@",camera.szCameraSerialNO,_userData[@"camerano"]);
                if (camera.IsOnline) {
                    [self.vgView ViewCamera:camera.szCameraSerialNO platformID:0 viewType:0x00 viewSubType:0];
                }else{
                    kPE(@"摄像头不可播放,请联系幼儿园", 1);
                }
                
//                
            }
        }
        
    }
    
    
    

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
    MyLog(@"user认证成功");
    
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
    //NSLog(@"视频数据");
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
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
        if (self.cameraImageView.image ==nil) {
            kPdismiss;
        }
        if (cameraImage != nil) {

            [self.cameraImageView performSelectorOnMainThread:@selector(setImage:) withObject:cameraImage waitUntilDone:YES];
           // [cameraImage release];

        }
    
    [pool release];

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

- (int)OnGetQuality:	(VGSetup *)setup fps:(BYTE)cFps res:(BYTE)cRes bps:(WORD)wBps{
    return 1;
}
- (int)OnGetDayInfo:	(VGSetup *)setup days:(NSArray *)tDays{
    return 1;
}
- (int)OnGetHourInfo:	(VGSetup *)setup day:(DWORD)dwDay hours:(NSArray *)tHours{
    return 1;
}
- (int)OnGetMinuteInfo:	(VGSetup *)setup day:(DWORD)dwDay hour:(INT)nHour minutes:(NSArray *)tMinutes{
    return 1;
}
- (int)OnSetupError:	(VGSetup *)setup eno:(int)nErrNo{
    return 1;
}


- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
-(void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [self.vgSetup Stop];
    self.user.userDelegate = nil;
    self.vgView.viewDelegate = nil;
    self.vgSetup.setupDelegate = nil;
    
}
-(void)dealloc
{
    [super dealloc];
    
    ffmpegDecoder.Close();
    [_cameraImageView release];
    [_cameraListArray release];
    [_userData release];
    MyLog(@"--dealloc-");
}

@end
