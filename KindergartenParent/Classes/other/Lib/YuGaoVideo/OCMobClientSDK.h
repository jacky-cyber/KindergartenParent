//
//  OCMobClientSDK.h
//  iViewer-New
//
//  Created by apple on 13-11-28.
//  Copyright 2013 HuangSiwen. All rights reserved.
//

#import "OCMobClientObject.h"

@class VGUser;
@class VGView;
@class VGSetup;
@protocol VGUserDelegate;
@protocol VGViewDelegate;
@protocol VGSetupDelegate;

#pragma mark -

@interface VGMobClientSDK : NSObject
+ (BOOL)MobClientSDKInit;
+ (void)MobClientSDKFinish;
+ (BOOL)MobClientSDKReInit;
@end

#pragma mark -

@interface VGListCache : NSObject
- (int)GetNvrList:(NSMutableArray *)tList cameraSrc:(int)nCameraSrc mediaSrc:(int)nMediaSrc;
- (int)GetCameraList:(NSMutableArray *)tList cameraSrc:(int)nCameraSrc mediaSrc:(int)nMediaSrc
          platformID:(DWORD)dwPlatformID parentID:(DWORD)dwParentID 
           onlyChild:(bool)bOnlyChild onlyValid:(bool)bOnlyValid;
- (int)GetGroupList:(NSMutableArray *)tList cameraSrc:(int)nCameraSrc mediaSrc:(int)nMediaSrc
         platformID:(DWORD)dwPlatformID parentID:(DWORD)dwParentID 
          onlyChild:(bool)bOnlyChild onlyValid:(bool)bOnlyValid;
- (int)ClearPlatformList;
@end

#pragma mark -

@interface VGUser : NSObject
@property (nonatomic, assign) id<VGUserDelegate> userDelegate;
- (id)initWithDelegate:(id<VGUserDelegate>)delegate andUserType:(DWORD)dwUserType;
- (int)Login:(NSString *)pDSvrIP username:(NSString *)pUserName password:(NSString *)pPassword;
- (int)SetPushInfoWithBpush:(BOOL)bPush userName:(NSString *)userName token:(NSData *)token os:(int)os language:(int)language;
- (int)GetCameraList;
- (VGListCache *)GetListCache;
- (int)directLoginWithIP:(NSString *)IP port:(NSString *)port userName:(NSString *)userName passWord:(NSString *)passWord;
- (int)lanExploreLoginWithDeviceType:(int)ndeviceType deviceID:(DWORD)dwDeviceID userName:(NSString *)userName passWord:(NSString *)passWord;
- (int)lanExploreStart;
- (int)lanExploreStop;
- (int)lanExploreFlush;
@end

#pragma mark -

@protocol VGUserDelegate <NSObject>
- (int)OnConnect:           (VGUser *)user;
- (int)OnAuthenticate:      (VGUser *)user userInfo:(UserInfo *)userInfo;
- (int)OnNewListInfo:       (VGUser* )user type:(int)nListType platformID:(DWORD)dwPlatformID
          thisFinish:(BOOL)bThisFinish allFinish:(BOOL)bAllFinish;
- (int)OnUserError:         (VGUser *)user eno:(int)nErrNo;
@end

#pragma mark -

@interface VGView : NSObject
@property (nonatomic, assign) id<VGViewDelegate> viewDelegate;
@property (nonatomic, assign) NSInteger viewSerialNumber;
- (id)initWithDelegate:(id<VGViewDelegate>)delegate user:(VGUser *)user;
- (int)ViewCamera:(NSString *)pSerialNo platformID:(int)nPlatformID viewType:(int)nViewType viewSubType:(int)nViewSubType;
@end

#pragma mark -

@protocol VGViewDelegate  <NSObject>
- (int)OnConnectInfo:		(VGView *)view cameraCon:(DevConInfo *)tCameraCon relayCon:(DevConInfo *)tRelayCon;
- (int)OnConnect:           (VGView *)view devConSucc:(DevConSuccInfo *)devConSucc;
- (int)OnAuthenticate:		(VGView *)view;
- (int)OnVideoData:			(VGView *)view data:(MediaData *)mdVideo;
- (int)OnAudioData:			(VGView *)view data:(MediaData *)mdAudio;
- (int)OnPlaybackFinished:	(VGView *)view;
- (int)OnViewError:			(VGView *)view eno:(int)nErrNo;
@end

#pragma mark -

@interface VGSetup : NSObject
@property (nonatomic, assign) id<VGSetupDelegate> setupDelegate;
@property (nonatomic, assign) NSInteger setupSerialNumber;
- (id)initWithDelegate:(id<VGSetupDelegate>)delegate view:(VGView *)view;
- (int)RealtimePlay;
- (int)PTZControl:(int)nCmd;
- (int)DOControl:(int)nTime;
- (int)GetQuality;
- (int)SetQuality:(BYTE)cFps res:(BYTE)cRes bps:(WORD)wBps;
- (int)SetRecordType:(int)nType;
- (int)GetDayInfo;
- (int)GetHourInfo:(DWORD)dwDay;
- (int)GetMinuteInfo:(DWORD)dwDay hour:(int)nHour;
- (int)PlaybackPlay:(DWORD)dwPlaySec;
- (int)Pause;
- (int)Seek:(DWORD)dwPlaySec;
- (int)Resume;
- (int)PlayMotion:(int)nOn;
- (int)PlaySpeed:(char)cPlaySpeed;
- (int)Stop;
- (int)SwitchAudio:(int)nOn;
- (int)SwitchVoice:(int)nOn;
- (int)SendAudioData:(PUCHAR)pData len:(int)nLen;
@end

#pragma mark -

@protocol VGSetupDelegate <NSObject>
- (int)OnGetQuality:	(VGSetup *)setup fps:(BYTE)cFps res:(BYTE)cRes bps:(WORD)wBps;
- (int)OnGetDayInfo:	(VGSetup *)setup days:(NSArray *)tDays;
- (int)OnGetHourInfo:	(VGSetup *)setup day:(DWORD)dwDay hours:(NSArray *)tHours;
- (int)OnGetMinuteInfo:	(VGSetup *)setup day:(DWORD)dwDay hour:(INT)nHour minutes:(NSArray *)tMinutes;
- (int)OnSetupError:	(VGSetup *)setup eno:(int)nErrNo;
@end
