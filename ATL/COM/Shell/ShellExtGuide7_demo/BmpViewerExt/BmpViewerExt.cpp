// BmpViewerExt.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f BmpViewerExtps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "BmpViewerExt.h"

#include "BmpViewerExt_i.c"
#include "BmpCtxMenuExt.h"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_BmpCtxMenuExt, CBmpCtxMenuExt)
END_OBJECT_MAP()

class CBmpViewerExtApp : public CWinApp
{
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CBmpViewerExtApp)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CBmpViewerExtApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CBmpViewerExtApp, CWinApp)
    //{{AFX_MSG_MAP(CBmpViewerExtApp)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CBmpViewerExtApp theApp;

BOOL CBmpViewerExtApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_BMPVIEWEREXTLib);
    return CWinApp::InitInstance();
}

int CBmpViewerExtApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    return (AfxDllCanUnloadNow()==S_OK && _Module.GetLockCount()==0) ? S_OK : S_FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer()
{
CRegKey reg;
LONG    lRet;
    
    lRet = reg.Open ( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                      KEY_SET_VALUE );

    if ( ERROR_SUCCESS != lRet )
        return HRESULT_FROM_WIN32(lRet);

    lRet = reg.SetValue ( _T("Bitmap thumbnail ctx menu extension"),
                          _T("{D6F469CD-3DC7-408F-BB5F-74A1CA2647C9}") );

    if ( ERROR_SUCCESS != lRet )
        return HRESULT_FROM_WIN32(lRet);

    reg.Close();

    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(false);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer()
{
CRegKey reg;

    if ( ERROR_SUCCESS == 
         reg.Open ( HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                    KEY_SET_VALUE ) )
        {
        reg.DeleteValue ( _T("{D6F469CD-3DC7-408F-BB5F-74A1CA2647C9}") );
        reg.Close();
        }

    return _Module.UnregisterServer(false);
}
