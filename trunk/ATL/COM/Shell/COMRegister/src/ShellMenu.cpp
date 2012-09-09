// ShellMenu.cpp : Implementation of CShellMenu
#include "stdafx.h"
#include "COMRegister.h"
#include "ShellMenu.h"

#include <atlcom.h>
#include <atlconv.h>
#include <stdio.h>

void LogTrace(LPCTSTR pszFormat, ...)   
{
	va_list pArgs; 
	char szMessageBuffer[16380]={0}; 
	va_start( pArgs, pszFormat );  
	_vsntprintf( szMessageBuffer, 16380, pszFormat, pArgs);
	va_end( pArgs ); 
	OutputDebugString(szMessageBuffer); 
} 

/////////////////////////////////////////////////////////////////////////////
// CShellMenu

HRESULT CShellMenu::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT lpdobj, HKEY hkeyProgID)
{
	if(NULL == lpdobj)
	{
		return E_INVALIDARG;
	}

	FORMATETC feFmtEtc	= { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM mdmSTG	= { TYMED_HGLOBAL };
				
	if(FAILED(lpdobj->GetData(&feFmtEtc, &mdmSTG)))
	{
		return E_INVALIDARG;
	}

	HDROP hDrop = (HDROP)GlobalLock(mdmSTG.hGlobal);
    if(NULL == hDrop)
	{
        return E_INVALIDARG;
	}

	int nDropCount = ::DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
	
	if(nDropCount > 1)
		m_bMultiFile = true;
	else
		m_bMultiFile = false;

	if(nDropCount == 1)
	{
		::DragQueryFile(hDrop, 0, m_FileName, MAX_PATH);
	}

	return S_OK;
}

HRESULT CShellMenu::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
	//if(!(uFlags & CMF_DEFAULTONLY))
	if(!m_bMultiFile)
	{
		HBITMAP hBmp = NULL;

		::InsertMenu(hmenu, indexMenu, MF_BYPOSITION, idCmdFirst++, _T("&Register"));
		hBmp = ::LoadBitmap(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDB_BITMAP_UNREGISTER));
		if(m_hUnRegisterBmp != NULL)
			SetMenuItemBitmaps(hmenu, indexMenu, MF_BYPOSITION, m_hUnRegisterBmp, NULL);

		indexMenu++;
		::InsertMenu(hmenu, indexMenu, MF_BYPOSITION, idCmdFirst++, _T("&UnRegister"));
		if(m_hRegisterBmp != NULL)
			SetMenuItemBitmaps(hmenu, indexMenu, MF_BYPOSITION, m_hRegisterBmp, NULL);

		return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(2));
	}
	return MAKE_HRESULT(SEVERITY_SUCCESS, 0, USHORT(0));
}

HRESULT CShellMenu::GetCommandString( UINT idCmd, UINT uType, UINT *pwReserved, LPSTR pszName, UINT cchMax )
{
	return S_OK;
}

HRESULT CShellMenu::InvokeCommand(LPCMINVOKECOMMANDINFO lpici)
{
    if ( 0 != HIWORD( lpici->lpVerb ))
        return E_INVALIDARG;

    switch ( LOWORD( lpici->lpVerb ))
    {
	case 0:
		COMRegister(m_FileName, true);
		break;
	case 1:
		COMRegister(m_FileName, false);
		break;
    }
	return S_OK;
}

void CShellMenu::COMRegister(char* COMName, bool bRegister)
{
	char strCmdLine[MAX_PATH] = { 0 };

	if(bRegister)
	{

		sprintf(strCmdLine, "regsvr32 \"%s\"", COMName);
		::WinExec((LPSTR)strCmdLine, SW_HIDE);
		//::MessageBox(NULL, strCmdLine, "abce", MB_OK);		
	}
	else
	{
		
		sprintf(strCmdLine, "regsvr32 /u \"%s\"", COMName);
		::WinExec((LPSTR)strCmdLine, SW_HIDE);
		//::MessageBox(NULL, strCmdLine, "abce", MB_OK);
		//LogTrace(strCmdLine);
	}
	LogTrace(strCmdLine);
}