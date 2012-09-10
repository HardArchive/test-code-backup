//////////////////////////////////////////////////////////////////////
//
// SimpleMsgBoxClassFactory.cpp: implementation of the CSimpleMsgBoxClassFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleMsgBoxClassFactory.h"
#include "SimpleMsgBoxImpl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CSimpleMsgBoxClassFactory::CSimpleMsgBoxClassFactory()
{
    m_uRefCount = 0;
    g_uDllLockCount++;

    TRACE(">>> SimpleMsgBoxSvr: Constructing a CSimpleMsgBoxClassFactory, DLL ref count = %d\n", g_uDllLockCount );
}

CSimpleMsgBoxClassFactory::~CSimpleMsgBoxClassFactory()
{
    g_uDllLockCount--;

    TRACE(">>> SimpleMsgBoxSvr: Destructing a CSimpleMsgBoxClassFactory, DLL ref count = %d\n", g_uDllLockCount );
}


//////////////////////////////////////////////////////////////////////
// IUnknown methods

STDMETHODIMP_(ULONG) CSimpleMsgBoxClassFactory::AddRef()
{
    TRACE(">>> SimpleMsgBoxSvr: CSimpleMsgBoxClassFactory::AddRef(), new ref count = %d\n", m_uRefCount+1 );

    return ++m_uRefCount;               // Increment this object's reference count.
}

STDMETHODIMP_(ULONG) CSimpleMsgBoxClassFactory::Release()
{
ULONG uRet = --m_uRefCount;             // Decrement this object's reference count.
    
    TRACE(">>> SimpleMsgBoxSvr: CSimpleMsgBoxClassFactory::Release(), new ref count = %d\n", m_uRefCount );

    if ( 0 == m_uRefCount )             // Releasing last reference?
        delete this;

    return uRet;
}

STDMETHODIMP CSimpleMsgBoxClassFactory::QueryInterface ( REFIID riid, void** ppv )
{
HRESULT hrRet = S_OK;

    TRACE(">>> SimpleMsgBoxSvr: In CSimpleMsgBoxClassFactory::QueryInterface()\n" );

    // Check that ppv really points to a void*.

    if ( IsBadWritePtr ( ppv, sizeof(void*) ))
        return E_POINTER;

    // Standard QI initialization - set *ppv to NULL.

    *ppv = NULL;

    // If the client is requesting an interface we support, set *ppv.

    if ( InlineIsEqualGUID ( riid, IID_IUnknown ))
        {
        *ppv = (IUnknown*) this;
        TRACE(">>> SimpleMsgBoxSvr: Client QIed for IUnknown\n" );
        }
    else if ( InlineIsEqualGUID ( riid, IID_IClassFactory ))
        {
        *ppv = (IClassFactory*) this;
        TRACE(">>> SimpleMsgBoxSvr: Client QIed for IClassFactory\n" );
        }
    else
        {
        hrRet = E_NOINTERFACE;
        TRACE(">>> SimpleMsgBoxSvr: Client QIed for unsupported interface\n" );
        }

    // If we're returning an interface pointer, AddRef() it.

    if ( S_OK == hrRet )
        {
        ((IUnknown*) *ppv)->AddRef();
        }

    return hrRet;
}


//////////////////////////////////////////////////////////////////////
// IClassFactory methods

STDMETHODIMP CSimpleMsgBoxClassFactory::CreateInstance ( IUnknown* pUnkOuter,
                                                         REFIID    riid,
                                                         void**    ppv )
{
HRESULT hrRet;
CSimpleMsgBoxImpl* pMsgbox;

    TRACE(">>> SimpleMsgBoxSvr: In CSimpleMsgBoxClassFactory::CreateInstance()\n" );

    // We don't support aggregation, so pUnkOuter must be NULL.

    if ( NULL != pUnkOuter )
        return CLASS_E_NOAGGREGATION;

    // Check that ppv really points to a void*.

    if ( IsBadWritePtr ( ppv, sizeof(void*) ))
        return E_POINTER;

    *ppv = NULL;

    // Create a new COM object!

    pMsgbox = new CSimpleMsgBoxImpl;

    if ( NULL == pMsgbox )
        return E_OUTOFMEMORY;

    // QI the object for the interface the client is requesting.

    hrRet = pMsgbox->QueryInterface ( riid, ppv );

    // If the QI failed, delete the COM object since the client isn't able
    // to use it (the client doesn't have any interface pointers on the object).

    if ( FAILED(hrRet) )
        delete pMsgbox;

    return hrRet;
}

STDMETHODIMP CSimpleMsgBoxClassFactory::LockServer ( BOOL fLock )
{
    // Increase/decrease the DLL ref count, according to the fLock param.

    fLock ? g_uDllLockCount++ : g_uDllLockCount--;

    TRACE(">>> SimpleMsgBoxSvr: In CSimpleMsgBoxClassFactory::LockServer(), new DLL ref count = %d\n", g_uDllLockCount );
    
    return S_OK;
}
