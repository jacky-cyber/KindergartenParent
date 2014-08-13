// TsDecoderManager.h: interface for the CTsDecoderManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSDECODERMANAGER_H__6B32ECF6_C2DA_464C_B5DD_CC16A239FF39__INCLUDED_)
#define AFX_TSDECODERMANAGER_H__6B32ECF6_C2DA_464C_B5DD_CC16A239FF39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "singleton.h"
#include "TsDecoder.h"

typedef std::set<CTsDecoder*> SET_TSDECODER;

class CTsDecoderManager  
{
	DECLARE_SINGLETON(CTsDecoderManager)
public:
	CTsDecoderManager();
	virtual ~CTsDecoderManager();

	CTsDecoder* GetDecoder(ITsDecoderSink* pSink, DWORD dwUserData);
	bool        DelDecoder(CTsDecoder* pDecoder);

private:
	SET_TSDECODER m_setDecoder;
};
#endif // !defined(AFX_TSDECODERMANAGER_H__6B32ECF6_C2DA_464C_B5DD_CC16A239FF39__INCLUDED_)
