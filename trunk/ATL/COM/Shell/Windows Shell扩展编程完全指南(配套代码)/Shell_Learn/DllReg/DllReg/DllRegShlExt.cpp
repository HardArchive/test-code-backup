// DllRegShlExt.cpp : CDllRegShlExt ��ʵ��

#include "stdafx.h"
#include "DllRegShlExt.h"

#include "dllmain.h"
// CDllRegShlExt
#include "ProgressDlg.h"
CDllRegShlExt::CDllRegShlExt()
{
	//add by ray 2014-05-13 11:22
	m_hRegBmp = LoadBitmap ( _Module.GetModuleInstance(),
		MAKEINTRESOURCE(IDB_REGISTERBMP) );
	m_hUnregBmp = LoadBitmap ( _Module.GetModuleInstance(),
		MAKEINTRESOURCE(IDB_UNREGISTERBMP) );
}

HRESULT CDllRegShlExt::Initialize ( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID )
{
	TCHAR szFile [MAX_PATH];
	TCHAR szFolder [MAX_PATH];
	TCHAR szCurrDir [MAX_PATH];
	TCHAR* pszLastBackslash;
	UINT uNumFiles;
	HDROP hdrop;
	FORMATETC etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
	STGMEDIUM stg = { TYMED_HGLOBAL };
	HINSTANCE hinst;
	bool bChangedDir = false;
	HRESULT (STDAPICALLTYPE* pfn)();

	// �����ݶ����ж�ȡ�ļ��б�. ���Ǵ洢��HDROP ��ʽ��
	// ��ˣ�ȡ�� HDROP �������ʹ���Ϸ�API
	if ( FAILED( pDataObj->GetData (&etc, &stg)))
		return E_INVALIDARG;

	//ȡ��HDROP ���.
	hdrop = (HDROP)GlobalLock ( stg.hGlobal );
	if ( NULL == hdrop )
	{
		ReleaseStgMedium ( &stg );
		return E_INVALIDARG;
	}

	// ����ڸò������м����ļ���ѡ��.
	uNumFiles = DragQueryFile ( hdrop, 0xFFFFFFFF, NULL, 0 );

	for ( UINT uFile = 0; uFile < uNumFiles; uFile++ )
	{
		//ȡ����һ���ļ���.
		if ( 0 == DragQueryFile ( hdrop, 
			uFile, szFile, MAX_PATH ))
			continue;
		//���ż����ļ�.
		hinst = LoadLibrary ( szFile ); 
		if ( NULL == hinst)
			continue; 

		// ��ȡ DllRegisterServer()�����ĵ�ַ;
		(FARPROC&) pfn = GetProcAddress ( hinst, "DllRegisterServer" );
		// ���û����, �������ļ�.
		if ( NULL == pfn )
		{
			FreeLibrary ( hinst );
			continue;
		}
		// ��ȡDllUnregisterServer()�����ĵ�ַ;
		(FARPROC&) pfn = GetProcAddress ( hinst, "DllUnregisterServer" );
		// ����У����ǾͿ��Դ�����ļ�, �������䵽���ǵ��ļ����б���ȥ(m_lsFiles).
		if ( NULL != pfn )
		{
			m_lsFiles.push_back ( szFile );
		}
		FreeLibrary ( hinst );
	} // end for 

	// �ͷ���Դ.
	GlobalUnlock ( stg.hGlobal );
	ReleaseStgMedium ( &stg );
	// ��������ҵ����Բ������ļ�, ���� S_OK.
	// ����,����E_INVALIDARG�����ǵĴ���Ͳ����ٱ��һ��¼����á�
	return ( m_lsFiles.size() > 0 ) ? S_OK : E_INVALIDARG;
}


HRESULT CDllRegShlExt::QueryContextMenu ( HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags )
{
	UINT uCmdID = uidFirstCmd; 

	// ��� CMF_DEFAULTONLY ��־���������ǲ����κβ���. 
	if ( uFlags & CMF_DEFAULTONLY ) 
	{
		return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
	}

	// ��� register/unregister ��.
	InsertMenu (hmenu, uMenuIndex,MF_STRING | MF_BYPOSITION, uCmdID++, _T("Register server(s)"));

	// Ϊregister������λͼ.
	if ( NULL != m_hRegBmp )
	{
		SetMenuItemBitmaps ( hmenu, uMenuIndex, MF_BYPOSITION, m_hRegBmp, NULL );
	}

	uMenuIndex++;

	InsertMenu (hmenu,uMenuIndex,MF_STRING | MF_BYPOSITION, uCmdID++, _T("Unregister server(s)"));

	// ���� unregister ���λͼ.
	if ( NULL != m_hUnregBmp )
	{
		SetMenuItemBitmaps ( hmenu, uMenuIndex, MF_BYPOSITION, m_hUnregBmp, NULL
			);
	}

	//���, ���Ǹ����������������˼����˵���.
	return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 2 );
}


#include <atlconv.h>
HRESULT CDllRegShlExt::GetCommandString ( UINT uCmdID, UINT uFlags,              UINT* puReserved, LPSTR szName, UINT cchMax )
{
	LPCTSTR szPrompt;
	USES_CONVERSION; 
	if ( uFlags & GCS_HELPTEXT )
	{
		switch ( uCmdID ) 
		{ 
		case 0: szPrompt = _T("Register all selected COM servers");
			break; 
		case 1: szPrompt = _T("Unregister all selected COM servers"); 
			break; 
		default:  return E_INVALIDARG; 
			break; 
		}

		// ���������ַ������ṩ�Ļ�������. ���Shell��ҪUnicode�ַ�����������Ҫת��szName �� LPCWSTR.
		if ( uFlags & GCS_UNICODE )
		{
			lstrcpynW ( (LPWSTR) szName, T2CW(szPrompt), cchMax );
		}
		else
		{
			lstrcpynA ( szName, T2CA(szPrompt), cchMax );
		}
	}
	else if ( uFlags & GCS_VERB )
	{
		// Copy the verb name into the supplied buffer.  If the shell wants
		// a Unicode string, we need to case szName to an LPCWSTR.

		switch ( uCmdID )
		{
		case 0:
			if ( uFlags & GCS_UNICODE )
			{
				lstrcpynW ( (LPWSTR) szName, L"DllRegSvr", cchMax );
			}
			else
			{
				lstrcpynA ( szName, "DllRegSvr", cchMax );
			}
			break;

		case 1:
			if ( uFlags & GCS_UNICODE )
			{
				lstrcpynW ( (LPWSTR) szName, L"DllUnregSvr", cchMax );
			}
			else
			{
				lstrcpynA ( szName, "DllUnregSvr", cchMax );
			}
			break;

		default:
			ATLASSERT(0);           // should never get here
			return E_INVALIDARG;
			break;
		}
	}

	return S_OK;

}



HRESULT CDllRegShlExt::InvokeCommand (LPCMINVOKECOMMANDINFO pCmdInfo)
{
	// ��� lpVerb ָ��һ�ַ���, ���Դ˴ε���. 
	if ( 0 != HIWORD( pCmdInfo->lpVerb ))
		return E_INVALIDARG;

	// ���lpVerb �ǲ���������ӵ�����(0 �� 1) 
	switch ( LOWORD( pCmdInfo->lpVerb )) 
	{ 
	case 0: 
	case 1:                        {
		CProgressDlg dlg ( &m_lsFiles, pCmdInfo );
		dlg.DoModal();
		return S_OK; 
								   }
								   break;
	default: 
		return E_INVALIDARG; 
		break; 
	}
}        
