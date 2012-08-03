
// WebPageDownLoadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WebPageDownLoad.h"
#include "WebPageDownLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <DownLoad.h>
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


// CWebPageDownLoadDlg 对话框




CWebPageDownLoadDlg::CWebPageDownLoadDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWebPageDownLoadDlg::IDD, pParent)
	, m_csSavePath(_T(""))
	, m_csEditUrl(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWebPageDownLoadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_SAVE_PATH, m_csSavePath);
	DDX_Text(pDX, IDC_EDIT_URL, m_csEditUrl);
}

BEGIN_MESSAGE_MAP(CWebPageDownLoadDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SELECT, &CWebPageDownLoadDlg::OnBnClickedButtonSelect)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &CWebPageDownLoadDlg::OnBnClickedButtonDownload)
END_MESSAGE_MAP()


// CWebPageDownLoadDlg 消息处理程序

BOOL CWebPageDownLoadDlg::OnInitDialog()
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
	//m_csEditUrl = _T"请输入要下载的网址……";

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWebPageDownLoadDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CWebPageDownLoadDlg::OnPaint()
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
HCURSOR CWebPageDownLoadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWebPageDownLoadDlg::OnBnClickedButtonSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//true 打开  false保存
	CFileDialog hFileDlg(false,NULL,NULL,
		OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST,
		TEXT("网页文件 (*.html)|*.html|所有文件(*.*)|*.*|"),NULL);
	if(hFileDlg.DoModal() == IDOK)
	{
		m_csSavePath = hFileDlg.GetPathName();
		m_csSavePath += _T(".html");		
		UpdateData(FALSE);
	}

	//OPENFILENAMEA ofn;       
	//char szFile[260];       
	//ZeroMemory(&ofn, sizeof(ofn));
	//ofn.lStructSize = sizeof(ofn);
	//ofn.hwndOwner = NULL;
	//ofn.lpstrFile = szFile;
	//ofn.lpstrFile[0] = '/0';
	//ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = "网页文件/0*.html/0";
	//ofn.nFilterIndex = 1;
	//ofn.lpstrFileTitle = NULL;
	//ofn.nMaxFileTitle = 0;
	//ofn.lpstrInitialDir = NULL;
	//ofn.Flags = 0;

	////if (GetOpenFileNameA(&ofn)==FALSE) 
	//if (GetSaveFileNameA(&ofn)==FALSE) 
	//	return;
	//m_csSavePath = ofn.lpstrFile;

	//m_csSavePath += _T(".html");
	//UpdateData(FALSE);
}

void CWebPageDownLoadDlg::OnBnClickedButtonDownload()
{
	// TODO: 在此添加控件通知处理程序代码
	int iRet = 0;
	UpdateData(TRUE);
	QNA::CDownLoad clsDL;
	if (!m_csEditUrl.GetLength())
	{
		::MessageBox(NULL, _T("请输入要下载的网址！！！！！"), _T("提示"), MB_OK);
		return;
	}
	if (!m_csSavePath.GetLength())
	{
		::MessageBox(NULL, _T("请选择要保存的位置！！！！！"), _T("提示"), MB_OK);
		return;
	}
	iRet = clsDL.DownLoadFile(m_csEditUrl.GetBuffer(), m_csSavePath.GetBuffer());

	::MessageBox(NULL, _T("下载完成！！！！！"), _T("提示"), MB_OK);
}
