// BmpCtxMenuExt.cpp : Implementation of CBmpCtxMenuExt
#include "stdafx.h"
#include "BmpViewerExt.h"
#include "BmpCtxMenuExt.h"
#include <atlconv.h>

/////////////////////////////////////////////////////////////////////////////
// CBmpCtxMenuExt

const LONG CBmpCtxMenuExt::m_lMaxThumbnailSize = 64;
const LONG CBmpCtxMenuExt::m_l3DBorderWidth = 2;
const LONG CBmpCtxMenuExt::m_lMenuItemSpacing = 4;
const LONG CBmpCtxMenuExt::m_lTotalBorderSpace = 2*(m_lMenuItemSpacing+m_l3DBorderWidth);

STDMETHODIMP CBmpCtxMenuExt::Initialize (
                               LPCITEMIDLIST pidlFolder,
                               LPDATAOBJECT  pDO,
                               HKEY          hkeyProgID )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

COleDataObject dataobj;
HGLOBAL        hglobal;
HDROP          hdrop;
bool           bOK = false;

    dataobj.Attach ( pDO, FALSE );      // FALSE = don't release IDataObject interface when destroyed

    // Get the first selected file name.  I'll keep this simple and just check
    // the first name to see if it's a .BMP.

    hglobal = dataobj.GetGlobalData ( CF_HDROP );

    if ( NULL == hglobal )
        return E_INVALIDARG;

    hdrop = (HDROP) GlobalLock ( hglobal );

    if ( NULL == hdrop )
        return E_INVALIDARG;

    // Get the name of the first selected file.

    if ( DragQueryFile ( hdrop, 0, m_szFile, MAX_PATH ))
        {
        // Is its extension .BMP?
        if ( PathMatchSpec ( m_szFile, _T("*.bmp") ))
            {
            // Load the bitmap and attach our CBitmap object to it.
            HBITMAP hbm = (HBITMAP) LoadImage ( NULL, m_szFile, IMAGE_BITMAP, 0, 0, 
                                                LR_LOADFROMFILE );

            if ( NULL != hbm )
                {
                // We loaded the bitmap, so attach the CBitmap to it.
                VERIFY( m_bmp.Attach ( hbm ) );
                bOK = true;
                }
            }
        }

    GlobalUnlock ( hglobal );

    return bOK ? S_OK : E_FAIL;
}

STDMETHODIMP CBmpCtxMenuExt::QueryContextMenu (
                               HMENU hmenu, UINT uIndex, UINT uidCmdFirst,
                               UINT uidCmdLast, UINT uFlags )
{
    // If the flags include CMF_DEFAULTONLY then we shouldn't do anything.

    if ( uFlags & CMF_DEFAULTONLY )
        {
        return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
        }

    // Get the version of the shell.  If it's > 4, we can use an owner-drawn 
    // item.

bool bUseOwnerDraw = false;
HINSTANCE hinstShell;

    hinstShell = GetModuleHandle ( _T("shell32") );

    if ( NULL != hinstShell )
        {
        DLLGETVERSIONPROC pProc;

        pProc = (DLLGETVERSIONPROC) GetProcAddress ( hinstShell, "DllGetVersion" );

        if ( NULL != pProc )
            {
            DLLVERSIONINFO rInfo = { sizeof(DLLVERSIONINFO) };

            if ( SUCCEEDED( pProc ( &rInfo ) ))
                {
                // Check for a major version greater than 4 (like on Win 2K),
                // or minor version >= 71 (95/NT4 with Active Desktop).

                if ( rInfo.dwMajorVersion > 4  ||
                     rInfo.dwMinorVersion >= 71  )
                    {
                    bUseOwnerDraw = true;
                    }
                }
            }
        }

MENUITEMINFO mii;

    mii.cbSize = sizeof(MENUITEMINFO);
    mii.fMask  = MIIM_ID | MIIM_TYPE;
    mii.fType  = bUseOwnerDraw ? MFT_OWNERDRAW : MFT_BITMAP;
    mii.wID    = uidCmdFirst;

    if ( !bUseOwnerDraw )
        {
        // NOTE: This will put the full-sized bitmap in the menu, which is
        // obviously a bit less than optimal.  Compressing the bitmap down
        // to a thumbnail is left as an exercise.

        mii.dwTypeData = (LPTSTR) m_bmp.GetSafeHandle();
        }

    InsertMenuItem ( hmenu, uIndex, TRUE, &mii );

    // Store the menu item's ID so we can check against it later when
    // WM_MEASUREITEM/WM_DRAWITEM are sent.
        
    m_uOurItemID = uidCmdFirst;

    // Tell the shell we added 1 top-level menu item.

    return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 1 );
}

