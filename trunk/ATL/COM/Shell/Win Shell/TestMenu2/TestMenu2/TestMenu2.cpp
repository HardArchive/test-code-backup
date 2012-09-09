// TestMenu2.cpp : DLL ������ʵ�֡�

#include "stdafx.h"
#include "resource.h"
#include "TestMenu2.h"

class CTestMenu2Module : public CAtlDllModuleT< CTestMenu2Module >
{
public :
	DECLARE_LIBID(LIBID_TestMenu2Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TESTMENU2, "{C42A502E-1E18-4126-9652-DE3E478773F0}")
};

CTestMenu2Module _AtlModule;


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
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - �����ϵͳע������Ƴ�
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
