// ScanPortDlg.cpp 
//
//作者 何嘉宁

#include "stdafx.h"
#include "ScanPort.h"
#include "ScanPortDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//线程返回的错误代码，调试用
#define ERROR_CREATE_SOCKET		-1L
#define ERROR_MODIFY_FIONBIO	-2L
#define ERROR_SELECT			-3L
#define ERROR_SELECT_TIMEOUT	-4L

//线程发回的消息
#define SCAN_THREAD				WM_USER+100
//线程发回的消息的wParam参数定义
#define DOSCAN_FIND_PORT		1		//发现一个开放端口
#define DOSCAN_END_PORT			2		//完成一个端口扫描
#define STARTSCAN_COMPLETE		3		//完成所有扫描

//
#define MAX_SEPECIEFIED_PORTS	256		//对于扫描指定的端口的最大数

//全局变量
HWND g_hWnd = NULL;						//处理消息的窗口句柄
unsigned long g_ulAddr = INADDR_NONE;	//扫描的主机地址
DWORD g_dwTimeOut = 1000;				//连接超时时间，以ms计
bool g_bTerminate = false;				//是否用户发出结束扫描的标志
short g_nMaxThread = 200;				//最大允许的扫描线程数，经试验不宜大于200
short g_nThreadCount = 0;				//当前正在扫描的进程数
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

struct tag_PORTS
{
	BOOL bSepecifiedPort;
	union
	{
		struct
		{
			unsigned short iStartPort;
			unsigned short iEndPort;			
		};
		struct
		{
			unsigned short nCount;
			unsigned short nArrOfPorts[MAX_SEPECIEFIED_PORTS];
		};		
	};
};

bool IsIpString(const char* pszHostOrIp)
{
	if (strlen(pszHostOrIp) > 15)
	{
		return false;
	}
	
	//创建一个输入字符的副本，防止被修改
	char szHostOrIpCopy[16];
	strcpy(szHostOrIpCopy, pszHostOrIp);
	
	int nPart = 0;
	char* pszPart = strtok(szHostOrIpCopy, ".");
	
	while (pszPart != NULL)
	{
		for (unsigned int i=0; i<strlen(pszPart); i++)
		{
			if (pszPart[i]>'9' || pszPart[i]<'0')
			{
				return false;
			}
		}
		if (unsigned int(atoi(pszPart)) > 255)
		{
			return false;
		}
		nPart++;
		pszPart = strtok(NULL, ".");
	}	
	return nPart == 4;	
}

unsigned long GetAddr(const char* pszHostOrIp)
{
	if (*pszHostOrIp == '\0')
	{
		return inet_addr("127.0.0.1");
	}
	
	if (IsIpString(pszHostOrIp))
	{
		return inet_addr(pszHostOrIp);
	}
	
	hostent* pHostent = gethostbyname(pszHostOrIp);
	if (pHostent == NULL)
	{
		return INADDR_NONE;
	}
	return  (*(unsigned long*)*pHostent->h_addr_list);
}

DWORD WINAPI DoScanPort(LPVOID lpParam)
{
	DWORD dwRet;
	short nPort = *(short*)	lpParam;
	delete lpParam;
	
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
	{
		AfxMessageBox("创建套接字失败!");
		dwRet = ERROR_CREATE_SOCKET;
	}
	else
	{		
		unsigned long flag = 1; 	
		if ((ioctlsocket(sock, FIONBIO, &flag) != 0))
		{
			AfxMessageBox("未能改为非阻塞模式!");
			dwRet = ERROR_MODIFY_FIONBIO;
		}
		else
		{
			sockaddr_in severAddr;
			severAddr.sin_family = AF_INET;
			severAddr.sin_port = htons(nPort); 		
			severAddr.sin_addr.S_un.S_addr = g_ulAddr;
			connect(sock, (sockaddr*)&severAddr, sizeof(severAddr));
			
			struct fd_set mask;			
			FD_ZERO(&mask);
			FD_SET(sock, &mask);
			
			struct timeval timeout;
			timeout.tv_sec = g_dwTimeOut / 1000;
			timeout.tv_usec = g_dwTimeOut % 1000;
			
			switch(select(0, NULL, &mask, NULL, &timeout))
			{
			case -1:
				dwRet = ERROR_SELECT;
				break;
				
			case 0:
				dwRet = ERROR_SELECT_TIMEOUT;
				break;
				
			default:		
				dwRet = ERROR_SUCCESS;
			};
		}		
		closesocket(sock);
	}
	g_nThreadCount --;
	if (dwRet == ERROR_SUCCESS)
	{
		::SendMessage(g_hWnd, SCAN_THREAD, DOSCAN_FIND_PORT, nPort);
	}
	else
	{
		::SendMessage(g_hWnd, SCAN_THREAD, DOSCAN_END_PORT, nPort);
	}
	return dwRet;
}

