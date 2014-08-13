#ifndef __VCODECVIDEO_H__
#define __VCODECVIDEO_H__

#include "stdafx.h"

extern "C"
{
	//////////////////////////////////////////////////////////////////////////
	const unsigned long vcodec_video_none  = 0x0000;
	const unsigned long vcodec_video_h264  = 0x0100;
	const unsigned long vcodec_video_mpeg4 = 0x0200;

	const unsigned long vdecoder_video_none  = vcodec_video_none;
	const unsigned long vdecoder_video_h264  = vcodec_video_h264;
	const unsigned long vdecoder_video_mpeg4 = vcodec_video_mpeg4;

	struct vdecoder_video_param
	{
		const unsigned char *buf;	/*[in]*/
		long  bufLen;				/*[in]*/
		long  width;				/*[out]*/
		long  height;				/*[out]*/
		long  linesize[4];			/*[out]*/	
		unsigned char *data[4];		/*[out]*/
		unsigned long format;		/*[out]*/
	};
	struct vdecodervideohandle{};
	vdecodervideohandle* vdecoder_video_open(unsigned long decoderid);
	long vdecoder_video_decode(vdecodervideohandle *handle, vdecoder_video_param *decdata);
	long vdecoder_video_close(vdecodervideohandle *handle);

	struct vencoder_video_param
	{

	};
	struct vencodervideohandle{};
	vdecodervideohandle* vencoder_video_open(unsigned long encoderid);
	long vencoder_video_decode(vdecodervideohandle *handle, vencoder_video_param *encodata);
	long vencoder_video_close(vdecodervideohandle *handle);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	const unsigned long vformat_video_none     = 0x0000;
	const unsigned long vformat_video_gbr24p   = 0x0001;	// planar GBR 4:4:4, 24bpp
	const unsigned long vformat_video_yuv444p  = 0x0002;	// planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
	const unsigned long vformat_video_yuv422p  = 0x0003;	// planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
	const unsigned long vformat_video_yuv420p  = 0x0004;	// planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
	//////////////////////////////////////////////////////////////////////////
};

#endif	// __VCODECVIDEO_H__
