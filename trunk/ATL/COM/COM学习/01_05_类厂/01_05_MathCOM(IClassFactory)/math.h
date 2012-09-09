/*@**#---2003-10-29 21:33:44 (tulip)---#**@

#include "interface.h"*/

#include "MathCOM.h"//�����ӵģ����滻����Ķ���

class CMath : public ISimpleMath,
			  public IAdvancedMath
{
private:
	ULONG m_cRef;

private:
	int calcFactorial(int nOp);
	int calcFabonacci(int nOp);

public:
	CMath();
	//IUnknown Method
	STDMETHOD(QueryInterface)(REFIID riid, void **ppv);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	//	ISimpleMath Method
	STDMETHOD (Add)(int nOp1, int nOp2,int * pret);
	STDMETHOD (Subtract)(int nOp1, int nOp2,int *pret);
	STDMETHOD (Multiply)(int nOp1, int nOp2,int *pret);
	STDMETHOD (Divide)(int nOp1, int nOp2,int * pret);

	//	IAdvancedMath Method
	STDMETHOD (Factorial)(int nOp,int *pret);
	STDMETHOD (Fabonacci)(int nOp,int *pret);
};