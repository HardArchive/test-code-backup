// SendToClone.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f SendToCloneps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "SendToClone.h"

#include "SendToClone_i.c"
#include "SendToShlExt.h"

#pragma comment(lib, "shlwapi.lib")

CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_SendToShlExt, CSendToShlExt)
END_OBJECT_MAP()

class CSendToCloneApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSendToCloneApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSendToCloneApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CSendToCloneApp, CWinApp)
	//{{AFX_MSG_MAP(CSendToCloneApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CSendToCloneApp theApp;

BOOL CSendToCloneApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_SENDTOCLONELib);

    SetRegistryKey ( _T("Mike's Classy Software") );

    return CWinApp::InitInstance();
}

int CSendToCloneApp::ExitInstance()
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
    // Add ourself to the approved extensions list on NT.
    if ( 0 == (GetVersion() & 0x80000000) )
        {
        CRegKey reg;
        LONG    lRet;

        lRet = reg.Open ( HKEY_LOCAL_MACHINE,
                          _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS != lRet )
            return E_ACCESSDENIED;

        lRet = reg.SetValue ( _T("SendTo Any Folder Clone"),
                              _T("{B7F3240E-0E29-11D4-8D3B-80CD3621FB09}") );

        if ( ERROR_SUCCESS != lRet )
            return E_ACCESSDENIED;
        }

    // Create a file in the SendTo directory so we show up in the menu.
LPITEMIDLIST pidl;
TCHAR        szSendtoPath [MAX_PATH];
HANDLE       hFile;
LPMALLOC     pMalloc;

    if ( SUCCEEDED( SHGetSpecialFolderLocation ( NULL, CSIDL_SENDTO, &pidl )))
        {
        if ( SHGetPathFromIDList ( pidl, szSendtoPath ))
            {
            PathAppend ( szSendtoPath, _T("Some other folder.SendToClone") );

            hFile = CreateFile ( szSendtoPath, GENERIC_WRITE, FILE_SHARE_READ,
                                 NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

            CloseHandle ( hFile );
            }

        if ( SUCCEEDED( SHGetMalloc ( &pMalloc )))
            {
            pMalloc->Free ( pidl );
            pMalloc->Release();
            }
        }

    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    // Remove ourself from the approved extensions list on NT.
    if ( 0 == (GetVersion() & 0x80000000) )
        {
        CRegKey reg;
        LONG    lRet;

        lRet = reg.Open ( HKEY_LOCAL_MACHINE, 
                          _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Shell Extensions\\Approved"),
                          KEY_SET_VALUE );

        if ( ERROR_SUCCESS == lRet )
            {
            reg.DeleteValue ( _T("{B7F3240E-0E29-11D4-8D3B-80CD3621FB09}") );
            reg.Close();
            }
        }

    // Delete the file from the SendTo directory.
LPITEMIDLIST pidl;
TCHAR        szSendtoPath [MAX_PATH];
LPMALLOC     pMalloc;

    if ( SUCCEEDED( SHGetSpecialFolderLocation ( NULL, CSIDL_SENDTO, &pidl )))
        {
        if ( SHGetPathFromIDList ( pidl, szSendtoPath ))
            {
            PathAppend ( szSendtoPath, _T("Some other folder.SendToClone") );

            DeleteFile ( szSendtoPath );
            }

        if ( SUCCEEDED( SHGetMalloc ( &pMalloc )))
            {
            pMalloc->Free ( pidl );
            pMalloc->Release();
            }
        }

    return _Module.UnregisterServer(TRUE);
}
