
#include "vcodecvideo.h"
#include "vdecodervideoimp.h"


extern "C"
{
	vdecodervideohandle * __stdcall vdecoder_video_open(unsigned long decoderid)
	{
		Vdecoder_video_param *pdecoder = NULL;
		try
		{
			switch(decoderid)
			{
			case vdecoder_video_h264:
				pdecoder = new Vdecoder_video_param(CODEC_ID_H264, MKTAG('h','2','6','4'));
				break;
			case vdecoder_video_mpeg4:
				pdecoder = new Vdecoder_video_param(CODEC_ID_MPEG4, MKTAG('x','v','i','d'));
				break;
			default:
				pdecoder = NULL;
				break;
			}
		}
		catch(...){
			pdecoder = NULL;
		}
		return reinterpret_cast<vdecodervideohandle *>(pdecoder);
	}

	long __stdcall vdecoder_video_decode(vdecodervideohandle *handle, vdecoder_video_param *decdata)
	{
		Vdecoder_video_param *pdecoder = reinterpret_cast<Vdecoder_video_param *>(handle);
		if(pdecoder == NULL){
			return -1;
		}		
		return pdecoder->Decode(decdata);
	}

	long __stdcall vdecoder_video_close(vdecodervideohandle *handle)
	{
		Vdecoder_video_param *pdecoder = reinterpret_cast<Vdecoder_video_param *>(handle);
		if(pdecoder == NULL){
			return -1;
		}
		delete pdecoder;
		pdecoder = NULL;
		return 0;
	}

	vdecodervideohandle * __stdcall vencoder_video_open(unsigned long encoderid)
	{
		//UNREFERENCED_PARAMETER(encoderid);
		return NULL;
	}

	long __stdcall vencoder_video_decode(vdecodervideohandle *handle, vencoder_video_param *encodata)
	{
		//UNREFERENCED_PARAMETER(handle);
		//UNREFERENCED_PARAMETER(encodata);
		return -1;
	}

	long __stdcall vencoder_video_close(vdecodervideohandle *handle)
	{
		//UNREFERENCED_PARAMETER(handle);
		return -1;
	}
};
