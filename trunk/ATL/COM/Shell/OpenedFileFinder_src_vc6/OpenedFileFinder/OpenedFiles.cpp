#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "OpenedFiles.h"

#include "OpenedFiles_i.c"
#include "InterFaceCls.h"


CComModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_InterFaceCls, CInterFaceCls)
END_OBJECT_MAP()

class CChkOpenFileApp : public CWinApp
{
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChkOpenFileApp)
	public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CChkOpenFileApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CChkOpenFileApp, CWinApp)
	//{{AFX_MSG_MAP(CChkOpenFileApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CChkOpenFileApp theApp;

BOOL CChkOpenFileApp::InitInstance()
{
    _Module.Init(ObjectMap, m_hInstance, &LIBID_CHKOPENFILELib);
    return CWinApp::InitInstance();
}

int CChkOpenFileApp::ExitInstance()
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
	OSVERSIONINFO info = { 0 }; 
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&info); 
	if(((info.dwMajorVersion != 5) || (info.dwMinorVersion != 1)) 
        && info.dwMajorVersion != 6 )
	{
		// since i have build tthe driver only for widows xp, i cannot run this on other os versions.
		AfxMessageBox( L"Sorry this can only be installed only in windows XP and Vista" );
		return E_FAIL;
	}
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    return _Module.UnregisterServer(FALSE);
}


