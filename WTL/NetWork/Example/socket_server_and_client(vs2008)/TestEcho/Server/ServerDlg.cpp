
// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"


// CServerDlg dialog

const LPCTSTR CServerDlg::ADDRESS = _T("0.0.0.0");
const USHORT CServerDlg::PORT = 5555;


CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServerDlg::IDD, pParent), m_Server(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INFO, m_Info);
	DDX_Control(pDX, IDC_START, m_Start);
	DDX_Control(pDX, IDC_STOP, m_Stop);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START, &CServerDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CServerDlg::OnBnClickedStop)
	ON_MESSAGE(USER_INFO_MSG, OnUserInfoMsg)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	CString strTitle;
	GetWindowText(strTitle);
	strTitle.Format(_T("%s - (%s:%d)"), strTitle, ADDRESS, PORT);
	SetWindowText(strTitle);

	::SetMainWnd(this);
	::SetInfoList(&m_Info);
	SetAppState(ST_STOPED);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerDlg::SetAppState(EnAppState state)
{
	m_enState = state;

	m_Start.EnableWindow(m_enState == ST_STOPED);
	m_Stop.EnableWindow(m_enState == ST_STARTED);
}

void CServerDlg::OnBnClickedStart()
{
	SetAppState(ST_STARTING);

	if(m_Server.Start(ADDRESS, PORT))
	{
		::LogServerStart(ADDRESS, PORT);
		SetAppState(ST_STARTED);
	}
	else
	{
		::LogServerStartFail(m_Server.GetLastError(), m_Server.GetLastErrorDesc());
		SetAppState(ST_STOPED);
	}
}

void CServerDlg::OnBnClickedStop()
{
	SetAppState(ST_STOPING);

	if(m_Server.Stop())
	{
		::LogServerStop();
		SetAppState(ST_STOPED);
	}
	else
	{
		ASSERT(FALSE);
	}
}

LRESULT CServerDlg::OnUserInfoMsg(WPARAM wp, LPARAM lp)
{
	info_msg* msg = (info_msg*)wp;

	::LogInfoMsg(msg);

	info_msg::Destruct(msg);

	return 0;
}

ISocketListener::EnHandleResult CServerDlg::OnSend(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	::PostOnSend(dwConnectionID, pData, iLength);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CServerDlg::OnReceive(DWORD dwConnectionID, const BYTE* pData, int iLength)
{
	::PostOnReceive(dwConnectionID, pData, iLength);

	if(m_Server.Send(dwConnectionID, pData, iLength))
		return ISocketListener::HR_OK;
	else
		return ISocketListener::HR_ERROR;
}

ISocketListener::EnHandleResult CServerDlg::OnClose(DWORD dwConnectionID)
{
	::PostOnClose(dwConnectionID);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CServerDlg::OnError(DWORD dwConnectionID, EnSocketOperation enOperation, int iErrorCode)
{
	::PostOnError(dwConnectionID, enOperation, iErrorCode);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CServerDlg::OnAccept(DWORD dwConnectionID)
{
	::PostOnAccept(dwConnectionID);
	return ISocketListener::HR_OK;
}

ISocketListener::EnHandleResult CServerDlg::OnServerShutdown()
{
	::PostOnShutdown();
	return ISocketListener::HR_OK;
}
