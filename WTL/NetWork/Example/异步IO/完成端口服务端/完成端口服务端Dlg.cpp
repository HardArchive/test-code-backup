// 完成端口服务端Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "完成端口服务端.h"
#include "完成端口服务端Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define MSGSIZE 1024

typedef enum//枚举 状态
{
  RECV_POSTED
}OPERATION_TYPE;

typedef struct//扩展重叠结构
{
 WSAOVERLAPPED  overlap;
 WSABUF         Buffer;
 char           szMessage[MSGSIZE];
 DWORD          NumberOfBytesRecvd;
 DWORD          Flags;
 OPERATION_TYPE OperationType;
}PER_IO_OPERATION_DATA, *LPPER_IO_OPERATION_DATA;

struct CompletionPortAndMyHwnd 
{//目的是在工作线程中得到窗口指针
	HANDLE CompletionPort;
	CMyDlg* my_win;
}compleportandHwnd;

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
// CMyDlg dialog

CMyDlg::CMyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDlg, CDialog)
	//{{AFX_MSG_MAP(CMyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlg message handlers

BOOL CMyDlg::OnInitDialog()
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMyDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyDlg::OnPaint() 
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
HCURSOR CMyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//*********************************************************************************
BOOL CMyDlg::InItSocket()//装载套接字库
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

