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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR tszUrl[MAX_PATH] = {0};
	TCHAR tszDomain[MAX_PATH] = {0};
	memset(tszUrl, 0, sizeof(tszUrl));
	memset(tszDomain, 0, sizeof(tszDomain));

	GetDlgItemText(IDC_EDIT_URL, tszDomain, MAX_PATH);
	if (!_tcslen(tszDomain))
	{
		//::MessageBox(NULL, "���������������������", "��ʾ������", MB_OK);
		EDITBALLOONTIP   t ;
		t.cbStruct = sizeof(t) ;
		t.pszTitle = L"error" ;
		t.pszText = L"Please input correct domain!" ;
		t.ttiIcon = TTI_ERROR ;
		SendDlgItemMessage (IDC_EDIT_URL, EM_SHOWBALLOONTIP, 0, (LPARAM)&t) ;
		return 0;
	}

	_stprintf(tszUrl, _T("http://%s/"), tszDomain);

	//���URL�Ƿ���Ч ��ʽΪ  http:// + ���� + /
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR tszUrl[MAX_PATH] = {0};
	TCHAR tszDomain[MAX_PATH] = {0};
	memset(tszUrl, 0, sizeof(tszUrl));
	memset(tszDomain, 0, sizeof(tszDomain));

	GetDlgItemText(IDC_EDIT_URL, tszDomain, MAX_PATH);
	if (!_tcslen(tszDomain))
	{
		//::MessageBox(NULL, "���������������������", "��ʾ������", MB_OK);
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
	char	szRecvBuf[1024];		// �������ݻ�����
	DWORD   dwNumberOfBytesRead;	// ���������ش�С

	TCHAR tszUrl[MAX_PATH] = {0};
	HINTERNET	hNetOpen;   
	HINTERNET   hConnect;   
	HINTERNET   hRequest;   
	//��ʼ��һ��Ӧ�ó�����ʹ�� WinINet ����
	hNetOpen = InternetOpen(_T("InetAll"), INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (NULL == hNetOpen)
	{
		return false;
	}
	_stprintf(tszUrl, _T("http://%s/"), ptszDomain);

	hConnect = InternetConnect(hNetOpen, ptszDomain, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 1);

	//HINTERNET HttpOpenRequest(
	//	__in HINTERNET hConnect,// InternetConnect �������صľ��
	//	__in LPCTSTR lpszVerb,// ��������GET, PUT, POST��Ҳ��������Ϊ NULL ���ᱻ����Ĭ�ϵ� GET ����
	//	__in LPCTSTR lpszObjectName,// һ��������������Դ���ַ�����������һ��Ĭ��ҳ��ʱ���������ָ��һ���մ�
	//	__in LPCTSTR lpszVersion,// HTTP �汾���������Ϊ NULL ʱ��Ĭ��ʹ��""HTTP/1.1""
	//	__in LPCTSTR lpszReferer,// ˵����lpszObjectName��ȡ���ĸ��ļ���������ΪNULL
	//	__in LPCTSTR *lplpszAcceptTypes, // ��һ��ָ��LPCTSTR�����ָ�룡������һ��NULLָ�������ָ���˳�����ܵ����ݵ����ͣ���Ϊ���򲻽��� �κ����͵����ݣ���Ϊ�մ���ȼ���""text/*""�����������ı��ļ������ͼƬ���ļ���ֻ����ĳ���ض����ļ�����������"image/gif, image/jpeg"�ķ�ʽ�����ڸ����������� �뿴����
	//	__in DWORD dwFlags,// һ�㶼��������Ϊ 0
	//	__in DWORD_PTRdwContext // һ�㶼��������Ϊ 0
	//	);
	
	//����HEAD��ͨ��HEAD����ļ���С�����ͽ���У��
	hRequest = HttpOpenRequest(hConnect, _T("POST"), NULL, _T("HTTP/1.1"), _T(""), NULL, INTERNET_FLAG_RELOAD, 0);


	memset(szRecvBuf,0,1024);
	// ��ʼ��ȡ����
	bool bRet = InternetReadFile(hRequest, szRecvBuf, 1024, &dwNumberOfBytesRead);

	return true;
}