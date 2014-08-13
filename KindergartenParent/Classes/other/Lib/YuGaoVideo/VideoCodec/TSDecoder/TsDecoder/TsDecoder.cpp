// TsDecoder.cpp: implementation of the CTsDecoder class.
//
//////////////////////////////////////////////////////////////////////

#include "TsDecoder.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*uint8_t* CTsDecoder::m_pBufPoint;*/
/*uint8_t* CTsDecoder::m_pDataPoint;*/
/*uint8_t CTsDecoder::m_szRawStreamBuf[MAX_RAWDATA_LEN];*/
/*uint8_t CTsDecoder::m_szTsStreamBuf[MAX_TSBUFFER_LEN];*/

#ifdef WIN32
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
  #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
#endif

#define VERSION_1	1
#define VERSION_2	2 //增加摄像机时间和录像时间的描述
#define VERSION_3	3 //正装倒装
#define VERSION_4	4 //PES Packet Length
#define POS_VERSION	23
#define POS_CAMTIME_SEC		25
#define POS_CAMTIME_USEC	33
#define POS_RECTIME_SEC		42
#define POS_RECTIME_USEC	50
#define POS_EXINFO			58
#define POS_PES_LENGTH		59

CTsDecoder::CTsDecoder(ITsDecoderSink* pSink, DWORD dwUserData) 
: m_pSink(pSink), m_dwUserData(dwUserData)
{
	ResetBuffer();
}

CTsDecoder::~CTsDecoder()
{
}

void CTsDecoder::ResetBuffer()
{
	m_nVersion = 0;
	m_nPts = m_nLastPts = 0;
	m_nBufIndexA = 0;
	m_nBufIndexV = 0;
	m_IsStart = 0;
	m_nPayloadLen = 0;
	m_eWaitSDT	= F_Dark;
	m_eSDT		= F_Dark;
	m_ePATOrPMT	= F_Dark;
	m_nLastBufLen = 0;
	memset(&m_tCamTime, 0, sizeof(timeval));
	memset(&m_tRecTime, 0, sizeof(timeval));
	m_bExinfo = 0;
	m_pBufPoint = NULL;
	m_pDataPoint = NULL;
}

bool CTsDecoder::DecodeTsStream(unsigned char* pData, int nLen)
{
    TSShowDebug("DecodeTsStream1: m_pSink = %p, nLen:%d, m_nLastBufLen:%d\n",m_pSink, nLen, m_nLastBufLen);

	if (m_pSink == NULL) 
    {
        TSShowDebug("-=-=-=-=--=-=-=-=--=-=-=-=-false 1-=-=-=-=--=-=-=-=--=-=-=-=-\n");
        return false;
    }
    
	int nDataLen = 0, nPid = 0;
	if (!CheckStream(pData, nLen))
    {
        TSShowDebug("-=-=-=-=--=-=-=-=--=-=-=-=-false 2-=-=-=-=--=-=-=-=--=-=-=-=-\n");
        return false;
    }
    
	//point to Ts Stream
	m_pBufPoint = m_szTsStreamBuf;
	m_pDataPoint = m_szTsStreamBuf;
	while (nLen >= TS_PACKET_SIZE) {
		TsPacketType_e eTPType = GetPacketType(m_pBufPoint, nPid);
		Opr_e eOpr = GetOpr(eTPType);
		if (eOpr == Opr_Write) {
			MediaType_e eDataType = (MediaType_e)0;
			if (nPid == H264_PID) eDataType = Media_H264;	//Video
			else if (nPid == GSM_PID) eDataType = Media_Gsm;//audio

			DecTsPacket(m_pBufPoint, nPid, nDataLen);
			bool ret = WriteToBuffer(m_pDataPoint, nDataLen, eDataType);
			if (ret == false) 
			{
				TSShowDebug("-=-=-=-=--=-=-=-=--=-=-=-=-false 3-=-=-=-=--=-=-=-=--=-=-=-=-\n");
				return false;
			}
		} else if (eOpr == Opr_CallBack) {
			if (false == Callback(false)) return false;
			m_eSDT = F_Light;		
		} else if (eOpr == Opr_ToSDT) {
			nLen -= ToNextPacket(nLen); continue;
		}

		DecTsPacket(NULL, nPid, nDataLen);
		nLen -= TS_PACKET_SIZE;

		uint32_t nBufIndex = m_nBufIndexA + m_nBufIndexV;
	//	TSShowDebug("DecodeTsStream2:m_nPayloadLen = %d.nBufIndex = %d.m_nBufIndexA = %d.m_nBufIndexV=%d.\n", 
	//				m_nPayloadLen, nBufIndex, m_nBufIndexA, m_nBufIndexV);

		//Check Data For CallBack
		if ( (m_nPayloadLen-6 <= nBufIndex) && nBufIndex ) {
			if (false == Callback(true)) return false;
		}
	}
    //put the remain data to m_szTsStreamBuf
 	m_nLastBufLen += nLen;
	if (m_nLastBufLen) memcpy(m_szTsStreamBuf, m_pBufPoint, m_nLastBufLen);
	return true;
}

