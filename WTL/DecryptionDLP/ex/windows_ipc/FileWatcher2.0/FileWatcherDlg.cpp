// FileWatcherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileWatcher.h"
#include "FileWatcherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CFileWatcherDlg dialog

CFileWatcherDlg::CFileWatcherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileWatcherDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileWatcherDlg)
	m_csStrPath = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileWatcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileWatcherDlg)
	DDX_Control(pDX, IDC_LIST_DATA, m_csListData);
	DDX_Control(pDX, IDC_EDIT_PATH, m_csEdtPath);
	DDX_Control(pDX, IDC_BTN_CTRL, m_csBtnCtrl);
	DDX_Text(pDX, IDC_EDIT_PATH, m_csStrPath);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileWatcherDlg, CDialog)
	//{{AFX_MSG_MAP(CFileWatcherDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_DATA, OnRclickListData)
	ON_COMMAND(IDR_MNU_OPEN, OnMnuOpen)
	ON_COMMAND(IDR_MNU_DELSEL, OnMnuDelsel)
	ON_COMMAND(IDR_MNU_DELALL, OnMnuDelall)
	ON_BN_CLICKED(IDC_BTN_CTRL, OnBtnCtrl)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileWatcherDlg message handlers

BOOL CFileWatcherDlg::OnInitDialog()
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
	
	//=======> 初始化标识符
	this->m_bStart = FALSE;
	//======> 初始化视图控件
	this->InitializeListCtrl();
	//======> 创建托盘图标
	this->NotifyTray(TRUE);
	//=====> 窗口置顶
	this->SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileWatcherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileWatcherDlg::OnPaint() 
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
HCURSOR CFileWatcherDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】初始化视图列表控件
//【参数】无
//【描述】创建栏目，设置扩展样式
void CFileWatcherDlg::InitializeListCtrl()
{
	this->m_csListData.InsertColumn(0, "时间", LVCFMT_LEFT, 75);
	this->m_csListData.InsertColumn(1, "动作", LVCFMT_LEFT, 50);
	this->m_csListData.InsertColumn(2, "类型", LVCFMT_LEFT, 75);
	this->m_csListData.InsertColumn(3, "信息", LVCFMT_LEFT, 500);

	//========> 设置扩展样式
	this->m_csListData.SetExtendedStyle(
		LVS_EX_GRIDLINES |
		LVS_EX_FULLROWSELECT |
		LVS_EX_FLATSB 
		);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】选择目录路径
//【参数】无
//【描述】调用SH函数，弹出目录选择对话框，选择目录
CString CFileWatcherDlg::SelectPath()
{
	BROWSEINFO stBrowseInfo;  
	ITEMIDLIST *lpstIDLst = NULL;
	char szPath[MAX_PATH] = {0};
   
	//===========> 初始化结构
	stBrowseInfo.hwndOwner = this->m_hWnd;  
	stBrowseInfo.pidlRoot = NULL;  
	stBrowseInfo.pszDisplayName = NULL;
	stBrowseInfo.lpszTitle = "请选择目录";  
	stBrowseInfo.ulFlags = BIF_RETURNONLYFSDIRS;  
	stBrowseInfo.lpfn = NULL;  
	stBrowseInfo.lParam = 0;  
	stBrowseInfo.iImage = 0;

	//=======> 弹出目录选择对话框
	if ((lpstIDLst = ::SHBrowseForFolder(&stBrowseInfo)) == FALSE)
	{
		return "";
	}
	//========> 取得目录路径
	if (::SHGetPathFromIDList(lpstIDLst, szPath) == FALSE)
	{
		return "";
	}

	return szPath;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】创建程序托盘
//【参数】bCreate 如果TRUE，创建托盘，反之销毁托盘
//【描述】为程序创建托盘，显示在任务栏下方
BOOL CFileWatcherDlg::NotifyTray(BOOL bCreate)
{
	NOTIFYICONDATA stNotify = {0};

	stNotify.cbSize = sizeof(NOTIFYICONDATA);
	stNotify.hIcon = this->m_hIcon;
	stNotify.hWnd = this->GetSafeHwnd();
	strcpy(stNotify.szTip, "目录监控程序（双击显示窗口）");
	stNotify.uCallbackMessage = WM_MY_NOTIFY;
	stNotify.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	stNotify.uID = IDR_MAINFRAME;

	if (bCreate == TRUE)
	{
		return ::Shell_NotifyIcon(NIM_ADD, &stNotify);
	}
	return ::Shell_NotifyIcon(NIM_DELETE, &stNotify);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】视图控件右击过程
//【参数】略
//【描述】右击的是否显示弹出菜单
void CFileWatcherDlg::OnRclickListData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	CMenu csMenu;
	POINT stPos = {0};

	//======> 取得当前鼠标位置
	::GetCursorPos(&stPos);

	//====> 加载资源菜单
	csMenu.LoadMenu(IDR_MNU_POP);
	//===========> 判断是否选中有项目
	if (this->m_csListData.GetSelectionMark() == -1)
	{
		csMenu.EnableMenuItem(IDR_MNU_OPEN, MF_GRAYED);
		csMenu.EnableMenuItem(IDR_MNU_DELSEL, MF_GRAYED);
	}
	//==========> 判断是否存在项目
	if (this->m_csListData.GetItemCount() == 0)
	{
		csMenu.EnableMenuItem(IDR_MNU_DELALL, MF_GRAYED);
	}
	//======> 弹出菜单
	csMenu.GetSubMenu(0)->TrackPopupMenu(
		TPM_LEFTALIGN,
		stPos.x,
		stPos.y,
		this);	
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】菜单“打开”过程
//【参数】略
//【描述】打开视图控件缓冲区中的路径
void CFileWatcherDlg::OnMnuOpen() 
{
	int nItem = 0;
	DWORD dwAct = 0;
	CString csStrShell, csStrInfo;
	HINSTANCE hInst = NULL;

	this->UpdateData();

	nItem = this->m_csListData.GetSelectionMark();
	dwAct = this->m_csListData.GetItemData(nItem);
	switch (dwAct)
	{
	case CWatcher::actRenameNew:
		{
			//=======> 取得内容
			csStrInfo = this->m_csListData.GetItemText(nItem, 3);
			//======== 计算路径
			csStrShell = 
				this->m_csStrPath + 
				"\\" + 
				csStrInfo.Mid(csStrInfo.Find(" -> ", 0), csStrInfo.GetLength());
		}
		break;
	case CWatcher::actRemote:
		{
			MessageBox("你会数据恢复吗?");
			// 返回
			return;
		}
		break;
	case CWatcher::actCreate:
	case CWatcher::actChange:
		{
			//======> 计算路径
			csStrShell = 
				this->m_csStrPath + 
				"\\" + 
				this->m_csListData.GetItemText(nItem, 3);
		}
		break;
	}		
	//======> 打开
	hInst = ::ShellExecute(NULL, 
		"open", 
		csStrShell,
		NULL,
		NULL,
		SW_SHOWNORMAL);
	if (hInst == NULL)
	{
		MessageBox("你当我是生产队?");
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】菜单“删除选择”过程
//【参数】略
//【描述】删除视图控件当前选中的项目
void CFileWatcherDlg::OnMnuDelsel() 
{
	this->m_csListData.DeleteItem(
		this->m_csListData.GetSelectionMark()
		);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】菜单“删除所有”过程
//【参数】略
//【描述】删除视图控件所有项目
void CFileWatcherDlg::OnMnuDelall() 
{
	this->m_csListData.DeleteAllItems();
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】开始\结束目录监视
//【参数】略
//【描述】调用CWatcher::Create开始创建目录监视
void CFileWatcherDlg::OnBtnCtrl() 
{
	this->UpdateData();

	if (this->m_bStart == FALSE)
	{
		if (this->m_csWatcher.Create(this->m_csStrPath) == FALSE)
		{
			this->MessageBox("监视目录失败");
		}
		else
		{
			this->m_bStart = TRUE;
			this->m_csEdtPath.EnableWindow(FALSE);
			this->m_csBtnCtrl.SetWindowText("停止");
		}
	}
	else
	{
		this->m_csWatcher.Destroy();
		this->m_bStart = FALSE;
		this->m_csEdtPath.EnableWindow(TRUE);
		this->m_csBtnCtrl.SetWindowText("开始");
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】未转换的消息处理过程
//【参数】略
//【描述】继承此虚拟函数来处理，在Edit里面的双击事件
BOOL CFileWatcherDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		if (pMsg->hwnd == this->m_csEdtPath.m_hWnd)
		{
			//=========> 判断是否已经开始
			if (this->m_bStart == FALSE)
			{
				//======> 取得选中目录纪路径
				this->m_csEdtPath.SetWindowText(this->SelectPath());
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】消息处理过程
//【参数】略
//【描述】继承此虚拟函数来处理，在托盘里面的双击事件
LRESULT CFileWatcherDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_MY_NOTIFY)
	{
		if (lParam == WM_LBUTTONDBLCLK)
		{
			//=====> 显示窗口
			ShowWindow(SW_RESTORE);
			//=====> 激活窗口
			SetForegroundWindow();
		}
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】程序退出过程
//【参数】略
//【描述】删除了调用基类的OnOK事件，以免按回车退出程序
void CFileWatcherDlg::OnOK()
{
	// CDialog::OnOK() 取消了此事件
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】程序退出过程
//【参数】略
//【描述】退出时候，弹出消息框，提示用户时候退出。销毁监视，删除托盘图标
void CFileWatcherDlg::OnCancel()
{
	if (::AfxMessageBox("你是否要退出程序?", MB_YESNO | MB_ICONSTOP) == IDYES)
	{
		this->NotifyTray(FALSE);
		this->m_csWatcher.Destroy();
		CDialog::OnCancel();
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】窗口尺寸改变过程
//【参数】略
//【描述】主要利用此过程完成程序最小化的时候隐藏
void CFileWatcherDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (nType == SIZE_MINIMIZED)
	{
		this->ShowWindow(SW_HIDE);
	}
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】判断是否是一个文件夹
//【参数】lpszPath 判断路径
//【描述】取得文件夹属性，然后，与 比较，返回
BOOL CFileWatcherDlg::IsFolder(LPCTSTR lpszPath)
{
	DWORD dwAttrib = 0;
	dwAttrib = GetFileAttributes(lpszPath);
	return (dwAttrib != -1) && 
		((dwAttrib & FILE_ATTRIBUTE_DIRECTORY)?TRUE:FALSE);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//【名称】监控过程
//【参数】dwAction 动作类型。csStrName1, csStrName2 相关名称
//【描述】处理动作，不要调用Destroy。来自CWatcher转发
void CFileWatcherDlg::OnWatch(DWORD dwAct, CString csStrName1, CString csStrName2)
{
	CString csStrTime, csStrAct, csStrType, 
		csStrPath , csStrInfo;
	int nItem = 0;

	//=======> 取得时间
	csStrTime = ::CTime::GetCurrentTime().Format("%H：%M：%S");
	//=========> 取得全路径
	csStrPath = this->m_csStrPath + "\\" + csStrName1; // Only 1
	//========> 判断类型，生成字符串
	if (this->IsFolder(csStrPath) == TRUE)
	{
		csStrType = "文件夹";
	}
	else
	{
		csStrType = "文件";
	}

	//==========> 判断类型
	switch (dwAct)
	{
	case CWatcher::actCreate:
		{
			csStrAct = "创建";
			csStrInfo = csStrName1;
		}
		break;
	case CWatcher::actRemote:
		{
			csStrAct = "删除";
			csStrInfo = csStrName1;
		}
		break;
	case CWatcher::actRenameNew:
		{
			csStrAct = "重命";
			csStrInfo = csStrName1 + " -> " + csStrName2;
		}
		break;
	case CWatcher::actChange:
		{
			csStrAct = "更改";
			csStrInfo = csStrName1;
		}
		break;
	}
	
	//============> 添加信息到视图控件
	nItem = this->m_csListData.InsertItem(0, csStrTime);
	this->m_csListData.SetItemText(nItem, 1, csStrAct);
	this->m_csListData.SetItemText(nItem, 2, csStrType);
	this->m_csListData.SetItemText(nItem, 3, csStrInfo);
	//=========> 保存动作到控件内部 
	this->m_csListData.SetItemData(nItem, dwAct);
}

