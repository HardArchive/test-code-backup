// ShellMenu.h : Declaration of the CShellMenu

#ifndef __SHELLMENU_H_
#define __SHELLMENU_H_

#include "resource.h"       // main symbols
#include <shlobj.h>
#include <comdef.h>
//
struct __declspec(uuid("000214e4-0000-0000-c000-000000000046")) IContextMenu; _COM_SMARTPTR_TYPEDEF(IContextMenu, __uuidof(IContextMenu)); 
/////////////////////////////////////////////////////////////////////////////
// CShellMenu
class ATL_NO_VTABLE CShellMenu : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShellMenu, &CLSID_ShellMenu>,
	public IDispatchImpl<IShellMenu, &IID_IShellMenu, &LIBID_COMREGISTERLib>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CShellMenu()
	{
		m_hRegisterBmp		= ::LoadBitmap(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDB_BITMAP_REGISTER));
		m_hUnRegisterBmp	= ::LoadBitmap(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDB_BITMAP_UNREGISTER));
	}
	~CShellMenu()
	{
		DeleteObject(m_hRegisterBmp);
		DeleteObject(m_hUnRegisterBmp);
	}

DECLARE_REGISTRY_RESOURCEID(IDR_SHELLMENU)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CShellMenu)
	COM_INTERFACE_ENTRY(IShellMenu)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()

private:

	TCHAR		m_FileName[MAX_PATH];
	bool		m_bMultiFile;
	HBITMAP		m_hRegisterBmp;
	HBITMAP		m_hUnRegisterBmp;

private:

	void COMRegister(char* COMName, bool bRegister = true);

public:

    // IShellExtInit
    STDMETHOD(Initialize)		( LPCITEMIDLIST, LPDATAOBJECT, HKEY );
	
    // IContextMenu
    STDMETHOD(GetCommandString)	( UINT, UINT, UINT*, LPSTR, UINT );
    STDMETHOD(InvokeCommand)	( LPCMINVOKECOMMANDINFO );
    STDMETHOD(QueryContextMenu)	( HMENU, UINT, UINT, UINT, UINT );

};

#endif //__SHELLMENU_H_

//void COMRegister(char* COMName, bool bRegister)
//{
//	CString strCmdLine = "regsvr32 ";
//	if(bRegister)
//	{
//		
//		strCmdLine.Format("regsvr32 \"%s\"",COMName);
//		::WinExec((LPSTR)(LPCTSTR)strCmdLine, SW_HIDE);
//	}
//	else
//	{
//		strCmdLine.Format("regsvr32 /u \"%s\"",COMName);
//		::WinExec((LPSTR)(LPCTSTR)strCmdLine, SW_HIDE);
//	}
//}