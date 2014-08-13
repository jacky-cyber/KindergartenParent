#ifndef TS_DATASTRUCT_H
#define TS_DATASTRUCT_H
#ifdef WIN32
#pragma warning(disable: 4786)
#include <stdarg.h>
#include <windows.h>
#endif
#include <stdarg.h>
#include <set>
extern bool g_bTSShowLog;

typedef unsigned long DWORD;
typedef unsigned int  uint32_t;
typedef unsigned char uint8_t;
#ifdef WIN32
typedef LONGLONG            int64_t;
#else
typedef long long			int64_t;
#endif

#define TS_DEF_FPS           5
#define LEN_COM_HEADER       8
#define LEN_TSLOG_ITEM       520
#define TS_PACKET_SIZE	     188
#define MAX_RAWDATA_LEN      5000*TS_PACKET_SIZE
#define MAX_TSBUFFER_LEN     5000*TS_PACKET_SIZE

//frame type
typedef enum {
	UN_FRAME = 0x00,
	I_FRAME	 = 0x01,
	P_FRAME  = 0x02
}FrameType_e;

//media type
typedef enum {
	Media_H264 = 1,
	Media_Gsm
}MediaType_e;

/* pids */
#define PAT_PID                 0x0000
#define PMT_PID                 0x0fff
#define SDT_PID                 0x0011
#define H264_PID				0x0100
#define PCR_PID                 H264_PID
#define GSM_PID                 0x0101

typedef enum {
	TP_UN  = 0x00,
	TP_SDT = 0x01,
	TP_PAT = 0x02,
	TP_PMT = 0x03,
	TP_PES = 0x04
}TsPacketType_e;

/* table ids */
#define PAT_TID   0x00
#define PMT_TID   0x02
#define SDT_TID   0x42

//stream type
#define STREAM_TYPE_VIDEO_MPEG1     0x01
#define STREAM_TYPE_VIDEO_MPEG2     0x02
#define STREAM_TYPE_AUDIO_MPEG1     0x03
#define STREAM_TYPE_AUDIO_MPEG2     0x04
#define STREAM_TYPE_PRIVATE_SECTION 0x05
#define STREAM_TYPE_PRIVATE_DATA    0x06
#define STREAM_TYPE_AUDIO_AAC       0x0f
#define STREAM_TYPE_VIDEO_MPEG4     0x10
#define STREAM_TYPE_VIDEO_H264      0x1b
#define STREAM_TYPE_AUDIO_AC3       0x81
#define STREAM_TYPE_AUDIO_DTS       0x8a


#ifdef WIN32
#define TS_VSNPRINTF		_vsnprintf
#else
#define TS_VSNPRINTF		vsnprintf
#endif

inline void TSShowDebug(const char *szMessage, ...)
{
	if(!g_bTSShowLog) return;
	char buff[LEN_TSLOG_ITEM+1];
	
	va_list var_arg;
	va_start(var_arg,szMessage);
	TS_VSNPRINTF(buff, LEN_TSLOG_ITEM, szMessage, var_arg);
	va_end (var_arg);

	buff[strlen(buff)] = '\0';
#ifdef WIN32
	OutputDebugStringA( buff );
#else
	printf("%s", buff);
#endif
}



#endif



