// First_ATL.cpp : CFirst_ATL 的实现

#include "stdafx.h"
#include "First_ATL.h"


// CFirst_ATL


STDMETHODIMP CFirst_ATL::AddNumbers(LONG Num1, LONG Num2, LONG* ReturnVal)
{
	// TODO: 在此添加实现代码

	*ReturnVal = Num1 + Num2;
	return S_OK;
}
