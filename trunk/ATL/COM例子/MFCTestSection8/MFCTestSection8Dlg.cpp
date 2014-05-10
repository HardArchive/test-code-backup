// MFCTestSection8Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTestSection8.h"
#include "MFCTestSection8Dlg.h"

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


// CMFCTestSection8Dlg �Ի���




CMFCTestSection8Dlg::CMFCTestSection8Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCTestSection8Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSink = new CSink(this);
}

CMFCTestSection8Dlg::~CMFCTestSection8Dlg()
{
	m_pSink->Release();
}

void CMFCTestSection8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGINAX1, m_axLogin);
}

BEGIN_MESSAGE_MAP(CMFCTestSection8Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMFCTestSection8Dlg ��Ϣ�������

BOOL CMFCTestSection8Dlg::OnInitDialog()
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

	//�������A
	hr = m_spPasswordSpObj.CoCreateInstance(CLSID_PasswordSpObj);

	if (SUCCEEDED(hr))
	{
		//����������֤����ʧ�ܵĴ���
		hr = m_spPasswordSpObj->put_ErrorCountAllowed(5);

		DWORD dw = 0;

		//ע���¼�
		hr = m_spPasswordSpObj.Advise(m_pSink, DIID__IPasswordSpObjEvents, &dw);
	}

	//������Ҫ�ǿ��ж�
	m_axLogin.put_JudgeNull(TRUE);

	//�������B�Ľ�����ʾ�ı�
	m_axLogin.SetText(_T("�û�����"), _T("���룺"),_T("��¼"));

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCTestSection8Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCTestSection8Dlg::OnPaint()
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
HCURSOR CMFCTestSection8Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CMFCTestSection8Dlg, CDialog)
	ON_EVENT(CMFCTestSection8Dlg, IDC_LOGINAX1, 1, CMFCTestSection8Dlg::OnLoginLoginax1, VTS_BSTR VTS_BSTR)
END_EVENTSINK_MAP()

void CMFCTestSection8Dlg::OnLoginLoginax1(LPCTSTR bstrUserName, LPCTSTR bstrPassword)
{
	HRESULT hr = E_FAIL;

	CComBSTR bstrA(bstrUserName);
	CComBSTR bstrB(bstrPassword);
	VARIANT_BOOL varbResult = VARIANT_FALSE;

	//��֤�û�
	hr = m_spPasswordSpObj->CheckPassword(bstrA, bstrB, &varbResult);

	if (SUCCEEDED(hr))
	{
		if (varbResult)
		{
			MessageBox(_T("��¼�ɹ���"));
			OnOK();
		}
		else
		{
			MessageBox(_T("��¼ʧ�ܣ�"));
		}
	}
}


void CMFCTestSection8Dlg::OnMsg(BSTR bstrMsg)
{
	MessageBoxW(bstrMsg);
}