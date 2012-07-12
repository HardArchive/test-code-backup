
// StandbyDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StandbyDemo.h"
#include "StandbyDemoDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "TipDialog.h"
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


// CStandbyDemoDlg �Ի���




CStandbyDemoDlg::CStandbyDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStandbyDemoDlg::IDD, pParent)
{
	hTimer = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStandbyDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStandbyDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CHILDACTIVATE()
	ON_BN_CLICKED(IDC_BUTTON1, &CStandbyDemoDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CStandbyDemoDlg ��Ϣ�������

BOOL CStandbyDemoDlg::OnInitDialog()
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
	m_pTipDialog = NULL;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CStandbyDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CStandbyDemoDlg::OnPaint()
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
HCURSOR CStandbyDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CStandbyDemoDlg::OnChildActivate()
{
	CDialog::OnChildActivate();

	// TODO: �ڴ˴������Ϣ����������
}

LRESULT CStandbyDemoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (WM_POWERBROADCAST == message)
	{
		switch (wParam)
		{
		case PBT_APMQUERYSUSPEND:
			TRACE("PBT_APMQUERYSUSPEND_����ǰ��ʾ��Ϣ_��Ҫ����ɲ��ܹ�����¼�//��֧��WIN7");
			OutputDebugString("��Ҫ����ɲ��ܹ�����¼���Ϣ֪ͨ-�ɵȴ���ʱ������\r\n");
			::OutputDebugString( _T( "׼��������" ) );					
			hTimer=::CreateWaitableTimer(NULL,TRUE,_T("lvzehong"));//����һ���ɵȴ��ļ�ʱ������
			if(!hTimer)	break;
			hTimer=OpenWaitableTimer(TIMER_ALL_ACCESS,TRUE,_T("lvzehong"));		//Ϊ���е�һ���ɵȴ���ʱ�����󴴽�һ���¾��							
			LARGE_INTEGER liDueTime;	
			//�������ʱ��Ϊ10�롣
			OutputDebugString("����15�붨ʱ��/r/n");
			liDueTime.QuadPart = 15*1000*1000*(-10);							
			SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, TRUE);
			// ����һ���ɵȴ���ʱ����������Ϊδ���ź�״̬,ϵͳ�Զ��ָ�
			break;
		case PBT_APMRESUMESUSPEND:
			TRACE("PBT_APMRESUMESUSPEND_��������ʾ��Ϣ_֪ͨ�ӹ���״̬�ָ����¼�");
			if(hTimer)
			{
				CancelWaitableTimer(hTimer);
				CloseHandle(hTimer);
				//pThis->IsSelectChange = FALSE;  
				::OutputDebugString( _T( "���ѳɹ�" ) );
			}
			break;
		case PBT_APMSUSPEND:
			TRACE("PBT_APMSUSPEND_ϵͳ����ǰ�յ����¼�\r\n");
			break;
		case PBT_APMRESUMEAUTOMATIC:
			TRACE("PBT_APMRESUMEAUTOMATIC_�Զ��ӹ���״̬�ָ�ʱ���յ����¼�\r\n");
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CStandbyDemoDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_pTipDialog)
	{
		m_pTipDialog = new CTipDialog;
		m_pTipDialog->Create(IDD_DIALOG1, this);
		m_pTipDialog->SetPromptMessage("����5���Ѿ������ܼ��뾡��黹������");
	}
	else
		m_pTipDialog->SetPromptMessage("����15���Ѿ������ܼ��뾡��黹������");
	m_pTipDialog->ShowWindow(SW_SHOW);
}
