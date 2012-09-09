// DirBkgndExt.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f DirBkgndExtps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "DirBkgndExt.h"

#include "DirBkgndExt_i.c"
#include "BkgndCtxMenuExt.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_BkgndCtxMenuExt, CBkgndCtxMenuExt)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_DIRBKGNDEXTLib);
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
CRegKey reg;
LONG    lRet;

    if ( 0 == (GetVersion() & 0x80000000) )
        {    
        lRet = reg.Open ( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS != lRet )
            return HRESULT_FROM_WIN32(lRet);

        lRet = reg.SetValue ( _T("Test directory background ctx menu extension"),
                              _T("{9E5E1445-6CEA-4761-8E45-AA19F654571E}") );

        if ( ERROR_SUCCESS != lRet )
            return HRESULT_FROM_WIN32(lRet);

        reg.Close();
        }

    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
CRegKey reg;

    if ( 0 == (GetVersion() & 0x80000000) )
        {
        if ( ERROR_SUCCESS == 
             reg.Open ( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                        KEY_SET_VALUE ) )
            {
            reg.DeleteValue ( _T("{9E5E1445-6CEA-4761-8E45-AA19F654571E}") );
            reg.Close();
            }
        }

    return _Module.UnregisterServer(TRUE);
}
