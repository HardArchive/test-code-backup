// TestMenu3.cpp : DLL ������ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "TestMenu3.h"

class CTestMenu3Module : public CAtlDllModuleT< CTestMenu3Module >
{
public :
	DECLARE_LIBID(LIBID_TestMenu3Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TESTMENU3, "{9FF7D6CF-335E-4F46-8B31-1394E1335772}")
};

CTestMenu3Module _AtlModule;


// DLL ��ڵ�
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}


// ����ȷ�� DLL �Ƿ���� OLE ж��
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// ����һ���๤���Դ������������͵Ķ���
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - ������ӵ�ϵͳע���
STDAPI DllRegisterServer(void)
{
    // ע��������Ϳ�����Ϳ��е����нӿ�
    HRESULT hr = _AtlModule.DllRegisterServer(FALSE);
	return hr;
}


// DllUnregisterServer - �����ϵͳע������Ƴ�
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer(FALSE);
	return hr;
}


