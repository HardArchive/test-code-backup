// MFCTest2Section7Demo1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTest2Section7Demo1.h"
#include "MFCTest2Section7Demo1Dlg.h"

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


// CMFCTest2Section7Demo1Dlg �Ի���




CMFCTest2Section7Demo1Dlg::CMFCTest2Section7Demo1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCTest2Section7Demo1Dlg::IDD, pParent)
{
	CoInitialize(NULL);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSink = new CSink(this);
}

CMFCTest2Section7Demo1Dlg::~CMFCTest2Section7Demo1Dlg()
{
	m_pSink->Release();
	CoUninitialize();
}

void CMFCTest2Section7Demo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCTest2Section7Demo1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMFCTest2Section7Demo1Dlg ��Ϣ�������

BOOL CMFCTest2Section7Demo1Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

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

	HRESULT hr = E_FAIL;
	CComPtr<IHelloSOE> spHelloSOE;

	//�����򵥶���
	hr = spHelloSOE.CoCreateInstance(CLSID_HelloSOE);

	if (SUCCEEDED(hr))
	{
		//�¼�ע��
		DWORD dw = 0;
		hr = spHelloSOE.Advise(m_pSink, DIID__IHelloSOEEvents, &dw);

		if (SUCCEEDED(hr))
		{
			//�򵥶���ļӷ�
			hr = spHelloSOE->Add(9,99);
		}
	}


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCTest2Section7Demo1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCTest2Section7Demo1Dlg::OnPaint()
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
//
HCURSOR CMFCTest2Section7Demo1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMFCTest2Section7Demo1Dlg::OnAddResult(LONG lResult)
{
	//�򵥶�����¼�����

	CString str;
	str.Format(_T("�ӷ�����Ľ���ǣ�%ld"), lResult);

	AfxMessageBox(str);
}