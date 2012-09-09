// Mathe.cpp : CMathe µÄÊµÏÖ

#include "stdafx.h"
#include "Mathe.h"
#include ".\mathe.h"


// CMathe

//IMathe
STDMETHODIMP CMathe::Add(LONG n1, LONG n2, LONG* pnVal)
{
	*pnVal = n1 + n2;

	return S_OK;
}
// IStr
STDMETHODIMP CMathe::Cat(BSTR s1, BSTR s2, BSTR* psVal)
{
	CComBSTR s( s1 );
	s.AppendBSTR( s2 );

	*psVal = s.Copy();

	return S_OK;
}
// IMathe2
STDMETHODIMP CMathe::Mul(LONG n1, LONG n2, LONG* pnVal)
{
	*pnVal = n1 * n2;

	return S_OK;
}
