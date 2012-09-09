/*@**#---2003-10-29 21:32:35 (tulip)---#**@

#include "interface.h"  */
#include "math.h"


STDMETHODIMP CMath::QueryInterface(REFIID riid, void **ppv)
{//	��������ʵ��dynamic_cast�Ĺ��ܣ�������dynamic_cast���������ء�
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

	reinterpret_cast<IUnknown *>(*ppv)->AddRef();	//����Ҫ��������Ϊ���ü�������������
	return S_OK;
}

STDMETHODIMP_(ULONG) CMath::AddRef()
{
	return ++m_cRef;
}

STDMETHODIMP_(ULONG) CMath::Release()
{
	ULONG res = --m_cRef;	//��ʹ����ʱ�������޸ĺ�����ü���ֵ��������
	if(res == 0)			//	��Ϊ�ڶ����Ѿ����ٺ������������������ݽ��ǷǷ���
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