TsPacketType_e CTsDecoder::GetPacketType(uint8_t* pData, int& nPid)
{
	uint8_t *q = pData;
	TsPacketType_e eRet = TP_UN;
    if (*q++ != 0x47) return eRet;
	nPid = ((*q & 0x1f) << 8) | *(q+1);
	switch(nPid) {
	case SDT_PID: eRet = TP_SDT; break;
	case PAT_PID: eRet = TP_PAT; break;
	case PMT_PID: eRet = TP_PMT; break;
	case GSM_PID: eRet = TP_PES; break;
	case H264_PID: eRet = TP_PES; break;
	default: break;
	}
	return eRet;
}

Opr_e CTsDecoder::GetOpr(TsPacketType_e eType)
{
	switch(eType) {
	case TP_SDT:
	//	TSShowDebug("TP_SDT eWaitSDT %d eSDT %d\n", m_eWaitSDT, m_eSDT);
        m_nPayloadLen = 0; m_nBufIndexA = 0; m_nBufIndexV = 0;
		if (m_eWaitSDT == F_Light) m_eWaitSDT = F_Dark;
		if (m_eSDT == F_Gray) return Opr_None;
		if (m_eSDT == F_Dark) {m_eSDT = F_Light; return Opr_None;}
		if (m_eSDT == F_Light) {m_eSDT = F_HighLight; return Opr_CallBack;}
		break;
	case TP_PAT:
	case TP_PMT:
    //	TSShowDebug("TP_PAT TP_PMT, eWaitSDT %d eSDT %d ePATOrPMT %d\n", m_eWaitSDT, m_eSDT, m_ePATOrPMT);
		if (m_eWaitSDT == F_Light) return Opr_ToSDT;
		if (m_eSDT == F_Gray) m_eSDT = F_Light;
		if (m_eSDT == F_Dark) return Opr_None;
		if (m_ePATOrPMT == F_Dark) m_ePATOrPMT = F_Light;
		break;
	case TP_PES:
	//	TSShowDebug("TP_PES, eWaitSDT %d eSDT %d\n", m_eWaitSDT, m_eSDT);
		if (m_eWaitSDT == F_Light) return Opr_ToSDT;
		if (m_eSDT == F_Light) return Opr_Write;
		break;
	default:
	//	TSShowDebug("TP_UN, eWaitSDT %d eSDT %d\n", m_eWaitSDT, m_eSDT);
		if (m_eWaitSDT == F_Light) return Opr_ToSDT;
		if (m_eWaitSDT == F_Dark) {
			m_eWaitSDT = F_Light;
			m_eSDT = F_Gray;
			m_ePATOrPMT = F_Dark;
		}
		return Opr_ToSDT;
	}
	return Opr_None;
}

int CTsDecoder::ToNextPacket(int nMaxLen)
{
	int i = 1;
	uint8_t *q = m_pBufPoint;
	while (1) {
		if ((*(++q) == 0x47) || (++i >= nMaxLen)) {
			m_pBufPoint += i;
	//		TSShowDebug("Skip %dByte\n", i);
			return i;
		}
	}
}