STDMETHODIMP CBmpCtxMenuExt::GetCommandString (
                               UINT uCmd, UINT uFlags, UINT* puReserved,
                               LPSTR pszName, UINT cchMax )
{
static LPCTSTR szHelpString = _T("Click the thumbnail to view the entire picture.");

    USES_CONVERSION;

    // Check idCmd, it must be 0 since we have only one menu item.
    if ( 0 != uCmd )
        return E_INVALIDARG;

    // If Explorer is asking for a help string, copy our string into the
    // supplied buffer.
    if ( uFlags & GCS_HELPTEXT )
        {
        if ( uFlags & GCS_UNICODE )
            {
            // We need to cast pszName to a Unicode string, and then use the
            // Unicode string copy API.
            lstrcpynW ( (LPWSTR) pszName, T2CW(szHelpString), cchMax );
            }
        else
            {
            // Use the ANSI string copy API to return the help string.
            lstrcpynA ( pszName, T2CA(szHelpString), cchMax );
            }
        }

    return S_OK;
}

STDMETHODIMP CBmpCtxMenuExt::InvokeCommand ( LPCMINVOKECOMMANDINFO pInfo )
{
    // If lpVerb really points to a string, ignore this function call and bail out.
    if ( 0 != HIWORD( pInfo->lpVerb ))
        return E_INVALIDARG;

    // The command ID must be 0 since we only have one menu item.
    if ( 0 != LOWORD( pInfo->lpVerb ))
        return E_INVALIDARG;

    // Open the bitmap in the default paint program.
    ShellExecute ( pInfo->hwnd, _T("open"), m_szFile, NULL, NULL, SW_SHOWNORMAL );

    return S_OK;
}

STDMETHODIMP CBmpCtxMenuExt::HandleMenuMsg ( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // res is a dummy LRESULT variable.  It's not used (IContextMenu2::HandleMenuMsg()
    // doesn't provide a way to return values), but it's here so that the code 
    // in MenuMessageHandler() can be the same regardless of which interface it
    // gets called through (IContextMenu2 or 3).

LRESULT res;

    return MenuMessageHandler ( uMsg, wParam, lParam, &res );
}

STDMETHODIMP CBmpCtxMenuExt::HandleMenuMsg2 ( UINT uMsg, WPARAM wParam, LPARAM lParam,
                                              LRESULT* pResult )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // For messages that have no return value, pResult is NULL.  This is
    // _very_ mean on the part of MS since it forces us to check whether
    // pResult is valid before using it.  You'd think that a pointer to
    // a "return value" would always be valid, but alas no.
    // If it is NULL, I create a dummy LRESULT variable, so the code in
    // MenuMessageHandler() will always have a valid pResult pointer.

    if ( NULL == pResult )
        {
        LRESULT res;
        return MenuMessageHandler ( uMsg, wParam, lParam, &res );
        }
    else
        {
        return MenuMessageHandler ( uMsg, wParam, lParam, pResult );
        }
}

STDMETHODIMP CBmpCtxMenuExt::MenuMessageHandler ( UINT uMsg, WPARAM wParam,
                                                  LPARAM lParam, LRESULT* pResult )
{
    switch ( uMsg )
        {
        case WM_MEASUREITEM:
            {
            return OnMeasureItem ( (MEASUREITEMSTRUCT*) lParam, pResult );
            }
        break;

        case WM_DRAWITEM:
            {
            return OnDrawItem ( (DRAWITEMSTRUCT*) lParam, pResult );
            }
        break;
        }

    return S_OK;
}

