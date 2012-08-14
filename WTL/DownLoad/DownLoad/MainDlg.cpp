// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"

//#ifdef UNICODE
//#include <xstring>
//#else
//#include <string>
//#endif 
//using namespace std;
#include <DownLoad.h>
//using namespace QNA;
BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	//int cxScreen,cyScreen;
	//cxScreen = GetSystemMetrics(SM_CXSCREEN);
	//cyScreen = GetSystemMetrics(SM_CYSCREEN);
	//SetWindowPos(::GetForegroundWindow(), 0, 0, cxScreen, cyScreen, SWP_SHOWWINDOW);
	//SetWindowPos(::GetForegroundWindow(),0,0,0,0,SWP_SHOWWINDOW);
	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedButtonSelect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR tszFilePath[MAX_PATH] = {0};

	OPENFILENAME ofn = {0};       
	TCHAR tszFile[MAX_PATH] = {0};       
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = this->m_hWnd;
	ofn.lpstrFile = tszFile;
	ofn.lpstrFile[0] = ' ';
	ofn.nMaxFile = sizeof(tszFile);
	ofn.lpstrFilter = _T("网页文件(*.html)\0*.html\0;网页文件(*.htm)\0*.htm\0;All Files(*.*)\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle =  NULL;
	ofn.lpstrTitle = _T("选择保存网页文件路径");
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = 0;

	if (GetSaveFileName(&ofn)==FALSE)
	{
		//::MessageBox(this->m_hWnd, _T("TIP"), _T("Get save file name error!!!"), MB_OK);
		return 1;
	}

	_tcscpy_s(tszFilePath, ofn.lpstrFile);

	int iLen = _tcslen(tszFilePath);
	PTCHAR ptTem = tszFilePath+iLen-5;
	if (_tcscmp(tszFilePath+iLen-5, _T(".html")) && _tcscmp(tszFilePath+iLen-4, _T(".htm")))
	{
		_stprintf(tszFilePath, _T("%s.html"), tszFilePath);
	}
	//m_csSavePath += _T(".html");

	SetDlgItemText(IDC_EDIT_SAVE_PATH, tszFilePath);

	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonDownload(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	int iRet = 0;
	TCHAR tszUrl[MAX_PATH] = {0};
	TCHAR tszSavePath[MAX_PATH] = {0};
	QNA::CDownLoad clsDL;

	GetDlgItemText(IDC_EDIT_URL, tszUrl, MAX_PATH);
	GetDlgItemText(IDC_EDIT_SAVE_PATH, tszSavePath, MAX_PATH);
	iRet = _tcslen(tszUrl);
	if (iRet>MAX_PATH || iRet<4)
	{
		::MessageBox(this->m_hWnd, _T("请确认要下载的网址是否正确！！！"), _T("提示"), MB_OK);
		return 0;
	}
	iRet = _tcslen(tszSavePath);
	if (iRet>MAX_PATH || iRet<7)
	{
		::MessageBox(this->m_hWnd, _T("请确认要保存的位置是否正确！！！"), _T("提示"), MB_OK);
		return 0;
	}

	::EnableWindow(GetDlgItem(IDC_EDIT_URL), FALSE);
	::EnableWindow(GetDlgItem(IDC_EDIT_SAVE_PATH), FALSE);

	iRet = clsDL.DownLoadFile(tszUrl, tszSavePath);


	if (1 != iRet)
	{
		QNA::TRACE(_T("下载出错,返回值:%d;URL:%s;保存路径:%s;\r\n"), iRet, tszUrl, tszSavePath);
		::MessageBox(this->m_hWnd, _T("下载出错！！！！！"), _T("提示"), MB_OK);
	}
	else
		::MessageBox(this->m_hWnd, _T("下载完成！！！！！"), _T("提示"), MB_OK);
	::EnableWindow(GetDlgItem(IDC_EDIT_URL), TRUE);
	::EnableWindow(GetDlgItem(IDC_EDIT_SAVE_PATH), TRUE);
	return 1;
}


LRESULT CMainDlg::OnBnClickedButtonAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}
