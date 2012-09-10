//////////////////////////////////////////////////////////////////////
//
// SimpleMsgBoxImpl.cpp: implementation of the CSimpleMsgBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimpleMsgBoxImpl.h"
#include "../SimpleMsgBoxComDef.h"         // for __uuidof info

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

CSimpleMsgBoxImpl::CSimpleMsgBoxImpl()
{
    m_uRefCount = 0;
    g_uDllLockCount++;

    TRACE(">>> SimpleMsgBoxSvr: Constructing a CSimpleMsgBoxImpl, DLL ref count = %d\n", g_uDllLockCount );
}

CSimpleMsgBoxImpl::~CSimpleMsgBoxImpl()
{
    g_uDllLockCount--;

    TRACE(">>> SimpleMsgBoxSvr: Destructing a CSimpleMsgBoxImpl, DLL ref count = %d\n", g_uDllLockCount );
}


//////////////////////////////////////////////////////////////////////
// IUnknown methods

STDMETHODIMP_(ULONG) CSimpleMsgBoxImpl::AddRef()
{
    TRACE(">>> SimpleMsgBoxSvr: CSimpleMsgBoxImpl::AddRef(), new ref count = %d\n", m_uRefCount+1 );

    return ++m_uRefCount;               // Increment this object's reference count.
}

STDMETHODIMP_(ULONG) CSimpleMsgBoxImpl::Release()
{
ULONG uRet = --m_uRefCount;             // Decrement this object's reference count.

    TRACE(">>> SimpleMsgBoxSvr: CSimpleMsgBoxImpl::Release(), new ref count = %d\n", m_uRefCount );

    if ( 0 == m_uRefCount )             // Releasing last reference?
        delete this;

    return uRet;
}

STDMETHODIMP CSimpleMsgBoxImpl::QueryInterface ( REFIID riid, void** ppv )
{
HRESULT hrRet = S_OK;

    TRACE(">>> SimpleMsgBoxSvr: In CSimpleMsgBoxImpl::QueryInterface()\n" );

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
    else if ( InlineIsEqualGUID ( riid, __uuidof(ISimpleMsgBox) ))
        {
        *ppv = (ISimpleMsgBox*) this;
        TRACE(">>> SimpleMsgBoxSvr: Client QIed for ISimpleMsgBox\n" );
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
// ISimpleMsgBox methods

STDMETHODIMP CSimpleMsgBoxImpl::DoSimpleMsgBox ( HWND hwndParent, BSTR bsMessageText )
{
_bstr_t bsMsg = bsMessageText;
LPCTSTR szMsg = (TCHAR*) bsMsg;         // Use _bstr_t to convert the string to ANSI if necessary.

    TRACE(">>> SimpleMsgBoxSvr: In CSimpleMsgBoxImpl::DoSimpleMsgBox()\n" );

    MessageBox ( hwndParent, szMsg, _T("Simple Message Box"), MB_OK );

    return S_OK;
}
