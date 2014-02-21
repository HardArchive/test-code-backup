// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "accelDemoView.h"
#include "MainFrm.h"
#include "keyAssignDlg.h"

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}


#define GSEC_ACCTABLE _T("Software\\codeproject demos\\accelEditDemo")
LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndToolBar);

	CreateSimpleStatusBar();

	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL, WS_EX_CLIENTEDGE);
	m_view.SetWindowText(_T("use Customize menu!"));

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	ATLASSERT(!m_hAccel); // too early yet
	// load accelerators from registry (if any)
	m_paCustomKeys = new CAccelCombo;
	if(!m_paCustomKeys)
		return -1; // what are the chances of that?
	m_paCustomKeys->ReadProfile(GSEC_ACCTABLE);

	return 0;
}

void CMainFrame::CustomAccelerators()
{
	ATLASSERT(m_hAccel);
	ATLASSERT(m_paCustomKeys);
	m_hAccelDefault = m_hAccel;
	if(m_paCustomKeys->GetCount()) {
		m_hAccel = *m_paCustomKeys;
		m_paCustomKeys->UpdateMenuKeys(GetMenu()); // show custom mnemonics
	}
}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: add code to initialize document

	return 0;
}

LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST);	// toolbar is first 1st band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnCustomizeKeys(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ATLASSERT(m_hAccel); // existing keys
	ATLASSERT(m_paCustomKeys);
	ATLASSERT(m_paCustomKeys->GetCount()==0 ||  *m_paCustomKeys == m_hAccel);

	HMENU hm = GetMenu();
	CKeyAssignDlg dlg(hm, m_hAccel, m_hAccelDefault);
	if(dlg.DoModal()==IDOK && dlg.m_nAccel/*dirty indicator*/) {
		ATLASSERT(dlg.m_pNewAccels);
		if(m_paCustomKeys->SetAccelTable(dlg.m_pNewAccels, dlg.m_nAccel)) {
			m_hAccel = *m_paCustomKeys; // use the new keys from now on
			m_paCustomKeys->UpdateMenuKeys(hm);
			ATLASSERT(m_hAccelDefault != *m_paCustomKeys); // no way murinjo
		}
	}

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	m_paCustomKeys->WriteProfile(GSEC_ACCTABLE);
	delete m_paCustomKeys;
	m_paCustomKeys = 0;
	return 0;
}
