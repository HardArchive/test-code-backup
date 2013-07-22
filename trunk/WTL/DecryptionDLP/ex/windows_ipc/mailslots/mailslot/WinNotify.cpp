#include "stdafx.h"
#include "WinNotify.h"
#include "MainFrm.h"
#include "WinNotifyDoc.h"
#include "WinNotifyView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CWinNotifyApp, CWinApp)
	//{{AFX_MSG_MAP(CWinNotifyApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CWinNotifyApp::CWinNotifyApp()
{
}

CWinNotifyApp* GetApp() 
{ 
  return static_cast<CWinNotifyApp*>(AfxGetApp()); 
}

CWinNotifyApp theApp;

BOOL CWinNotifyApp::InitInstance()
{
  //Load the two icons which will be used by the main window
  m_hMessageIcon = LoadIcon(IDR_MAINFRAME2);
  m_hNoMessageIcon = LoadIcon(IDR_MAINFRAME);

  //Let the document template manager do its stuff
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWinNotifyDoc),
		RUNTIME_CLASS(CMainFrame),
		RUNTIME_CLASS(CWinNotifyView));
	AddDocTemplate(pDocTemplate);
  OnFileNew();

	return TRUE;
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWinNotifyApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}



