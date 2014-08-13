// TsDecoder.h: interface for the CTsDecoder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSDECODER_H__C30C5E50_E687_438A_8CD2_58F235D21BA9__INCLUDED_)
#define AFX_TSDECODER_H__C30C5E50_E687_438A_8CD2_58F235D21BA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TsDecoderInterface.h"
#include "TSDataStruct.h"
#ifdef WIN32
#include <winsock.h>
#else
#include <sys/time.h>
#endif
enum Opr_e {Opr_None, Opr_Write, Opr_CallBack, Opr_ToSDT};
enum FlagType_e {F_Dark, F_Gray, F_Light, F_HighLight};

class CTsDecoder : public ITsDecoder  
{
public:
	CTsDecoder(ITsDecoderSink* pSink, DWORD dwUserData);
	virtual ~CTsDecoder();

	//ITsDecoder
	virtual void ResetBuffer();
	virtual bool DecodeTsStream(unsigned char* pData, int nLen);

private:
	void DecTsPacket(uint8_t* pData, int nPid, int& nDataLen);
	void DecSDT();
	void DecPAT();
	void DecPMT();
	int  DecPES(uint8_t* pData, MediaType_e eType);
	void DecPesHeader();

	int  ToNextPacket(int nMaxLen);
	TsPacketType_e GetPacketType(uint8_t* pData, int& nPid);
	Opr_e GetOpr(TsPacketType_e eType);
	bool CheckStream(uint8_t* pData, int& nLen);
	int  FindRealData();
	bool WriteToBuffer(void* pSrc, int nLen, MediaType_e nDataType);
	bool Callback(bool bReal);
	void DecVersion();
	long HexStr2Long(uint8_t* pData);
private:
	DWORD m_dwUserData;

	int		m_nVersion;
	int64_t m_nPts, m_nLastPts;
	uint32_t m_nBufIndexA;
	uint32_t m_nBufIndexV;
	int m_IsStart;
	int m_nPayloadLen;
	FlagType_e m_eWaitSDT;
	FlagType_e m_eSDT;
	FlagType_e m_ePATOrPMT;
	int m_nLastBufLen;//last remain data len : <188
	ITsDecoderSink* m_pSink;
	struct timeval m_tCamTime;
	struct timeval m_tRecTime;
	uint8_t m_bExinfo;

	/*static*/ uint8_t* m_pBufPoint;
	/*static*/ uint8_t* m_pDataPoint;
	/*static*/ uint8_t m_szRawVideoBuf[MAX_RAWDATA_LEN];
	/*static*/ uint8_t m_szRawAudioBuf[MAX_RAWDATA_LEN];
	/*static*/ uint8_t m_szTsStreamBuf[MAX_TSBUFFER_LEN];
};
#endif // !defined(AFX_TSDECODER_H__C30C5E50_E687_438A_8CD2_58F235D21BA9__INCLUDED_)
