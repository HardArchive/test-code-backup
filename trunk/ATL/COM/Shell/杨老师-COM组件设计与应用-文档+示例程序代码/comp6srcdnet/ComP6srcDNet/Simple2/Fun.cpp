// Fun.cpp : CFun ��ʵ��

#include "stdafx.h"
#include "Fun.h"
#include ".\fun.h"


// CFun

STDMETHODIMP CFun::Add(LONG n1, LONG n2, LONG* pVal)
{
	VARIANT v; v.vt=VT_BSTR; v.bstrVal=SysAllocString(L"Hello,���"); 
	VARIANT v1; v1.vt = VT_BOOL; v1.boolVal = VARIANT_TRUE;
	*pVal = n1 + n2;

	return S_OK;
}

STDMETHODIMP CFun::Cat(BSTR s1, BSTR s2, BSTR* pVal)
{
/************** ��ȫ�� API ��ʽʵ�ֵ� BSTR �ַ������� ***************
	int nLen1 = ::SysStringLen( s1 );
	int nLen2 = ::SysStringLen( s2 );

	*pVal = ::SysAllocStringLen( s1, nLen1 + nLen2 );
	if( nLen2 )
	{
		::memcpy( *pVal + nLen1, s2, nLen2 * sizeof(WCHAR) );
//		wcscat( *pVal, s2 );	// ��� s2 �а��� L'\0'����ᱻ�ضϡ�
	}

	return S_OK;
*********************************************************************/


//*************** �� CComBSTR ��װ��ʵ�� BSTR �ַ������� *************
	CComBSTR sResult( s1 );
	sResult.AppendBSTR( s2 );

	*pVal = sResult.Copy();		// ������һ������
//	*pVal = sResult.Detach();	// ʹ CComBSTR ������ڲ� BSTR ָ�����롣���ڲ������������ٶ��Կ졣

	return S_OK;
//********************************************************************/
}
