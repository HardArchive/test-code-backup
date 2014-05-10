// Section3Demo1.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <oaidl.h>
#include <atlbase.h>
#include <atlcomcli.h>


int _tmain(int argc, _TCHAR* argv[])
{
	//BSTR的使用
	{
		BSTR bstrA = SysAllocString(L"Hello BSTR");
		BSTR bstrB = SysAllocStringLen(bstrA, SysStringLen(bstrA));
		SysFreeString(bstrA);
		SysFreeString(bstrB);
	}

	//用VARIANT保存LONG
	{
		VARIANT var;
		VariantInit(&var);

		var.vt = VT_I4;
		var.lVal = 100;

		VariantClear(&var);

		int i = 0;
		i++;
	}

	//用VARIANT保存FLOAT
	{
		VARIANT var;
		VariantInit(&var);
		
		var.vt = VT_R4;
		var.fltVal = 1.23f;

		VariantClear(&var);
	}

	//用VARIANT保存BSTR
	{
		VARIANT var;
		VariantInit(&var);
		
		var.vt = VT_BSTR;
		var.bstrVal = SysAllocString(L"Hello World");

		VariantClear(&var);
	}

	//用VARIANT保存布尔类型
	{
		VARIANT var;
		VariantInit(&var);

		var.vt = VT_BOOL;
		var.boolVal = VARIANT_FALSE;

		VariantClear(&var);
	}

	//从VARIANT读取相应类型的值
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

	//COM数据类型的转换，LONG转成FLOAT
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

	//COM数据类型的转换，LONG转成BSTR
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

	//CComVariant的构造方法
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

	//CComVariant的赋值方法
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

	//CComVariant与VARIANT的关联
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
		varB.Attach(&varA);		//之后无需调用VariantClear(&varA);

		//Detach
		varC.Detach(&varD);
		VariantClear(&varD);	//需要调用VariantClear(&varD);
	}

	return 0;
}

