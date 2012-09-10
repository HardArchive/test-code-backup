//////////////////////////////////////////////////////////////////////
//
// SimpleMsgBoxClassFactory.h: interface for the CSimpleMsgBoxClassFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLEMSGBOXCLASSFACTORY_H__E66A448C_57A8_448B_B78D_E86E8A66F098__INCLUDED_)
#define AFX_SIMPLEMSGBOXCLASSFACTORY_H__E66A448C_57A8_448B_B78D_E86E8A66F098__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimpleMsgBoxClassFactory : public IClassFactory
{
public:
    CSimpleMsgBoxClassFactory();
    virtual ~CSimpleMsgBoxClassFactory();

    // IUnknown
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv);

    // IClassFactory
    STDMETHOD(CreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppv);
    STDMETHOD(LockServer)(BOOL fLock);

protected:
    ULONG m_uRefCount;
};

#endif // !defined(AFX_SIMPLEMSGBOXCLASSFACTORY_H__E66A448C_57A8_448B_B78D_E86E8A66F098__INCLUDED_)
