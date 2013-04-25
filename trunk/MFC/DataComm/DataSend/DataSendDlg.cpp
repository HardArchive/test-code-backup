// DataSendDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataSend.h"
#include "DataSendDlg.h"
#include ".\datasenddlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUFFER_SIZE  0x100          // ���ڴ��ַͨ��ʱ���������ڴ�.
#define WM_COMM      WM_USER+100

const UINT wm_nRegMsg = RegisterWindowMessage("reg_data");
const UINT wm_nMemMsg = RegisterWindowMessage("mem_data");
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDataSendDlg �Ի���



CDataSendDlg::CDataSendDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataSendDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pWnd = NULL;

}

void CDataSendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataSendDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_USERMSG_SEND, OnBnClickedButtonUsermsgSend)
	ON_BN_CLICKED(IDC_BUTTON_REGMSG_SEND, OnBnClickedButtonRegmsgSend)
	ON_BN_CLICKED(IDC_BUTTON_COPYDATA_SEND, OnBnClickedButtonCopydataSend)
	ON_BN_CLICKED(IDC_BUTTON_MEM_SEND, OnBnClickedButtonMemSend)
	ON_BN_CLICKED(IDC_BUTTON_FILEMAP_SEND, OnBnClickedButtonFilemapSend)
	ON_BN_CLICKED(IDC_BUTTON_CLIPBOARD_SEND, OnBnClickedButtonClipboardSend)
END_MESSAGE_MAP()


// CDataSendDlg ��Ϣ�������

BOOL CDataSendDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��\������...\���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_pWnd = CWnd::FindWindow(NULL,_T("DataRecv")); // ����DataRecv����.
	if(m_pWnd == NULL)
	{
		AfxMessageBox(TEXT("Unable to find DataRecv."));
		//return;
	}
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CDataSendDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDataSendDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CDataSendDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataSendDlg::OnBnClickedButtonUsermsgSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT uMsg = 0;
	CString csUserMsg = "";
	GetDlgItemText(IDC_EDIT_USERMSG, csUserMsg);
	
	uMsg = atoi(csUserMsg);
	m_pWnd->SendMessage(WM_COMM, NULL, (LPARAM)uMsg);// ����.

}

void CDataSendDlg::OnBnClickedButtonRegmsgSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT uMsg = 0;
	CString csRegMsg = "";
	GetDlgItemText(IDC_EDIT_REGMSG, csRegMsg);
	uMsg = atoi(csRegMsg);
	m_pWnd->SendMessage(wm_nRegMsg, NULL, (LPARAM)uMsg);// ����.

}

void CDataSendDlg::OnBnClickedButtonCopydataSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString csCPDMsg = "";
	COPYDATASTRUCT cpd = {0}; // ��COPYDATASTRUCT�ṹ��ֵ.
	GetDlgItemText(IDC_EDIT_COPEDATA, csCPDMsg);
	cpd.dwData = 0;
	cpd.cbData = csCPDMsg.GetLength();
	cpd.lpData = (void*)csCPDMsg.GetBuffer(cpd.cbData);
	m_pWnd->SendMessage(WM_COPYDATA,NULL,(LPARAM)&cpd);// ����.
}

void CDataSendDlg::OnBnClickedButtonMemSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD PID = 0;
	HANDLE hProcess = NULL;
	LPVOID lpBaseAddress = NULL;
	char  data[BUFFER_SIZE] = {0}; 
	CString csMemMsg = "";
	// ��ȡ���̺�.
	GetWindowThreadProcessId(m_pWnd->m_hWnd, (DWORD*)&PID );
	hProcess = OpenProcess (PROCESS_ALL_ACCESS,FALSE,PID);

	// ���������ڴ�.
	lpBaseAddress = VirtualAllocEx(hProcess, 0, BUFFER_SIZE, 
		MEM_COMMIT, PAGE_READWRITE);		

    GetDlgItemText(IDC_EDIT_MEM, csMemMsg);
	strcpy(data, csMemMsg);

	// ���ַ���д��hProcess���̵��ڴ�.
	WriteProcessMemory(hProcess, lpBaseAddress, data, BUFFER_SIZE, NULL);

	// ���ͻ�ַ��DataRecv����.
	m_pWnd->SendMessage(wm_nMemMsg, NULL, (LPARAM)lpBaseAddress);

	// �ȴ����ճ����������.
	Sleep(100);

	// �ͷ������ڴ�.
	VirtualFreeEx(hProcess, lpBaseAddress, 0, MEM_RELEASE);
}

void CDataSendDlg::OnBnClickedButtonFilemapSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	HANDLE hMapping = NULL;   
	LPSTR lpData = NULL;
	CString csFileMap = "";
	
	// �����ڴ�ӳ�����.
	hMapping = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL,
		PAGE_READWRITE, 0, BUFFER_SIZE, "MYSHARE");   
	if(hMapping == NULL)   
	{   
		AfxMessageBox("CreateFileMapping() failed.");
		return;
	}

	// ���ļ�����ͼӳ�䵽һ�����̵ĵ�ַ�ռ��ϣ�����LPVOID���͵��ڴ�ָ��.
	lpData = (LPSTR)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);   
	if(lpData == NULL)   
	{   
		AfxMessageBox("MapViewOfFile() failed.");
		return;
	}

	GetDlgItemText(IDC_EDIT_FILEMAP, csFileMap);

	// �����ӳ���ڴ�д����.
	sprintf(lpData, csFileMap);   

	// �ͷ�ӳ���ڴ�.
	UnmapViewOfFile(lpData);   
}

void CDataSendDlg::OnBnClickedButtonClipboardSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString csClipBoard = "";
	//CString strData=m_strClipBoard;  // �������.
	GetDlgItemText(IDC_EDIT_CLIPBOARD, csClipBoard);

	// ��ϵͳ������.
	if (!OpenClipboard()) return;

	// ʹ��֮ǰ�����ϵͳ������.
	EmptyClipboard();

	// ����һ�ڴ棬��С����Ҫ�������ַ����Ĵ�С�����ص��ڴ���ƾ��.
	HGLOBAL hClipboardData;
	hClipboardData = GlobalAlloc(GMEM_DDESHARE, csClipBoard.GetLength()+1);

	// �ڴ���ƾ������������ֵΪָ�����ڴ���ƾ�����ڵ��ض����ݸ�ʽ��ָ��.
	char * pchData;
	pchData = (char*)GlobalLock(hClipboardData);

	// �����ر�����ֵ����ȫ���ڴ�.
	strcpy(pchData, LPCSTR(csClipBoard));

	// ��������ȫ���ڴ���ƾ������.
	GlobalUnlock(hClipboardData);

	// ͨ��ȫ���ڴ�����Ҫ���������ݷŵ���������.
	SetClipboardData(CF_TEXT,hClipboardData);

	// ʹ�����رռ�����.
	CloseClipboard();
}
