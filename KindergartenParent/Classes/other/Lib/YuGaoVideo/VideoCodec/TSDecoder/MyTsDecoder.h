/*
 *  MyTsDecoder.h
 *  Camera
 *
 *  Created by apple on 11-1-18.
 *  Copyright 2011 HuangSiwen. All rights reserved.
 *
 */

// MyTsDecoder.h: interface for the CMyTsDecoder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEST_H__11C4D6EA_B77B_4B64_8823_22CA581969A8__INCLUDED_)
#define AFX_TEST_H__11C4D6EA_B77B_4B64_8823_22CA581969A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DebugLog.h"
#include "SafeLock.h"
#include "TsDecoderInterface.h"

#define kFrameData          @"kFrameData"
#define kFrameLen           @"kFrameLen"
#define kFrameType          @"kFrameType"
#define kFrameFlipType      @"kFrameFlipType"
#define kFrameDeltaTime     @"kFrameDeltaTime"
#define kFrameCamTime       @"kFrameCamTime"
#define kFrameRecTime       @"kFrameRecTime"

enum {
    MediaTypeVideo = 1,
    MediaTypeAudio = 2,
};
typedef unsigned char MediaType_t;


class CMyTsDecoder : public ITsDecoderSink  
{
public:
	CMyTsDecoder();
	virtual ~CMyTsDecoder();

    virtual void OnGetRawStream( ITsDecoder* pHandle, unsigned long ulUsrData,  
								unsigned char* pData, int nLen, int deltaTime,
                                unsigned char ucMediaType,
								struct timeval tCamTime, struct timeval tRecTime,
                                unsigned char ucExinfo);

	void Start();
	void Stop();
	void Decode(unsigned char* pData, int nLen);
    
    void RegCallback_ObjC(NSObject *object, SEL selectorVideo, SEL selectorAudio);

private:
    void TimeSec2Str(struct timeval& tTime, char* pTime);

private:
	ITsDecoder* m_TsDecoder;
    CriticalSection m_csDecoder;
    
    NSObject *m_callbackObject_OC;
    SEL       m_callbackSelector_OC_Video;
    SEL       m_callbackSelector_OC_Audio;

};

void write2file(unsigned char* pData, int nLen);

#endif // !defined(AFX_TEST_H__11C4D6EA_B77B_4B64_8823_22CA581969A8__INCLUDED_)