DWORD WINAPI StartScan(LPVOID lpParam)
{	
	tag_PORTS* pScanParam = (tag_PORTS*)lpParam;
	
	DWORD dwThreadId;
	unsigned short i;
	
	if (pScanParam->bSepecifiedPort)
	{
		for(i=0; i<=pScanParam->nCount; i++)
		{
			if (g_bTerminate)	
			{
				break;	//用户已发出结束扫描命令
			}
			while(g_nThreadCount >= g_nMaxThread)
			{
				Sleep(10);
			}			
			
			if (CreateThread(NULL, 0, DoScanPort, (LPVOID)new short(pScanParam->nArrOfPorts[i]), 0, &dwThreadId) != NULL)
			{
				g_nThreadCount ++;
			}
		}	
	}
	else
	{		
		for(i=pScanParam->iStartPort; i<=pScanParam->iEndPort; i++)
		{
			if (g_bTerminate)	
			{
				break;	//用户已发出结束扫描命令
			}
			while(g_nThreadCount >= g_nMaxThread)
			{
				Sleep(10);
			}	
			
			
			if (CreateThread(NULL, 0, DoScanPort, (LPVOID)new short(i), 0, &dwThreadId) != NULL)
			{
				g_nThreadCount ++;
			}
		}
	}
	
	//等待各端口扫描线程结束
	while (g_nThreadCount > 0)
	{
		Sleep(50);
	}
	::SendMessage(g_hWnd, SCAN_THREAD, STARTSCAN_COMPLETE, 0);
	delete pScanParam;
	return ERROR_SUCCESS;
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
// CScanPortDlg dialog

CScanPortDlg::CScanPortDlg(CWnd* pParent /*=NULL*/)
: CDialog(CScanPortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScanPortDlg)
	m_csHostOrIp = _T("");
	m_bIsSpecifiedPort = FALSE;
	m_nMaxThread = 200;
	m_iStartPort = 1;
	m_dwTimeOut = 1000;
	m_iEndPort = 1024;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hThread = NULL;
}

void CScanPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScanPortDlg)
	DDX_Control(pDX, IDC_SPECIFIED_PORTS, m_ctrSpecifiedPorts);
	DDX_Control(pDX, IDC_IS_SPECIFIED_PORT, m_ctrCheck);
	DDX_Control(pDX, IDC_TREE, m_ctrResultTree);
	DDX_Control(pDX, IDC_SCAN, m_ctrBeginScan);
	DDX_Text(pDX, IDC_HOST_OR_IP, m_csHostOrIp);
	DDX_Check(pDX, IDC_IS_SPECIFIED_PORT, m_bIsSpecifiedPort);
	DDX_Text(pDX, IDC_MAX_THREAD, m_nMaxThread);
	DDV_MinMaxUInt(pDX, m_nMaxThread, 50, 200);
	DDX_Text(pDX, IDC_START_PORT, m_iStartPort);
	DDX_Text(pDX, IDC_TIME_OUT, m_dwTimeOut);
	DDV_MinMaxDWord(pDX, m_dwTimeOut, 0, 100000);
	DDX_Text(pDX, IDC_END_PORT, m_iEndPort);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScanPortDlg, CDialog)
//{{AFX_MSG_MAP(CScanPortDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_IS_SPECIFIED_PORT, OnIsSpecifiedPort)
ON_BN_CLICKED(IDC_SCAN, OnScan)
ON_WM_DESTROY()
ON_WM_TIMER()
	ON_EN_CHANGE(IDC_SPECIFIED_PORTS, OnChangeSpecifiedPorts)
	//}}AFX_MSG_MAP
ON_MESSAGE(SCAN_THREAD, OnScanThread)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScanPortDlg message handlers

BOOL CScanPortDlg::OnInitDialog()
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
	m_pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
	m_pProgressCtrl->SetStep(1);
	ReadDefaultPorts();
	m_csHostOrIp = "localhost";
	UpdateData(false);
	
	WSADATA wsaData;	
	BOOL ret = WSAStartup(MAKEWORD(2,2), &wsaData);
	if(ret != 0)
	{
		MessageBox("初始化网络协议失败!");
		return -1;
	}	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CScanPortDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CScanPortDlg::OnPaint() 
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

HCURSOR CScanPortDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CScanPortDlg::OnIsSpecifiedPort() 
{
	BOOL bCheck = m_ctrCheck.GetCheck();
	m_ctrSpecifiedPorts.EnableWindow(bCheck);
}

