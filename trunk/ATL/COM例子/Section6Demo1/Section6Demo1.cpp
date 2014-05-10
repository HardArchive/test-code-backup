// Section6Demo1.cpp : DLL 导出的实现。


#include "stdafx.h"
#include "resource.h"
#include "Section6Demo1.h"


class CSection6Demo1Module : public CAtlDllModuleT< CSection6Demo1Module >
{
public :
	DECLARE_LIBID(LIBID_Section6Demo1Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_SECTION6DEMO1, "{13A4C392-7169-436D-9CB7-ABD39268311C}")
};

CSection6Demo1Module _AtlModule;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL 入口点
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




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

