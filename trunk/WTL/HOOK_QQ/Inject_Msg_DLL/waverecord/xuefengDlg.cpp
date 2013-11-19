// xuefengDlg.cpp : implementation file
//

#include "stdafx.h"
#include "xuefeng.h"
#include "xuefengDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// 为 DirectSound 函数添加头文件
#include <dsound.h>
// 为 DirectSound 函数添加库文件
#pragma comment ( lib, "dsound.lib" )
#pragma comment ( lib, "dxguid.lib" )

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXuefengDlg dialog

CXuefengDlg::CXuefengDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXuefengDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXuefengDlg)
	m_nChannels=1;//音频通道数
	m_nSamplesPerSec=11025;//采样频率
	m_wBitsPerSample=16;//采样大小

	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CXuefengDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXuefengDlg)
	DDX_Control(pDX, IDC_COMBO_BitsPerSample, m_BitsPerSample);
	DDX_Control(pDX, IDC_COMBO_Channels, m_Channels);
	DDX_Control(pDX, IDC_COMBO_SamplesPerSec, m_SamplesPerSec);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXuefengDlg, CDialog)
	//{{AFX_MSG_MAP(CXuefengDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_REC, OnButtonRec)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXuefengDlg message handlers

BOOL CXuefengDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow ( FALSE );
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow ( FALSE );
	m_Channels.SetCurSel(0);
	m_SamplesPerSec.SetCurSel(0);
	m_BitsPerSample.SetCurSel(0);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXuefengDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXuefengDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXuefengDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CXuefengDlg::OnButtonPlay() 
{
	// TODO: Add your control notification handler code here
		if ( !m_HwMCI.DeviceIDValid () )
	{
		if ( !m_HwMCI.Open ( "机遇.mp3" ) )
			return;
	}

	ASSERT ( m_HwMCI.DeviceIDValid () );
	m_HwMCI.Play ();
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow ( TRUE );
	
}

void CXuefengDlg::OnButtonRec() 
{
	// TODO: Add your control notification handler code here
	UpdateData ( TRUE );
	if(m_Channels.GetCurSel()==0)
	{
		m_nChannels=1;
	}
	else
	{
		m_nChannels=2;
	}
	if(m_SamplesPerSec.GetCurSel()==0)
	{
		m_nSamplesPerSec=11025;
	}
	else if(m_SamplesPerSec.GetCurSel()==1)
	{
		m_nSamplesPerSec=44100;
	}
	else
	{
		m_nSamplesPerSec=8000;
	}
	if(m_BitsPerSample.GetCurSel()==0)
	{
		m_wBitsPerSample=16;
	}
	else
	{
		m_wBitsPerSample=8;
	}

	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow ( TRUE );
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow ( TRUE );
	m_HwMCI.Rec ( m_nChannels,m_nSamplesPerSec,m_wBitsPerSample);
	
}

void CXuefengDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	if ( m_HwMCI.Save ( "test.wav" ) )
	{
		MessageBox ( "Save wave file to [test.wav] success" );
	}
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow ( FALSE );
	
}

void CXuefengDlg::OnButtonStop() 
{
	// TODO: Add your control notification handler code here
	m_HwMCI.Stop ();
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow ( FALSE );
	
}