STDMETHODIMP CBmpCtxMenuExt::OnMeasureItem ( MEASUREITEMSTRUCT* pmis, LRESULT* pResult )
{
BITMAP bm;
LONG   lThumbWidth;
LONG   lThumbHeight;

    // Check that we're getting called for our own menu item.

    if ( m_uOurItemID != pmis->itemID )
        return S_OK;

    m_bmp.GetBitmap ( &bm );

    m_lBmpWidth = bm.bmWidth;
    m_lBmpHeight = bm.bmHeight;

    // Calculate the bitmap thumbnail size.

    lThumbWidth = (m_lBmpWidth <= m_lMaxThumbnailSize) ? m_lBmpWidth : 
                    m_lMaxThumbnailSize;

    lThumbHeight = (m_lBmpHeight <= m_lMaxThumbnailSize) ? m_lBmpHeight :
                     m_lMaxThumbnailSize;

    // Calculate the size of the menu item, which is the size of the thumbnail +
    // the border and padding (which provides some space at the edges of the item).
                     
    m_lItemWidth = lThumbWidth + m_lTotalBorderSpace;
    m_lItemHeight = lThumbHeight + m_lTotalBorderSpace;
    
    // Store the size of the item in the MEASUREITEMSTRUCT.

    pmis->itemWidth = m_lItemWidth;
    pmis->itemHeight = m_lItemHeight;

    *pResult = TRUE;            // we handled the message

    return S_OK;
}

STDMETHODIMP CBmpCtxMenuExt::OnDrawItem ( DRAWITEMSTRUCT* pdis, LRESULT* pResult )
{
CDC   dcBmpSrc;
CDC*  pdcMenu = CDC::FromHandle ( pdis->hDC );
CRect rcItem ( pdis->rcItem );  // RECT of our menu item
CRect rcDraw;                   // RECT in which we'll be drawing

    // Check that we're getting called for our own menu item.

    if ( m_uOurItemID != pdis->itemID )
        return S_OK;
            
    // rcDraw will first be set to the RECT that we set in WM_MEASUREITEM.
    // It will get deflated as we go along.

    rcDraw.left = (rcItem.right + rcItem.left - m_lItemWidth) / 2;
    rcDraw.top = (rcItem.top + rcItem.bottom - m_lItemHeight) / 2;
    rcDraw.right = rcDraw.left + m_lItemWidth;
    rcDraw.bottom = rcDraw.top + m_lItemHeight;

    // Shrink rcDraw to account for the padding space around
    // the thumbnail.

    rcDraw.DeflateRect ( m_lMenuItemSpacing, m_lMenuItemSpacing );

    // Fill in the background of the menu item.

    if ( pdis->itemState & ODS_SELECTED )
        {
        pdcMenu->FillSolidRect ( rcItem, GetSysColor ( COLOR_HIGHLIGHT ));
        }
    else
        {
        pdcMenu->FillSolidRect ( rcItem, GetSysColor ( COLOR_3DFACE ));
        }

    // Draw the sunken 3D border.

    for ( int i = 1; i <= m_l3DBorderWidth; i++ )
        {
        pdcMenu->Draw3dRect ( rcDraw, GetSysColor ( COLOR_3DDKSHADOW ),
                              GetSysColor ( COLOR_3DHILIGHT ));

        rcDraw.DeflateRect ( 1, 1 );
        }

    // Create a new DC and select the original bitmap into it.

    dcBmpSrc.CreateCompatibleDC ( pdcMenu );
    dcBmpSrc.SelectObject ( &m_bmp );

    // Blit the bitmap to the menu DC.  This is the quick 'n' dirty
    // way of doing it.  Making it look nice is left as an exercise. :)

    pdcMenu->StretchBlt ( rcDraw.left, rcDraw.top, rcDraw.Width(), rcDraw.Height(),
                          &dcBmpSrc, 0, 0, m_lBmpWidth, m_lBmpHeight, SRCCOPY );

    *pResult = TRUE;            // we handled the message

    return S_OK;
}
