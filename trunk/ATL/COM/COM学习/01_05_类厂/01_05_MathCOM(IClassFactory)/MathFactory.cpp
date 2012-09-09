// MathFactory.cpp: implementation of the CMathFactory class.
//
//////////////////////////////////////////////////////////////////////
#include "math.h"
#include "MathFactory.h"

extern LONG g_cObjectAndLocks;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


STDMETHODIMP_(ULONG) CMathFactory::AddRef(void)
{
	return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CMathFactory::Release(void)
{
	return ::InterlockedDecrement(&m_cRef);
}

STDMETHODIMP CMathFactory::QueryInterface(REFIID riid,void ** ppv)
{
	*ppv=NULL;
	if(riid==IID_IUnknown||riid==IID_IClassFactory)
	{
		*ppv=static_cast<IClassFactory *>(this);
		reinterpret_cast<IUnknown*>(*ppv)->AddRef();
		return S_OK;
	}
	else
		return (*ppv=0),E_NOINTERFACE;
}

STDMETHODIMP CMathFactory::CreateInstance(IUnknown * pUnkOuter,REFIID riid,void ** ppv)
{
	*ppv=NULL;

	//现在不支持聚合
	if(pUnkOuter!=NULL)
		return CLASS_E_NOAGGREGATION;

	CMath * pMath=new CMath;
	if(pMath==NULL)
		return E_OUTOFMEMORY;

	HRESULT hr=pMath->QueryInterface(riid,ppv);

	if(FAILED(hr))
		delete pMath;

	return hr;

}

STDMETHODIMP CMathFactory::LockServer(BOOL fLock)
{
	if(fLock)
		::InterlockedIncrement(&g_cObjectAndLocks);
	else 
		::InterlockedDecrement(&g_cObjectAndLocks);
	return NOERROR;

}

