#include "StdAfx.h"
#include "Sink.h"
#include "MFCTest2Section7Demo1Dlg.h"

CSink::CSink(CMFCTest2Section7Demo1Dlg *pDlg)
{
	m_pDlg = pDlg;
	m_ulCount = 1;
}

CSink::~CSink(void)
{
}

HRESULT STDMETHODCALLTYPE CSink::QueryInterface( 
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject)
{
	if (riid == IID_IDispatch || riid == IID_IUnknown || riid == DIID__IHelloSOEEvents)
	{
		*ppvObject = static_cast<IDispatch*>(this);
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	AddRef();
	return S_OK;
}

ULONG STDMETHODCALLTYPE CSink::AddRef( void)
{
	return ++m_ulCount;
}

ULONG STDMETHODCALLTYPE CSink::Release( void)
{
	if (0 == --m_ulCount)
	{
		delete this;
		return 0;
	}

	return m_ulCount;
}

HRESULT STDMETHODCALLTYPE CSink::GetTypeInfoCount( 
	/* [out] */ UINT *pctinfo)
{
	return E_NOTIMPL;		//不实现
}

HRESULT STDMETHODCALLTYPE CSink::GetTypeInfo( 
	/* [in] */ UINT iTInfo,
	/* [in] */ LCID lcid,
	/* [out] */ ITypeInfo **ppTInfo)
{
	return E_NOTIMPL;		//不实现
}

HRESULT STDMETHODCALLTYPE CSink::GetIDsOfNames( 
	/* [in] */ REFIID riid,
	/* [size_is][in] */ LPOLESTR *rgszNames,
	/* [in] */ UINT cNames,
	/* [in] */ LCID lcid,
	/* [size_is][out] */ DISPID *rgDispId)
{
	return E_NOTIMPL;		//不实现
}

HRESULT STDMETHODCALLTYPE CSink::Invoke( 
	/* [in] */ DISPID dispIdMember,
	/* [in] */ REFIID riid,
	/* [in] */ LCID lcid,
	/* [in] */ WORD wFlags,
	/* [out][in] */ DISPPARAMS *pDispParams,
	/* [out] */ VARIANT *pVarResult,
	/* [out] */ EXCEPINFO *pExcepInfo,
	/* [out] */ UINT *puArgErr)
{
	if (dispIdMember == 1)
	{
		if (m_pDlg != NULL)
		{
			m_pDlg->OnAddResult(pDispParams->rgvarg[0].lVal);
		}
	}

	return S_OK;
}