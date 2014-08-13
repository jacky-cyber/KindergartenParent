#ifndef TS_DECODER_INTERFACE_H
#define TS_DECODER_INTERFACE_H

#ifdef WIN32
#ifdef MPEGTSDECODER_EXPORTS
#define DLLFUNC _declspec(dllexport)
#else
#define DLLFUNC _declspec(dllimport)
#endif
#else
#define DLLFUNC
#endif

#ifndef interface
#define interface               struct
#endif

#ifndef PUREFUC
#define PUREFUC =0
#endif

interface ITsDecoder
{
	virtual void ResetBuffer() PUREFUC;
	virtual bool DecodeTsStream(unsigned char* pData, int nLen) PUREFUC;
};

// pData : raw data's first address
// nLen  : raw data's length
// deltaTime : deltaTime(ms) between two frame 
// nFrameType: 1:IFrame, 0:PFrame
// ucMediaType : 1:h264; 2:gsm
// ucExinfo : bit0:Mirror bit1:turndown bit2:KeyFrame
interface ITsDecoderSink
{
	virtual void OnGetRawStream( ITsDecoder* pHandle, unsigned long ulUsrData,  
								unsigned char* pData, int nLen, int deltaTime,
                                unsigned char ucMediaType,
								struct timeval tCamTime, struct timeval tRecTime,
                                unsigned char ucExinfo 
                                ) PUREFUC;
};

DLLFUNC void TsDecoder_Init();
DLLFUNC void TsDecoder_Final();

DLLFUNC ITsDecoder* Reg_TsDecoderHandle( ITsDecoderSink* pSink, 
										 unsigned long ulUsrData);
DLLFUNC bool UnReg_TsDecoderHandle(ITsDecoder* pHandle);

#endif

