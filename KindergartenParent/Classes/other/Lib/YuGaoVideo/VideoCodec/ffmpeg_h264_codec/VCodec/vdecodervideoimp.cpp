#include "stdafx.h"
#include "vdecodervideoimp.h"
#include "TLibAVCodecKit.h"
//#include "vmemory.h"

namespace vcodec
{
	Vdecoder_video_param_proxy::Vdecoder_video_param_proxy(void)
	{
//		m_pCpuId = NULL;
		m_nThreadNumber = 0;
		m_pAVCodec = NULL;
		m_pAVCtx = NULL;
		m_pFrame = NULL;
		m_pFFBuffer	= NULL;
		m_nFFBufferSize	= 0;

		ff_avcodec_default_get_buffer = avcodec_default_get_buffer;
		ff_avcodec_default_release_buffer = avcodec_default_release_buffer;
		ff_avcodec_default_reget_buffer = avcodec_default_reget_buffer;
//		m_pCpuId = VCreateCpuId();
	}

	Vdecoder_video_param_proxy::~Vdecoder_video_param_proxy(void)
	{
		m_pAVCodec = NULL;
		if(m_pAVCtx != NULL)
		{
			if(m_pAVCtx->intra_matrix){
				free(m_pAVCtx->intra_matrix); 
			}
			if(m_pAVCtx->inter_matrix){
				free(m_pAVCtx->inter_matrix); 
			}
			if(m_pAVCtx->extradata){
				free(m_pAVCtx->extradata); 
			}
			if(m_pFFBuffer){
				free(m_pFFBuffer); 
			}
			if(m_pAVCtx->slice_offset){
				av_freep(&m_pAVCtx->slice_offset); 
			}
			if(m_pAVCtx->codec){
				avcodec_close(m_pAVCtx); 
			}
			m_nThreadNumber = 0;
			av_freep(&m_pAVCtx);
			m_pAVCtx = NULL;
			m_pFFBuffer = NULL;
			m_nFFBufferSize = 0;
		}
		if(m_pFrame != NULL)
		{
			av_freep(&m_pFrame);
			m_pFrame = NULL;
		}
		/*
		if(m_pCpuId != NULL)
		{
			m_pCpuId->Destroy();
			m_pCpuId = NULL;
		}
		*/
	}

	bool Vdecoder_video_param_proxy::Init(CodecID codecID, ULONG codecTag)
	{
		m_pAVCodec = avcodec_find_decoder(codecID);
		if(m_pAVCodec == NULL){
			return false; 
		}
		m_pAVCtx = avcodec_alloc_context3(m_pAVCodec);
		if(m_pAVCtx == NULL){
			return false; 
		}
		m_pFrame = avcodec_alloc_frame();
		if(m_pFrame == NULL){
			return false; 
		}
		m_nThreadNumber = 1;
		m_pAVCtx->width	= 0;
		m_pAVCtx->height = 0;
		m_pAVCtx->codec_tag	= codecTag;
		m_pAVCtx->intra_matrix = (uint16_t*)calloc(sizeof(uint16_t),64);
		m_pAVCtx->inter_matrix = (uint16_t*)calloc(sizeof(uint16_t),64);
		m_pAVCtx->codec_id = codecID;
		m_pAVCtx->workaround_bugs = 1;
		m_pAVCtx->error_concealment = 3;
//		m_pAVCtx->error_recognition = 1;
		m_pAVCtx->idct_algo = 0;
		m_pAVCtx->skip_loop_filter = AVDISCARD_DEFAULT;
//		m_pAVCtx->dsp_mask = AV_CPU_FLAG_FORCE;
		/*
		if(m_pCpuId != NULL){
			m_pAVCtx->dsp_mask |= m_pCpuId->GetFeatures();
		}
		*/
		m_pAVCtx->debug_mv = 0;
		ConnectTo (m_pAVCtx);
		if(avcodec_open2(m_pAVCtx, m_pAVCodec, NULL) < 0){
			return false; 
		}
		avcodec_flush_buffers (m_pAVCtx);	
		return true;
	}

	LONG Vdecoder_video_param_proxy::Decode(vdecoder_video_param *decdata)
	{
		if(decdata == NULL){
			return -1; 
		}
		if(decdata->buf == NULL){
			return -1;
		}
		if(decdata->bufLen <= 0){
			return -1;
		}
        
        /*
		if(decdata->bufLen + FF_INPUT_BUFFER_PADDING_SIZE > m_nFFBufferSize)
		{
			m_nFFBufferSize	= decdata->bufLen + FF_INPUT_BUFFER_PADDING_SIZE;
			m_pFFBuffer	= (PBYTE)realloc(m_pFFBuffer, m_nFFBufferSize);
		}
		memcpy(m_pFFBuffer, decdata->buf, decdata->bufLen);
		memset(m_pFFBuffer+decdata->bufLen, 0, FF_INPUT_BUFFER_PADDING_SIZE);
         */
        
		int got_picture = 0;
		int used_bytes = 0;
		AVPacket avpkt;
		memset(&avpkt, 0, sizeof(avpkt));
		av_init_packet(&avpkt);
		avpkt.data = (uint8_t*)decdata->buf;  // m_pFFBuffer;
		avpkt.size = decdata->bufLen;
		avpkt.flags = AV_PKT_FLAG_KEY;
        used_bytes = avcodec_decode_video2(m_pAVCtx, m_pFrame, &got_picture, &avpkt);
		if(!got_picture){
			return -1;
		}
		if(!m_pFrame->data[0]){
			return -1;
		}			
		decdata->width = m_pAVCtx->width;
		decdata->height = m_pAVCtx->height;
		decdata->linesize[0] = m_pFrame->linesize[0];
		decdata->linesize[1] = m_pFrame->linesize[1];
		decdata->linesize[2] = m_pFrame->linesize[2];
		decdata->linesize[3] = m_pFrame->linesize[3];
		decdata->data[0] = m_pFrame->data[0];
		decdata->data[1] = m_pFrame->data[1];
		decdata->data[2] = m_pFrame->data[2];
		decdata->data[3] = m_pFrame->data[3];
		decdata->format = PFormat2VFormat(static_cast<PixelFormat>(m_pAVCtx->pix_fmt));
		return used_bytes;
	}	

	Vdecoder_video_param::Vdecoder_video_param(CodecID codecID, ULONG codecTag) 
	: m_proxy(NULL),
	  m_bInit(false),
	  m_codecID(codecID),
	  m_codecTag(codecTag)
	{
		m_proxy = NULL;
		m_bInit = true;
		LibAVCodec_Init_Imp();
	}

	Vdecoder_video_param::~Vdecoder_video_param(void)
	{
		delete m_proxy;
		m_proxy = NULL;
	}

	LONG Vdecoder_video_param::Decode(vdecoder_video_param *decdata)
	{
		Reset();
		try
		{
			if(m_proxy != NULL){
				return m_proxy->Decode(decdata); 	
			}				
		}
		catch (...){
			m_bInit = true;
		}			
		return -1;
	}

	void Vdecoder_video_param::Reset(void)
	{
		if(!m_bInit){
			return;
		}
		m_bInit = false;
		delete m_proxy;
		m_proxy = new Vdecoder_video_param_proxy;
		if(m_proxy == NULL){
			return;
		}			
		if(!m_proxy->Init(m_codecID, m_codecTag))
		{
			delete m_proxy;
			m_proxy = NULL;
		}
	}
}