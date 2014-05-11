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
//	//ÊÍ·Å¾ÉµÄ×Ö·û´®
//	//SysFreeString(*pVal);
//
//	////ÉêÇëÐÂµÄ×Ö·û´®
//	//*pVal = SysAllocStringLen(m_bstrDescription, SysStringLen(m_bstrDescription));
//	return S_OK;
//}
//
//STDMETHODIMP CHelloSampleObject::put_Description(BSTR newVal)
//{
//	// TODO: Add your implementation code here
//	////ÊÍ·Å¾ÉµÄ×Ö·û´®
//	//SysFreeString(m_bstrDescription);
//
//	////ÉêÇëÐÂµÄ×Ö·û´®
//	//m_bstrDescription = SysAllocStringLen(newVal, SysStringLen(newVal));
//	return S_OK;
//}
