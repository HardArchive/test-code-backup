// HelloSampleObject.cpp : Implementation of CHelloSampleObject

#include "stdafx.h"
#include "HelloSampleObject.h"


// CHelloSampleObject


STDMETHODIMP CHelloSampleObject::Sumlong(LONG lA, LONG lB, LONG* plSum)
{
	// TODO: Add your implementation code here
	*plSum = lA + lB;
	return S_OK;
}
//
//STDMETHODIMP CHelloSampleObject::get_Description(BSTR* pVal)
//{
//	// TODO: Add your implementation code here
//	//�ͷžɵ��ַ���
//	//SysFreeString(*pVal);
//
//	////�����µ��ַ���
//	//*pVal = SysAllocStringLen(m_bstrDescription, SysStringLen(m_bstrDescription));
//	return S_OK;
//}
//
//STDMETHODIMP CHelloSampleObject::put_Description(BSTR newVal)
//{
//	// TODO: Add your implementation code here
//	////�ͷžɵ��ַ���
//	//SysFreeString(m_bstrDescription);
//
//	////�����µ��ַ���
//	//m_bstrDescription = SysAllocStringLen(newVal, SysStringLen(newVal));
//	return S_OK;
//}
