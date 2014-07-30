//
//  SoundTool.h
//  03.音频工具
//
//  Created by apple on 14-3-11.
//  Copyright (c) 2014年 itcast. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "Singleton.h"

typedef void(^PlayerPlayCompletion)();

@interface SoundTool : NSObject
single_interface(SoundTool)

// 录音的当前时间
@property (nonatomic, assign) NSTimeInterval currentTime;

// 开始录音
- (void)startRecord;
// 停止录音
- (void)stopRecord;
// 开始播放

-(NSString*)recordPath;
-(NSString *)mp3Path;

-(NSString*)fileName;
-(NSInteger)soundTimeLong;
- (void)startPlayWithCompletin:(PlayerPlayCompletion)completion;

@end
