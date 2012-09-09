// TestMenu3.cpp : DLL 导出的实现。

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


// DLL 入口点
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}


// 用于确定 DLL 是否可由 OLE 卸载
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// 返回一个类工厂以创建所请求类型的对象
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - 将项添加到系统注册表
STDAPI DllRegisterServer(void)
{
    // 注册对象、类型库和类型库中的所有接口
    HRESULT hr = _AtlModule.DllRegisterServer(FALSE);
	return hr;
}


// DllUnregisterServer - 将项从系统注册表中移除
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer(FALSE);
	return hr;
}


