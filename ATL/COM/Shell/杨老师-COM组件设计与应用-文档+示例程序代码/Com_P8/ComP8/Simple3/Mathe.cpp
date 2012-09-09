// Mathe.cpp : Implementation of CMathe
#include "stdafx.h"
#include "Simple3.h"
#include "Mathe.h"

/////////////////////////////////////////////////////////////////////////////
// CMathe


STDMETHODIMP CMathe::Add(long n1, long n2, long *pnVal)
{
	*pnVal = n1 + n2;

	return S_OK;
}

STDMETHODIMP CMathe::Cat(BSTR s1, BSTR s2, BSTR *psVal)
{
	CComBSTR s( s1 );
	s.AppendBSTR( s2 );

	*psVal = s.Copy();

	return S_OK;
}

STDMETHODIMP CMathe::Mul(long n1, long n2, long *pnVal)
{
	*pnVal = n1 * n2;

	return S_OK;
}
