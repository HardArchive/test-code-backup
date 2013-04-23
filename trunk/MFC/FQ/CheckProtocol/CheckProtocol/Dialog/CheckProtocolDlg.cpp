
// CheckProtocolDlg.cpp : 实现文件

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "CheckProtocolDlg.h"
#include "AddPackRuleDlg.h"
#include "AddWebRuleDlg.h"
#include "LoginTestDlg.h"
#include "FindCaptureRuleDlg.h"
#include "DownloadDlg.h"
#include "UpdataDownload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CCheckProtocolDlg 对话框
CCheckProtocolDlg::CCheckProtocolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckProtocolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckProtocolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCheckProtocolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(IDR_MENU_PACKFILE, &CCheckProtocolDlg::OnMenuPackfile)
	ON_COMMAND(IDR_MENU_WEBFILE, &CCheckProtocolDlg::OnMenuWebfile)
	ON_COMMAND(IDR_MENU_LOGIN_TEST, &CCheckProtocolDlg::OnMenuLoginTest)
	ON_COMMAND(IDR_MENU_SEND_TEST, &CCheckProtocolDlg::OnMenuSendTest)
	ON_COMMAND(IDR_MENU_FIND_PACP, &CCheckProtocolDlg::OnMenuFindPacp)
	ON_COMMAND(ID_MENU_DOWNLOAD, &CCheckProtocolDlg::OnMenuDownload)
	ON_COMMAND(ID_MENU_UPDATA_DOWNLOAD, &CCheckProtocolDlg::OnMenuUpdataDownload)
	ON_COMMAND(ID_MENU_VOICE, &CCheckProtocolDlg::OnMenuVoice)
END_MESSAGE_MAP()


// CCheckProtocolDlg 消息处理程序

BOOL CCheckProtocolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
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
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP1);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CCheckProtocolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCheckProtocolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCheckProtocolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CCheckProtocolDlg::OnMenuPackfile()
{
	// TODO: 在此添加命令处理程序代码
	CAddPackRuleDlg PackDlg;
	PackDlg.DoModal();
}

void CCheckProtocolDlg::OnMenuWebfile()
{
	// TODO: 在此添加命令处理程序代码
	CAddWebRuleDlg Webdlg;
	Webdlg.DoModal();
}

void CCheckProtocolDlg::OnMenuLoginTest()
{
	// TODO: 在此添加命令处理程序代码
	CLoginTestDlg loginDlg;
	loginDlg.DoModal();
}

void CCheckProtocolDlg::OnMenuSendTest()
{
	// TODO: 在此添加命令处理程序代码
// 	CSendTestDlg sendDlg;
// 	sendDlg.DoModal();
}

void CCheckProtocolDlg::OnMenuFindPacp()
{
	// TODO: 在此添加命令处理程序代码
	CFindCaptureRuleDlg findRuleDlg;
	findRuleDlg.DoModal();
}

void CCheckProtocolDlg::OnMenuDownload()
{
	// TODO: 在此添加命令处理程序代码
	CDownloadDlg  DLdlg;
	DLdlg.DoModal();
}	

void CCheckProtocolDlg::OnMenuUpdataDownload()
{
	// TODO: 在此添加命令处理程序代码
	CUpdataDownload UpdataDlg;
	UpdataDlg.DoModal();
}

void CCheckProtocolDlg::OnMenuVoice()
{
	// TODO: 在此添加命令处理程序代码
	
}
