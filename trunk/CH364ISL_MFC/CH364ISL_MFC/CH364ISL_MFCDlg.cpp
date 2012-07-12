// CH364ISL_MFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CH364ISL_MFC.h"
#include "CH364ISL_MFCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#include "Lib/CH366.h"
//#pragma comment(lib, "Lib/CH366.lib")
#include "Ch366Dll.h"

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


// CCH364ISL_MFCDlg �Ի���




CCH364ISL_MFCDlg::CCH364ISL_MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCH364ISL_MFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_iRadio = 0;
	m_iCutover = 0;
	m_hTimer = NULL;
}

void CCH364ISL_MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCH364ISL_MFCDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_SWITCH, &CCH364ISL_MFCDlg::OnBnClickedButtonSwitch)
	ON_BN_CLICKED(IDC_RADIO_INTRANET, &CCH364ISL_MFCDlg::OnBnClickedRadioIntranet)
	ON_BN_CLICKED(IDC_RADIO_EXTRANET, &CCH364ISL_MFCDlg::OnBnClickedRadioExtranet)
	ON_BN_CLICKED(IDC_RADIO_REBOOT, &CCH364ISL_MFCDlg::OnBnClickedRadioReboot)
	ON_BN_CLICKED(IDC_RADIO_HIBERNATE, &CCH364ISL_MFCDlg::OnBnClickedRadioHibernate)
	ON_BN_CLICKED(IDC_RADIO_SHUTDOWN, &CCH364ISL_MFCDlg::OnBnClickedRadioShutdown)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CCH364ISL_MFCDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CCH364ISL_MFCDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()


// CCH364ISL_MFCDlg ��Ϣ�������

BOOL CCH364ISL_MFCDlg::OnInitDialog()
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
	if (!InitCH366DLL())
	{
		return false;
	}
	
	//CheckRadioButton(IDC_RADIO_INTRANET, IDC_RADIO_EXTRANET, IDC_RADIO_EXTRANET);
	//m_obj366.Init();
	//if (m_obj366.GetCardExist())
	//{
	//	SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"��");
	//	if (m_obj366.GetNetWork())
	//	{
	//		SetDlgItemText(IDC_EDIT_NETWORK, L"����");

	//		m_iRadio = 2;
	//		((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(TRUE);//ѡ��
	//		((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(FALSE);//��ѡ��
	//	}
	//	else
	//	{
	//		SetDlgItemText(IDC_EDIT_NETWORK, L"����");
	//		m_iRadio = 1;
	//		((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(TRUE);//ѡ��
	//		((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(FALSE);//��ѡ��
	//	}
	//}
	//else
	//{
	//	SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"û��");
	//	SetDlgItemText(IDC_EDIT_NETWORK, L"δ֪");
	//}
	int iNetReturn = 0;
	if (1 != OpenCH366())
	{
		SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"�޿�");
		SetDlgItemText(IDC_EDIT_NETWORK, L"δ֪");
		::MessageBox(NULL, L"��366��ʧ�ܣ����ܿ�δ��á���", L"error", MB_OK);
		
	}
	else
	{
		SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"366");
		iNetReturn = GetNet();//1Ϊ������2Ϊ����
		if (1== iNetReturn)
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"����");

			m_iRadio = 2;
			((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(TRUE);//ѡ��
			((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(FALSE);//��ѡ��
		}
		else if (2 == iNetReturn)
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"����");
			m_iRadio = 1;
			((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(TRUE);//ѡ��
			((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(FALSE);//��ѡ��
		}
		else
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"δ֪");
			::MessageBox(NULL, L"��ȡ����״̬ʧ�ܣ����ܿ�δ��á���", L"error", MB_OK);
		}

	}
	m_iCutover = 2;
	((CButton *)GetDlgItem(IDC_RADIO_HIBERNATE))->SetCheck(TRUE);//ѡ��

	




	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CCH364ISL_MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCH364ISL_MFCDlg::OnPaint()
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
HCURSOR CCH364ISL_MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCH364ISL_MFCDlg::OnBnClickedButtonSwitch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iRet = 0;
	int iPowerOff = 0;
	BOOL bNetbit = FALSE, bInterFace = FALSE;
	
	if (0 == m_iRadio)
	{
	    ::MessageBox(NULL, L"��ѡ���л����������ͣ�", L"TIP", MB_OK);
		return;
	}

	//ȷ���л�����  bNetbitΪ1ʱ�е�����
	bNetbit = (1==m_iRadio)?FALSE:TRUE;
	//ȷ���л�ģʽ 
	iPowerOff = (1==m_iCutover)?1:((2==m_iCutover)?2:3);
	
	iRet = ChangeNet(bNetbit, iPowerOff, bInterFace);

	if (iRet)
	{
		::MessageBox(NULL, L"�л�ʧ�ܣ�������", L"Error", MB_OK);
	}

}

void CCH364ISL_MFCDlg::OnBnClickedRadioIntranet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iRadio = 1;
	((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(TRUE);//ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(FALSE);//��ѡ��
}

void CCH364ISL_MFCDlg::OnBnClickedRadioExtranet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iRadio = 2;
	((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(TRUE);//ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(FALSE);//��ѡ��
}

void CCH364ISL_MFCDlg::OnBnClickedRadioReboot()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iCutover = 1;
	((CButton *)GetDlgItem(IDC_RADIO_REBOOT))->SetCheck(TRUE);//ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_HIBERNATE))->SetCheck(FALSE);//��ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_SHUTDOWN))->SetCheck(FALSE);//��ѡ��
}

void CCH364ISL_MFCDlg::OnBnClickedRadioHibernate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iCutover = 2;
	((CButton *)GetDlgItem(IDC_RADIO_REBOOT))->SetCheck(FALSE);//ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_HIBERNATE))->SetCheck(TRUE);//��ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_SHUTDOWN))->SetCheck(FALSE);//��ѡ��
}

void CCH364ISL_MFCDlg::OnBnClickedRadioShutdown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_iCutover = 3;
	((CButton *)GetDlgItem(IDC_RADIO_REBOOT))->SetCheck(FALSE);//ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_HIBERNATE))->SetCheck(FALSE);//��ѡ��
	((CButton *)GetDlgItem(IDC_RADIO_SHUTDOWN))->SetCheck(TRUE);//��ѡ��
}

//�ȴ���ʱ������ĳһʱ����߹̶���ʱ���������Լ���֪ͨ�źŵ��ں˶���ͨ��ʹ��
//SetWaitableTimer��������ʱ��͹涨ʱ���������ں˶������û���ʱ����Set_Timer���� ��
//��Щ���ƣ� ���Ĳ�����ڣ� �ȴ���ʱ���ǿ����ڶ��̵߳��ں˶��� ���û���ʱ������ʱ��
//ֻ��һ���̵߳õ�֪ͨ��
LRESULT CCH364ISL_MFCDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	//WM_POWERBROADCASTͨ�� WindowProc() ��������֪��Դ�����¼��Ѿ�/�������У���Ӧ�ù㲥����Ϣ
	//if (message == WM_POWERBROADCAST)
	//{
	//	switch(wParam)
	//	{//SDK�����У�32λWindows��ִ�е�Դ����ʱ�ᷢ��WM_POWERBROADCAST��Ϣ����Ҫ�������߾�Ҫ�������Ϣ��
	//	case PBT_APMQUERYSUSPEND: //��Ҫ����ɲ��ܹ�����¼�
	//		{ 
	//			//����һ���ɵȴ��ļ�ʱ������     
	//			m_hTimer = ::CreateWaitableTimer(
	//				NULL,
	//				TRUE,              //TRUE���ֶ����ü�ʱ����FALSE���Զ����ü�ʱ���� 
	//				_T("Lzyj")
	//				);
	//			if(!m_hTimer)	
	//				break;
	//			m_hTimer = OpenWaitableTimer(TIMER_ALL_ACCESS, TRUE, _T("Lzyj"));		//Ϊ���е�һ���ɵȴ���ʱ�����󴴽�һ���¾��	
	//			LARGE_INTEGER liDueTime;			
	//			liDueTime.QuadPart= 15 * 1000 * 1000 * (-10);
	//			
	//			// ����һ���ɵȴ���ʱ����������Ϊδ���ź�״̬	
	//			SetWaitableTimer(
	//				m_hTimer,
	//				&liDueTime,  //��ʱ����һ�δ�����ʱ�䣻 
	//				0,          //��һ�δ���֮�󣬼�ʱ��Ӧ����������Ƶ�ȴ�����
	//				NULL,
	//				NULL,
	//				TRUE);	
	//			break;			
	//		}

	//	case PBT_APMRESUMESUSPEND:  // ֪ͨ�ӹ���״̬�ָ����¼� 
	//		{
	//			OutputDebugString(L"\r\n");
	//			if(m_hTimer)
	//			{
	//				CancelWaitableTimer(m_hTimer);  //ȡ����ʱ������Զ���ᴥ���ˣ������ٵ���SetWaitableTimer��������������
	//				CloseHandle(m_hTimer);
	//				::OutputDebugString( _T( "���ѳɹ�" ) );
	//			}
	//			//��һ���Ƿ���ͨ�������л���ʼ��Ϊ��
	//			break;
	//		}
	//	}	
	//}

	return CDialog::WindowProc(message, wParam, lParam);
}



