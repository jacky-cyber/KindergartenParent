#include "stdafx.h"
#include "TLibAVCodecKit.h"
#include "vcodecvideo.h"

namespace vcodec
{
	void LibAVCodec_Init_Imp(void)
	{
//		need edit lock		
//		VLockAuto autolock(g_lock);
		static bool bInit = false;
		if(!bInit)
		{ 
			bInit = true;
			avcodec_register_all();
		}
	}

	PixelFormat VFormat2PFormat(unsigned long format)
	{
		switch(format)
		{
		case vformat_video_gbr24p:
//			return PIX_FMT_GBR24P;
            return PIX_FMT_RGB24;    
		case vformat_video_yuv444p:
			return PIX_FMT_YUV444P;
		case vformat_video_yuv422p:
			return PIX_FMT_YUV422P;
		case vformat_video_yuv420p:
			return PIX_FMT_YUV420P;
		default:
			return PIX_FMT_NONE;
		}
	}

	unsigned long PFormat2VFormat(PixelFormat format)
	{
		switch(format)
		{
//		case PIX_FMT_GBR24P:
        case PIX_FMT_RGB24:
			return vformat_video_gbr24p;
		case PIX_FMT_YUV444P:
		case PIX_FMT_YUVJ444P:
			return vformat_video_yuv444p;
		case PIX_FMT_YUV422P:
		case PIX_FMT_YUVJ422P:
			return vformat_video_yuv422p;
		case PIX_FMT_YUV420P:
		case PIX_FMT_YUVJ420P:
			return vformat_video_yuv420p;
		default:			
			return vformat_video_none;
		}
	}
}
