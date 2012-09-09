// TxtIconShlExt.cpp : Implementation of CTxtIconShlExt
#include "stdafx.h"
#include "TxtFileIcons.h"
#include "TxtIconShlExt.h"

/////////////////////////////////////////////////////////////////////////////
// CTxtIconShlExt

/*
********** METHOD 1 **********
This method returns a filename/index pair to the shell, telling it where the
icon is.  This is the easier way, but it requires you to keep track of the
resource IDs of the icons *and* keep them in the right order!
*/
/*
STDMETHODIMP CTxtIconShlExt::GetIconLocation (
                                 UINT uFlags,  LPTSTR szIconFile, UINT cchMax,
                                 int* piIndex, UINT* pwFlags )
{
DWORD     dwFileSizeLo, dwFileSizeHi;
DWORDLONG ldwSize;
HANDLE    hFile;

    // First, open the file and get its length.

    hFile = CreateFile ( m_szFilename, GENERIC_READ, FILE_SHARE_READ, NULL,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if ( INVALID_HANDLE_VALUE == hFile )
        {
        return S_FALSE;                 // tell the shell to use a default icon
        }

    dwFileSizeLo = GetFileSize ( hFile, &dwFileSizeHi );

    CloseHandle ( hFile );

    // Check that GetFileSize() succeeded.

    if ( (DWORD) -1 == dwFileSizeLo  &&  GetLastError() != NO_ERROR )
        {
        return S_FALSE;                 // tell the shell to use a default icon
        }

    // The icons are all in this DLL, so get the full path to the DLL, which
    // we'll return through the szIconFile parameter.

TCHAR szModulePath[MAX_PATH];

    GetModuleFileName ( _Module.GetModuleInstance(), szModulePath, MAX_PATH );
    lstrcpyn ( szIconFile, szModulePath, cchMax );

    // Decide which icon to use based on the file size.

    ldwSize = ((DWORDLONG) dwFileSizeHi)<<32 | dwFileSizeLo;

    if ( 0 == ldwSize )
        *piIndex = 0;
    else if ( ldwSize < 4096 )
        *piIndex = 1;
    else if ( ldwSize < 8192 )
        *piIndex = 2;
    else 
        *piIndex = 3;

    // pwFlags is set to zero to get the default behavior from Explorer.  You 
    // can set it to GIL_SIMULATEDOC to have Explorer put the icon we return in
    // a "dog-eared paper" icon, and use _that_ as the icon for the file.

    *pwFlags = GIL_SIMULATEDOC;
    //*pwFlags = 0;

    return S_OK;
}

STDMETHODIMP CTxtIconShlExt::Extract ( 
                                 LPCTSTR pszFile, UINT nIconIndex, 
                                 HICON* phiconLarge, HICON* phiconSmall,
                                 UINT nIconSize )
{
    return S_FALSE;                     // Tell the shell to do the extracting itself.
}
*/

/*
********** METHOD 2 **********
This method stores the file size in the CTxtIconShlExt object, and does the
icon extraction manually.  This way is more complicated, but I prefer it since
you don't have to do bookkeeping of your resource IDs.
*/
STDMETHODIMP CTxtIconShlExt::GetIconLocation (
                                 UINT uFlags,  LPTSTR szIconFile, UINT cchMax,
                                 int* piIndex, UINT* pwFlags )
{
DWORD  dwFileSizeLo, dwFileSizeHi;
HANDLE hFile;

    hFile = CreateFile ( m_szFilename, GENERIC_READ, FILE_SHARE_READ, NULL,
                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

    if ( INVALID_HANDLE_VALUE == hFile )
        {
        return S_FALSE;                 // tell the shell to use a default icon
        }

    dwFileSizeLo = GetFileSize ( hFile, &dwFileSizeHi );

    CloseHandle ( hFile );

    if ( (DWORD) -1 == dwFileSizeLo  &&  GetLastError() != NO_ERROR )
        {
        return S_FALSE;                 // tell the shell to use a default icon
        }

    m_ldwFileSize = ((DWORDLONG) dwFileSizeHi)<<32 | dwFileSizeLo;

    // Tell the shell not to look at the name/index, and don't check the icon
    // cache.  Note that the GIL_NOTFILENAME flag doesn't really have any effect;
    // if we fill in szIconFile/piIndex, Explorer will still check them against
    // the cache.  So the only flag that's really important is GIL_DONTCACHE.

    *pwFlags = GIL_NOTFILENAME | GIL_DONTCACHE;

    return S_OK;
}

STDMETHODIMP CTxtIconShlExt::Extract ( 
                                 LPCTSTR pszFile, UINT nIconIndex, 
                                 HICON* phiconLarge, HICON* phiconSmall,
                                 UINT nIconSize )
{
UINT uIconID;

    // Determine which icon to use, depending on the file size.

    if ( 0 == m_ldwFileSize )
        uIconID = IDI_ZERO_BYTES;
    else if ( m_ldwFileSize < 4096 )
        uIconID = IDI_UNDER_4K;
    else if ( m_ldwFileSize < 8192 )
        uIconID = IDI_UNDER_8K;
    else 
        uIconID = IDI_OVER_8K;

    // Load up the icons!

    *phiconLarge = (HICON) LoadImage ( _Module.GetResourceInstance(), 
                                       MAKEINTRESOURCE(uIconID), IMAGE_ICON,
                                       32, 32, LR_DEFAULTCOLOR );

    *phiconSmall = (HICON) LoadImage ( _Module.GetResourceInstance(), 
                                       MAKEINTRESOURCE(uIconID), IMAGE_ICON,
                                       16, 16, LR_DEFAULTCOLOR );

    return S_OK;
}
