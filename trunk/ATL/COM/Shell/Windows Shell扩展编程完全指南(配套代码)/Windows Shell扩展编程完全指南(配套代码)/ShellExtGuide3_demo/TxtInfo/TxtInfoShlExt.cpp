// TxtInfoShlExt.cpp : Implementation of CTxtInfoShlExt
#include "stdafx.h"
#include "TxtInfo.h"
#include "TxtInfoShlExt.h"

#include <atlconv.h>

/////////////////////////////////////////////////////////////////////////////
// CTxtInfoShlExt

HRESULT CTxtInfoShlExt::Load ( LPCOLESTR wszFilename, DWORD dwMode )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());    // init MFC
    UNREFERENCED_PARAMETER(dwMode);

    // Let CString convert the filename to ANSI if necessary.
    m_sFilename = wszFilename;

    return S_OK;
}

HRESULT CTxtInfoShlExt::GetInfoTip ( DWORD dwFlags, LPWSTR* ppwszTip )
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());    // init MFC

LPMALLOC   pMalloc;
CStdioFile file;
DWORD      dwFileSize;
CString    sFirstLine;
BOOL       bReadLine;
CString    sTooltip;

    UNREFERENCED_PARAMETER(dwFlags);
    USES_CONVERSION;

    // Try to open the file.
    if ( !file.Open ( m_sFilename , CFile::modeRead | CFile::shareDenyWrite ))
        return E_FAIL;

    // Get an IMalloc interface from the shell.
    if ( FAILED( SHGetMalloc ( &pMalloc )))
        return E_FAIL;

    // Get the size of the file.
    dwFileSize = file.GetLength();

    // Read in the first line from the file.
    bReadLine = file.ReadString ( sFirstLine );

    sTooltip.Format ( _T("File size: %lu"), dwFileSize );

    // If we were able to read in the first line, add it to the tooltip.
    if ( bReadLine )
        {
        sTooltip += _T("\n");
        sTooltip += sFirstLine;
        }

    // Allocate a buffer for Explorer.  Note that the must pass the string 
    // back as a Unicode string, so the string length is multiplied by the 
    // size of a Unicode character.
    *ppwszTip = (LPWSTR) pMalloc->Alloc ( (1 + lstrlen(sTooltip)) * sizeof(wchar_t) );

    // Release the IMalloc interface now that we're done using it.
    pMalloc->Release();

    if ( NULL == *ppwszTip )
        {
        return E_OUTOFMEMORY;
        }

    // Use the Unicode string copy function to put the tooltip text in the buffer.
    wcscpy ( *ppwszTip, T2COLE((LPCTSTR) sTooltip) );

    return S_OK;
}
