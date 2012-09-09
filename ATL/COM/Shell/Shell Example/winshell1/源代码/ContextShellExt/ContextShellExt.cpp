// ContextShellExt.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f ContextShellExtps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "ContextShellExt.h"

#include "ContextShellExt_i.c"
#include "MenuShellExt.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_MenuShellExt, CMenuShellExt)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_CONTEXTSHELLEXTLib);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        _Module.Term();
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    return (_Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    // registers object, typelib and all interfaces in typelib
	if ( 0 == (GetVersion() & 0x80000000UL) )
	{
        CRegKey reg;
        LONG    lRet;
		
        lRet = reg.Open ( HKEY_LOCAL_MACHINE,
			_T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
			KEY_SET_VALUE );
		
        if ( ERROR_SUCCESS != lRet )
            return E_ACCESSDENIED;
		
        lRet = reg.SetValue ( _T("Menu extension"), 
			_T("{A7275E1B-E709-4484-AFA7-659369738F07}") );
		
        if ( ERROR_SUCCESS != lRet )
            return E_ACCESSDENIED;
	}   
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	if ( 0 == (GetVersion() & 0x80000000UL) )
	{
        CRegKey reg;
        LONG    lRet;
		
        lRet = reg.Open ( HKEY_LOCAL_MACHINE,
			_T("Software\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
			KEY_SET_VALUE );
		
        if ( ERROR_SUCCESS == lRet )
		{
            lRet = reg.DeleteValue ( _T("{A7275E1B-E709-4484-AFA7-659369738F07}") );
		}
		lRet = reg.Open ( HKEY_LOCAL_MACHINE,
			_T("Software"),
			KEY_SET_VALUE );
		if ( ERROR_SUCCESS == lRet )
		{
            lRet = reg.DeleteSubKey(_T("VirtualDesk"));
		}
		
		
	}
    return _Module.UnregisterServer(TRUE);
}


