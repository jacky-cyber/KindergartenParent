#include "TsDecoderInterface.h"
#include "TsDecoderManager.h"

#if DEBUG
#define TSShowLog    true
#else
#define TSShowLog    false;
#endif // DEBUG

bool g_bTSShowLog = TSShowLog;


DLLFUNC void TsDecoder_Init()
{
}

DLLFUNC void TsDecoder_Final()
{
	CTsDecoderManager::DeleteInstance();
}

DLLFUNC ITsDecoder* Reg_TsDecoderHandle( ITsDecoderSink* pSink, 
										 unsigned long ulUsrData)
{
	return CTsDecoderManager::Instance()->GetDecoder(pSink, ulUsrData);
}

DLLFUNC bool UnReg_TsDecoderHandle(ITsDecoder* pHandle)
{
	return CTsDecoderManager::Instance()->DelDecoder(static_cast<CTsDecoder*>(pHandle));
}

