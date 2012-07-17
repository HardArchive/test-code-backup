// FolderDlg.cpp: implementation of the CFolderDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "Stdafx.h"
#include "FolderDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFolderDlg::CFolderDlg(HWND hOwner, LPCTSTR lpszTitle)
{
    ZeroMemory(&m_clBrowseInfo, sizeof(m_clBrowseInfo));
    
    m_clBrowseInfo.hwndOwner = hOwner;
    m_clBrowseInfo.lpszTitle = lpszTitle;
    m_clBrowseInfo.ulFlags = BIF_EDITBOX;
    m_clBrowseInfo.lpfn = BrowseCallbackProc;
    
    ZeroMemory(m_szSelectPath, sizeof(m_szSelectPath));
    m_hOwnerWnd = hOwner;
    
    m_nWinX = 440;
    m_nWinY = 280;
}

CFolderDlg::~CFolderDlg()
{
}

int CALLBACK CFolderDlg::BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    CString  strTemp;
    
    switch (uMsg)
    {
    case BFFM_INITIALIZED:
        {
            CFolderDlg *pCurrent = (CFolderDlg *) lpData;
            
            if (!pCurrent->m_strMainTitle.IsEmpty())
                ::SetWindowText(hWnd, pCurrent->m_strMainTitle);
            
            if (pCurrent->m_szSelectPath[0] != '\0')
            {
                ::SendMessage(
                    hWnd, BFFM_SETSELECTION,
                    TRUE, (LPARAM)pCurrent->m_szSelectPath
                );
            }
            ::SetWindowPos(
                hWnd, NULL,
                pCurrent->m_nWinX,
                pCurrent->m_nWinY,
                0, 0,
                SWP_NOZORDER | SWP_NOSIZE
            );
        }
        break;
        
    case BFFM_SELCHANGED:
        {
            TCHAR szCurPath[MAX_PATH];
            
            SHGetPathFromIDList((LPCITEMIDLIST)lParam, szCurPath);
            SendMessage(hWnd, BFFM_SETSTATUSTEXT, 0, (LPARAM) szCurPath);
        }
        break;
        
    case BFFM_VALIDATEFAILED:
        break;
        
    default:
        break;
    }
    
    return 0;
}

int CFolderDlg::DoModal(int nFolderId)
{
    int nResult = IDCANCEL;
    
    LPMALLOC pMalloc = NULL;
    LPITEMIDLIST pItemIDList = NULL;
    
    if (FAILED(::SHGetSpecialFolderLocation(m_hOwnerWnd, nFolderId, &pItemIDList)))
        goto Exit0;
    
    m_clBrowseInfo.pidlRoot = pItemIDList;
    m_clBrowseInfo.lParam = (LPARAM) this;
    
    pItemIDList = ::SHBrowseForFolder(&m_clBrowseInfo);
    if (NULL == pItemIDList)
        goto Exit0;
    
    if (!SHGetPathFromIDList(pItemIDList, m_szSelectPath))
        goto Exit0;
    
    if (S_OK == ::SHGetMalloc(&pMalloc))
        pMalloc->Free(pItemIDList);
    
    nResult = IDOK;
    
Exit0:
    if (pMalloc != NULL)
    {
        pMalloc->Release();
        pMalloc = NULL;
    }
    
    return nResult;
}

int CFolderDlg::BrowseForFolder(BROWSEINFO BrowseInfo, CString &strSelectPath)
{
    int nResult = IDCANCEL;
    
    LPMALLOC pMalloc = NULL;
    LPITEMIDLIST pItemIDList = NULL;
    
    pItemIDList = ::SHBrowseForFolder(&BrowseInfo);
    if (NULL == pItemIDList)
        goto Exit0;
    
    if (!SHGetPathFromIDList(pItemIDList, strSelectPath.GetBuffer(MAX_PATH)))
        goto Exit0;
    strSelectPath.ReleaseBuffer();
    
    if (S_OK == ::SHGetMalloc(&pMalloc))
        pMalloc->Free(pItemIDList);
    
    nResult = IDOK;
    
Exit0:
    if (pMalloc != NULL)
    {
        pMalloc->Release();
        pMalloc = NULL;
    }
    
    return nResult;
}

CString CFolderDlg::GetSelectPath()
{
    return m_szSelectPath;
}

BOOL CFolderDlg::SetSelectPath(LPCTSTR lpszSelectPath)
{
    ::lstrcpy(m_szSelectPath, lpszSelectPath);
    return TRUE;
}
