// First_ATL.cpp : CFirst_ATL ��ʵ��

#include "stdafx.h"
#include "First_ATL.h"


// CFirst_ATL


STDMETHODIMP CFirst_ATL::AddNumbers(LONG Num1, LONG Num2, LONG* ReturnVal)
{
	// TODO: �ڴ����ʵ�ִ���

	*ReturnVal = Num1 + Num2;
	return S_OK;
}
