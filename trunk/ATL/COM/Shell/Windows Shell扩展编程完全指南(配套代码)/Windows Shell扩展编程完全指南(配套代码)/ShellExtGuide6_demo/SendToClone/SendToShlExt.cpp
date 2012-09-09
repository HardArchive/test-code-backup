// SendToShlExt.cpp : Implementation of CSendToShlExt
#include "stdafx.h"
#include "SendToClone.h"
#include "SendToShlExt.h"
#include "SendToCloneDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CSendToShlExt

HRESULT CSendToShlExt::DragEnter ( IDataObject* pDataObj, DWORD grfKeyState,
                                   POINTL pt, DWORD* pdwEffect )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());    // init MFC

COleDataObject dataobj;
TCHAR          szItem [MAX_PATH];
UINT           uNumFiles;
HGLOBAL        hglobal;
HDROP          hdrop;

    dataobj.Attach ( pDataObj, FALSE ); // attach to the IDataObject, don't auto-release it

    // Read the list of items from the data object.  They're stored in HDROP
    // form, so just get the HDROP handle and then use the drag 'n' drop APIs
    // on it.

    hglobal = dataobj.GetGlobalData ( CF_HDROP );

    if ( NULL != hglobal )
        {
        hdrop = (HDROP) GlobalLock ( hglobal );

        uNumFiles = DragQueryFile ( hdrop, 0xFFFFFFFF, NULL, 0 );

        for ( UINT uFile = 0; uFile < uNumFiles; uFile++ )
            {
            if ( 0 != DragQueryFile ( hdrop, uFile, szItem, MAX_PATH ))
                {
                m_lsDroppedFiles.AddTail ( szItem );
                }
            }

        GlobalUnlock ( hglobal );
        }

    if ( m_lsDroppedFiles.GetCount() > 0 ) 
        {
        *pdwEffect = DROPEFFECT_COPY;
        return S_OK;
        }
    else
        {
        *pdwEffect = DROPEFFECT_NONE;
        return E_INVALIDARG;
        }
}

HRESULT CSendToShlExt::Drop ( IDataObject* pDataObj, DWORD grfKeyState,
                              POINTL pt, DWORD* pdwEffect )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());    // init MFC

CSendToCloneDlg dlg ( &m_lsDroppedFiles );

    dlg.DoModal();

    *pdwEffect = DROPEFFECT_COPY;
    return S_OK;
}
