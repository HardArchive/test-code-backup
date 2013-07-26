// PiPeServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PiPeServer.h"
#include "PiPeServerDlg.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

bool bLoop=false;
HANDLE g_hPipe = NULL;
HWND g_hWnd = NULL;
HANDLE ThreadHanlde;
DWORD _stdcall PipeFunc(LPVOID Param);


DWORD _stdcall ListenProc(LPVOID Param)
{
	SECURITY_ATTRIBUTES sa;
	SECURITY_DESCRIPTOR * PSD;
	PSD=(SECURITY_DESCRIPTOR *)LocalAlloc(LPTR,SECURITY_DESCRIPTOR_MIN_LENGTH);
	InitializeSecurityDescriptor(PSD,SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(PSD,true,NULL,false);
	sa.nLength=sizeof(sa);
	sa.bInheritHandle=true;
	sa.lpSecurityDescriptor=PSD;

	while(bLoop)
	{
		//创建一个有名管道
		HANDLE hPipe=CreateNamedPipe("\\\\.\\pipe\\mynamedpipe",
			PIPE_ACCESS_DUPLEX ,
			PIPE_TYPE_MESSAGE |
			PIPE_READMODE_MESSAGE |
			PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			4096,
			4096,
			INFINITE,
			&sa
			);
		g_hPipe = hPipe;
		if(hPipe==INVALID_HANDLE_VALUE)
		{
			TRACE("\nCreate Named Pipe Error....\n");
			::MessageBox(NULL, "Create Named Pipe Error....", "123", MB_OK);
			return 0L; 
		}
		else
		{
			TRACE("\nCreate Named Pipe Success.....\n");
		}
		//侦听
		int fConnect =ConnectNamedPipe(hPipe, NULL);
		if(fConnect || GetLastError()==ERROR_PIPE_CONNECTED)
		{
			HANDLE hThread;
			DWORD dwThreadID;
			//创建一个新的线程用于和客户端的交互
			hThread=CreateThread(NULL,
				0,
				(LPTHREAD_START_ROUTINE)PipeFunc,
				(LPVOID)hPipe,
				0,
				&dwThreadID);
			if(hThread==INVALID_HANDLE_VALUE)
			{
				TRACE("\nCreate Thread Faild...\n");		
				::MessageBox(NULL, "Create Thread Faild...", "123", MB_OK);
				return 0L;
			}
			else
			{
				TRACE("\nOne Client Is Connected .... HANDLE :%d\n",hPipe);
			}

		}
		else
		{
			::MessageBox(NULL, "ConnectNamePipe Faild ...", "123", MB_OK);
			CloseHandle(hPipe);
			TRACE("\nConnectNamePipe Faild ...\n");
			return 0L;
		}
	}
	LocalFree(PSD);
	TRACE("\nNamed Pipe Listen Thread Is Stoped....\n");
	return 0L;
}
DWORD _stdcall PipeFunc(LPVOID Param)
{
	HANDLE hPipe=(HANDLE) Param;
	static int idx=0;
	TCHAR szBuf[BUFSIZ];
	DWORD dwRead,dwWrite;
	int  fWrite,fRead;
	while(true)
	{
		DWORD tStart=timeGetTime();
		memset(szBuf, 0, sizeof(TCHAR)*BUFSIZ);
		fRead=ReadFile(hPipe,
			szBuf,
			BUFSIZ,
			&dwRead,
			NULL);	

		if(!fRead || dwRead==0)
		{
			TRACE("\nReadFile Faild ...\n");
			return 0L;
		}
		else
		{
			COPYDATASTRUCT cpd = {0}; // 给COPYDATASTRUCT结构赋值.
			cpd.dwData = 0;
			cpd.cbData = _tcslen(szBuf);
			cpd.lpData = (PVOID)szBuf;
			SendMessage(g_hWnd, WM_COPYDATA, NULL , (LPARAM)&cpd);// 发送.

			fWrite = WriteFile(hPipe,
				szBuf,
				dwRead,
				&dwWrite,
				NULL);
			if(!fWrite || dwWrite==0)
			{
				TRACE("\nWrite File Faild ...\n");
			}
		}
		DWORD tStop=timeGetTime();
		TRACE("\nOnce  alternation  Complete With Time : %d\n",tStop-tStart);
		FlushFileBuffers(hPipe);
	}
	DisconnectNamedPipe(hPipe);
	::MessageBox(NULL, "DisconnectNamedPipe", "123", MB_OK);
	CloseHandle(hPipe);
	return NO_ERROR;
}

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
// CPiPeServerDlg dialog

CPiPeServerDlg::CPiPeServerDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPiPeServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPiPeServerDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPiPeServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPiPeServerDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPiPeServerDlg, CDialog)
	//{{AFX_MSG_MAP(CPiPeServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_STOP, OnStop)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CPiPeServerDlg::OnBnClickedButtonSend)
	ON_WM_COPYDATA()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPiPeServerDlg message handlers

BOOL CPiPeServerDlg::OnInitDialog()
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
	g_hWnd = this->m_hWnd;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPiPeServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPiPeServerDlg::OnPaint() 
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
HCURSOR CPiPeServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPiPeServerDlg::OnOK() 
{
}

void CPiPeServerDlg::OnCancel() 
{
}

void CPiPeServerDlg::OnExit() 
{
	CDialog::OnOK();

}

void CPiPeServerDlg::OnClose() 
{
	OnExit();
}

void CPiPeServerDlg::OnStart() 
{
	bLoop=true;
	DWORD dwThreadID;
	ThreadHanlde=CreateThread(NULL,
		0,
		(LPTHREAD_START_ROUTINE)ListenProc,
		NULL,
		0,
		&dwThreadID);
	if(ThreadHanlde==INVALID_HANDLE_VALUE)
	{
		TRACE("\nStart Listen Thread Faild....\n");
	}
	else
	{
		TRACE("\nName Pipe Listen Thread Is Staring....\n");
	}	
}

void CPiPeServerDlg::OnStop() 
{
	bLoop=false;
	if(TerminateThread(ThreadHanlde,0L))
		TRACE("\nStoped Listen Thread....\n");
	else
		TRACE("\nCan not Stop Listen Thread....\n");
}

void CPiPeServerDlg::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR tszTem[MAX_PATH] = {0};
	GetDlgItemText(IDC_EDIT_SEND_SHOW, tszTem, MAX_PATH);

	DWORD dwWrite = WriteFile(g_hPipe,
		tszTem,
		_tcslen(tszTem),
		&dwWrite,
		NULL);
}

BOOL CPiPeServerDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//PCOPYDATASTRUCT pstuCPD = (PCOPYDATASTRUCT)wParam; // 给COPYDATASTRUCT结构赋值.
	SetDlgItemText(IDC_STATIC_RECV_SHOW, (PTCHAR)pCopyDataStruct->lpData);
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}
