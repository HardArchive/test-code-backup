// MP3TagViewer.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f MP3TagViewerps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "MP3TagViewer.h"

#include "MP3TagViewer_i.c"
#include "MP3ColExt.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_MP3ColExt, CMP3ColExt)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_MP3TAGVIEWERLib);
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
    // On NT, add our extension to the list of approved extensions.

    if ( 0 == (GetVersion() & 0x80000000) )
        {
        CRegKey reg;
        LONG    lRet;

        lRet = reg.Open ( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS != lRet )
            return E_ACCESSDENIED;

        lRet = reg.SetValue ( _T("MP3 ID3v1 viewer column ext"), 
                              _T("{AC146E80-3679-4BCA-9BE4-E36512573E6C}") );

        reg.Close();

        if ( ERROR_SUCCESS != lRet )
            return E_ACCESSDENIED;
        }

    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    // On NT, remove our extension from the list of approved extensions.

    if ( 0 == (GetVersion() & 0x80000000) )
        {
        CRegKey reg;
        LONG    lRet;

        lRet = reg.Open ( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS == lRet )
            {
            reg.DeleteValue ( _T("{AC146E80-3679-4BCA-9BE4-E36512573E6C}") );
            reg.Close();
            }
        }

    return _Module.UnregisterServer(TRUE);
}


