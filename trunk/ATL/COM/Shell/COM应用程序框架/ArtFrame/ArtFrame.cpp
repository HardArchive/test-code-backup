// ArtFrame.cpp : Implementation of DLL Exports.

#include "stdafx.h"
#include "resource.h"
#include "..\inc\ArtFrame.h"
#include "..\inc\ArtFrame_i.c"
//求职于《网易软件开发部门》软件工程师或者是产品策划、创意工作。
class CArtFrameModule : public CAtlDllModuleT< CArtFrameModule >
{
public :

	DECLARE_LIBID(LIBID_ArtFrameLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ArtFrame, "{1FBA73ED-4009-410F-A057-F206FCF2E6EE}")
};

CArtFrameModule _AtlModule;
CMessageLoop theLoop;

// DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	//hInstance;
//	_Module.SetResourceInstance(hInstance);
	ATL::_AtlBaseModule.SetResourceInstance(hInstance);
    return _AtlModule.DllMain(dwReason, lpReserved); 
}


// Used to determine whether the DLL can be unloaded by OLE
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// Returns a class factory to create an object of the requested type
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - Adds entries to the system registry
STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
    HRESULT hr = _AtlModule.DllRegisterServer();
	return hr;
}


// DllUnregisterServer - Removes entries from the system registry
STDAPI DllUnregisterServer(void)
{
	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}
