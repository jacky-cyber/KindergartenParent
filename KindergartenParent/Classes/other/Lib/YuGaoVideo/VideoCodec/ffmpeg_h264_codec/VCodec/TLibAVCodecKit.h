#ifndef __TLIBAVCODECKIT_H__
#define __TLIBAVCODECKIT_H__

namespace vcodec
{
	void LibAVCodec_Init_Imp(void);
	PixelFormat VFormat2PFormat(unsigned long format);
	unsigned long PFormat2VFormat(PixelFormat format);
}

using namespace vcodec;

#endif	// __TLIBAVCODECKIT_H__
