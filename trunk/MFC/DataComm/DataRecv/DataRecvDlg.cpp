// DataRecvDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataRecv.h"
#include "DataRecvDlg.h"
#include ".\datarecvdlg.h"

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


// CDataRecvDlg �Ի���



CDataRecvDlg::CDataRecvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDataRecvDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataRecvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataRecvDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_FILEMAP_READ, OnBnClickedButtonFilemapRead)
	ON_BN_CLICKED(IDC_BUTTON_CLIPOARD_READ, OnBnClickedButtonClipoardRead)
	ON_MESSAGE(WM_COMM, OnUserReceiveMsg)	
	ON_REGISTERED_MESSAGE(wm_nRegMsg,OnRegReceiveMsg) 
	ON_REGISTERED_MESSAGE(wm_nMemMsg,OnRegMemMsg) 
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CDataRecvDlg ��Ϣ�������

BOOL CDataRecvDlg::OnInitDialog()
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
	
	return TRUE;  // ���������˿ؼ��Ľ��㣬���򷵻� TRUE
}

void CDataRecvDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDataRecvDlg::OnPaint() 
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
HCURSOR CDataRecvDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataRecvDlg::OnBnClickedButtonFilemapRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	
	HANDLE hMapping = NULL; 
	LPSTR lpData = NULL; 
	CString csFileMap = "";
	
	// �����ڴ�ӳ�����.
	hMapping = CreateFileMapping((HANDLE)0xFFFFFFFF,
		NULL, PAGE_READWRITE, 0, 0x100, "MYSHARE"); 
	if(hMapping==NULL) 
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

	// �����ӳ���ڴ�����ݸ������ر���.
	csFileMap.Format("%s", lpData);

	// �ͷ�ӳ���ڴ�.
	UnmapViewOfFile(lpData);

	SetDlgItemText(IDC_EDIT_FILEMAP, csFileMap);

	// ��������.
	UpdateData(FALSE);
}

void CDataRecvDlg::OnBnClickedButtonClipoardRead()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString csClipBoard = "";
	// ��ϵͳ������.
	if (!OpenClipboard()) return; 

	// �жϼ������ϵ������Ƿ���ָ�������ݸ�ʽ.
	if (IsClipboardFormatAvailable(CF_TEXT)|| IsClipboardFormatAvailable(CF_OEMTEXT))
	{
		// �Ӽ������ϻ������.
		HANDLE hClipboardData = GetClipboardData(CF_TEXT);

		// ͨ�����ڴ������������ָ��ָ����ʽ���ݵ�ָ��.
		char *pchData = (char*)GlobalLock(hClipboardData);

		// ���ر����������.
		csClipBoard = pchData;

		// ���ڴ�������.
		GlobalUnlock(hClipboardData);
	}
	else
	{ 
		AfxMessageBox("There is no text (ANSI) data on the Clipboard.");
	}

	// ʹ�����رռ�����.
	CloseClipboard();
	
	// ��������.
	SetDlgItemText(IDC_EDIT_CLIPBOARD, csClipBoard);
}

LRESULT CDataRecvDlg::OnUserReceiveMsg(WPARAM wParam,LPARAM lParam)
{
	CString csUserMsg = "";
	csUserMsg.Format("%d\n",int(lParam));
	SetDlgItemText(IDC_EDIT_USERMSG, csUserMsg);
	// ��������.
	UpdateData(FALSE);
	return 0;
}

LRESULT CDataRecvDlg::OnRegReceiveMsg(WPARAM wParam,LPARAM lParam)
{
	CString csRegMsg = "";
	csRegMsg.Format("%d\n", int(lParam));
	SetDlgItemText(IDC_EDIT_REGMSG, csRegMsg);
	UpdateData(FALSE);
	return 0;
}

BOOL CDataRecvDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString csCPDMsg = "";

	csCPDMsg = (LPSTR)pCopyDataStruct->lpData;

	// ���ʵ�ʳ��ȵ��ַ���.
	csCPDMsg = csCPDMsg.Left(pCopyDataStruct->cbData);
	SetDlgItemText(IDC_EDIT_COPYDATA, csCPDMsg);

	// ��������.
	UpdateData(FALSE);

	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}

LRESULT CDataRecvDlg::OnRegMemMsg(WPARAM wParam,LPARAM lParam)
{
	char data[BUFFER_SIZE] = {0};
	CString csMem = "";
	LPVOID lpBaseAddress = (LPVOID)lParam;

	// ���ַ���д��hProcess���̵��ڴ�.
	HANDLE hProcess = GetCurrentProcess();

	ReadProcessMemory(hProcess, lpBaseAddress, data, BUFFER_SIZE, NULL);

	csMem = data;

	SetDlgItemText(IDC_EDIT_MEM, csMem);

	// ��������.
	UpdateData(FALSE);
	return 0;
}

