// BkgndCtxMenuExt.cpp : Implementation of CBkgndCtxMenuExt
#include "stdafx.h"
#include "DirBkgndExt.h"
#include "BkgndCtxMenuExt.h"

/////////////////////////////////////////////////////////////////////////////
// CBkgndCtxMenuExt

STDMETHODIMP CBkgndCtxMenuExt::Initialize (
                                 LPCITEMIDLIST pidlFolder,
                                 LPDATAOBJECT  pDO,
                                 HKEY          hkeyProgID )
{
    // pidlFolder is the PIDL of the directory that was clicked in.  pDO is
    // NULL, since there is no selection to operate on.
    // We get the conventional path of the directory from its PIDL with the
    // SHGetPathFromIDList() API.

    return SHGetPathFromIDList ( pidlFolder, m_szDirClickedIn ) ? S_OK : E_INVALIDARG;
}

STDMETHODIMP CBkgndCtxMenuExt::QueryContextMenu (
                                 HMENU hmenu, UINT uIndex, UINT uidCmdFirst,
                                 UINT uidCmdLast, UINT uFlags )
{
UINT uCmdID = uidCmdFirst;

    // First insert item 0 - the English version.

    InsertMenu ( hmenu, uIndex, MF_BYPOSITION, uCmdID++, _T("CLICK HERE!") );

    // uIndex seems to be -1 all the time (tested on 98 and 2K).  If it _is_ -1,
    // we can't increment it, 'cause it would overflow to 0, which would make our
    // second item appear at the _top_ of the menu.

    if ( 0xFFFFFFFF != uIndex )
        uIndex++;

    // Then insert item 1 - the French version.

    InsertMenu ( hmenu, uIndex, MF_BYPOSITION, uCmdID, _T("CLIQUEZ ICI!") );

    return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 2 );
}

STDMETHODIMP CBkgndCtxMenuExt::GetCommandString (
                                 UINT uCmd, UINT uFlags, UINT* puReserved,
                                 LPSTR pszName, UINT cchMax )
{
static LPCTSTR szEnglishHelpString = _T("This is our extension that doesn't do much.");
static LPCTSTR szFrenchHelpString = _T("Voici notre extension qui ne fait rien d'important.");

    USES_CONVERSION;

    // Check idCmd, it must be 0 or 1 since we have two menu items.
    if ( uCmd > 1)
        return E_INVALIDARG;

    // If Explorer is asking for a help string, copy our string into the
    // supplied buffer.
    if ( uFlags & GCS_HELPTEXT )
        {
        if ( uFlags & GCS_UNICODE )
            {
            // We need to cast pszName to a Unicode string, and then use the
            // Unicode string copy API.
            lstrcpynW ( (LPWSTR) pszName, 
                        T2CW( (0 == uCmd) ? szEnglishHelpString : szFrenchHelpString ),
                        cchMax );
            }
        else
            {
            // Use the ANSI string copy API to return the help string.
            lstrcpynA ( pszName, 
                        T2CA( (0 == uCmd) ? szEnglishHelpString : szFrenchHelpString ),
                        cchMax );
            }
        }

    return S_OK;
}

STDMETHODIMP CBkgndCtxMenuExt::InvokeCommand ( LPCMINVOKECOMMANDINFO pInfo )
{
WORD wCmd = LOWORD( pInfo->lpVerb );

    // If lpVerb really points to a string, ignore this function call and bail out.
    if ( 0 != HIWORD( pInfo->lpVerb ))
        return E_INVALIDARG;

    // The command ID must be 0 or 1 since we have two menu items.
    if ( wCmd > 1 )
        return E_INVALIDARG;

TCHAR szMessage [MAX_PATH + 32];

    if ( 0 == wCmd )
        {
        // Item 0 is the English one...
        wsprintf ( szMessage, _T("Thanks for clicking in\n%s"), m_szDirClickedIn );
        }
    else
        {
        // and item 1 is the French one.
        wsprintf ( szMessage, _T("Merci d'avoir cliqué dans\n%s"), m_szDirClickedIn );
        }

    MessageBox ( pInfo->hwnd, szMessage, _T("Dir Background Extension"),
                 MB_ICONINFORMATION );

    return S_OK;
}
