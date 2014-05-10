// HelloSimpleObject.cpp : CHelloSimpleObject 的实现

#include "stdafx.h"
#include "HelloSimpleObject.h"


// CHelloSimpleObject


STDMETHODIMP CHelloSimpleObject::SumLong(LONG lA, LONG lB, LONG* plSum)
{
	*plSum = lA + lB;

	return S_OK;
}

STDMETHODIMP CHelloSimpleObject::get_Description(BSTR* pVal)
{
	//释放旧的字符串
	SysFreeString(*pVal);

	//申请新的字符串
	*pVal = SysAllocStringLen(m_bstrDescription, SysStringLen(m_bstrDescription));

	return S_OK;
}

STDMETHODIMP CHelloSimpleObject::put_Description(BSTR newVal)
{
	//释放旧的字符串
	SysFreeString(m_bstrDescription);

	//申请新的字符串
	m_bstrDescription = SysAllocStringLen(newVal, SysStringLen(newVal));

	return S_OK;
}
