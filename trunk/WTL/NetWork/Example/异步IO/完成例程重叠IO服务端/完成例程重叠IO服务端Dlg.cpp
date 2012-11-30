// 完成例程重叠IO服务端Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "完成例程重叠IO服务端.h"
#include "完成例程重叠IO服务端Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct//扩展重叠结构 尾随数据
{
	WSAOVERLAPPED overlap;//原重叠结构	
	WSABUF        Buffer;
	char          szMessage[MAX_BUF_SIZE];//数据接收缓冲
	DWORD         NumberOfBytesRecvd;
	DWORD         Flags; 
	SOCKET        sClient;
	CIODlg *      my_win;	//窗口指针
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;
SOCKET g_sNewClientConnection;
BOOL   g_bNewConnectionArrived;//新连接到达

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
// CIODlg dialog

CIODlg::CIODlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIODlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIODlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIODlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIODlg, CDialog)
	//{{AFX_MSG_MAP(CIODlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIODlg message handlers

BOOL CIODlg::OnInitDialog()
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
	g_bNewConnectionArrived=FALSE;
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIODlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIODlg::OnPaint() 
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
HCURSOR CIODlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//********************************************************************************
BOOL CIODlg::InItSocket()//装载套接字库
{
	WSADATA data = {0};
	if(WSAStartup(MAKEWORD(2, 2), &data))//2.2的套接字
		return FALSE;
	if ( LOBYTE(data.wVersion) !=2 || HIBYTE(data.wVersion) != 2 ){
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}
void CIODlg::OnButton2() //开启服务端
{
	HANDLE m_hConnectThread=CreateThread(NULL,0,ThreadFunc,this,0,NULL);
	//此处this是传过去的本窗口指针 用于取一些值或是控制控件	
}

DWORD WINAPI ThreadFunc(LPVOID pParam)
{
	CIODlg *pDlg=(CIODlg *)pParam;//取出传过来的窗口指针
	ASSERT(pDlg != NULL);//抛出异常
//***********************************************	
	//装载socket库
	if (!pDlg->InItSocket())
	{
		AfxMessageBox("装载套接字库出错！");
		return FALSE;
	}
	//
	pDlg->sSever=socket(AF_INET , SOCK_STREAM , IPPROTO_TCP); //创建套接字
    if (INVALID_SOCKET==pDlg->sSever)
    {
		AfxMessageBox(_T("创建套接字失败！"));
		return FALSE;
    }
	//
	sockaddr_in serve;
	serve.sin_family=AF_INET;
	serve.sin_addr.s_addr = INADDR_ANY;
	serve.sin_port=htons(1125);
	if (SOCKET_ERROR==bind( pDlg->sSever,(sockaddr*)&serve,sizeof(sockaddr_in)))
	{
		AfxMessageBox(_T("绑定端口失败！"));
		goto __Error_End;
	}
	//
	if (SOCKET_ERROR==listen(pDlg->sSever,5))
    {
		AfxMessageBox(_T("监听失败！"));
		goto __Error_End;
    }
	//***************************
	pDlg->SetDlgItemText(IDC_EDIT1,"服务端开启成功!");

	CreateThread(NULL, 0, WorkerThread,pParam, 0, NULL); //工作者线程 传窗口指针
	
	while (TRUE)
	{
		SOCKADDR_IN sCilentaddr;
		int len=sizeof(struct sockaddr);
		//接受连接
		g_sNewClientConnection = accept(pDlg->sSever, (struct sockaddr *)&sCilentaddr, &len);		
		CString cilentIPstr=inet_ntoa(sCilentaddr.sin_addr);//连接IP

        pDlg->cilentin.cinsock=g_sNewClientConnection;//加入动态数组中
		pDlg->cilentin.cinIP=cilentIPstr;
		pDlg->m_Cilentinfo.Add(pDlg->cilentin);	
		
		g_bNewConnectionArrived=TRUE;//有新的连接,置状态

		CString cstr;
		cstr.Format("IP:%s连上来了！",cilentIPstr);
		pDlg->SetDlgItemText(IDC_EDIT1,cstr);//简单显示   
	}
	
__Error_End:
	closesocket(pDlg->sSever);
	return TRUE;
}
DWORD WINAPI WorkerThread(LPVOID lpParameter)
{
	CIODlg *pDlg=(CIODlg *)lpParameter;//取出传过来的窗口指针
	
	LPPER_IO_OPERATION_DATA lpPerIOData = NULL;
    while (TRUE)//无限循环
	{
		if (g_bNewConnectionArrived)//处理新连接为真时的情况 当有一个新连接时
		{
			lpPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(
				GetProcessHeap(),
				HEAP_ZERO_MEMORY,
				sizeof(PER_IO_OPERATION_DATA));//申请重叠结构的内存

			lpPerIOData->Buffer.len=MAX_BUF_SIZE;
			lpPerIOData->Buffer.buf=lpPerIOData->szMessage;
			lpPerIOData->sClient=g_sNewClientConnection;//新连接套接字放到重叠结构中
			lpPerIOData->my_win=pDlg;//传窗口指针		
						
			//关键 WSARecv把套接字和完成例程关联起来			
			WSARecv(lpPerIOData->sClient,
				&lpPerIOData->Buffer,
				1,
				&lpPerIOData->NumberOfBytesRecvd,
				&lpPerIOData->Flags,
				&lpPerIOData->overlap,
				CompletionROUTINE); 	
			
			g_bNewConnectionArrived= FALSE;//重置新连接为假以接收下一个          
			
		}
		SleepEx(1000,TRUE);//进入可随时唤醒状态		
	}	
	return TRUE;
}

//完成例程
void CALLBACK CompletionROUTINE(DWORD dwError,DWORD cbTransferred,LPWSAOVERLAPPED lpOverlapped,DWORD dwFlags)
{
	LPPER_IO_OPERATION_DATA lpPerIOData=(LPPER_IO_OPERATION_DATA)lpOverlapped;//取出传过来的结构
	int i=0;

	if (dwError != 0 || cbTransferred == 0)
	{
		// Connection was closed by client
		//客户端关闭套接字
		for (i=0;i<lpPerIOData->my_win->m_Cilentinfo.GetSize();i++)//以套接字为标，找出动态数组相应IP
		{
			if (lpPerIOData->sClient==lpPerIOData->my_win->m_Cilentinfo.GetAt(i).cinsock)
			{
				break;
			}
		}          
		CString cip;
	    cip.Format("客户%s离开",lpPerIOData->my_win->m_Cilentinfo.GetAt(i).cinIP);
		lpPerIOData->my_win->m_Cilentinfo.RemoveAt(i);//从动态数组中删除这个 节点		
		lpPerIOData->my_win->SetDlgItemText(IDC_EDIT1,cip);
		
		closesocket(lpPerIOData->sClient);//关闭这个套接字
		HeapFree(GetProcessHeap(), 0, lpPerIOData);//释放申请的扩展重叠结构内存
	}
	else
	{		
		//********************************************************************
		//主处理客户端信息 可以在这里开出一个线程，比如用于传文件
		lpPerIOData->szMessage[cbTransferred] = '\0';//接收到的信息
			
		//传回给客户端
		for (i=0;i<lpPerIOData->my_win->m_Cilentinfo.GetSize();i++)//以套接字为标，找出动态数组相应IP
		{
			if (lpPerIOData->sClient==lpPerIOData->my_win->m_Cilentinfo.GetAt(i).cinsock)
			{
				break;
			}
		}   
		CString sMsg;
		sMsg.Format("%s:%s",lpPerIOData->my_win->m_Cilentinfo.GetAt(i).cinIP,lpPerIOData->szMessage);
		lpPerIOData->my_win->SetDlgItemText(IDC_EDIT1,sMsg);
		
		for (i=0;i<lpPerIOData->my_win->m_Cilentinfo.GetSize();i++)//回传给所有动态数组中的客户
		{
			send(lpPerIOData->my_win->m_Cilentinfo.GetAt(i).cinsock, sMsg, strlen(sMsg), 0);
		}		
		//********************************************************************    
		// Launch another asynchronous operation
		//重新激活另一个WSARecv异步操作
		//等下一个信息
		memset(&lpPerIOData->overlap, 0, sizeof(WSAOVERLAPPED));//清空overlap数据
		lpPerIOData->Buffer.len = MAX_BUF_SIZE;
		lpPerIOData->Buffer.buf = lpPerIOData->szMessage;    
		WSARecv(lpPerIOData->sClient,
			&lpPerIOData->Buffer,
			1,
			&lpPerIOData->NumberOfBytesRecvd,
			&lpPerIOData->Flags,
			&lpPerIOData->overlap,
			CompletionROUTINE);
	}	
}

void CIODlg::OnButton1() //发送
{
	if (m_Cilentinfo.GetSize()>0)
	{		
		CString strMsgt;
		GetDlgItemText(IDC_EDIT2,strMsgt);
		SetDlgItemText(IDC_EDIT2,"");
		CString strMsg;
		strMsg.Format("Sever:%s",strMsgt);
		for (int i=0;i<m_Cilentinfo.GetSize();i++)//给动态数组中的套接字每个发
		{
			send(m_Cilentinfo.GetAt(i).cinsock, strMsg, strlen(strMsg), 0);
		}
	}
}
