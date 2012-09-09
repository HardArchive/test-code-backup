// HardLinkShlExt.cpp : Implementation of CHardLinkShlExt
#include "stdafx.h"
#include "HardLink.h"
#include "HardLinkShlExt.h"
#include <shlwapi.h>
#include <atlconv.h>

/////////////////////////////////////////////////////////////////////////////
// CHardLinkShlExt IShellExtInit methods

HRESULT CHardLinkShlExt::Initialize ( LPCITEMIDLIST pidlFolder,
                                      LPDATAOBJECT  pDO,
                                      HKEY          hProgID )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

COleDataObject dataobj;
HGLOBAL        hglobal;
HDROP          hdrop;
TCHAR          szRoot [MAX_PATH];
TCHAR          szFileSystemName [256];
TCHAR          szFile [MAX_PATH];
UINT           uFile, uNumFiles;

    m_bitmap.LoadBitmap ( IDB_LINKBITMAP );

    dataobj.Attach ( pDO, FALSE );      // FALSE = don't release IDataObject interface when destroyed

    // Get the name of the directory where the files were dropped.

    if ( !SHGetPathFromIDList ( pidlFolder, m_szFolderDroppedIn ))
        {
        return E_FAIL;
        }

    // Get the root of the target folder, and see if it's on an NTFS volume.

    lstrcpy ( szRoot, m_szFolderDroppedIn );
    PathStripToRoot ( szRoot );

    if ( !GetVolumeInformation ( szRoot, NULL, 0, NULL, NULL, NULL, 
                                 szFileSystemName, 256 ))
        {
        // Couldn't determine file system type.
        return E_FAIL;
        }

#ifndef NOT_ON_WIN2K
    if ( 0 != lstrcmpi ( szFileSystemName, _T("ntfs") ))
        {
        // The file system isn't NTFS, so it doesn't support hard links.
        return E_FAIL;
        }
#endif

    // Make sure the target dir ends in a backslash, to make later processing easier.

    PathAddBackslash ( m_szFolderDroppedIn );

    // Get a list of all the objects that were dropped.

    hglobal = dataobj.GetGlobalData ( CF_HDROP );

    if ( NULL == hglobal )
        return E_INVALIDARG;

    hdrop = (HDROP) GlobalLock ( hglobal );

    if ( NULL == hdrop )
        return E_INVALIDARG;

    // Determine how many files were dropped.

    uNumFiles = DragQueryFile ( hdrop, 0xFFFFFFFF, NULL, 0 );

    // Check each dropped item.  If there are any directories present,
    // we have to bail out, since a directory can't be linked to.
    // We also have to check that the dropped items reside on the same
    // volume as the directory where they were dropped - hard links can only
    // be made on the same volume.

    for ( uFile = 0; uFile < uNumFiles; uFile++ )
        {
        if ( DragQueryFile ( hdrop, uFile, szFile, MAX_PATH ))
            {
            if ( PathIsDirectory ( szFile ))
                {
                // We found a directory!  Bail out.
                m_lsDroppedFiles.RemoveAll();
                break;
                }

            if ( !PathIsSameRoot ( szFile, m_szFolderDroppedIn ))
                {
                // Dropped files came from a different volume - bail out.
                m_lsDroppedFiles.RemoveAll();
                break;
                }

            // Add the file to our list of dropped files.
            m_lsDroppedFiles.AddTail ( szFile );
            }
        }   // end for

    GlobalUnlock ( hglobal );

    return ( m_lsDroppedFiles.GetCount() > 0 ) ? S_OK : E_FAIL;
}


/////////////////////////////////////////////////////////////////////////////
// CHardLinkShlExt IContextMenu methods

HRESULT CHardLinkShlExt::QueryContextMenu ( HMENU hmenu,       UINT uMenuIndex,
                                            UINT  uidFirstCmd, UINT uidLastCmd,
                                            UINT  uFlags )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    // If the flags include CMF_DEFAULTONLY then we shouldn't do anything.
    if ( uFlags & CMF_DEFAULTONLY )
        {
        return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 0 );
        }

    // Add the hard link menu item.

    InsertMenu ( hmenu, uMenuIndex, MF_STRING | MF_BYPOSITION, uidFirstCmd,
                 _T("Create &Hard Link(s) Here") );

    if ( NULL != m_bitmap.GetSafeHandle() )
        {
        SetMenuItemBitmaps ( hmenu, uMenuIndex, MF_BYPOSITION, m_bitmap, NULL );
        }

    // Return 1 to tell the shell that we added 1 top-level menu item.

    return MAKE_HRESULT ( SEVERITY_SUCCESS, FACILITY_NULL, 1 );
}


