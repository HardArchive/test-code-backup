// TestMenu2.cpp : DLL 导出的实现。

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
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - 将项从系统注册表中移除
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
