// DataRecvDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataRecv.h"
#include "DataRecvDlg.h"
#include ".\datarecvdlg.h"

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


// CDataRecvDlg 对话框



CDataRecvDlg::CDataRecvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataRecvDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataRecvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataRecvDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FILEMAP_READ, OnBnClickedButtonFilemapRead)
	ON_BN_CLICKED(IDC_BUTTON_CLIPOARD_READ, OnBnClickedButtonClipoardRead)
	ON_MESSAGE(WM_COMM, OnUserReceiveMsg)	
	ON_REGISTERED_MESSAGE(wm_nRegMsg,OnRegReceiveMsg) 
	ON_REGISTERED_MESSAGE(wm_nMemMsg,OnRegMemMsg) 
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CDataRecvDlg 消息处理程序

BOOL CDataRecvDlg::OnInitDialog()
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
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CDataRecvDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDataRecvDlg::OnPaint() 
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
HCURSOR CDataRecvDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataRecvDlg::OnBnClickedButtonFilemapRead()
{
	// TODO: 在此添加控件通知处理程序代码	
	HANDLE hMapping = NULL; 
	LPSTR lpData = NULL; 
	CString csFileMap = "";
	
	// 创建内存映像对象.
	hMapping = CreateFileMapping((HANDLE)0xFFFFFFFF,
		NULL, PAGE_READWRITE, 0, 0x100, "MYSHARE"); 
	if(hMapping==NULL) 
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

	// 给这段映像内存的数据赋给本地变量.
	csFileMap.Format("%s", lpData);

	// 释放映像内存.
	UnmapViewOfFile(lpData);

	SetDlgItemText(IDC_EDIT_FILEMAP, csFileMap);

	// 更新数据.
	UpdateData(FALSE);
}

void CDataRecvDlg::OnBnClickedButtonClipoardRead()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csClipBoard = "";
	// 打开系统剪贴板.
	if (!OpenClipboard()) return; 

	// 判断剪贴板上的数据是否是指定的数据格式.
	if (IsClipboardFormatAvailable(CF_TEXT)|| IsClipboardFormatAvailable(CF_OEMTEXT))
	{
		// 从剪贴板上获得数据.
		HANDLE hClipboardData = GetClipboardData(CF_TEXT);

		// 通过给内存句柄加锁，获得指向指定格式数据的指针.
		char *pchData = (char*)GlobalLock(hClipboardData);

		// 本地变量获得数据.
		csClipBoard = pchData;

		// 给内存句柄解锁.
		GlobalUnlock(hClipboardData);
	}
	else
	{ 
		AfxMessageBox("There is no text (ANSI) data on the Clipboard.");
	}

	// 使用完后关闭剪贴板.
	CloseClipboard();
	
	// 更新数据.
	SetDlgItemText(IDC_EDIT_CLIPBOARD, csClipBoard);
}

LRESULT CDataRecvDlg::OnUserReceiveMsg(WPARAM wParam,LPARAM lParam)
{
	CString csUserMsg = "";
	csUserMsg.Format("%d\n",int(lParam));
	SetDlgItemText(IDC_EDIT_USERMSG, csUserMsg);
	// 更新数据.
	UpdateData(FALSE);
	return 0;
}

LRESULT CDataRecvDlg::OnRegReceiveMsg(WPARAM wParam,LPARAM lParam)
{
	CString csRegMsg = "";
	csRegMsg.Format("%d\n", int(lParam));
	SetDlgItemText(IDC_EDIT_REGMSG, csRegMsg);
	UpdateData(FALSE);
	return 0;
}

BOOL CDataRecvDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString csCPDMsg = "";

	csCPDMsg = (LPSTR)pCopyDataStruct->lpData;

	// 获得实际长度的字符串.
	csCPDMsg = csCPDMsg.Left(pCopyDataStruct->cbData);
	SetDlgItemText(IDC_EDIT_COPYDATA, csCPDMsg);

	// 更新数据.
	UpdateData(FALSE);

	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

LRESULT CDataRecvDlg::OnRegMemMsg(WPARAM wParam,LPARAM lParam)
{
	char data[BUFFER_SIZE] = {0};
	CString csMem = "";
	LPVOID lpBaseAddress = (LPVOID)lParam;

	// 把字符串写入hProcess进程的内存.
	HANDLE hProcess = GetCurrentProcess();

	ReadProcessMemory(hProcess, lpBaseAddress, data, BUFFER_SIZE, NULL);

	csMem = data;

	SetDlgItemText(IDC_EDIT_MEM, csMem);

	// 更新数据.
	UpdateData(FALSE);
	return 0;
}