HRESULT CHardLinkShlExt::InvokeCommand ( LPCMINVOKECOMMANDINFO pInfo )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

TCHAR    szNewFilename [MAX_PATH+32];   // +32 in case a path overruns MAX_PATH (we'll handle this case and error out)
CString  sSrcFile;
TCHAR    szSrcFileTitle [MAX_PATH];
CString  sMessage;
UINT     uLinkNum;
POSITION pos;

    // Double-check that we're getting called for our own menu item - lpVerb
    // must be 0.

    if ( 0 != pInfo->lpVerb )
        {
        return E_INVALIDARG;
        }

    // Start enumerating the files we're going to make links for.

    pos = m_lsDroppedFiles.GetHeadPosition();

    // Sanity check - the list must not be empty.

    ASSERT ( NULL != pos );


    while ( NULL != pos )
        {
        // Get the next source filename.
    
        sSrcFile = m_lsDroppedFiles.GetNext ( pos );

        // Remove the path - this reduces "C:\xyz\foo\stuff.exe" to "stuff.exe"

        lstrcpy ( szSrcFileTitle, sSrcFile );
        PathStripPath ( szSrcFileTitle );

        // Make the filename for the hard link - we'll first try 
        // "Hard link to stuff.exe"

        wsprintf ( szNewFilename, _T("%sHard link to %s"), m_szFolderDroppedIn,
                   szSrcFileTitle );

        // Check if that filename already exists.  If so, we'll try
        // "Hard link (2) to stuff.exe", incrementing the number up to an
        // arbitrary limit of 99.

        for ( uLinkNum = 2;
              PathFileExists ( szNewFilename )  &&  uLinkNum < 100; 
              uLinkNum++ )
            {
            // Try another filename for the link.

            wsprintf ( szNewFilename, _T("%sHard link (%u) to %s"),
                       m_szFolderDroppedIn, uLinkNum, szSrcFileTitle );

            // If the resulting filename is longer than MAX_PATH, show an 
            // error message.

            if ( lstrlen ( szNewFilename ) >= MAX_PATH )
                {
                sMessage.Format ( _T("Failed to make a link to %s. The resulting filename would be too long.\n\nDo you want to continue making links?"),
                                  (LPCTSTR) sSrcFile );

                if ( IDNO == MessageBox ( pInfo->hwnd, sMessage, _T("Create Hard Links"),
                                          MB_ICONQUESTION | MB_YESNO ))
                    {
                    break;
                    }
                else
                    {
                    continue;
                    }
                }
            }

        // If we hit our limit of 100 links, tell the user what's up.

        if ( 100 == uLinkNum )
            {
            sMessage.Format ( _T("Failed to make a link to %s. Reached limit of 100 links in a single directory.\n\nDo you want to continue making links?"),
                              (LPCTSTR) sSrcFile );

            if ( IDNO == MessageBox ( pInfo->hwnd, sMessage, _T("Create Hard Links"),
                                      MB_ICONQUESTION | MB_YESNO ))
                {
                break;
                }
            else
                {
                continue;
                }
            }

        // Make the link!

#ifdef NOT_ON_WIN2K
        MessageBox(pInfo->hwnd, szNewFilename, "The new link would be:", MB_OK);
#else
        if ( !CreateHardLink ( szNewFilename, sSrcFile, NULL ))
            {
            LPVOID pvMsgBuf = NULL;
            DWORD  dwLastErr = GetLastError();
            int    nChoice;

            FormatMessage ( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | 
                              FORMAT_MESSAGE_IGNORE_INSERTS,
                            NULL, dwLastErr,
                            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                            (LPTSTR) &pvMsgBuf, 0, NULL );

            sMessage.Format ( _T("Failed to make a link to %s.\n\nError 0x%08lX - %s\nDo you want to continue making links?"),
                              (LPCTSTR) sSrcFile, dwLastErr,
                              pvMsgBuf ? pvMsgBuf : _T("(No description available)") );

            nChoice = MessageBox ( pInfo->hwnd, sMessage, _T("Create Hard Links"),
                                   MB_ICONQUESTION | MB_YESNO );

            if ( NULL != pvMsgBuf )
                {
                LocalFree ( pvMsgBuf );
                }

            if ( IDNO == nChoice )
                {
                break;
                }
            }
#endif
        }   // end while (pos != NULL)

    return S_OK;
}