//the function : if (m_nBufIndexA or m_nBufIndexV != 0) indicate have RawData in buffer
//				 then callback the sink 
bool CTsDecoder::Callback(bool bReal)
{
    TSShowDebug("CTsDecoder::Callback(): m_nVersion = %d.\n", m_nVersion);
	if (m_pSink && bReal) {
		if (m_nVersion <= VERSION_2) {
            //1:I frame, 0:P frame
			if (m_ePATOrPMT == F_Light) m_bExinfo = (1<<2);
			else m_bExinfo = 0;
		}
		
		if (m_nBufIndexA) {
			if ((m_nBufIndexA+16) > MAX_RAWDATA_LEN) return false;
			memset(m_szRawAudioBuf+m_nBufIndexA, 0, 16);
			m_pSink->OnGetRawStream(this, m_dwUserData, 
									m_szRawAudioBuf, m_nBufIndexA,
									(m_nPts-m_nLastPts)/90,
									(unsigned char)Media_Gsm, 
									m_tCamTime, m_tRecTime,
									m_bExinfo);
		}
		if (m_nBufIndexV) {
			if ((m_nBufIndexV+16) > MAX_RAWDATA_LEN) return false;
			memset(m_szRawVideoBuf+m_nBufIndexV, 0, 16);
			m_pSink->OnGetRawStream(this, m_dwUserData, 
									m_szRawVideoBuf, m_nBufIndexV, 
									(m_nPts-m_nLastPts)/90,
									(unsigned char)Media_H264,
									m_tCamTime, m_tRecTime,
									m_bExinfo);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	m_nBufIndexA = 0;
	m_nBufIndexV = 0;
	//Clear Flag
	m_eWaitSDT	= F_Dark;
	m_eSDT		= F_Dark;
	m_ePATOrPMT	= F_Dark;
	return true;
}

//pData:the data to write, nDataLen:the data len, eDataType:the data type 0:video/1:audio
//the function : write the raw data to buffer
//				 if video, then write to m_szRawVideoBuf
//				 if audio, then write to m_szRawAudioBuf
bool CTsDecoder::WriteToBuffer(void* pData, int nDataLen, MediaType_e eDataType)
{
	if ((pData == NULL)||(nDataLen < 0)) return false;
	if (nDataLen == 0) return true;
	if (eDataType == Media_H264) {
		if ((m_nBufIndexV+nDataLen) > MAX_RAWDATA_LEN) return false;
		memcpy(m_szRawVideoBuf+m_nBufIndexV, pData, nDataLen);
		m_nBufIndexV += nDataLen;
	} else if (eDataType == Media_Gsm) {
		if ((m_nBufIndexA+nDataLen) > MAX_RAWDATA_LEN) return false;
		memcpy(m_szRawAudioBuf+m_nBufIndexA, pData, nDataLen);
		m_nBufIndexA += nDataLen;
	}
	return true;
}

bool CTsDecoder::CheckStream(uint8_t* pData, int& nLen)
{
	if ( (!pData)||(nLen <= 0) ) return false;
	//the remain data + the pData = the Ts Stream
	if ((m_nLastBufLen + nLen) > MAX_TSBUFFER_LEN) return false;
	memcpy(m_szTsStreamBuf + m_nLastBufLen, pData, nLen);
	nLen += m_nLastBufLen;
	m_nLastBufLen = nLen % TS_PACKET_SIZE;
	nLen = (nLen / TS_PACKET_SIZE) * TS_PACKET_SIZE;
	return true;
}

void CTsDecoder::DecTsPacket(uint8_t* pData, int nPid, int& nDataLen)
{
	switch(nPid) {
	case SDT_PID: DecSDT(); break;
	case PAT_PID: DecPAT(); break;
	case PMT_PID: DecPMT(); break;
	case GSM_PID: if (pData) nDataLen = DecPES(pData, Media_Gsm); break;
	case H264_PID: if (pData) nDataLen = DecPES(pData, Media_H264); break;
	default: break;
	}
}

long CTsDecoder::HexStr2Long(uint8_t* pData)
{
	long lVal = 0;
	long lTmp = 0;
	
	for(int i = 0; i < 8; i++) 
	{
		if(     (pData[i] >= '0') && (pData[i] <= '9')) lTmp = pData[i] - '0';
		else if((pData[i] >= 'a') && (pData[i] <= 'f')) lTmp = pData[i] - 'a' + 10;
		else if((pData[i] >= 'A') && (pData[i] <= 'F')) lTmp = pData[i] - 'A' + 10;

		for(int j = 0; j < 7-i; j++) lTmp *= 16;
		lVal += lTmp;
	}

	return lVal;
}

void CTsDecoder::DecVersion()
{
	uint8_t *q = m_pBufPoint;
	m_nVersion = (int)*(q+POS_VERSION);
	if (m_nVersion >= VERSION_2) {
		m_tCamTime.tv_sec = HexStr2Long(q+POS_CAMTIME_SEC);
		m_tCamTime.tv_usec = HexStr2Long(q+POS_CAMTIME_USEC);
		m_tRecTime.tv_sec = HexStr2Long(q+POS_RECTIME_SEC);
		m_tRecTime.tv_usec = HexStr2Long(q+POS_RECTIME_USEC);
	}
	if (m_nVersion >= VERSION_3) m_bExinfo = *(q+POS_EXINFO);
	if (m_nVersion >= VERSION_4) {
		uint8_t *p = q+POS_PES_LENGTH;
		m_nPayloadLen = (*p << 24) | (*(p+1) << 16) | (*(p+2) << 8) | *(p+3);
	}
}

void CTsDecoder::DecSDT()
{
	DecVersion();
	m_IsStart = 1;
	m_pBufPoint += TS_PACKET_SIZE;
}

void CTsDecoder::DecPAT()
{
	m_pBufPoint += TS_PACKET_SIZE;
}

void CTsDecoder::DecPMT()
{
	m_pBufPoint += TS_PACKET_SIZE;
}

void CTsDecoder::DecPesHeader()
{
    uint8_t *q = m_pDataPoint;
	uint8_t tmp;

	m_nLastPts = m_nPts;

	//packet_start_code_prefix 24bit
	//stream_id 8bit
	for (int i = 0; i < 4; i++) {
		tmp = *q++;
		if (tmp == 0xe0) break;
	}
	//PES_packet_length
	//From ver 4, we decoder Pes Packet length from SDT 
	if(0 == m_nPayloadLen) m_nPayloadLen = (*q << 8) | *(++q);
	else ++q;
	m_nPayloadLen = m_nPayloadLen - 8;
	q++;
	//'10', PES_scrambling_control, PES_priority
	//data_alignment_indicator, copyright, original_or_copy
	if (*q++ != 0x80) return; 
	//flags
    if (*q++ != 0x80) return; 
	//PES_header_data_len
    if (*q++ != 0x05) return; 

	m_nPts = (*q & 0xe) << 29; q++;
	m_nPts |= (*q) << 22; q++;
	m_nPts |= (*q & 0xfe) << 14; q++;
	m_nPts |= (*q) << 7; q++;
	m_nPts |= (*q & 0xfe) >> 1; q++;

    m_pDataPoint = q;
}

int CTsDecoder::FindRealData()
{
	int data_len;
	uint8_t *q = m_pDataPoint;
	int flag = 1;
	
	m_IsStart = 0;
	
	// AUD NAL	
	if ( (q[0]!=0x00)||(q[1]!=0x00)||(q[2]!=0x00)||
		 (q[3]!=0x01)||(q[4]!=0x09)||(q[5]!=0xe0) ) {
		 flag = 0;
	}
	if (flag) q += 6;
	m_pDataPoint = q;
	data_len = TS_PACKET_SIZE - (q - m_pBufPoint);
	return data_len;
}

int CTsDecoder::DecPES(uint8_t* pData, MediaType_e eType)
{	
	uint8_t *q = pData + 3;
	uint8_t val, offset;
	val = *q++;
	if ( m_IsStart||((val>=0x30)&&(val<=0x3f)) ) {
		offset = *q++;
		q += offset;
	}
	m_pDataPoint = q;
	if (m_IsStart) DecPesHeader();
	q = m_pDataPoint;
	/* data len */
	int data_len = TS_PACKET_SIZE - (q - m_pBufPoint);
	if ((m_IsStart)&&(eType == Media_H264)) data_len = FindRealData();
	m_pBufPoint += TS_PACKET_SIZE;
	if ( (data_len<0)||(data_len>184) ) data_len = 0;
	return data_len;
}
