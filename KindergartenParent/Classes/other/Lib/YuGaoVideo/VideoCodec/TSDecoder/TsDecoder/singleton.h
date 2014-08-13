// Singleton.h: macro definition to make a class Singleton
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SINGLETON_H__7BC6B67A_01B4_44A8_A74A_ECDF7E723FFA__INCLUDED_)
#define AFX_SINGLETON_H__7BC6B67A_01B4_44A8_A74A_ECDF7E723FFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DECLARE_SINGLETON( CSingleton ) \
private: \
    static CSingleton* m_pInstance; \
    static bool m_bInstantialized; \
public: \
    static CSingleton* Instance() \
{ \
    if ( !m_bInstantialized ) \
{ \
    m_bInstantialized = true; \
    m_pInstance = new CSingleton; \
} \
    return m_pInstance; \
} \
    static void DeleteInstance() \
{ \
	m_bInstantialized = false; \
    delete m_pInstance; \
    m_pInstance = NULL; \
}

#define IMPLEMENT_SINGLETON(CSingleton) \
    bool CSingleton::m_bInstantialized = false; \
CSingleton* CSingleton::m_pInstance = NULL;\

#endif //!defined(AFX_SINGLETON_H__7BC6B67A_01B4_44A8_A74A_ECDF7E723FFA__INCLUDED_)
