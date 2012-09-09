// DispSimple.cpp : CDispSimple ��ʵ��

#include "stdafx.h"
#include "DispSimple.h"
#include ".\dispsimple.h"


// CDispSimple


STDMETHODIMP CDispSimple::Add(VARIANT v1, VARIANT v2, VARIANT* pVal)
{
	::VariantInit( pVal );	// ��Զ��ʼ������ֵ�Ǹ���ϰ��

	CComVariant v_1( v1 );
	CComVariant v_2( v2 );

	if((v1.vt & VT_I4) && (v2.vt & VT_I4) )	// ���������������
	{	// ����Ƚ�û��ʹ�� == ����ʹ��������� & ����֪������Ϊʲô��
		v_1.ChangeType( VT_I4 );	// ת��Ϊ����
		v_2.ChangeType( VT_I4 );	// ת��Ϊ����

		pVal->vt = VT_I4;
		pVal->lVal = v_1.lVal + v_2.lVal;	// �ӷ�
	}
	else
	{
		v_1.ChangeType( VT_BSTR );	// ת��Ϊ�ַ���
		v_2.ChangeType( VT_BSTR );	// ת��Ϊ�ַ���

		CComBSTR bstr( v_1.bstrVal );
		bstr.AppendBSTR( v_2.bstrVal );	// �ַ�������

		pVal->vt = VT_BSTR;
		pVal->bstrVal = bstr.Detach();
	}
	return S_OK;
}

STDMETHODIMP CDispSimple::Upper(BSTR str, BSTR* pVal)
{
	*pVal = NULL;	// ��Զ��ʼ������ֵ�Ǹ���ϰ��

	CComBSTR s(str);
	s.ToUpper();	// ת��Ϊ��д

	*pVal = s.Copy();

	return S_OK;
}
