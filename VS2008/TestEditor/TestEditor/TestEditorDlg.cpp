
// TestEditorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestEditor.h"
#include "TestEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//注意：这俩文件来自Scintilla的include目录 
#include "scintilla/Scintilla.h"
#include "scintilla/SciLexer.h"

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


// CTestEditorDlg 对话框




CTestEditorDlg::CTestEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestEditorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestEditorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CTestEditorDlg 消息处理程序

BOOL CTestEditorDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestEditorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTestEditorDlg::OnPaint()
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
HCURSOR CTestEditorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestEditorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	//m_ScintillaWnd.MoveWindow(0, 0, cx, cy); 
	::MoveWindow(m_hScintillaWnd, 0, 0, cx, cy, FALSE);
}

int CTestEditorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	//m_ScintillaWnd.Create(   
	//	WS_EX_CLIENTEDGE,   
	//	WS_CHILD | WS_VISIBLE,   
	//	CRect(0, 0, lpCreateStruct->cx,lpCreateStruct->cy),   
	//	this, 10000); 
	//CWnd::CreateEx(dwExStyle, L"Scintilla", L"", dwStyle, rect, pParentWnd, nID);   
	m_hScintillaWnd = ::CreateWindow(
		_T("Scintilla"), 
		NULL,
		WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE, 
		0, 0, lpCreateStruct->cx, lpCreateStruct->cy,
		this->m_hWnd,
		NULL, NULL, NULL); 

	//SendEditor(SCI_GETDIRECTFUNCTION, 0, 0);
	//SendEditor(SCI_GETDIRECTPOINTER, 0, 0);

	return 0;
}

//当Scintilla控件发生事件时，会用WM_NOTITY消息通知父窗体。
BOOL CTestEditorDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 在此添加专用代码和/或调用基类
	LPNMHDR pnmh = (LPNMHDR)lParam;   
	struct SCNotification* scn = (struct SCNotification*)lParam;   

	switch(pnmh->code)   
	{    
	//case SCN_CHARADDED:   

		/* Hey, Scintilla just told me that a new */  
		/* character was added to the Edit Control.*/  
		/* Now i do something cool with that char. */  
		//break; 
	case SCN_MODIFIED:   
		//This notification is sent when the text or    
		//styling of the document changes or is about    
		//to change   
	case SCN_ZOOM:   
		//This notification is generated when the user   
		//zooms the display using the keyboard or the   
		//SCI_SETZOOM method is called.   
		UpdateLineNumberWidth();   
		break;  

		//case ...   
	}  

	return CDialog::OnNotify(wParam, lParam, pResult);
}

//显示行号，首先需要在m_hScintillaWnd创建后立即调用以下函数，并在Scintilla通知主窗口SCN_MODIFIED，SCN_ZOOM时调用
void CTestEditorDlg::UpdateLineNumberWidth(void)   
{   
	char tchLines[32];   
	int  iLineMarginWidthNow;   
	int  iLineMarginWidthFit;   

	sprintf(tchLines," %i ", SendEditor(SCI_GETLINECOUNT, 0, 0));   
	//设置行号
	iLineMarginWidthNow = SendEditor(SCI_GETMARGINWIDTHN, 0, 0);
	iLineMarginWidthFit = SendEditor(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)tchLines);
	if (iLineMarginWidthNow != iLineMarginWidthFit)     
	{   
		SendEditor(SCI_SETMARGINWIDTHN, 0, iLineMarginWidthFit);     
	}   
} 