// TsDecoderManager.cpp: implementation of the CTsDecoderManager class.
//
//////////////////////////////////////////////////////////////////////

#include "TsDecoderManager.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SINGLETON(CTsDecoderManager)
CTsDecoderManager::CTsDecoderManager()
{

}

CTsDecoderManager::~CTsDecoderManager()
{
	SET_TSDECODER::iterator pos = m_setDecoder.begin();
	for(; m_setDecoder.end() != pos; ++pos)  delete *pos;
	m_setDecoder.clear();
}

CTsDecoder* CTsDecoderManager::GetDecoder( ITsDecoderSink* pSink, 
										   DWORD dwUserData )
{
	if (!pSink) return NULL;
	CTsDecoder* pDecoder = new CTsDecoder(pSink, dwUserData);
	if (!pDecoder) return NULL;
	m_setDecoder.insert(pDecoder);
	return pDecoder;
}

bool CTsDecoderManager::DelDecoder(CTsDecoder* pDecoder)
{
	SET_TSDECODER::iterator pos = m_setDecoder.find(pDecoder);
	if(m_setDecoder.end() != pos) {
		delete pDecoder;
		m_setDecoder.erase(pos);
		return true;
	}
	return false;
}
