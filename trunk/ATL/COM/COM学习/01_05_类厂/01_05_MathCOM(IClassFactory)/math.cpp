/*@**#---2003-10-29 21:32:35 (tulip)---#**@

#include "interface.h"  */
#include "math.h"


STDMETHODIMP CMath::QueryInterface(REFIID riid, void **ppv)
{//	这里这是实现dynamic_cast的功能，但由于dynamic_cast与编译器相关。
	if(riid == IID_ISimpleMath)
		*ppv = static_cast<ISimpleMath *>(this);
	else if(riid == IID_IAdvancedMath)
		*ppv = static_cast<IAdvancedMath *>(this);
	else if(riid == IID_IUnknown)
		*ppv = static_cast<ISimpleMath *>(this);
	else {
		*ppv = 0;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();	//这里要这样是因为引用计数是针对组件的
	return S_OK;
}

STDMETHODIMP_(ULONG) CMath::AddRef()
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CMath::Release()
{
	ULONG res = --m_cRef;	//　使用临时变量把修改后的引用计数值缓存起来
	if(res == 0)			//	因为在对象已经销毁后再引用这个对象的数据将是非法的
		delete this;
	return res;
}

STDMETHODIMP CMath::Add(int nOp1, int nOp2,int * pret)
{
	 *pret=nOp1+nOp2;
	 return S_OK;
}

STDMETHODIMP CMath::Subtract(int nOp1, int nOp2,int * pret)
{
	*pret= nOp1 - nOp2;
	return S_OK;
}

STDMETHODIMP CMath::Multiply(int nOp1, int nOp2,int * pret)
{
	*pret=nOp1 * nOp2;
	return S_OK;
}

STDMETHODIMP CMath::Divide(int nOp1, int nOp2,int * pret)
{
	*pret= nOp1 / nOp2;
	return S_OK;
}

int CMath::calcFactorial(int nOp)
{
	if(nOp <= 1)
		return 1;

	return nOp * calcFactorial(nOp - 1);
}

STDMETHODIMP CMath::Factorial(int nOp,int * pret)
{
	*pret=calcFactorial(nOp);
	return S_OK;
}

int CMath::calcFabonacci(int nOp)
{
	if(nOp <= 1)
		return 1;

	return calcFabonacci(nOp - 1) + calcFabonacci(nOp - 2);
}

STDMETHODIMP CMath::Fabonacci(int nOp,int * pret)
{
	*pret=calcFabonacci(nOp);
	return S_OK;
}


CMath::CMath()
{
	m_cRef=0;
}
