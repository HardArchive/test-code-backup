// DataSendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataSend.h"
#include "DataSendDlg.h"
#include ".\datasenddlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUFFER_SIZE  0x100          // 用内存地址通信时分配的最大内存.
#define WM_COMM      WM_USER+100

const UINT wm_nRegMsg = RegisterWindowMessage("reg_data");
const UINT wm_nMemMsg = RegisterWindowMessage("mem_data");
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDataSendDlg 对话框



CDataSendDlg::CDataSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataSendDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pWnd = NULL;

}

void CDataSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataSendDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_USERMSG_SEND, OnBnClickedButtonUsermsgSend)
	ON_BN_CLICKED(IDC_BUTTON_REGMSG_SEND, OnBnClickedButtonRegmsgSend)
	ON_BN_CLICKED(IDC_BUTTON_COPYDATA_SEND, OnBnClickedButtonCopydataSend)
	ON_BN_CLICKED(IDC_BUTTON_MEM_SEND, OnBnClickedButtonMemSend)
	ON_BN_CLICKED(IDC_BUTTON_FILEMAP_SEND, OnBnClickedButtonFilemapSend)
	ON_BN_CLICKED(IDC_BUTTON_CLIPBOARD_SEND, OnBnClickedButtonClipboardSend)
END_MESSAGE_MAP()


// CDataSendDlg 消息处理程序

BOOL CDataSendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_pWnd = CWnd::FindWindow(NULL,_T("DataRecv")); // 查找DataRecv进程.
	if(m_pWnd == NULL)
	{
		AfxMessageBox(TEXT("Unable to find DataRecv."));
		//return;
	}
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CDataSendDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDataSendDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CDataSendDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataSendDlg::OnBnClickedButtonUsermsgSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT uMsg = 0;
	CString csUserMsg = "";
	GetDlgItemText(IDC_EDIT_USERMSG, csUserMsg);
	
	uMsg = atoi(csUserMsg);
	m_pWnd->SendMessage(WM_COMM, NULL, (LPARAM)uMsg);// 发送.

}

void CDataSendDlg::OnBnClickedButtonRegmsgSend()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT uMsg = 0;
	CString csRegMsg = "";
	GetDlgItemText(IDC_EDIT_REGMSG, csRegMsg);
	uMsg = atoi(csRegMsg);
	m_pWnd->SendMessage(wm_nRegMsg, NULL, (LPARAM)uMsg);// 发送.

}

void CDataSendDlg::OnBnClickedButtonCopydataSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csCPDMsg = "";
	COPYDATASTRUCT cpd = {0}; // 给COPYDATASTRUCT结构赋值.
	GetDlgItemText(IDC_EDIT_COPEDATA, csCPDMsg);
	cpd.dwData = 0;
	cpd.cbData = csCPDMsg.GetLength();
	cpd.lpData = (void*)csCPDMsg.GetBuffer(cpd.cbData);
	m_pWnd->SendMessage(WM_COPYDATA,NULL,(LPARAM)&cpd);// 发送.
}

void CDataSendDlg::OnBnClickedButtonMemSend()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD PID = 0;
	HANDLE hProcess = NULL;
	LPVOID lpBaseAddress = NULL;
	char  data[BUFFER_SIZE] = {0}; 
	CString csMemMsg = "";
	// 获取进程号.
	GetWindowThreadProcessId(m_pWnd->m_hWnd, (DWORD*)&PID );
	hProcess = OpenProcess (PROCESS_ALL_ACCESS,FALSE,PID);

	// 分配虚拟内存.
	lpBaseAddress = VirtualAllocEx(hProcess, 0, BUFFER_SIZE, 
		MEM_COMMIT, PAGE_READWRITE);		

    GetDlgItemText(IDC_EDIT_MEM, csMemMsg);
	strcpy(data, csMemMsg);

	// 把字符串写入hProcess进程的内存.
	WriteProcessMemory(hProcess, lpBaseAddress, data, BUFFER_SIZE, NULL);

	// 发送基址给DataRecv进程.
	m_pWnd->SendMessage(wm_nMemMsg, NULL, (LPARAM)lpBaseAddress);

	// 等待接收程序接收数据.
	Sleep(100);

	// 释放虚拟内存.
	VirtualFreeEx(hProcess, lpBaseAddress, 0, MEM_RELEASE);
}

void CDataSendDlg::OnBnClickedButtonFilemapSend()
{
	// TODO: 在此添加控件通知处理程序代码	
	HANDLE hMapping = NULL;   
	LPSTR lpData = NULL;
	CString csFileMap = "";
	
	// 创建内存映像对象.
	hMapping = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL,
		PAGE_READWRITE, 0, BUFFER_SIZE, "MYSHARE");   
	if(hMapping == NULL)   
	{   
		AfxMessageBox("CreateFileMapping() failed.");
		return;
	}

	// 将文件的视图映射到一个进程的地址空间上，返回LPVOID类型的内存指针.
	lpData = (LPSTR)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);   
	if(lpData == NULL)   
	{   
		AfxMessageBox("MapViewOfFile() failed.");
		return;
	}

	GetDlgItemText(IDC_EDIT_FILEMAP, csFileMap);

	// 给这段映像内存写数据.
	sprintf(lpData, csFileMap);   

	// 释放映像内存.
	UnmapViewOfFile(lpData);   
}

void CDataSendDlg::OnBnClickedButtonClipboardSend()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csClipBoard = "";
	//CString strData=m_strClipBoard;  // 获得数据.
	GetDlgItemText(IDC_EDIT_CLIPBOARD, csClipBoard);

	// 打开系统剪贴板.
	if (!OpenClipboard()) return;

	// 使用之前，清空系统剪贴板.
	EmptyClipboard();

	// 分配一内存，大小等于要拷贝的字符串的大小，返回的内存控制句柄.
	HGLOBAL hClipboardData;
	hClipboardData = GlobalAlloc(GMEM_DDESHARE, csClipBoard.GetLength()+1);

	// 内存控制句柄加锁，返回值为指向那内存控制句柄所在的特定数据格式的指针.
	char * pchData;
	pchData = (char*)GlobalLock(hClipboardData);

	// 将本地变量的值赋给全局内存.
	strcpy(pchData, LPCSTR(csClipBoard));

	// 给加锁的全局内存控制句柄解锁.
	GlobalUnlock(hClipboardData);

	// 通过全局内存句柄将要拷贝的数据放到剪贴板上.
	SetClipboardData(CF_TEXT,hClipboardData);

	// 使用完后关闭剪贴板.
	CloseClipboard();
}
