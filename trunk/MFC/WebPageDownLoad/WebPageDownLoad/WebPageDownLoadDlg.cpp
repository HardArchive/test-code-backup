
// WebPageDownLoadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WebPageDownLoad.h"
#include "WebPageDownLoadDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <DownLoad.h>
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


// CWebPageDownLoadDlg �Ի���




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


// CWebPageDownLoadDlg ��Ϣ�������

BOOL CWebPageDownLoadDlg::OnInitDialog()
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
	//m_csEditUrl = _T"������Ҫ���ص���ַ����";

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWebPageDownLoadDlg::OnPaint()
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
HCURSOR CWebPageDownLoadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWebPageDownLoadDlg::OnBnClickedButtonSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	//true ��  false����
	CFileDialog hFileDlg(false,NULL,NULL,
		OFN_FILEMUSTEXIST | OFN_READONLY | OFN_PATHMUSTEXIST,
		TEXT("��ҳ�ļ� (*.html)|*.html|�����ļ�(*.*)|*.*|"),NULL);
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
	//ofn.lpstrFilter = "��ҳ�ļ�/0*.html/0";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iRet = 0;
	UpdateData(TRUE);
	QNA::CDownLoad clsDL;
	if (!m_csEditUrl.GetLength())
	{
		::MessageBox(NULL, _T("������Ҫ���ص���ַ����������"), _T("��ʾ"), MB_OK);
		return;
	}
	if (!m_csSavePath.GetLength())
	{
		::MessageBox(NULL, _T("��ѡ��Ҫ�����λ�ã���������"), _T("��ʾ"), MB_OK);
		return;
	}
	iRet = clsDL.DownLoadFile(m_csEditUrl.GetBuffer(), m_csSavePath.GetBuffer());

	::MessageBox(NULL, _T("������ɣ���������"), _T("��ʾ"), MB_OK);
}
