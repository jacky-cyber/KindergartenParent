//
//  SoundTool.m
//  03.音频工具
//
//  Created by apple on 14-3-11.
//  Copyright (c) 2014年 itcast. All rights reserved.
//

#import "SoundTool.h"
#include <AVFoundation/AVFoundation.h>
#import "NSString+Helper.h"
#import "lame.h"
NSString * const recorderFileName = @"recorder.caf" ;

@interface SoundTool() <AVAudioPlayerDelegate>
{
    AVAudioRecorder     *_recorder;
    AVAudioPlayer       *_player;
    
    NSURL               *_recordURL;        // 录音文件URL
    
    PlayerPlayCompletion    _playCompletionBlock;
    
    NSString *_fileName;//录音文件本地路径
    
    NSString *_fileNm;//文件名称
    
}

@end

@implementation SoundTool
single_implementation(SoundTool)

- (id)init
{
    self = [super init];
    
    if (self) {
        // 准备录音机
        [self prepareRecorder];
    }
    
    return self;
}

#pragma mark - 录音方法
#pragma mark 录音机设置
- (NSDictionary *)recorderSettings
{
    NSMutableDictionary *setting = [[NSMutableDictionary alloc] init];
    
    

    //录音设置
    NSMutableDictionary *settings = [[NSMutableDictionary alloc] init];
    //录音格式 无法使用
    [settings setValue :[NSNumber numberWithInt:kAudioFormatLinearPCM] forKey: AVFormatIDKey];
    //采样率
    [settings setValue :[NSNumber numberWithFloat:11025.0] forKey: AVSampleRateKey];//44100.0
    //通道数
    [settings setValue :[NSNumber numberWithInt:2] forKey: AVNumberOfChannelsKey];
    //线性采样位数
    //[recordSettings setValue :[NSNumber numberWithInt:16] forKey: AVLinearPCMBitDepthKey];
    //音频质量,采样质量
    [settings setValue:[NSNumber numberWithInt:AVAudioQualityMin] forKey:AVEncoderAudioQualityKey];

    
    // 音频格式
    [setting setValue:[NSNumber numberWithInt:kAudioFormatLinearPCM] forKey:AVFormatIDKey];
    // 音频采样率,采样率越高,录制的质量越好,文件越大
    [setting setValue:[NSNumber numberWithFloat:8000.0] forKey:AVSampleRateKey];
    // 音频通道数
    [setting setValue:[NSNumber numberWithInt:1] forKey:AVNumberOfChannelsKey];
    // 线性音频的位深度
    [setting setValue:[NSNumber numberWithInt:8]forKey:AVLinearPCMBitDepthKey];
    
    return settings;
}

#pragma mark 准备录音机
- (void)prepareRecorder
{
    // 1. 设置声音回话的类别,允许录音
    AVAudioSession *session = [AVAudioSession sharedInstance];
    [session setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
    // 激活回话类别
    [session setActive:YES error:nil];
    
    // 2. 设置录音机
    //NSString* path = [[NSBundle mainBundle] pathForResource:@"gaoxiao" ofType:@"caf"];
    _fileNm = [@"" appendDateTime];
    _fileName = [NSString stringWithFormat:@"%@.caf",_fileNm];
    //_fileName = path;
    _recordURL = [_fileName  appendToDocumentURL];
    _recorder = [[AVAudioRecorder alloc] initWithURL:_recordURL settings:[self recorderSettings] error:nil];
    
    // 准备录音
    [_recorder prepareToRecord];
}

#pragma mark - 成员方法
- (void)startRecord
{
    // 如果当前正在录音
    if (_recorder.isRecording) {
        return;
    }
    [_recorder record];
}

- (void)stopRecord
{
    // 如果正在录音,才需要停止
    if (_recorder.isRecording) {
        _currentTime = _recorder.currentTime;
        [self audio_PCMtoMP3];
        [_recorder stop];
    }
}

#pragma mark - 播放的方法
#pragma mark 准备播放器
- (AVAudioPlayer *)preparePlayer
{
    AVAudioPlayer *player = [[AVAudioPlayer alloc] initWithContentsOfURL:_recordURL error:nil];
    
    player.numberOfLoops = 0;
    [player prepareToPlay];
    
    player.delegate = self;
    
    return player;
}

#pragma mark 开始播放
- (void)startPlayWithCompletin:(PlayerPlayCompletion)completion
{
    _playCompletionBlock = completion;
    
    // 如果正在录音,或者正在播放
    if (_recorder.isRecording || _player.isPlaying) {
        return;
    }
    
    _player = [self preparePlayer];
    [_player play];
}

#pragma mark - 播放器代理方法
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{    
    if (_playCompletionBlock != nil) {
        _playCompletionBlock();
    }
}
#pragma mark -返回文件的路径
-(NSString *)recordPath
{
    return [[NSString stringWithFormat:@"%@",_fileName] documentsPath];
}
#pragma mark -返回文件的路径
-(NSString *)mp3Path
{
    return [[NSString stringWithFormat:@"%@.mp3",_fileNm] documentsPath];
}
-(NSString *)fileName
{
    return [NSString stringWithFormat:@"%@.mp3",_fileNm];
}
-(NSInteger)soundTimeLong
{
    return [[NSString stringWithFormat:@"%f",_player.duration] integerValue];
}
- (void)audio_PCMtoMP3
{
    NSString *cafFilePath = [self recordPath];
    
    if (_currentTime <1.0) {
        NSFileManager* fileManager=[NSFileManager defaultManager];
        [fileManager removeItemAtPath:cafFilePath error:nil];
        return;
    }
    
    
    NSString *mp3FilePath = [[NSString stringWithFormat:@"%@.mp3",_fileNm] documentsPath];
    
    @try {
        int read, write;
        
        FILE *pcm = fopen([cafFilePath cStringUsingEncoding:1], "rb");  //source 被转换的音频文件位置
        fseek(pcm, 4*1024, SEEK_CUR);                                   //skip file header
        FILE *mp3 = fopen([mp3FilePath cStringUsingEncoding:1], "wb");  //output 输出生成的Mp3文件位置
        
        const int PCM_SIZE = 8192;
        const int MP3_SIZE = 8192;
        short int pcm_buffer[PCM_SIZE*2];
        unsigned char mp3_buffer[MP3_SIZE];
        
        lame_t lame = lame_init();
        lame_set_in_samplerate(lame, 11025.0);
        lame_set_VBR(lame, vbr_default);
        lame_init_params(lame);
        
        do {
            read = fread(pcm_buffer, 2*sizeof(short int), PCM_SIZE, pcm);
            if (read == 0)
                write = lame_encode_flush(lame, mp3_buffer, MP3_SIZE);
            else
                write = lame_encode_buffer_interleaved(lame, pcm_buffer, read, mp3_buffer, MP3_SIZE);
            
            fwrite(mp3_buffer, write, 1, mp3);
            
        } while (read != 0);
        
        lame_close(lame);
        fclose(mp3);
        fclose(pcm);
    }
    @catch (NSException *exception) {
        NSLog(@"%@",[exception description]);
    }
    @finally {
        
        NSFileManager* fileManager=[NSFileManager defaultManager];
        [fileManager removeItemAtPath:cafFilePath error:nil];
        
    }
}


@end
