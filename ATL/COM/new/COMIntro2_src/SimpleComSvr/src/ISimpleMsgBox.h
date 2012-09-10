//////////////////////////////////////////////////////////////////////
//
// ISimpleMsgBox interface definition
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISIMPLEMSG_H__E66A448D_57A8_448B_B78D_E86E8A66F098__INCLUDED_)
#define AFX_ISIMPLEMSG_H__E66A448D_57A8_448B_B78D_E86E8A66F098__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ISimpleMsgBox : public IUnknown
{
    // IUnknown
    STDMETHOD_(ULONG, AddRef)() PURE;
    STDMETHOD_(ULONG, Release)() PURE;
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv) PURE;

    // ISimpleMsgBox
    STDMETHOD(DoSimpleMsgBox)(HWND hwndParent, BSTR bsMessageText) PURE;
};

#endif // defined(AFX_ISIMPLEMSG_H__E66A448D_57A8_448B_B78D_E86E8A66F098__INCLUDED_)