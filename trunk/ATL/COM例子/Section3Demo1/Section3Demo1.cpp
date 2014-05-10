// Section3Demo1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <oaidl.h>
#include <atlbase.h>
#include <atlcomcli.h>


int _tmain(int argc, _TCHAR* argv[])
{
	//BSTR��ʹ��
	{
		BSTR bstrA = SysAllocString(L"Hello BSTR");
		BSTR bstrB = SysAllocStringLen(bstrA, SysStringLen(bstrA));
		SysFreeString(bstrA);
		SysFreeString(bstrB);
	}

	//��VARIANT����LONG
	{
		VARIANT var;
		VariantInit(&var);

		var.vt = VT_I4;
		var.lVal = 100;

		VariantClear(&var);

		int i = 0;
		i++;
	}

	//��VARIANT����FLOAT
	{
		VARIANT var;
		VariantInit(&var);
		
		var.vt = VT_R4;
		var.fltVal = 1.23f;

		VariantClear(&var);
	}

	//��VARIANT����BSTR
	{
		VARIANT var;
		VariantInit(&var);
		
		var.vt = VT_BSTR;
		var.bstrVal = SysAllocString(L"Hello World");

		VariantClear(&var);
	}

	//��VARIANT���沼������
	{
		VARIANT var;
		VariantInit(&var);

		var.vt = VT_BOOL;
		var.boolVal = VARIANT_FALSE;

		VariantClear(&var);
	}

	//��VARIANT��ȡ��Ӧ���͵�ֵ
	{
		VARIANT var;
		VariantInit(&var);

		var.vt = VT_I4;
		var.lVal = 100;

		if (var.vt == VT_I4)
		{ 
			LONG lValue = var.lVal;
		}
		else if (var.vt == VT_R4)
		{
			FLOAT fValue = var.fltVal;
		}
		else if (var.vt == VT_BSTR)
		{
			BSTR bstrValue = var.bstrVal;
		}
		else if (var.vt == VT_BOOL)
		{
			VARIANT_BOOL varbValue = var.boolVal;
		}

		VariantClear(&var);
	}

	//COM�������͵�ת����LONGת��FLOAT
	{
		VARIANT var;
		VariantInit(&var);

		var.vt = VT_I4;
		var.lVal = 100;

		VariantChangeType(&var,&var,0, VT_R4);

		if (var.vt == VT_R4)
		{
			FLOAT fValue = var.fltVal;
		}

		VariantClear(&var);
	}

	//COM�������͵�ת����LONGת��BSTR
	{
		VARIANT var;
		VariantInit(&var);

		var.vt = VT_I4;
		var.lVal = 100;

		VariantChangeType(&var,&var,0, VT_BSTR);

		if (var.vt == VT_BSTR)
		{
			BSTR fValue = var.bstrVal;
		}

		VariantClear(&var);
	}

	//CComVariant�Ĺ��췽��
	{
		VARIANT varA;
		CComVariant varB;
		CComVariant varC(varA);
		CComVariant varD(varB);
		CComVariant varE(L"Hello CComVariant");
		CComVariant varF("CComVariant");
		CComVariant varG(true);
		CComVariant varH(100L);
		CComVariant varI(1.23f);
		//....

	}

	//CComVariant�ĸ�ֵ����
	{
		VARIANT varA;
		CComVariant varB;
		CComVariant varC;
		varC = varA;
		varC = varB;
		varC = true;
		varC = 100L;
		varC = 1.23f;
		varC = L"Hello CComVariant";
		varC = "Hello CComVariant";
		//....
	}

	//CComVariant��VARIANT�Ĺ���
	{
		VARIANT varA;
		VariantInit(&varA);
		varA.vt = VT_I4;
		varA.lVal = 100;
		
		CComVariant varB;

		CComVariant varC(100L);
		
		VARIANT varD;
		VariantInit(&varD);

		//Attach
		varB.Attach(&varA);		//֮���������VariantClear(&varA);

		//Detach
		varC.Detach(&varD);
		VariantClear(&varD);	//��Ҫ����VariantClear(&varD);
	}

	return 0;
}

