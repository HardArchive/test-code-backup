// MFCTestSection6Demo1Dlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCTestSection6Demo1.h"
#include "MFCTestSection6Demo1Dlg.h"

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


// CMFCTestSection6Demo1Dlg �Ի���

BEGIN_EVENTSINK_MAP(CMFCTestSection6Demo1Dlg, CDialog)
	ON_EVENT(CMFCTestSection6Demo1Dlg, IDC_HELLOCOMPOSITECONTROL1, 1, CMFCTestSection6Demo1Dlg::OnClickBtnHellocompositecontrol1, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


CMFCTestSection6Demo1Dlg::CMFCTestSection6Demo1Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMFCTestSection6Demo1Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCTestSection6Demo1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HELLOCOMPOSITECONTROL1, m_hcc);
}

BEGIN_MESSAGE_MAP(CMFCTestSection6Demo1Dlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCTestSection6Demo1Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCTestSection6Demo1Dlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMFCTestSection6Demo1Dlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMFCTestSection6Demo1Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CMFCTestSection6Demo1Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CMFCTestSection6Demo1Dlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CMFCTestSection6Demo1Dlg ��Ϣ�������

BOOL CMFCTestSection6Demo1Dlg::OnInitDialog()
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCTestSection6Demo1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMFCTestSection6Demo1Dlg::OnPaint()
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
HCURSOR CMFCTestSection6Demo1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCTestSection6Demo1Dlg::OnBnClickedButton1()
{
	MessageBox(m_hcc.get_Description());
}

void CMFCTestSection6Demo1Dlg::OnBnClickedButton2()
{
	m_hcc.put_Description(_T("��MFC���޸����������ԣ����Ǹ��Ͽؼ���"));
}

void CMFCTestSection6Demo1Dlg::OnBnClickedButton3()
{
	m_hcc.EnableButton(0, FALSE);
}

void CMFCTestSection6Demo1Dlg::OnBnClickedButton4()
{
	m_hcc.EnableButton(0, TRUE);
}

void CMFCTestSection6Demo1Dlg::OnBnClickedButton5()
{
	m_hcc.EnableButton(1, FALSE);
}

void CMFCTestSection6Demo1Dlg::OnBnClickedButton6()
{
	m_hcc.EnableButton(1, TRUE);
}

void CMFCTestSection6Demo1Dlg::OnClickBtnHellocompositecontrol1(long lBtnIndex, long lClickCountSum)
{
	CString msg;

	msg.Format(_T("�����%d�Σ�"), lClickCountSum);

	if (lBtnIndex == 0)
	{
		msg = CString(_T("Button1")) + msg;
	}
	else if (lBtnIndex == 1)
	{
		msg = CString(_T("Button2")) + msg;
	}

	MessageBox(msg);

}
