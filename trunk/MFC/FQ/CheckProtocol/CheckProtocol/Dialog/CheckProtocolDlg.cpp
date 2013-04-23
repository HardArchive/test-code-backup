
// CheckProtocolDlg.cpp : ʵ���ļ�

#include "stdafx.h"
#include "../CheckProtocol.h"
#include "CheckProtocolDlg.h"
#include "AddPackRuleDlg.h"
#include "AddWebRuleDlg.h"
#include "LoginTestDlg.h"
#include "FindCaptureRuleDlg.h"
#include "DownloadDlg.h"
#include "UpdataDownload.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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


// CCheckProtocolDlg �Ի���
CCheckProtocolDlg::CCheckProtocolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCheckProtocolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCheckProtocolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCheckProtocolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_COMMAND(IDR_MENU_PACKFILE, &CCheckProtocolDlg::OnMenuPackfile)
	ON_COMMAND(IDR_MENU_WEBFILE, &CCheckProtocolDlg::OnMenuWebfile)
	ON_COMMAND(IDR_MENU_LOGIN_TEST, &CCheckProtocolDlg::OnMenuLoginTest)
	ON_COMMAND(IDR_MENU_SEND_TEST, &CCheckProtocolDlg::OnMenuSendTest)
	ON_COMMAND(IDR_MENU_FIND_PACP, &CCheckProtocolDlg::OnMenuFindPacp)
	ON_COMMAND(ID_MENU_DOWNLOAD, &CCheckProtocolDlg::OnMenuDownload)
	ON_COMMAND(ID_MENU_UPDATA_DOWNLOAD, &CCheckProtocolDlg::OnMenuUpdataDownload)
	ON_COMMAND(ID_MENU_VOICE, &CCheckProtocolDlg::OnMenuVoice)
END_MESSAGE_MAP()


// CCheckProtocolDlg ��Ϣ�������

BOOL CCheckProtocolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP1);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCheckProtocolDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCheckProtocolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CCheckProtocolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CCheckProtocolDlg::OnMenuPackfile()
{
	// TODO: �ڴ���������������
	CAddPackRuleDlg PackDlg;
	PackDlg.DoModal();
}

void CCheckProtocolDlg::OnMenuWebfile()
{
	// TODO: �ڴ���������������
	CAddWebRuleDlg Webdlg;
	Webdlg.DoModal();
}

void CCheckProtocolDlg::OnMenuLoginTest()
{
	// TODO: �ڴ���������������
	CLoginTestDlg loginDlg;
	loginDlg.DoModal();
}

void CCheckProtocolDlg::OnMenuSendTest()
{
	// TODO: �ڴ���������������
// 	CSendTestDlg sendDlg;
// 	sendDlg.DoModal();
}

void CCheckProtocolDlg::OnMenuFindPacp()
{
	// TODO: �ڴ���������������
	CFindCaptureRuleDlg findRuleDlg;
	findRuleDlg.DoModal();
}

void CCheckProtocolDlg::OnMenuDownload()
{
	// TODO: �ڴ���������������
	CDownloadDlg  DLdlg;
	DLdlg.DoModal();
}	

void CCheckProtocolDlg::OnMenuUpdataDownload()
{
	// TODO: �ڴ���������������
	CUpdataDownload UpdataDlg;
	UpdataDlg.DoModal();
}

void CCheckProtocolDlg::OnMenuVoice()
{
	// TODO: �ڴ���������������
	
}
