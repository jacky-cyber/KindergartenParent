/*
 *  MyTsDecoder.mm
 *  Camera
 *
 *  Created by apple on 11-1-18.
 *  Copyright 2011 HuangSiwen. All rights reserved.
 *
 */

// MyTsDecoder.cpp: implementation of the CMyTsDecoder class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "MyTsDecoder.h"

#define LEN_TIME_STRING     20


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyTsDecoder::CMyTsDecoder()
{
	m_TsDecoder = NULL;
}

CMyTsDecoder::~CMyTsDecoder()
{
    m_callbackObject_OC   = NULL;
    m_callbackSelector_OC_Video = NULL;
    m_callbackSelector_OC_Audio = NULL;
}


//////////////////////////////////////////////////////////////////////
// other Method
//////////////////////////////////////////////////////////////////////

void CMyTsDecoder::Start()
{
    CSafeLock lock(&m_csDecoder);
    
	m_TsDecoder = Reg_TsDecoderHandle(this, 2);
}

void CMyTsDecoder::Stop()
{
    CSafeLock lock(&m_csDecoder);
    
    if (m_TsDecoder != NULL) {
        UnReg_TsDecoderHandle(m_TsDecoder);
        m_TsDecoder = NULL;        
    }
}

void CMyTsDecoder::Decode(unsigned char* pData, int nLen)
{
    CSafeLock lock(&m_csDecoder);
    
    debugPrintf("============CMyTsDecoder::Decode: Decode start~~~ len = %d.============\n", nLen);
    
    if (m_TsDecoder != NULL) {
     	m_TsDecoder->DecodeTsStream(pData, nLen);
    } else {
        debugPrintf("============CMyTsDecoder::Decode: m_TsDecoder == NULL============\n");
    }
    
    debugPrintf("============CMyTsDecoder::Decode: Decode end~~~.============\n");
}

void CMyTsDecoder:: RegCallback_ObjC(NSObject *object, SEL selectorVideo, SEL selectorAudio)
{
    m_callbackObject_OC         = object;
    m_callbackSelector_OC_Video = selectorVideo;
    m_callbackSelector_OC_Audio = selectorAudio;
}

// pData : raw data's first address
// nLen  : raw data's length
// deltaTime : deltaTime(ms) between two frame 
// nFrameType: 1:IFrame, 0:PFrame
// ucMediaType : 1:h264; 2:gsm
// ucExinfo : bit0:Mirror bit1:turndown bit2:KeyFrame
void CMyTsDecoder::OnGetRawStream( ITsDecoder* pHandle, unsigned long ulUsrData,  
                                  unsigned char* pData, int nLen, int deltaTime,
                                  unsigned char ucMediaType,
                                  struct timeval tCamTime, struct timeval tRecTime,
                                  unsigned char ucExinfo)
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

    debugPrintf("----------------------OnGetRawStream start----------------------\n");

    debugPrintf("OnGetRawStream: ucMediaType:%d, nLen:%d, tCamTime:%d, tRecTime:%d, ucExinfo:0x%02x.\n",
                ucMediaType, nLen, tRecTime.tv_sec, tCamTime.tv_sec, ucExinfo);
    
    int nFlipType  = ucExinfo >> 0 & 0x03;
    int nFrameType = ucExinfo >> 2 & 0x01;
    
    NSString *camTime = @"";    
    if (tCamTime.tv_sec != 0) {
        char szCamTime[LEN_TIME_STRING+1] = { 0 };
        TimeSec2Str(tCamTime, szCamTime);
        camTime = [NSString stringWithUTF8String:szCamTime];
    }
    NSString *recTime = @"";
    if (tRecTime.tv_sec != 0) {
        char szRecTime[LEN_TIME_STRING+1] = { 0 };
        TimeSec2Str(tRecTime, szRecTime);
        recTime = [NSString stringWithUTF8String:szRecTime];
    }
    
    NSDictionary *frameInfo = [NSDictionary dictionaryWithObjectsAndKeys:
                               [NSData dataWithBytes:pData length:nLen], kFrameData,
                               [NSNumber numberWithInt:nLen],            kFrameLen,
                               [NSNumber numberWithInt:deltaTime],       kFrameDeltaTime,
                               [NSNumber numberWithInt:nFrameType],      kFrameType,
                               [NSNumber numberWithInt:nFlipType],       kFrameFlipType,
                               camTime,                                  kFrameCamTime,
                               recTime,                                  kFrameRecTime,
                               nil];
    
    // video
    if (ucMediaType == MediaTypeVideo) {
                
        //write2file(pData, nLen);
        
        if ([m_callbackObject_OC respondsToSelector:m_callbackSelector_OC_Video])
        {
            [m_callbackObject_OC performSelector:m_callbackSelector_OC_Video withObject:frameInfo];
        }
    }
    
    // audio
    if (ucMediaType == MediaTypeAudio) {
        
        if ([m_callbackObject_OC respondsToSelector:m_callbackSelector_OC_Audio])
        {
            [m_callbackObject_OC performSelector:m_callbackSelector_OC_Audio withObject:frameInfo];
        }
        
        debugPrintf("~audio~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~audio~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~audio~\n");
   }
    
    debugPrintf("----------------------OnGetRawStream End-----------------------\n");
   [pool release];

}

void CMyTsDecoder::TimeSec2Str(struct timeval& tTime, char* pTime)
{
	struct tm* t2 = gmtime(&tTime.tv_sec);
	snprintf(pTime, LEN_TIME_STRING, "%04d-%02d-%02d %02d:%02d:%02d",
		     t2->tm_year+1900, t2->tm_mon+1,
			 t2->tm_mday, t2->tm_hour,
			 t2->tm_min, t2->tm_sec);
}

void write2file(unsigned char* pData, int nLen)
{
    static FILE *pf = NULL;
    if (!pf) {
        pf = fopen("/Users/apple/test/h264/testnvr.h264", "wb");
    }
        
    int n = fwrite(pData, 1, nLen, pf);
    
    debugPrintf("write2file: write len = %d.\n", n);
    
    //    fclose(filePtr);
}
