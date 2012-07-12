// CH364ISL_MFCDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CCH364ISL_MFCDlg 对话框




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


// CCH364ISL_MFCDlg 消息处理程序

BOOL CCH364ISL_MFCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (!InitCH366DLL())
	{
		return false;
	}
	
	//CheckRadioButton(IDC_RADIO_INTRANET, IDC_RADIO_EXTRANET, IDC_RADIO_EXTRANET);
	//m_obj366.Init();
	//if (m_obj366.GetCardExist())
	//{
	//	SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"有");
	//	if (m_obj366.GetNetWork())
	//	{
	//		SetDlgItemText(IDC_EDIT_NETWORK, L"内网");

	//		m_iRadio = 2;
	//		((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(TRUE);//选上
	//		((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(FALSE);//不选上
	//	}
	//	else
	//	{
	//		SetDlgItemText(IDC_EDIT_NETWORK, L"外网");
	//		m_iRadio = 1;
	//		((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(TRUE);//选上
	//		((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(FALSE);//不选上
	//	}
	//}
	//else
	//{
	//	SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"没有");
	//	SetDlgItemText(IDC_EDIT_NETWORK, L"未知");
	//}
	int iNetReturn = 0;
	if (1 != OpenCH366())
	{
		SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"无卡");
		SetDlgItemText(IDC_EDIT_NETWORK, L"未知");
		::MessageBox(NULL, L"打开366卡失败，可能卡未插好……", L"error", MB_OK);
		
	}
	else
	{
		SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"366");
		iNetReturn = GetNet();//1为内网，2为外网
		if (1== iNetReturn)
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"内网");

			m_iRadio = 2;
			((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(TRUE);//选上
			((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(FALSE);//不选上
		}
		else if (2 == iNetReturn)
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"外网");
			m_iRadio = 1;
			((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(TRUE);//选上
			((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(FALSE);//不选上
		}
		else
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"未知");
			::MessageBox(NULL, L"获取网络状态失败，可能卡未插好……", L"error", MB_OK);
		}

	}
	m_iCutover = 2;
	((CButton *)GetDlgItem(IDC_RADIO_HIBERNATE))->SetCheck(TRUE);//选上

	




	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CCH364ISL_MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CCH364ISL_MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCH364ISL_MFCDlg::OnBnClickedButtonSwitch()
{
	// TODO: 在此添加控件通知处理程序代码
	int iRet = 0;
	int iPowerOff = 0;
	BOOL bNetbit = FALSE, bInterFace = FALSE;
	
	if (0 == m_iRadio)
	{
	    ::MessageBox(NULL, L"请选择切换的网络类型！", L"TIP", MB_OK);
		return;
	}

	//确定切换网络  bNetbit为1时切到外网
	bNetbit = (1==m_iRadio)?FALSE:TRUE;
	//确定切换模式 
	iPowerOff = (1==m_iCutover)?1:((2==m_iCutover)?2:3);
	
	iRet = ChangeNet(bNetbit, iPowerOff, bInterFace);

	if (iRet)
	{
		::MessageBox(NULL, L"切换失败！！！！", L"Error", MB_OK);
	}

}

void CCH364ISL_MFCDlg::OnBnClickedRadioIntranet()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iRadio = 1;
	((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(TRUE);//选上
	((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(FALSE);//不选上
}

void CCH364ISL_MFCDlg::OnBnClickedRadioExtranet()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iRadio = 2;
	((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(TRUE);//选上
	((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(FALSE);//不选上
}

void CCH364ISL_MFCDlg::OnBnClickedRadioReboot()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iCutover = 1;
	((CButton *)GetDlgItem(IDC_RADIO_REBOOT))->SetCheck(TRUE);//选上
	((CButton *)GetDlgItem(IDC_RADIO_HIBERNATE))->SetCheck(FALSE);//不选上
	((CButton *)GetDlgItem(IDC_RADIO_SHUTDOWN))->SetCheck(FALSE);//不选上
}

void CCH364ISL_MFCDlg::OnBnClickedRadioHibernate()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iCutover = 2;
	((CButton *)GetDlgItem(IDC_RADIO_REBOOT))->SetCheck(FALSE);//选上
	((CButton *)GetDlgItem(IDC_RADIO_HIBERNATE))->SetCheck(TRUE);//不选上
	((CButton *)GetDlgItem(IDC_RADIO_SHUTDOWN))->SetCheck(FALSE);//不选上
}

void CCH364ISL_MFCDlg::OnBnClickedRadioShutdown()
{
	// TODO: 在此添加控件通知处理程序代码
	m_iCutover = 3;
	((CButton *)GetDlgItem(IDC_RADIO_REBOOT))->SetCheck(FALSE);//选上
	((CButton *)GetDlgItem(IDC_RADIO_HIBERNATE))->SetCheck(FALSE);//不选上
	((CButton *)GetDlgItem(IDC_RADIO_SHUTDOWN))->SetCheck(TRUE);//不选上
}

//等待定时器是在某一时间或者固定的时间间隔发出自己的通知信号的内核对象。通过使用
//SetWaitableTimer函数设置时间和规定时间间隔。该内核对象与用户定时器（Set_Timer（） ）
//有些类似， 最大的差别在于： 等待定时器是可用于多线程的内核对象， 而用户定时器报到时，
//只有一个线程得到通知。
LRESULT CCH364ISL_MFCDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	//WM_POWERBROADCAST通过 WindowProc() 函数（告知电源管理事件已经/正在运行）向应用广播的消息
	//if (message == WM_POWERBROADCAST)
	//{
	//	switch(wParam)
	//	{//SDK帮助中，32位Windows在执行电源管理时会发送WM_POWERBROADCAST消息，即要捕获休眠就要捕获该消息，
	//	case PBT_APMQUERYSUSPEND: //需要经许可才能挂起的事件
	//		{ 
	//			//创建一个可等待的计时器对象。     
	//			m_hTimer = ::CreateWaitableTimer(
	//				NULL,
	//				TRUE,              //TRUE，手动重置计时器，FALSE，自动重置计时器； 
	//				_T("Lzyj")
	//				);
	//			if(!m_hTimer)	
	//				break;
	//			m_hTimer = OpenWaitableTimer(TIMER_ALL_ACCESS, TRUE, _T("Lzyj"));		//为现有的一个可等待计时器对象创建一个新句柄	
	//			LARGE_INTEGER liDueTime;			
	//			liDueTime.QuadPart= 15 * 1000 * 1000 * (-10);
	//			
	//			// 启动一个可等待计时器。将它设为未发信号状态	
	//			SetWaitableTimer(
	//				m_hTimer,
	//				&liDueTime,  //计时器第一次触发的时间； 
	//				0,          //第一次触发之后，计时器应该以怎样的频度触发；
	//				NULL,
	//				NULL,
	//				TRUE);	
	//			break;			
	//		}

	//	case PBT_APMRESUMESUSPEND:  // 通知从挂起状态恢复的事件 
	//		{
	//			OutputDebugString(L"\r\n");
	//			if(m_hTimer)
	//			{
	//				CancelWaitableTimer(m_hTimer);  //取消计时器就永远不会触发了，除非再调用SetWaitableTimer来对它进行重置
	//				CloseHandle(m_hTimer);
	//				::OutputDebugString( _T( "唤醒成功" ) );
	//			}
	//			//上一次是否是通过休眠切换初始化为零
	//			break;
	//		}
	//	}	
	//}

	return CDialog::WindowProc(message, wParam, lParam);
}



void CCH364ISL_MFCDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	CloseCH366();
	CDialog::OnOK();
}

void CCH364ISL_MFCDlg::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码
	int iNetReturn = 0;
	if (g_bOpenFlag)
	{
		return ;
	}
	if (!InitCH366DLL())
	{
		::MessageBox(NULL, L"初始化CH366.DLL失败", L"", MB_OK);
		return;
	}
	if (1 != OpenCH366())
	{
		SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"无卡");
		SetDlgItemText(IDC_EDIT_NETWORK, L"未知");
		::MessageBox(NULL, L"打开366卡失败，可能卡未插好……", L"error", MB_OK);

	}
	else
	{
		SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"366");
		iNetReturn = GetNet();//1为内网，2为外网
		if (1== iNetReturn)
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"内网");

			m_iRadio = 2;
			((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(TRUE);//选上
			((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(FALSE);//不选上
		}
		else if (2 == iNetReturn)
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"外网");
			m_iRadio = 1;
			((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(TRUE);//选上
			((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(FALSE);//不选上
		}
		else
		{
			SetDlgItemText(IDC_EDIT_NETWORK, L"未知");
			::MessageBox(NULL, L"获取网络状态失败，可能卡未插好……", L"error", MB_OK);
		}

	}

}

void CCH364ISL_MFCDlg::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	int iNetReturn = 0;
	if (g_bOpenFlag)
	{
		CloseCH366();
		ReleaseCh366();
	}

	SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"无卡");
	//SetDlgItemText(IDC_EDIT_DEVICE_EXIST, L"366");
	//iNetReturn = GetNet();//1为内网，2为外网
	//if (1== iNetReturn)
	//{
	//	SetDlgItemText(IDC_EDIT_NETWORK, L"内网");

	//	m_iRadio = 2;
	//	((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(TRUE);//选上
	//	((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(FALSE);//不选上
	//}
	//else if (2 == iNetReturn)
	//{
	//	SetDlgItemText(IDC_EDIT_NETWORK, L"外网");
	//	m_iRadio = 1;
	//	((CButton *)GetDlgItem(IDC_RADIO_INTRANET))->SetCheck(TRUE);//选上
	//	((CButton *)GetDlgItem(IDC_RADIO_EXTRANET))->SetCheck(FALSE);//不选上
	//}
	//else
	//{
		SetDlgItemText(IDC_EDIT_NETWORK, L"未知");
	//	::MessageBox(NULL, L"获取网络状态失败，可能卡未插好……", L"error", MB_OK);
	//}
}

