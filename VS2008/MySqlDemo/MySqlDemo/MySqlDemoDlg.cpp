
// MySqlDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MySqlDemo.h"
#include "MySqlDemoDlg.h"
//#include "NetMySql.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int PaintTime=0;;
struct scale
{
	double xs1;
	double ys1;
	double xs2;
	double ys2;
};
scale s[100];
int i=0;
int WindowWidth;
int WindwoHeight;
int xOffset;
int yOffset;
BOOL   CALLBACK   EnumWindowsProc(   
								  HWND   hwnd,             //   handle   to   parent   window   
								  LPARAM   lParam       //   application-defined   value   
								  )   

{  
	CRect  rc;
	::GetWindowRect(hwnd,&rc);
	rc.left -= xOffset;
	rc.right -= xOffset;
	rc.top -= yOffset;
	rc.bottom -= yOffset;
	s[i].xs1=(double)rc.left/(double)WindowWidth;
	s[i].ys1=(double)rc.top/(double)WindwoHeight;
	s[i].xs2=(double)rc.Width()/(double)WindowWidth;
	s[i].ys2=(double)rc.Height()/(double)WindwoHeight;
	i++;
	return   TRUE;   
} 

BOOL   CALLBACK   SetWindowRectProc(   
									HWND   hwnd,             //   handle   to   parent   window   
									long  lParam       //   application-defined   value   
									)   
{   
	CRect rc;
	rc.left=WindowWidth*s[i].xs1;
	rc.top=WindwoHeight*s[i].ys1;
	rc.right=WindowWidth*s[i].xs1+WindowWidth*s[i].xs2;
	rc.bottom=WindwoHeight*s[i].ys1+WindwoHeight*s[i].ys2;
	::MoveWindow(hwnd,rc.left,rc.top,rc.Width(),rc.Height(),1);
	i++;

	return   TRUE;   
} 

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


// CMySqlDemoDlg �Ի���




CMySqlDemoDlg::CMySqlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMySqlDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	bRun=FALSE;
}

void CMySqlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_LEADSHIP, m_ListLeadShip);
	DDX_Control(pDX, IDC_HOTKEY1, m_HotKeyCtrl);
	DDX_Control(pDX, IDC_HOTKEY2, m_HotKeyCtrlSHOW);
}

BEGIN_MESSAGE_MAP(CMySqlDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TEST, &CMySqlDemoDlg::OnBnClickedButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CMySqlDemoDlg::OnBnClickedButtonSet)
	ON_WM_HOTKEY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMySqlDemoDlg ��Ϣ�������

