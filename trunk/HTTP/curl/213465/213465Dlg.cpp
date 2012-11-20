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
	//libcurl�ṩ��һ��C����API����ֱ�ӵ��á�������Ҫ�ᵽ��������������curl_global_init()�� curl_global_cleanup()��
	//libcurlҪ�õ�һϵ�е�ȫ�ֳ�����curl_global_init()�������ǳ�ʼ����Щ������������һЩȫ����Դ��
	//curl_global_cleanup()�����ͷ���Щ��Դ��
	//���һ������£��ڵ���libcurl����֮ǰ������ curl_global_init(CURL_GLOBAL_ALL)����ʼ�����ڵ�����Ϻ���curl_global_cleanup()�˳���
	return_code = curl_global_init(CURL_GLOBAL_WIN32);
	if (CURLE_OK != return_code) return;
	//curl_easy_init�˺�����Ҫ���ȱ����ã�����CRUL easy��������������������ö�Ҫ�õ���������
	//���û�е���curl_global_init()���ú����Զ����ã����ǿ��ǵ��̰߳�ȫ�����⣬����Լ�����curl_global_init()
	// ��ȡeasy handle
	CURL *easy_handle = curl_easy_init();
	if (NULL == easy_handle)
	{  
		curl_global_cleanup();
		return;
	}
	// ����easy handle����
	curl_easy_setopt(easy_handle, CURLOPT_URL, get_url.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_REFERER, get_Rurl.c_str());
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, &buffer);
	//�ύ��һ�������cookie 
	curl_easy_setopt(easy_handle, CURLOPT_COOKIEFILE,"cookis_open.txt");
		//�ѷ�������������cookie���浽cookie_open.txt
	curl_easy_setopt(easy_handle, CURLOPT_COOKIEJAR,"cookis_login.txt");

	// ִ����������
	curl_easy_perform(easy_handle); 
	// �ͷ���Դ
	curl_easy_cleanup(easy_handle);
	curl_global_cleanup();
	CString FileData;
	FileData=buffer.c_str();
	CFile	m_File;	
	m_File.Open("funckData.txt",CFile::modeWrite | CFile::modeCreate,NULL);
	m_File.Write(FileData,FileData.GetLength());
	m_File.Close();
}
