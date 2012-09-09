// InterFaceCls.cpp : Implementation of CInterFaceCls
#include "stdafx.h"
#include "OpenedFiles.h"
#include "InterFaceCls.h"
#include "MainDlg.h"
#include "MyContext.h"
/////////////////////////////////////////////////////////////////////////////
// CInterFaceCls

// Function will be called when the user right clicks on any folder
// this function copies the path of that folder to member variable
STDMETHODIMP CInterFaceCls::Initialize ( LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID )
{
    FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT,
                      -1, TYMED_HGLOBAL };
    STGMEDIUM stg = { TYMED_HGLOBAL };
    HDROP     hDrop;
	if( !pDataObj )
	{
	  if( pidlFolder )
	  {
		  TCHAR tcPath[ MAX_PATH ];
		  if( !SHGetPathFromIDList( pidlFolder , tcPath ))
		  {
			  return E_INVALIDARG;
		  }
		  else
		  {
			  m_csPath = tcPath;
			  m_csPath.MakeLower();
			  return S_OK;
		  }
	  }
	  else
	  {
			return E_INVALIDARG;
	  }
	}

  if ( FAILED( pDataObj->GetData ( &fmt, &stg ) ))
    return E_INVALIDARG;
 
  hDrop = (HDROP) GlobalLock ( stg.hGlobal );
 
  if ( NULL == hDrop )
    return E_INVALIDARG;


  UINT uNumFiles = DragQueryFile ( hDrop, 0xFFFFFFFF, NULL, 0 );
    HRESULT hr = S_OK;
  
  if ( 0 == uNumFiles )
    {
    GlobalUnlock ( stg.hGlobal );
    ReleaseStgMedium ( &stg );
    return E_INVALIDARG;
    }

  if ( 0 == DragQueryFile ( hDrop, 0, m_csPath.GetBuffer(MAX_PATH), MAX_PATH ) )
    hr = E_INVALIDARG;
  m_csPath.ReleaseBuffer();
  m_csPath.MakeLower();
  GlobalUnlock ( stg.hGlobal );
  ReleaseStgMedium ( &stg );
  return hr;
}

// Insert my menu into the other menu's
HRESULT CInterFaceCls::QueryContextMenu( HMENU hmenu, UINT uMenuIndex, UINT uidFirstCmd,  UINT uidLastCmd, UINT uFlags )
{
  // If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
  if ( uFlags & CMF_DEFAULTONLY )
    return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
  InsertMenu ( hmenu, uMenuIndex, MF_BYPOSITION, uidFirstCmd, _T("List Opened Files") ); 
  return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 1 );
}


// Function show the help string in the status bar of Explorer when the user places the menu over my menu
HRESULT CInterFaceCls::GetCommandString( UINT idCmd, UINT uFlags, UINT* pwReserved,  LPSTR pszName, UINT cchMax )
{
    USES_CONVERSION;
 
  // Check idCmd, it must be 0 since we have only one menu item.
  if ( 0 != idCmd )
    return E_INVALIDARG;
 
	// If Explorer is asking for a help string, copy our string into the
	// supplied buffer.
	if( uFlags & GCS_HELPTEXT )
    {
        LPCTSTR szText = _T("Lists all the opened files in this folder"); 
		if ( uFlags & GCS_UNICODE )
		{
			// We need to cast pszName to a Unicode string, and then use the
			// Unicode string copy API.
			lstrcpynW ( (LPWSTR) pszName, T2CW(szText), cchMax );
		}
		else
		{
			// Use the ANSI string copy API to return the help string.
			lstrcpynA ( pszName, T2CA(szText), cchMax );
		}
 
    return S_OK;
    }
 
  return E_INVALIDARG;
}

// Show the dialog
HRESULT CInterFaceCls::InvokeCommand( LPCMINVOKECOMMANDINFO pCmdInfo )
{
  
 if ( 0 != HIWORD( pCmdInfo->lpVerb ) )
    return E_INVALIDARG;
  
  switch ( LOWORD( pCmdInfo->lpVerb ) )
    {
    case 0:
      {
        AFX_MANAGE_STATE( AfxGetStaticModuleState() );
        CMyContext m_Context;
        MainDlg( m_csPath ).DoModal(); 
        return S_OK;
      }
    default:
      return E_INVALIDARG;
    break;
    }
}