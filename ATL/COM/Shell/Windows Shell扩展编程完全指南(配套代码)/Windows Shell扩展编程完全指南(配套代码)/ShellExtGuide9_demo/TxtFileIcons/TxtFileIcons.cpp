// TxtFileIcons.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f TxtFileIconsps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "TxtFileIcons.h"

#include "TxtFileIcons_i.c"
#include "TxtIconShlExt.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_TxtIconShlExt, CTxtIconShlExt)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance, &LIBID_TXTFILEICONSLib);
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
CRegKey key;
LONG lRet;

    // On NT/2K, put our extension in the "approved" list.

    if ( 0 == (GetVersion() & 0x80000000) )
        {
        lRet = key.Open ( HKEY_LOCAL_MACHINE, 
                          _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS == lRet )
            {
            lRet = key.SetValue ( _T("Text file icon extension"), 
                                  _T("{DF4F5AE4-E795-4C12-BC26-7726C27F71AE}") );

            key.Close();
            }

        if ( ERROR_SUCCESS != lRet )
            return HRESULT_FROM_WIN32(lRet);
        }

    // Store the current DefaultIcon so we can restore it when we're 
    // unregistered.  If there already is a backup of the DefaultIcon value,
    // then do nothing.
    // **NOTE**  In production code, you should not hardcode HKCR\txtfile
    // as the key holding info on text files, but instead read the key name
    // from the default value of HKCR\.txt to make your code robust.  I've
    // hard-coded "txtfile" here for simplicity.

    lRet = key.Open ( HKEY_CLASSES_ROOT, _T("txtfile\\DefaultIcon"), 
                      KEY_QUERY_VALUE | KEY_SET_VALUE );

    if ( ERROR_SUCCESS == lRet )
        {
        TCHAR szOldDefIcon [2*MAX_PATH];
        DWORD dwSize = sizeof(szOldDefIcon);
        DWORD dwType;

        // Check if there's already a backup of the DefaultIcon value.
        lRet = key.QueryValue ( szOldDefIcon, _T("OldDefIcon"), &dwSize );

        if ( ERROR_SUCCESS != lRet )
            {
            // The OldDefIcon backup doesn't exist, so save the current value.
            //
            // Note that I'm using the registry APIs directly in order to handle
            // any value type.  On Win2K the DefaultIcon value is of type
            // REG_EXPAND_SZ, which CRegKey doesn't handle.  (It only handles
            // REG_SZ and REG_DWORD.)

            dwSize = sizeof(szOldDefIcon);

            lRet = RegQueryValueEx ( key.m_hKey, NULL, NULL, &dwType,
                                     (LPBYTE) szOldDefIcon, &dwSize );

            if ( ERROR_SUCCESS == lRet )
                {
                RegSetValueEx ( key.m_hKey, _T("OldDefIcon"), 0, dwType, 
                                (const BYTE*) szOldDefIcon, dwSize );
                }
            }

        key.Close();
        }

    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
CRegKey key;
LONG lRet;

    // On NT/2K, remove our extension from the "approved" list.

    if ( 0 == (GetVersion() & 0x80000000) )
        {
        lRet = key.Open ( HKEY_LOCAL_MACHINE, 
                          _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS == lRet )
            {
            key.DeleteValue ( _T("{DF4F5AE4-E795-4C12-BC26-7726C27F71AE}") );
            key.Close();
            }
        }

    // Restore the old DefaultIcon value from the backup we made during
    // registration.
    // **NOTE**  In production code, you should not hardcode HKCR\txtfile
    // as the key holding info on text files, but instead read the key name
    // from the default value of HKCR\.txt to make your code robust.  I've
    // hard-coded "txtfile" here for simplicity.

    lRet = key.Open ( HKEY_CLASSES_ROOT, _T("txtfile\\DefaultIcon"), 
                      KEY_QUERY_VALUE | KEY_SET_VALUE );

    if ( ERROR_SUCCESS == lRet )
        {
        TCHAR szOldDefIcon [2*MAX_PATH];
        DWORD dwSize = sizeof(szOldDefIcon);
        DWORD dwType;

        // Check if there's already a backup of the DefaultIcon value.
        //
        // Note that I'm using the registry APIs directly in order to handle
        // any value type.  On Win2K the DefaultIcon value is of type
        // REG_EXPAND_SZ, which CRegKey doesn't handle.  (It only handles
        // REG_SZ and REG_DWORD.)

        lRet = RegQueryValueEx ( key.m_hKey, _T("OldDefIcon"), NULL, &dwType,
                                 (LPBYTE) szOldDefIcon, &dwSize );

        if ( ERROR_SUCCESS == lRet )
            {
            // The OldDefIcon backup exists, so write the contents back to the
            // default value of the DefaultIcon key...

            RegSetValueEx ( key.m_hKey, NULL, 0, dwType, 
                            (const BYTE*) szOldDefIcon, dwSize );

            // ...and get rid of the backup.

            key.DeleteValue ( _T("OldDefIcon") );
            }

        key.Close();
        }

    return _Module.UnregisterServer(TRUE);
}
