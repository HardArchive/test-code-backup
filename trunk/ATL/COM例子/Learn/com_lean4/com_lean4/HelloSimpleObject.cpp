// HelloSimpleObject.cpp : CHelloSimpleObject ��ʵ��

#include "stdafx.h"
#include "HelloSimpleObject.h"


// CHelloSimpleObject


STDMETHODIMP CHelloSimpleObject::Sumlong(LONG lA, LONG lB, LONG* plSum)
{
	// TODO: �ڴ����ʵ�ִ���
	*plSum = lA + lB;
	return S_OK;
}

STDMETHODIMP CHelloSimpleObject::get_Description(BSTR* pVal)
{
	// TODO: �ڴ����ʵ�ִ���
	//�ͷžɵ��ַ���
	SysFreeString(*pVal);

	//�����µ��ַ���
	*pVal = SysAllocStringLen(m_bstrDescription, SysStringLen(m_bstrDescription));
	return S_OK;
}

STDMETHODIMP CHelloSimpleObject::put_Description(BSTR newVal)
{
	// TODO: �ڴ����ʵ�ִ���
	//�ͷžɵ��ַ���
	SysFreeString(m_bstrDescription);

	//�����µ��ַ���
	m_bstrDescription = SysAllocStringLen(newVal, SysStringLen(newVal));
	return S_OK;
}
