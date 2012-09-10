//////////////////////////////////////////////////////////////////////
//
// SimpleMsgBoxImpl.h: interface for the CSimpleMsgBoxImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLEMSGBOXIMPL_H__44BE02AE_9619_47AF_8070_4EDA06A1E45C__INCLUDED_)
#define AFX_SIMPLEMSGBOXIMPL_H__44BE02AE_9619_47AF_8070_4EDA06A1E45C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../ISimpleMsgBox.h"           // interface definition

class CSimpleMsgBoxImpl : public ISimpleMsgBox  
{
public:
	CSimpleMsgBoxImpl();
	virtual ~CSimpleMsgBoxImpl();

    // IUnknown
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv);

    // ISimpleMsgBox
    STDMETHOD(DoSimpleMsgBox)(HWND hwndParent, BSTR bsMessageText);

protected:
    ULONG m_uRefCount;
};

#endif // !defined(AFX_SIMPLEMSGBOXIMPL_H__44BE02AE_9619_47AF_8070_4EDA06A1E45C__INCLUDED_)