void CMyDlg::OnButton1() 
{
	HANDLE m_hConnectThread=CreateThread(NULL,0,ThreadFunc,this,0,NULL);
	//此处this是传过去的本窗口指针 用于取一些值或是控制控件		
}
DWORD WINAPI ThreadFunc(LPVOID pParam)
{
	CMyDlg *pDlg=(CMyDlg *)pParam;//取出传过来的窗口指针
	ASSERT(pDlg != NULL);//抛出异常
	//***********************************************
	
	DWORD    dwThreadId;
	int    iaddrSize = sizeof(SOCKADDR_IN);//SOCKADDR_IN结构大小
	//HANDLE  CompletionPort = INVALID_HANDLE_VALUE;//完成端口
	SYSTEM_INFO     systeminfo;//系数信息，主要是用于取得CPU数量
	LPPER_IO_OPERATION_DATA lpPerIOData = NULL;//扩展重叠结构
	
	//创建一个完成端口，这里只是为了得到它的句柄，后面再和套接字关联起来
	compleportandHwnd.CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	compleportandHwnd.my_win=pDlg;//传到工作线程中的窗口指针
		//创建工作者线程
	GetSystemInfo(&systeminfo);
	for (unsigned int i=0;i<systeminfo.dwNumberOfProcessors;i++)//以CPU的数量来创建
	{
		CreateThread(NULL, 0, WorkerThread, &compleportandHwnd, 0, &dwThreadId);
		//把完成端口句柄传过去附带了窗口指针
	}	
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
	
	while (TRUE)
	{
		SOCKADDR_IN sCilentaddr;
		int len=sizeof(struct sockaddr);
		//接受连接
		pDlg->sClient = accept(pDlg->sSever, (struct sockaddr *)&sCilentaddr, &len);
		CString cilentIPstr=inet_ntoa(sCilentaddr.sin_addr);//连接IP
		CString cstr;
		cstr.Format("IP:%s连上来了！",cilentIPstr);
		pDlg->SetDlgItemText(IDC_EDIT1,cstr);//简单显示  
		//保存到动态数组
		pDlg->cilentin.cinsock=pDlg->sClient;//套接字
        pDlg->cilentin.cinIP= cilentIPstr;
		pDlg->m_Cilentinfo.Add(pDlg->cilentin);	//加入动态数组中
		
		//新接受的连接套接字关联到完成端口
		CreateIoCompletionPort((HANDLE)pDlg->sClient,compleportandHwnd.CompletionPort, (DWORD)pDlg->sClient, 0);
		//激活一个WSARecv异步操作
		lpPerIOData = (LPPER_IO_OPERATION_DATA)HeapAlloc(
			GetProcessHeap(),
			HEAP_ZERO_MEMORY,
			sizeof(PER_IO_OPERATION_DATA));//重叠结构申请内存
		lpPerIOData->Buffer.len = MSGSIZE;
		lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
		lpPerIOData->OperationType = RECV_POSTED;
		WSARecv(pDlg->sClient,
			&lpPerIOData->Buffer,
			1,
			&lpPerIOData->NumberOfBytesRecvd,
			&lpPerIOData->Flags,
			&lpPerIOData->overlap,
			NULL);
	}
	
__Error_End:
	//向完成端口中的线程发出退出信号
	PostQueuedCompletionStatus(compleportandHwnd.CompletionPort, 0xFFFFFFFF, 0, NULL);
	closesocket(pDlg->sSever);
	CloseHandle(compleportandHwnd.CompletionPort);//关闭完成端口
	WSACleanup();//拆卸套接字
	return TRUE;
}
//
DWORD WINAPI WorkerThread(LPVOID CompletionPortID)//工作者线程
{
  HANDLE CompletionPort=(HANDLE)compleportandHwnd.CompletionPort;//取出完成端口句柄 
  CMyDlg* pDlg=compleportandHwnd.my_win;//取出窗口指针
  SOCKET   sClient;
  DWORD  dwBytesTransferred;//重叠结构中的字节数  
  LPPER_IO_OPERATION_DATA lpPerIOData = NULL;

  while (TRUE)
  {
	  //取完成端口状态
	  GetQueuedCompletionStatus(CompletionPort,&dwBytesTransferred,(unsigned long *)&sClient,
		  (LPOVERLAPPED *)&lpPerIOData,INFINITE);
	  int i=0;
	  if (dwBytesTransferred == 0xFFFFFFFF)//如果是退出的信号,由PostQueuedCompletionStatus发出的
	  {		  
		  return TRUE;//返回
	  }
	  if (lpPerIOData->OperationType == RECV_POSTED)//重叠结构中的枚举值
	  {
		  if (dwBytesTransferred == 0)
		  {
			  //客户端关闭			  
			  for (i=0;i<pDlg->m_Cilentinfo.GetSize();i++)//以套接字为标，找出动态数组相应IP
			  {
				  if (sClient==pDlg->m_Cilentinfo.GetAt(i).cinsock)
				  {
					  break;
				  }
			  }          
			  CString cip;
			  cip.Format("客户%s离开",pDlg->m_Cilentinfo.GetAt(i).cinIP);
			  pDlg->m_Cilentinfo.RemoveAt(i);//从动态数组中删除这个 节点
 			  pDlg->SetDlgItemText(IDC_EDIT1,cip);

			  closesocket(sClient);//关闭客户端套接字
			  HeapFree(GetProcessHeap(),0,lpPerIOData); //释放这前申请的内存       
		  }else
		  {
			  //**********************************************************************
			  //处理客户端请求,可以起个线程什么的用于传文件之类的
			  lpPerIOData->szMessage[dwBytesTransferred] = '\0';//客户端发过来的信息
			  for (i=0;i<pDlg->m_Cilentinfo.GetSize();i++)//以套接字为标，找出动态数组相应IP
			  {
				  if (sClient==pDlg->m_Cilentinfo.GetAt(i).cinsock)
				  {
					  break;
				  }
			  }         
			  CString sMsg;
			  sMsg.Format("%s:%s",pDlg->m_Cilentinfo.GetAt(i).cinIP,lpPerIOData->szMessage);
			  pDlg->SetDlgItemText(IDC_EDIT1,sMsg);
			  for (i=0;i<pDlg->m_Cilentinfo.GetSize();i++)//回传给所有动态数组中的客户
			  {
				  send(pDlg->m_Cilentinfo.GetAt(i).cinsock, sMsg, strlen(sMsg), 0);
			  }				  
			  //**********************************************************************  
			  
			  //重新激活另一个WSARecv异步操作
			  memset(lpPerIOData, 0, sizeof(PER_IO_OPERATION_DATA));
			  lpPerIOData->Buffer.len = MSGSIZE;
			  lpPerIOData->Buffer.buf = lpPerIOData->szMessage;
			  lpPerIOData->OperationType = RECV_POSTED;
			  WSARecv(sClient,
				  &lpPerIOData->Buffer,
				  1,
				  &lpPerIOData->NumberOfBytesRecvd,
				  &lpPerIOData->Flags,
				  &lpPerIOData->overlap,
				  NULL);
		  }
	}
  }
  return TRUE;
 }

void CMyDlg::OnButton2() //发送
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
