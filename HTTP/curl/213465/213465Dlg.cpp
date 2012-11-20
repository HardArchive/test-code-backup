// 213465Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "213465.h"
#include "213465Dlg.h"


#include <curl.h>
#include <string>
using namespace std;
#pragma comment(lib, "libcurld_imp.lib")

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
// CMy213465Dlg dialog

CMy213465Dlg::CMy213465Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy213465Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy213465Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy213465Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy213465Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy213465Dlg, CDialog)
	//{{AFX_MSG_MAP(CMy213465Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy213465Dlg message handlers

BOOL CMy213465Dlg::OnInitDialog()
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

void CMy213465Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy213465Dlg::OnPaint() 
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
HCURSOR CMy213465Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


size_t writer(char *data, size_t size, size_t nmemb, 
			  string *writerData)
{
	if (writerData == NULL)  
		return 0; 
	
	size_t len = size*nmemb;  
	writerData->append(data, len);  
	return len; 
	
}

void CMy213465Dlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	string buffer;
	string get_url="http://captcha.qq.com/getimage?aid=549000912&r=0.05700144608719626&uin=385850781";
	string get_Rurl="http://i.qq.com/";

	CURLcode return_code;
	//libcurl提供了一组C语言API函数直接调用。首先需要提到的两个函数就是curl_global_init()和 curl_global_cleanup()。
	//libcurl要用到一系列的全局常量，curl_global_init()函数就是初始化这些变量，并分配一些全局资源；
	//curl_global_cleanup()则负责释放这些资源。
	//因此一般情况下，在调用libcurl函数之前，先用 curl_global_init(CURL_GLOBAL_ALL)做初始化，在调用完毕后，用curl_global_cleanup()退出。
	return_code = curl_global_init(CURL_GLOBAL_WIN32);
	if (CURLE_OK != return_code) return;
	//curl_easy_init此函数需要最先被调用，返回CRUL easy句柄；后续其他函数调用都要用到这个句柄。
	//如果没有调用curl_global_init()，该函数自动调用，但是考虑到线程安全的问题，最好自己调用curl_global_init()
	// 获取easy handle
	CURL *easy_handle = curl_easy_init();
	if (NULL == easy_handle)
	{  
		curl_global_cleanup();
		return;
	}
	// 设置easy handle属性
	curl_easy_setopt(easy_handle, CURLOPT_URL, get_url.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_REFERER, get_Rurl.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &buffer);
	//提交第一步保存的cookie 
	curl_easy_setopt(easy_handle, CURLOPT_COOKIEFILE,"cookis_open.txt");
		//把服务器发过来的cookie保存到cookie_open.txt
	curl_easy_setopt(easy_handle, CURLOPT_COOKIEJAR,"cookis_login.txt");

	// 执行数据请求
	curl_easy_perform(easy_handle); 
	// 释放资源
	curl_easy_cleanup(easy_handle);
	curl_global_cleanup();
	CString FileData;
	FileData=buffer.c_str();
	CFile	m_File;	
	m_File.Open("funckData.txt",CFile::modeWrite | CFile::modeCreate,NULL);
	m_File.Write(FileData,FileData.GetLength());
	m_File.Close();
}
