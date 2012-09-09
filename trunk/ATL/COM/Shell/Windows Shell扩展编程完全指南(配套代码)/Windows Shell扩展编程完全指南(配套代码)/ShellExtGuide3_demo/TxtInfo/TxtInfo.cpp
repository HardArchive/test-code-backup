// TxtInfo.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f TxtInfops.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "TxtInfo.h"

#include "TxtInfo_i.c"
#include "TxtInfoShlExt.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_TxtInfoShlExt, CTxtInfoShlExt)
END_OBJECT_MAP()

class CTxtInfoApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTxtInfoApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTxtInfoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CTxtInfoApp, CWinApp)
	//{{AFX_MSG_MAP(CTxtInfoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTxtInfoApp theApp;

BOOL CTxtInfoApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_TXTINFOLib);
    return CWinApp::InitInstance();
}

int CTxtInfoApp::ExitInstance()
{
    _Module.Term();
    return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
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

STDAPI DllRegisterServer(void)
{
    // On NT, add ourself to the list of approved shell extensions.
    if ( 0 == (GetVersion() & 0x80000000) )
        {
        CRegKey reg;
        LONG    lRet;

        lRet = reg.Open ( HKEY_LOCAL_MACHINE, 
                          _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS != lRet )
            return E_ACCESSDENIED;

        lRet = reg.SetValue ( _T("CTxtInfoShlExt extension"),
                              _T("{F4D78AE1-05AB-11D4-8D3B-444553540000}") );

        if ( lRet != ERROR_SUCCESS )
            return E_ACCESSDENIED;
        }


    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    if ( 0 == (GetVersion() & 0x80000000) )
        {
        CRegKey reg;
        LONG    lRet;

        lRet = reg.Open ( HKEY_LOCAL_MACHINE, 
                          _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS == lRet )
            {
            reg.DeleteValue ( _T("{F4D78AE1-05AB-11D4-8D3B-444553540000}") );
            }
        }

    return _Module.UnregisterServer(TRUE);
}
