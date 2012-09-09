// Property.cpp : CMyProperty µÄÊµÏÖ

#include "stdafx.h"
#include "Property.h"
#include ".\property.h"


// CMyProperty


STDMETHODIMP CMyProperty::get_str(BSTR* pVal)
{
	*pVal = m_str.Copy();
	return S_OK;
}

STDMETHODIMP CMyProperty::put_str(BSTR newVal)
{
	m_str = newVal;
	return S_OK;
}

STDMETHODIMP CMyProperty::get_integer(LONG* pVal)
{
	*pVal = m_integer;
	return S_OK;
}

STDMETHODIMP CMyProperty::put_integer(LONG newVal)
{
	m_integer = newVal;
	return S_OK;
}
