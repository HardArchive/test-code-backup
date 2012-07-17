// FolderDlg.h: interface for the CFolderDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOLDERDLG_H__89FDF289_89F5_460C_9C03_0A3CCF5CDB6D__INCLUDED_)
#define AFX_FOLDERDLG_H__89FDF289_89F5_460C_9C03_0A3CCF5CDB6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFolderDlg  
{
public:
    CFolderDlg(HWND hOwner, LPCTSTR lpszTitle = NULL);
    virtual ~CFolderDlg();
    
public:
    int DoModal(int nFolderId = CSIDL_DRIVES);
    
    BOOL SetSelectPath(LPCTSTR lpszSelectPath);
    CString GetSelectPath();
    
    int     m_nWinX;
    int     m_nWinY;
    
    CString m_strMainTitle;
    TCHAR   m_szSelectPath[MAX_PATH];
    BROWSEINFO m_clBrowseInfo;
    
    static int BrowseForFolder(BROWSEINFO BrowseInfo, CString& strSelectPath);
    
protected:
    HWND m_hOwnerWnd;
    
private:
    static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData);
};

#endif // !defined(AFX_FOLDERDLG_H__89FDF289_89F5_460C_9C03_0A3CCF5CDB6D__INCLUDED_)