void CScanPortDlg::OnScan() 
{
	if (m_hThread == NULL)
	{
		UpdateData();
		
		if ((g_ulAddr = GetAddr(m_csHostOrIp)) == INADDR_NONE)
		{
			AfxMessageBox("无法找到主机");
			return;
		}
		
		m_nHasComplete = 0;
		m_dwPercentSeconds = 0;
		m_pProgressCtrl->SetPos(0);		
		m_hParentTree = m_ctrResultTree.InsertItem(m_csHostOrIp+CTime::GetCurrentTime().Format("---扫描时间%H:%M:%S"));
		
		g_bTerminate = false;
		g_dwTimeOut = m_dwTimeOut;
		g_hWnd = m_hWnd;
		g_nMaxThread = m_nMaxThread;
		g_nThreadCount = 0;
		
		tag_PORTS* pScanParam = new tag_PORTS;
		pScanParam->bSepecifiedPort = m_bIsSpecifiedPort;		
		if (m_bIsSpecifiedPort)
		{
			//指定端口
			short nCount = 0;
			char buff[10];
			for(int i=0; i<m_ctrSpecifiedPorts.GetLineCount(); i++)
			{
				int len = m_ctrSpecifiedPorts.GetLine(i, buff, 9);
				if (len != 0)
				{
					buff[len] = '\0';
					pScanParam->nArrOfPorts[nCount++]= atoi(buff);
				}
			}
			pScanParam->nCount = nCount;
			m_pProgressCtrl->SetRange(0, pScanParam->nCount);
		}
		else
		{		
			pScanParam->iStartPort = m_iStartPort;
			pScanParam->iEndPort = m_iEndPort;
			m_pProgressCtrl->SetRange(0, m_iEndPort-m_iStartPort+1);
		}
		
		DWORD dwThreadId;
		m_hThread = CreateThread(NULL, 0, StartScan, (LPVOID)pScanParam, 0, &dwThreadId);
		m_ctrBeginScan.SetWindowText("结束扫描");
		SetTimer(1, 100, NULL);
	}
	else if (!g_bTerminate)
	{
		g_bTerminate = TRUE;
		m_ctrBeginScan.SetWindowText("正在结束...");
	}
}

void CScanPortDlg::OnDestroy() 
{
	TerminateThread(m_hThread, 0);
	m_hThread = NULL;	
	KillTimer(1);
	SaveDefaultPorts();
	
	WSACleanup();
	CDialog::OnDestroy();	
}

void CScanPortDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		CString s;
		m_dwPercentSeconds++;
		s.Format("用时%3d.%1d秒", m_dwPercentSeconds/10, m_dwPercentSeconds%10);
		GetDlgItem(IDC_TIME)->SetWindowText(s);
		
		m_pProgressCtrl->SetPos(m_nHasComplete);
	}
	CDialog::OnTimer(nIDEvent);
}

LRESULT CScanPortDlg::OnScanThread(WPARAM wParam, LPARAM lParam)
{
	char buff[10];
	switch(wParam)
	{
	case DOSCAN_FIND_PORT:
		sprintf(buff, "%d", short(lParam));
		m_ctrResultTree.InsertItem(buff, m_hParentTree);
		m_ctrResultTree.Expand(m_hParentTree, TVE_EXPAND);
		
	case DOSCAN_END_PORT:
		m_nHasComplete++;
		break;
		
	case STARTSCAN_COMPLETE:
		m_hThread = NULL;
		m_ctrBeginScan.SetWindowText("开始扫描");
		KillTimer(1);
		AfxMessageBox("本次扫描结束");
		break;
		
	default:
		break;		
	}
	return 1;
}

BOOL CScanPortDlg::PreTranslateMessage(MSG* pMsg) 
{
	//处理回车键
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return 0;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CScanPortDlg::ReadDefaultPorts()
{	
	try
	{
		CStdioFile f("DefaultPorts.txt", CFile::modeRead|CFile::typeText);
		CString s, ss;
		while (f.ReadString(ss))
		{
			s += ss;
			s += "\r\n";
		}
		GetDlgItem(IDC_SPECIFIED_PORTS)->SetWindowText(s);
		f.Close();
	}
	catch(...)
	{
		CString s = "13\r\n37\r\n80\r\n123\r\n135\r\n139\r\n489\r\n1002";		
		GetDlgItem(IDC_SPECIFIED_PORTS)->SetWindowText(s);	
	}
}

void CScanPortDlg::SaveDefaultPorts()
{
	CStdioFile f("DefaultPorts.txt", CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	char buff[10];
	for(int i=0; i<m_ctrSpecifiedPorts.GetLineCount(); i++)
	{
		int len = m_ctrSpecifiedPorts.GetLine(i, buff, 9);
		if (len != 0)
		{
			buff[len] = '\0';			
			f.WriteString(buff);
			f.WriteString("\r\n");
		}
	}
	f.Close();	
}

void CScanPortDlg::OnChangeSpecifiedPorts() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}