void CCH364ISL_MFCDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	CloseCH366();
	CDialog::OnOK();
}

void CCH364ISL_MFCDlg::OnBnClickedButtonOpen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iNetReturn = 0;
	if (g_bOpenFlag)
	{
		return ;
	}
	if (!InitCH366DLL())
	{
		::MessageBox(NULL, L"��ʼ��CH366.DLLʧ��", L"", MB_OK);
		return;
	}
	if (1 != OpenCH366())
	{
		SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"�޿�");
		SetDlgItemText(IDC_EDIT_NETWORK, L"δ֪");
		::MessageBox(NULL, L"��366��ʧ�ܣ����ܿ�δ��á���", L"error", MB_OK);

	}
	else
	{
		SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"366");
		iNetReturn = GetNet();//1Ϊ������2Ϊ����
		if (1== iNetReturn)
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"����");

			m_iRadio = 2;
			((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(TRUE);//ѡ��
			((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(FALSE);//��ѡ��
		}
		else if (2 == iNetReturn)
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"����");
			m_iRadio = 1;
			((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(TRUE);//ѡ��
			((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(FALSE);//��ѡ��
		}
		else
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"δ֪");
			::MessageBox(NULL, L"��ȡ����״̬ʧ�ܣ����ܿ�δ��á���", L"error", MB_OK);
		}

	}

}

void CCH364ISL_MFCDlg::OnBnClickedButtonClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int iNetReturn = 0;
	if (g_bOpenFlag)
	{
		CloseCH366();
		ReleaseCh366();
	}

	SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"�޿�");
	//SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"366");
	//iNetReturn = GetNet();//1Ϊ������2Ϊ����
	//if (1== iNetReturn)
	//{
	//	SetDlgItemText(IDC_EDIT_NETWORK, L"����");

	//	m_iRadio = 2;
	//	((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(TRUE);//ѡ��
	//	((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(FALSE);//��ѡ��
	//}
	//else if (2 == iNetReturn)
	//{
	//	SetDlgItemText(IDC_EDIT_NETWORK, L"����");
	//	m_iRadio = 1;
	//	((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(TRUE);//ѡ��
	//	((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(FALSE);//��ѡ��
	//}
	//else
	//{
		SetDlgItemText(IDC_EDIT_NETWORK, L"δ֪");
	//	::MessageBox(NULL, L"��ȡ����״̬ʧ�ܣ����ܿ�δ��á���", L"error", MB_OK);
	//}
}