BOOL CMySqlDemoDlg::OnInitDialog()
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
	LoadPathImage("scbk.bmp");
	bRun=TRUE;
	PostMessage(WM_SIZE,0,0);
	RECT rect;
	m_ListLeadShip.GetWindowRect(&rect);

	m_ListLeadShip.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_TRACKSELECT|LVS_EX_FLATSB|LVS_EX_UNDERLINEHOT|LVS_EX_GRIDLINES);
	m_ListLeadShip.InsertColumn(0, _T("lsid"), LVCFMT_IMAGE|LVCFMT_LEFT, 40);
	m_ListLeadShip.InsertColumn(1, _T("lsname"), LVCFMT_LEFT, 100);
	m_ListLeadShip.InsertColumn(2, _T("lssex"), LVCFMT_LEFT, 40);
	m_ListLeadShip.InsertColumn(3, _T("lsuid"), LVCFMT_LEFT, 70);
	m_ListLeadShip.InsertColumn(4, _T("lspwd"), LVCFMT_LEFT, 70);
	m_ListLeadShip.InsertColumn(5, _T("lsstate"), LVCFMT_LEFT, 50);
	m_ListLeadShip.InsertColumn(6, _T("hid"), LVCFMT_LEFT, 40);
	m_ListLeadShip.InsertColumn(7, _T("lid"), LVCFMT_LEFT, 40);
	m_ListLeadShip.InsertColumn(8, _T("lsoffice"), LVCFMT_LEFT, 100);


	m_ListLeadShip.SetHoverTime(500000);

	m_ListLeadShip.SetRedraw();



	InitMySql();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMySqlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMySqlDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		m_objCDC.CreateCompatibleDC(&dc);     //��mdת������dc���ݵ�DC	

		//���öԻ��򱳾�ͼƬ	
		m_objCDC.SelectObject(m_objBitmap);      //ȡ��Դλͼ

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
HCURSOR CMySqlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMySqlDemoDlg::InitMySql()
{
	int iTemp = 0;
	UINT iLen = 0;
	unsigned uiNum = 0;
	CString csTemp ;
	WCHAR wszTemp[MAX_PATH] = {0};
	
	char* pTemp = NULL;
	if (m_objMySql.Connect("10.0.100.202", "root", "tiptop", "confidentialcirculation", 3306))
	{
		uiNum = m_objMySql.ExecuteQuery("select * from `leadership`");
		for (int i=0; i<uiNum; i++)
		{
			//iTemp = m_objMySql.GetIntValue(i, "lsid");
			//swprintf(wszTemp, L"%d", iTemp);
			//m_ListLeadShip.InsertItem(i, wszTemp, NULL);  
			////m_SoftInfoList.InsertItem(dwCount-1, szExeFilePath, dwCount-1);
			//pTemp = m_objMySql.GetStringValue(i, "lsname", &iLen);
			//csTemp = pTemp;
			//m_ListLeadShip.SetItemText(i, 1, csTemp);
			////m_ListLeadShip.SetItemText(dwNum-1, 2, csTemp);

			//iTemp = m_objMySql.GetIntValue(i, "lssex");
			//swprintf(wszTemp, L"%d", iTemp);
			//m_ListLeadShip.SetItemText(i, 2, wszTemp);


			//pTemp = m_objMySql.GetStringValue(i, "lsuid", &iLen);
			//csTemp = pTemp;
			//m_ListLeadShip.SetItemText(i, 3, csTemp);

			//pTemp = m_objMySql.GetStringValue(i, "lspwd", &iLen);
			//csTemp = pTemp;
			//m_ListLeadShip.SetItemText(i, 4, csTemp);

			//iTemp = m_objMySql.GetIntValue(i, "lsstate");
			//swprintf(wszTemp, L"%d", iTemp);
			//m_ListLeadShip.SetItemText(i, 5, wszTemp);

			//iTemp = m_objMySql.GetIntValue(i, "hid");
			//swprintf(wszTemp, L"%d", iTemp);
			//m_ListLeadShip.SetItemText(i, 6, wszTemp);

			//iTemp = m_objMySql.GetIntValue(i, "lid");
			//swprintf(wszTemp, L"%d", iTemp);
			//m_ListLeadShip.SetItemText(i, 7, wszTemp);

			//pTemp = m_objMySql.GetStringValue(i, "lsoffice", &iLen);
			//csTemp = pTemp;
			//m_ListLeadShip.SetItemText(i, 8, csTemp);

			m_ListLeadShip.Update(i);	
		}


	}
}
void CMySqlDemoDlg::OnBnClickedButtonTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT uiLen = 0;
	unsigned uRow = 0;
	char* pTemp = NULL;
	uRow = m_objMySql.ExecuteQuery("select * from leadership where lsname = '���쵼'");
	pTemp = m_objMySql.GetStringValue(0, "lspwd", &uiLen);
}

void CMySqlDemoDlg::OnBnClickedButtonSet()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	WORD Modifiers = 0;
	WORD VirtualKeyCode =0;
	m_HotKeyCtrl.GetHotKey(VirtualKeyCode,Modifiers);
	
	RegisterHotKey(GetSafeHwnd(), 1, Modifiers, VirtualKeyCode);
	WORD Modifiers2 = 0;
	WORD VirtualKeyCode2 =0;
	m_HotKeyCtrlSHOW.GetHotKey(VirtualKeyCode2, Modifiers2);
	RegisterHotKey(GetSafeHwnd(), 2, Modifiers2, VirtualKeyCode2);
	//void SetHotKey(WORD wVirtualKeyCode, WORD wModifiers);

}

void CMySqlDemoDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (1 == nHotKeyId)
	{
		ShowWindow(SW_HIDE);
	}

	if (2 == nHotKeyId)
	{
		ShowWindow(SW_SHOW);
	}
	

	CDialog::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CMySqlDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(bRun)
	{
		if(PaintTime == 0)
		{
			PaintTime++;
			i=0;
			CRect rc1;
			GetClientRect(&rc1);
			WindowWidth=rc1.Width();
			WindwoHeight=rc1.Height();
			CRect rc2=rc1;
			ClientToScreen(&rc2);
			xOffset=rc2.left-rc1.left;
			yOffset=rc2.top-rc1.top;
			EnumChildWindows(this->GetSafeHwnd(),EnumWindowsProc,0);  
			ShowWindow(SW_SHOWMAXIMIZED);
		}
		else
		{
			CRect rc1;
			GetClientRect(&rc1);
			WindowWidth=rc1.Width();
			WindwoHeight=rc1.Height();
			ClientToScreen(&rc1);
			i=0;
			EnumChildWindows(this->GetSafeHwnd(),SetWindowRectProc,0); 
			m_ListLeadShip.UpdateData(FALSE);
			UpdateData(FALSE);
		}

	}

	// TODO: �ڴ˴������Ϣ����������
}

void CMySqlDemoDlg::LoadPathImage(PCHAR pszPath)
{
	RECT rect;
	GetClientRect(&rect);
	m_objBitmap.m_hObject = (HBITMAP)::LoadImage(NULL,pszPath,IMAGE_BITMAP,rect.right-rect.left,rect.bottom-rect.top,LR_LOADFROMFILE); //����Դλͼͼ��
}