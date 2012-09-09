// MathFactory.h: interface for the CMathFactory class.
//
//////////////////////////////////////////////////////////////////////

#ifndef MATHFACTORY_H
#define MATHFACTORY_H
#include <unknwn.h>

class CMathFactory  :public IClassFactory
{
public:
	CMathFactory():m_cRef(0){}

	//IUnknow Method
	STDMETHODIMP QueryInterface(REFIID,void**);
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	//IClassFactory Method
	STDMETHODIMP CreateInstance(IUnknown * ,REFIID ,void **);
	STDMETHODIMP LockServer(BOOL fLock);

protected:
	LONG m_cRef;

};
#endif 