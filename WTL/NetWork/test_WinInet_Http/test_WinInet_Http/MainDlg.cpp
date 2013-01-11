// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
//#include <string>
#include <shlwapi.h>
#pragma comment (lib, "shlwapi.lib")
#include <Wininet.h>

#pragma comment(lib,"Wininet.lib")

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);
	SetDlgItemText(IDC_EDIT_URL, _T("www.9cpp.com"));

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Add validation code 
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnBnClickedButtonVerify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR tszUrl[MAX_PATH] = {0};
	TCHAR tszDomain[MAX_PATH] = {0};
	memset(tszUrl, 0, sizeof(tszUrl));
	memset(tszDomain, 0, sizeof(tszDomain));

	GetDlgItemText(IDC_EDIT_URL, tszDomain, MAX_PATH);
	if (!_tcslen(tszDomain))
	{
		//::MessageBox(NULL, "请输入需检测的域名！！！", "提示！！！", MB_OK);
		EDITBALLOONTIP   t ;
		t.cbStruct = sizeof(t) ;
		t.pszTitle = L"error" ;
		t.pszText = L"Please input correct domain!" ;
		t.ttiIcon = TTI_ERROR ;
		SendDlgItemMessage (IDC_EDIT_URL, EM_SHOWBALLOONTIP, 0, (LPARAM)&t) ;
		return 0;
	}

	_stprintf(tszUrl, _T("http://%s/"), tszDomain);

	//检测URL是否有效 形式为  http:// + 域名 + /
	if (!UrlIs(tszUrl, URLIS_URL))
	{
		EDITBALLOONTIP   t ;
		t.cbStruct = sizeof(t) ;
		t.pszTitle = L"error" ;
		t.pszText = L"This url is not valid!" ;
		t.ttiIcon = TTI_ERROR ;
		SendDlgItemMessage (IDC_EDIT_URL, EM_SHOWBALLOONTIP, 0, (LPARAM)&t) ;
		return 0;
	}
	VerifyDomain(tszDomain);




	return 0;
}

LRESULT CMainDlg::OnBnClickedButtonOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR tszUrl[MAX_PATH] = {0};
	TCHAR tszDomain[MAX_PATH] = {0};
	memset(tszUrl, 0, sizeof(tszUrl));
	memset(tszDomain, 0, sizeof(tszDomain));

	GetDlgItemText(IDC_EDIT_URL, tszDomain, MAX_PATH);
	if (!_tcslen(tszDomain))
	{
		//::MessageBox(NULL, "请输入需检测的域名！！！", "提示！！！", MB_OK);
		EDITBALLOONTIP   t ;
		t.cbStruct = sizeof(t) ;
		t.pszTitle = L"error" ;
		t.pszText = L"Please input correct domain!" ;
		t.ttiIcon = TTI_ERROR ;
		SendDlgItemMessage (IDC_EDIT_URL, EM_SHOWBALLOONTIP, 0, (LPARAM)&t) ;
		return 0;
	}

	_stprintf(tszUrl, _T("http://%s/"), tszDomain);

	::ShellExecute (NULL, _T("open"), tszUrl, NULL, NULL, SW_SHOW) ;

	return 0;
}

bool CMainDlg::VerifyDomain(TCHAR* ptszDomain)
{
	char	szRecvBuf[1024];		// 接受数据缓冲区
	DWORD   dwNumberOfBytesRead;	// 服务器返回大小

	TCHAR tszUrl[MAX_PATH] = {0};
	HINTERNET	hNetOpen;   
	HINTERNET   hConnect;   
	HINTERNET   hRequest;   
	//初始化一个应用程序，以使用 WinINet 函数
	hNetOpen = InternetOpen(_T("InetAll"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (NULL == hNetOpen)
	{
		return false;
	}
	_stprintf(tszUrl, _T("http://%s/"), ptszDomain);

	hConnect = InternetConnect(hNetOpen, ptszDomain, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);

	//HINTERNET HttpOpenRequest(
	//	__in HINTERNET hConnect,// InternetConnect 函数返回的句柄
	//	__in LPCTSTR lpszVerb,// 动作，有GET, PUT, POST。也可以设置为 NULL ，会被当成默认的 GET 来用
	//	__in LPCTSTR lpszObjectName,// 一个描述你请求资源的字符串，当请求一个默认页面时令这个参数指向一个空串
	//	__in LPCTSTR lpszVersion,// HTTP 版本，这个参数为 NULL 时，默认使用""HTTP/1.1""
	//	__in LPCTSTR lpszReferer,// 说明了lpszObjectName是取自哪个文件，可以设为NULL
	//	__in LPCTSTR *lplpszAcceptTypes, // 是一个指向LPCTSTR数组的指针！数组以一个NULL指针结束。指定了程序接受的内容的类型，设为空则不接受 任何类型的内容，设为空串则等价于""text/*""，即不接受文本文件以外的图片等文件，只接受某种特定的文件可以用类似"image/gif, image/jpeg"的方式。关于更多内容类型 请看这里
	//	__in DWORD dwFlags,// 一般都可以设置为 0
	//	__in DWORD_PTRdwContext // 一般都可以设置为 0
	//	);
	
	//请求HEAD，通过HEAD获得文件大小及类型进行校验
	hRequest = HttpOpenRequest(hConnect, _T("POST"), NULL, _T("HTTP/1.1"), _T(""), NULL, INTERNET_FLAG_RELOAD, 0);


	memset(szRecvBuf,0,1024);
	// 开始读取数据
	bool bRet = InternetReadFile(hRequest, szRecvBuf, 1024, &dwNumberOfBytesRead);

	return true;
}