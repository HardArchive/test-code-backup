#include "StdAfx.h"
#include "MainDlg.h"
#include "PathUtil.h"

LRESULT CMainDlg::OnBnClickedButtonSelectSource(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	RG::CPathUtil clsPathUtil;
	clsPathUtil.GetSelectDirDlg(this->m_hWnd, IDC_EDIT_SOURCE_PATH, _T("选择路径2"));
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonSelectLink(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	RG::CPathUtil clsPathUtil;
	clsPathUtil.GetSelectDirDlg(this->m_hWnd, IDC_EDIT_LINK_PATH);
	//clsPathUtil.GetFileDlg(this->m_hWnd, IDC_EDIT_LINK_PATH, false, _T("网页文件(*.html)\0*.html\0;网页文件(*.htm)\0*.htm\0;All Files(*.*)\0*.*\0"));
	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonStart(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	int iInterval = GetDlgItemInt(IDC_EDIT_INTERVAL);
	TCHAR tszLinkPath[MAX_PATH] = {0};
	TCHAR tszSourcePath[MAX_PATH] = {0};
	GetDlgItemText(IDC_EDIT_LINK_PATH, tszLinkPath, MAX_PATH);
	GetDlgItemText(IDC_EDIT_SOURCE_PATH, tszSourcePath, MAX_PATH);
	
	if (!(_tcslen(tszSourcePath) && _tcslen(tszLinkPath)))
	{
		::MessageBox(NULL, _T("请输入或选择Source和Link路径!!!"), _T("TIP_RG"), MB_OK);
		return 0;
	}

	m_clsHandleFile.Start(tszSourcePath, tszLinkPath, iInterval);
	

	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码

	m_clsHandleFile.Stop();
	Sleep(100);
	::MessageBox(NULL, _T("已经停止！！！！"), _T("Stop"), MB_OK);
	return 0;
}
