// DispSimple.cpp : CDispSimple 的实现

#include "stdafx.h"
#include "DispSimple.h"
#include ".\dispsimple.h"


// CDispSimple


STDMETHODIMP CDispSimple::Add(VARIANT v1, VARIANT v2, VARIANT* pVal)
{
	::VariantInit( pVal );	// 永远初始化返回值是个好习惯

	CComVariant v_1( v1 );
	CComVariant v_2( v2 );

	if((v1.vt & VT_I4) && (v2.vt & VT_I4) )	// 如果都是整数类型
	{	// 这里比较没有使用 == ，而使用了运算符 & ，你知道这是为什么吗？
		v_1.ChangeType( VT_I4 );	// 转换为整数
		v_2.ChangeType( VT_I4 );	// 转换为整数

		pVal->vt = VT_I4;
		pVal->lVal = v_1.lVal + v_2.lVal;	// 加法
	}
	else
	{
		v_1.ChangeType( VT_BSTR );	// 转换为字符串
		v_2.ChangeType( VT_BSTR );	// 转换为字符串

		CComBSTR bstr( v_1.bstrVal );
		bstr.AppendBSTR( v_2.bstrVal );	// 字符串连接

		pVal->vt = VT_BSTR;
		pVal->bstrVal = bstr.Detach();
	}
	return S_OK;
}

STDMETHODIMP CDispSimple::Upper(BSTR str, BSTR* pVal)
{
	*pVal = NULL;	// 永远初始化返回值是个好习惯

	CComBSTR s(str);
	s.ToUpper();	// 转换为大写

	*pVal = s.Copy();

	return S_OK;
}
