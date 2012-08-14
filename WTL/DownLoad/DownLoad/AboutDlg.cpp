// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	m_HyperLink9CPP.Attach(GetDlgItem(IDC_STATIC_9CPP));
	m_HyperLink9CPP.SetLink(_T("¾ÍC++"), _T("http://www.9cpp.com/"), TRUE); //"mailto:jun_01@163.com"
	//m_LinkIcon.Attach(GetDlgItem(IDC_STATIC_ICON));
	//m_HyperLink9CPP.SetLink(NULL, _T("http://www.9cpp.com/"), FALSE);
	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
