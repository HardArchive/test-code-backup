// Fun.cpp : CFun 的实现

#include "stdafx.h"
#include "Fun.h"
#include ".\fun.h"


// CFun

STDMETHODIMP CFun::Add(LONG n1, LONG n2, LONG* pVal)
{
	VARIANT v; v.vt=VT_BSTR; v.bstrVal=SysAllocString(L"Hello,你好"); 
	VARIANT v1; v1.vt = VT_BOOL; v1.boolVal = VARIANT_TRUE;
	*pVal = n1 + n2;

	return S_OK;
}

STDMETHODIMP CFun::Cat(BSTR s1, BSTR s2, BSTR* pVal)
{
/************** 完全用 API 方式实现的 BSTR 字符串连接 ***************
	int nLen1 = ::SysStringLen( s1 );
	int nLen2 = ::SysStringLen( s2 );

	*pVal = ::SysAllocStringLen( s1, nLen1 + nLen2 );
	if( nLen2 )
	{
		::memcpy( *pVal + nLen1, s2, nLen2 * sizeof(WCHAR) );
//		wcscat( *pVal, s2 );	// 如果 s2 中包含 L'\0'，则会被截断。
	}

	return S_OK;
*********************************************************************/


//*************** 用 CComBSTR 包装类实现 BSTR 字符串连接 *************
	CComBSTR sResult( s1 );
	sResult.AppendBSTR( s2 );

	*pVal = sResult.Copy();		// 产生另一个副本
//	*pVal = sResult.Detach();	// 使 CComBSTR 对象和内部 BSTR 指针脱离。由于不产生副本，速度稍快。

	return S_OK;
//********************************************************************/
}
