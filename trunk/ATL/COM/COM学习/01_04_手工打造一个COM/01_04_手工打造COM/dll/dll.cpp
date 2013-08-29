// dll.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "dll.h"
#include <objbase.h>
#include <initguid.h>
#include "My.h"
#include "IMy_i.c"

//standard self-registration table
const char * g_RegTable[][3]={
	{"CLSID\\{A738E0FE-EF11-482a-AFE2-1164E4749F25}",0,"SSZ"},
	{"CLSID\\{A738E0FE-EF11-482a-AFE2-1164E4749F25}\\InprocServer32",0,(const char * )-1 /*��ʾ�ļ�����ֵ*/},
	{"CLSID\\{A738E0FE-EF11-482a-AFE2-1164E4749F25}\\ProgID",0,"SSZ.NETFISH.1"},
	{"tulip.MathCOM.1",0,"MathCOM"},
	{"tulip.MathCOM.1\\CLSID",0,"{A738E0FE-EF11-482a-AFE2-1164E4749F25}"},
};
HINSTANCE  g_hinstDll;
STDAPI DllRegisterServer(void);
STDAPI DllUnregisterServer(void);
STDAPI DllGetClassObject(REFCLSID rclsid ,REFIID riid,void **ppv);
STDAPI DllCanUnloadNow(void);


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{

	g_hinstDll=(HINSTANCE)hModule;
	return TRUE;
}

// DllRegisterServer - ������ӵ�ϵͳע���
STDAPI DllRegisterServer(void)
{
	HRESULT hr=S_OK;
	char szFileName [MAX_PATH];
	::GetModuleFileName(g_hinstDll,szFileName,MAX_PATH);

	int nEntries=sizeof(g_RegTable)/sizeof(*g_RegTable);
	for(int i =0;SUCCEEDED(hr)&&i<nEntries;i++)
	{
		const char * pszKeyName=g_RegTable[i][0];
		const char * pszValueName=g_RegTable[i][1];
		const char * pszValue=g_RegTable[i][2];

		if(pszValue==(const char *)-1)
		{
			pszValue=szFileName;
		}

		HKEY hkey;
		long err=::RegCreateKey(HKEY_CLASSES_ROOT,pszKeyName,&hkey);
		if(err==ERROR_SUCCESS)
		{
			err=::RegSetValueEx(hkey,pszValueName,0,REG_SZ,(const BYTE*)pszValue,(strlen(pszValue)+1));
			::RegCloseKey(hkey);
		}
		if(err!=ERROR_SUCCESS)
		{
			::DllUnregisterServer();
			hr=E_FAIL;
		}

	}
	return hr;
}

// DllUnregisterServer - �����ϵͳע������Ƴ�
STDAPI DllUnregisterServer(void)
{
	HRESULT hr=S_OK;
	char szFileName [MAX_PATH];
	::GetModuleFileName(g_hinstDll,szFileName,MAX_PATH);

	int nEntries=sizeof(g_RegTable)/sizeof(*g_RegTable);
	for(int i =0;SUCCEEDED(hr)&&i<nEntries;i++)
	{
		const char * pszKeyName=g_RegTable[i][0];
		long err=::RegDeleteKey(HKEY_CLASSES_ROOT,pszKeyName);
		if(err!=ERROR_SUCCESS)
			hr=S_FALSE;
	}

	return hr;
}

// ����һ���๤���Դ������������͵Ķ���
STDAPI DllGetClassObject(REFCLSID rclsid ,REFIID riid,void **ppv)
{
	static CMy *p=new CMy;
	if(rclsid==CLSID_SSZ)
		return p->QueryInterface(riid,ppv);

	return CLASS_E_CLASSNOTAVAILABLE;
}

// ����ȷ�� DLL �Ƿ���� OLE ж��
STDAPI DllCanUnloadNow(void)
{
	return S_OK;
}


