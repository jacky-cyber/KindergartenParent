#ifndef __VDECODERVIDEOIMP_H__
#define __VDECODERVIDEOIMP_H__

#include "vcodecvideo.h"
#include "TlibavcodecExt.h"

typedef long LONG;
typedef unsigned long ULONG;
typedef unsigned char* PBYTE;

namespace vcodec
{
	class Vdecoder_video_param_proxy : public vcodec::TlibavcodecExt
	{
	private:
		Vdecoder_video_param_proxy(const Vdecoder_video_param_proxy &);
		Vdecoder_video_param_proxy &operator=(const Vdecoder_video_param_proxy &);
	public:
		Vdecoder_video_param_proxy(void);
		virtual ~Vdecoder_video_param_proxy(void);
	public:
		bool Init(CodecID codecID, ULONG codecTag);
		LONG Decode(vdecoder_video_param *decdata);
	private:
//		need edit CPU Flag
//		VCpuId *m_pCpuId;
		LONG m_nThreadNumber;
		AVCodec *m_pAVCodec;
		AVCodecContext *m_pAVCtx;
		AVFrame *m_pFrame;
		PBYTE m_pFFBuffer;
		LONG m_nFFBufferSize;
	};

	class Vdecoder_video_param
	{
	private:
		Vdecoder_video_param(const Vdecoder_video_param &);
		Vdecoder_video_param &operator=(const Vdecoder_video_param &);
	public:
		Vdecoder_video_param(CodecID codecID, ULONG codecTag);
		virtual ~Vdecoder_video_param(void);
		LONG Decode(vdecoder_video_param *decdata);
	private:
		void Reset(void);
	private:
		Vdecoder_video_param_proxy *m_proxy;
		bool m_bInit;
		const CodecID m_codecID;
		const ULONG m_codecTag;
	};
}

using namespace vcodec;

#endif	// __VDECODERIMP_H__
