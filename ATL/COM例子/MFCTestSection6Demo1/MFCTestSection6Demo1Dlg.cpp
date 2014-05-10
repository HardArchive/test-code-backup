// MFCTestSection6Demo1Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCTestSection6Demo1.h"
#include "MFCTestSection6Demo1Dlg.h"

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


// CMFCTestSection6Demo1Dlg 对话框

BEGIN_EVENTSINK_MAP(CMFCTestSection6Demo1Dlg, CDialog)
	ON_EVENT(CMFCTestSection6Demo1Dlg, IDC_HELLOCOMPOSITECONTROL1, 1, CMFCTestSection6Demo1Dlg::OnClickBtnHellocompositecontrol1, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


CMFCTestSection6Demo1Dlg::CMFCTestSection6Demo1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCTestSection6Demo1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTestSection6Demo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HELLOCOMPOSITECONTROL1, m_hcc);
}

BEGIN_MESSAGE_MAP(CMFCTestSection6Demo1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTestSection6Demo1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCTestSection6Demo1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCTestSection6Demo1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCTestSection6Demo1Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCTestSection6Demo1Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCTestSection6Demo1Dlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CMFCTestSection6Demo1Dlg 消息处理程序

BOOL CMFCTestSection6Demo1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCTestSection6Demo1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCTestSection6Demo1Dlg::OnPaint()
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
//
HCURSOR CMFCTestSection6Demo1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCTestSection6Demo1Dlg::OnBnClickedButton1()
{
	MessageBox(m_hcc.get_Description());
}

void CMFCTestSection6Demo1Dlg::OnBnClickedButton2()
{
	m_hcc.put_Description(_T("在MFC中修改了描述属性，我是复合控件！"));
}

void CMFCTestSection6Demo1Dlg::OnBnClickedButton3()
{
	m_hcc.EnableButton(0, FALSE);
}

void CMFCTestSection6Demo1Dlg::OnBnClickedButton4()
{
	m_hcc.EnableButton(0, TRUE);
}

void CMFCTestSection6Demo1Dlg::OnBnClickedButton5()
{
	m_hcc.EnableButton(1, FALSE);
}

void CMFCTestSection6Demo1Dlg::OnBnClickedButton6()
{
	m_hcc.EnableButton(1, TRUE);
}

void CMFCTestSection6Demo1Dlg::OnClickBtnHellocompositecontrol1(long lBtnIndex, long lClickCountSum)
{
	CString msg;

	msg.Format(_T("被点击%d次！"), lClickCountSum);

	if (lBtnIndex == 0)
	{
		msg = CString(_T("Button1")) + msg;
	}
	else if (lBtnIndex == 1)
	{
		msg = CString(_T("Button2")) + msg;
	}

	MessageBox(msg);

}
