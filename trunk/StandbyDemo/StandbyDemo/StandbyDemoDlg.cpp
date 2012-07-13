
// StandbyDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StandbyDemo.h"
#include "StandbyDemoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "TipDialog.h"
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


// CStandbyDemoDlg 对话框




CStandbyDemoDlg::CStandbyDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStandbyDemoDlg::IDD, pParent)
{
	hTimer = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStandbyDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStandbyDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CHILDACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CStandbyDemoDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CStandbyDemoDlg 消息处理程序

BOOL CStandbyDemoDlg::OnInitDialog()
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
	m_pTipDialog = NULL;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStandbyDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CStandbyDemoDlg::OnPaint()
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
HCURSOR CStandbyDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CStandbyDemoDlg::OnChildActivate()
{
	CDialog::OnChildActivate();

	// TODO: 在此处添加消息处理程序代码
}

LRESULT CStandbyDemoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_POWERBROADCAST == message)
	{
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:
			TRACE("PBT_APMQUERYSUSPEND_待机前提示消息_需要经许可才能挂起的事件//不支持WIN7");
			OutputDebugString("需要经许可才能挂起的事件消息通知-可等待定时器……\r\n");
			::OutputDebugString( _T( "准备。。。" ) );					
			hTimer=::CreateWaitableTimer(NULL,TRUE,_T("lvzehong"));//创建一个可等待的计时器对象。
			if(!hTimer)	break;
			hTimer=OpenWaitableTimer(TIMER_ALL_ACCESS,TRUE,_T("lvzehong"));		//为现有的一个可等待计时器对象创建一个新句柄							
			LARGE_INTEGER liDueTime;	
			//设置相对时间为10秒。
			OutputDebugString("设置15秒定时器/r/n");
			liDueTime.QuadPart = 15*1000*1000*(-10);							
			SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, TRUE);
			// 启动一个可等待计时器。将它设为未发信号状态,系统自动恢复
			break;
		case PBT_APMRESUMESUSPEND:
			TRACE("PBT_APMRESUMESUSPEND_待机后提示消息_通知从挂起状态恢复的事件");
			if(hTimer)
			{
				CancelWaitableTimer(hTimer);
				CloseHandle(hTimer);
				//pThis->IsSelectChange = FALSE;  
				::OutputDebugString( _T( "唤醒成功" ) );
			}
			break;
		case PBT_APMSUSPEND:
			TRACE("PBT_APMSUSPEND_系统挂起前收到的事件\r\n");
			break;
		case PBT_APMRESUMEAUTOMATIC:
			TRACE("PBT_APMRESUMEAUTOMATIC_自动从挂起状态恢复时所收到的事件\r\n");
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CStandbyDemoDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_pTipDialog)
	{
		m_pTipDialog = new CTipDialog;
		m_pTipDialog->Create(IDD_DIALOG1, this);
		m_pTipDialog->SetPromptMessage("您有5个已经过期密件请尽快归还！！！");
	}
	else
		m_pTipDialog->SetPromptMessage("您有15个已经过期密件请尽快归还！！！");
	m_pTipDialog->ShowWindow(SW_SHOW);
}
