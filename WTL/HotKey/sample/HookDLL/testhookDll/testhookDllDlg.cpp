// testhookDllDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testhookDll.h"
#include "testhookDllDlg.h"
#include <direct.h>
#include "Global.h"
#include<tlhelp32.h> 
 
#define ERR_UNABLE_TO_KILL	1005
#define ERR_UNKNOWN_PROCESS 1006

//#include "testhookDll\HookDll.h"
//#pragma comment("lib","HookDLL.lib")
 
// _CtrlAltDel_Enable_Disable@4

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTesthookDllDlg dialog

CTesthookDllDlg::CTesthookDllDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTesthookDllDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTesthookDllDlg)
	m_win = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTesthookDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTesthookDllDlg)
 
	DDX_Control(pDX, IDC_BUTTON_DISABLE_ALL, m_disall);
	DDX_Control(pDX, IDC_BUTTON_OPEN_ALL, m_openall);
	DDX_Control(pDX, IDC_CHECK_TAB, m_tab);
	DDX_Control(pDX, IDC_CHECK_PrtSc, m_prtsc);
	DDX_Control(pDX, IDC_CHECKTaskmgr, m_taskmgr);
	DDX_Control(pDX, IDC_CHECK_ESC, m_esc);
	DDX_Control(pDX, IDC_CHECK_DELTE, m_delete);
	DDX_Control(pDX, IDC_CHECK_CTRL_Z, m_ctrl_z);
	DDX_Control(pDX, IDC_CHECK_CTRL_X, m_ctrl_x);
	DDX_Control(pDX, IDC_CHECK_CTRL_V, m_ctrl_v);
	DDX_Control(pDX, IDC_CHECK_CTRL_F, m_ctrl_f);
	DDX_Control(pDX, IDC_CHECK_CTRL_E, m_ctrl_e);
	DDX_Control(pDX, IDC_CHECK_CTRL_D, m_ctrl_d);
	DDX_Control(pDX, IDC_CHECK_CTRL_C, m_ctrl_c);
	DDX_Control(pDX, IDC_CHECK_CTRL_A, m_ctrl_a);
	DDX_Check(pDX, IDC_CHECK_WIN_LR, m_win);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTesthookDllDlg, CDialog)
	//{{AFX_MSG_MAP(CTesthookDllDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BNTShielding_ALL_HOTKEY, OnBNTShieldingALLHOTKEY)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_ALL, OnButtonOpenAll)
	ON_BN_CLICKED(IDC_BUTTON_DISABLE_ALL, OnButtonDisableAll)
	ON_BN_CLICKED(IDC_CHECK_CTRL_A, OnCheckCtrlA)
	ON_BN_CLICKED(IDC_CHECK_CTRL_C, OnCheckCtrlC)
	ON_BN_CLICKED(IDC_CHECK_CTRL_D, OnCheckCtrlD)
	ON_BN_CLICKED(IDC_CHECK_CTRL_E, OnCheckCtrlE)
	ON_BN_CLICKED(IDC_CHECK_CTRL_F, OnCheckCtrlF)
	ON_BN_CLICKED(IDC_CHECK_CTRL_V, OnCheckCtrlV)
	ON_BN_CLICKED(IDC_CHECK_CTRL_X, OnCheckCtrlX)
	ON_BN_CLICKED(IDC_CHECK_CTRL_Z, OnCheckCtrlZ)
	ON_BN_CLICKED(IDC_CHECKTaskmgr, OnCHECKTaskmgr)
	ON_BN_CLICKED(IDC_CHECK_TAB, OnCheckTab)
	ON_BN_CLICKED(IDC_CHECK_PrtSc, OnCHECKPrtSc)
	ON_BN_CLICKED(IDC_CHECK_DELTE, OnCheckDelte)
	ON_BN_CLICKED(IDC_CHECK_ESC, OnCheckEsc)
	ON_BN_CLICKED(IDC_BUTTONSAVASETING, OnButtonsavaseting)
	ON_BN_CLICKED(IDC_BUTTONDefault, OnBUTTONDefault)
	ON_WM_TIMER()
	ON_WM_CANCELMODE()
	ON_WM_CAPTURECHANGED()
	ON_BN_CLICKED(IDC_BUTTON_ACTION, OnButtonAction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTesthookDllDlg message handlers

//extern "C"	__declspec(dllimport) void sethook(HWND hwnd);

BOOL CTesthookDllDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
    
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	//////////////////////////////////////////////////////////////////////////

	//删除WS_EX_APPWINDOW扩展风格
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW,0);
	
	//ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);   
	//显示隐藏   
/*  
    // 这段代码OK
	WINDOWPLACEMENT   wp;   
	wp.length=sizeof(WINDOWPLACEMENT);   
	wp.flags=WPF_RESTORETOMAXIMIZED;   
	wp.showCmd=SW_HIDE;   
	SetWindowPlacement(&wp);
	AfxGetApp()->m_nCmdShow=SW_HIDE;
*/  
    EixtProc();
	CFileFind finder;
	BOOL bFind = finder.FindFile(_T("HookDLL.dll"));
    if (!bFind)
    {
		MessageBox("缺少HookDLL.dll，相关服务无法启动    ","",MB_ICONSTOP);
		SendMessage(WM_CLOSE,NULL,NULL);
		return FALSE ;
    }
    g_hDLL = LoadLibrary("HookDLL.dll");                    // 加载动态库
	CTesthookDllDlg::InitEx();                              // 初始化选项  ，必须在放在加载动态库之后
 	hookdll = (P_HookDLL)GetProcAddress(g_hDLL,"setHook");  // 开始加载钩子
  
    hookdll(m_hWnd);
	//////////////////////////////////////////////////////////////////////////
 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTesthookDllDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTesthookDllDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTesthookDllDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTesthookDllDlg::unhookFun()
{

}

void CTesthookDllDlg::OnBNTShieldingALLHOTKEY() 
{
	// TODO: Add your control notification handler code here
	
// 	P_HookDLL hookdll = (P_HookDLL)GetProcAddress(g_hDLL,"setHook");
//     //hookdll(m_hWnd);
}
void CTesthookDllDlg::unhookdll1()
{

}

void CTesthookDllDlg::OnButtonOpenAll() 
{
}

void CTesthookDllDlg::OnButtonDisableAll() 
{
	// TODO: Add your control notification handler code here
	
}

void CTesthookDllDlg::OnCheckCtrlA() 
{
	// TODO: Add your control notification handler code here

	if (set_Ctrl_A ==NULL )
	{
		set_Ctrl_A = (P_CTR_A)GetProcAddress(g_hDLL,"set_Ctrl_A");
	}

	if (m_ctrl_a.GetCheck())
	{
        set_Ctrl_A(false);
	}
	else
	{
        set_Ctrl_A(true);
	 
	}
    //hookdll(m_hWnd);
}

void CTesthookDllDlg::OnCheckCtrlC() 
{
	// TODO: Add your control notification handler code here
	if (set_Ctrl_C ==NULL )
	{
		set_Ctrl_C = (P_CTR_C)GetProcAddress(g_hDLL,"set_Ctrl_C");
	}
 
	if (m_ctrl_c.GetCheck())
	{
        set_Ctrl_C(false);
	}
	else
	{
        set_Ctrl_C(true); 
	}
    //hookdll(m_hWnd);
}


void CTesthookDllDlg::OnCheckCtrlD() 
{
	// TODO: Add your control notification handler code here
	if (set_Ctrl_D ==NULL )
	{
		set_Ctrl_D = (P_CTR_A)GetProcAddress(g_hDLL,"set_Ctrl_D");
	}
	
	if (m_ctrl_d.GetCheck())
	{
        set_Ctrl_D(false);
	}
	else
	{
        set_Ctrl_D(true);
	}
    //hookdll(m_hWnd);
}

void CTesthookDllDlg::OnCheckCtrlE() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if (set_Ctrl_E ==NULL )
	{
		set_Ctrl_E = (P_CTR_E)GetProcAddress(g_hDLL,"set_Ctrl_E");
	}
	
	if (m_ctrl_e.GetCheck())
	{
        set_Ctrl_E(false);
	}
	else
	{
        set_Ctrl_E(true);
	}
    //hookdll(m_hWnd);
}

void CTesthookDllDlg::OnCheckCtrlF() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if (set_Ctrl_F ==NULL )
	{
		set_Ctrl_F = (P_CTR_F)GetProcAddress(g_hDLL,"set_Ctrl_F");
	}
	
	if (m_ctrl_f.GetCheck())
	{
	
        set_Ctrl_F(false);
	}
	else
	{
        set_Ctrl_F(true);
	}
    //hookdll(m_hWnd);
}

void CTesthookDllDlg::OnCheckCtrlV() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if (set_Ctrl_V ==NULL )
	{
		set_Ctrl_V = (P_CTR_V)GetProcAddress(g_hDLL,"set_Ctrl_V");
	}
	
	if (m_ctrl_v.GetCheck())
	{

        set_Ctrl_V(false);
	} 
	else
	{ 
        set_Ctrl_V(true);
 
	}
    //hookdll(m_hWnd);
}

void CTesthookDllDlg::OnCheckCtrlX() 
{
	// TODO: Add your control notification handler code here
	if (set_Ctrl_X ==NULL )
	{
		set_Ctrl_X = (P_CTR_X)GetProcAddress(g_hDLL,"set_Ctrl_X");
	}
	
	if (m_ctrl_x.GetCheck())
	{
        set_Ctrl_X(false);
	}
	else
	{
        set_Ctrl_X(true);
	}
    //hookdll(m_hWnd);
}
 

void CTesthookDllDlg::OnCheckCtrlZ() 
{
	// TODO: Add your control notification handler code here
	if (set_Ctrl_Z==NULL )
	{
		set_Ctrl_Z = (P_CTR_Z)GetProcAddress(g_hDLL,"set_Ctrl_Z");
	}
	
	if (m_ctrl_z.GetCheck())
	{
        set_Ctrl_Z(false);
	}
	else
	{
        set_Ctrl_Z(true);
	}
    //hookdll(m_hWnd); 
}

void CTesthookDllDlg::OnCHECKTaskmgr() 
{
	// TODO: Add your control notification handler code here
	if (setTaskMgr==NULL )
	{
		setTaskMgr = (P_Taskmgr)GetProcAddress(g_hDLL,"set_Taskmgr");
	}
	
	if (m_taskmgr.GetCheck())
	{
        setTaskMgr(false);
	}
	else
	{
        setTaskMgr(true);
	}
    //hookdll(m_hWnd); 
}

void CTesthookDllDlg::OnCheckTab() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if (setTab==NULL )
	{
		setTab = (P_Table)GetProcAddress(g_hDLL,"set_Tab");
	}
	
	if (m_tab.GetCheck())
	{
        setTab(false);
	}
	else
	{
        setTab(true);
	}
    //hookdll(m_hWnd);
}

void CTesthookDllDlg::OnCHECKPrtSc() 
{
	// TODO: Add your control notification handler code here  set_Snashot
	if (setSnashot==NULL )
	{
		setSnashot = (P_Snashot)GetProcAddress(g_hDLL,"set_Snashot");
	}
	
	if (m_prtsc.GetCheck())
	{
        setSnashot(false);
	}
	else
	{
        setSnashot(true);
	}
    //hookdll(m_hWnd); 
}

void CTesthookDllDlg::OnCheckDelte() 
{
	// TODO: Add your control notification handler code here 
	if (setDelete==NULL )
	{
		setDelete = (P_Delete)GetProcAddress(g_hDLL,"set_Delete");
	}

	if (m_delete.GetCheck())
	{
        setDelete(false);
	}
	else
	{
        setDelete(true);
	}
    //hookdll(m_hWnd); 
}

void CTesthookDllDlg::OnCheckEsc() 
{
	// TODO: Add your control notification handler code here
//	unHook(m_hWnd);
	if (setEsc==NULL )
	{
		setEsc = (P_Esc)GetProcAddress(g_hDLL,"set_Esc");
	}
	
	if (m_esc.GetCheck())
	{
        setEsc(false);
	}
	else
	{
        setEsc(true);
	}
     //hookdll(m_hWnd); 
}

//////////////////////////////////////////////////////////////////////////
// 读写INI文件
bool CTesthookDllDlg::INIOperation()
{
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// 写配置文件
void CTesthookDllDlg::writeIni(CString section, CString key, CString value, CString path)
{
	CFileFind finder;
	BOOL bFind = finder.FindFile(path);
	if (!bFind)
	{
		_mkdir(pathName);
	}
	
	WritePrivateProfileString(section,key,value,(LPCSTR)path );
}

//////////////////////////////////////////////////////////////////////////
// 读取配置文件键值 并返回整型数据
INT CTesthookDllDlg::ReadIni(CString section, CString key, CString path)
{

	return GetPrivateProfileInt(section,key,1,(LPCSTR)path );
}

//////////////////////////////////////////////////////////////////////////
// 初始化连接
void CTesthookDllDlg::InitEx()
{
	UpdateData(FALSE);
	CFileFind finder;
	BOOL bFind = finder.FindFile(path);
	if (bFind)
	{
		loadCofigINI(path);
	}
	else
	{
		_mkdir(pathName);

		writeIni("taskmgr","key","1",path);
		writeIni("delete","key","1",path);
		writeIni("prtsc","key","1",path);
		writeIni("tab","key","1",path);
		writeIni("esc","key","1",path);
		writeIni("ctrl_a","key","1",path);
		writeIni("ctrl_c","key","1",path);
		writeIni("ctrl_d","key","1",path);
		writeIni("ctrl_e","key","1",path);
		writeIni("ctrl_f","key","1",path);
		writeIni("ctrl_v","key","1",path);   
		writeIni("ctrl_z","key","1",path);
	    writeIni("ctrl_x","key","1",path);

		m_taskmgr.SetCheck(TRUE);
		m_delete.SetCheck(TRUE);
		m_prtsc.SetCheck(TRUE);
		m_tab.SetCheck(TRUE);
		m_esc.SetCheck(TRUE);
		m_ctrl_a.SetCheck(TRUE); 
		m_ctrl_c.SetCheck(TRUE);
		m_ctrl_d.SetCheck(TRUE);
		m_ctrl_e.SetCheck(TRUE);
		m_ctrl_f.SetCheck(TRUE);
		m_ctrl_v.SetCheck(TRUE);
		m_ctrl_z.SetCheck(TRUE);
		m_ctrl_x.SetCheck(TRUE);
		UpdateData(TRUE);
	}


 
   
    UpdateData(TRUE);

	OnCHECKTaskmgr();
	OnCheckDelte();
	OnCHECKPrtSc();
	OnCheckTab();
	OnCheckEsc();
     
	OnCheckCtrlA();
	OnCheckCtrlC();
	OnCheckCtrlD();
	OnCheckCtrlE();
	OnCheckCtrlF();
	OnCheckCtrlV();
	OnCheckCtrlX();
 	OnCheckCtrlZ();

	
 
}
//////////////////////////////////////////////////////////////////////////
// 加载设置
void CTesthookDllDlg::loadCofigINI(CString path)
{
	m_taskmgr.SetCheck(ReadIni("taskmgr","key",path));
	m_delete.SetCheck(ReadIni("delete","key",path));
	m_prtsc.SetCheck(ReadIni("prtsc","key",path));
	m_tab.SetCheck(ReadIni("tab","key",path));
	m_esc.SetCheck(ReadIni("esc","key",path));
	m_ctrl_a.SetCheck(ReadIni("ctrl_a","key",path)); 
	m_ctrl_c.SetCheck(ReadIni("ctrl_c","key",path));
	m_ctrl_d.SetCheck(ReadIni("ctrl_d","key",path));
	m_ctrl_e.SetCheck(ReadIni("ctrl_e","key",path));
	m_ctrl_f.SetCheck(ReadIni("ctrl_f","key",path));
	m_ctrl_v.SetCheck(ReadIni("ctrl_v","key",path));
	m_ctrl_z.SetCheck(ReadIni("ctrl_z","key",path));
	m_ctrl_x.SetCheck(ReadIni("ctrl_x","key",path));
}
//////////////////////////////////////////////////////////////////////////
// 保存设置
void CTesthookDllDlg::OnButtonsavaseting() 
{
	// TODO: Add your control notification handler code here

	UINT MsgRnt= MessageBox("下次继续使用该配置？","",MB_YESNO);
	if (MsgRnt == IDNO)
	{
		return;
	}

	if( m_taskmgr.GetCheck())
	{
		writeIni("taskmgr","key","1",path);
	}
	else 
	{
        writeIni("taskmgr","key","0",path);
	}

	if( m_delete.GetCheck())
	{
		writeIni("delete","key","1",path);
	}
	else 
	{
		writeIni("delete","key","0",path);
	}

	if( m_prtsc.GetCheck())
	{
		writeIni("prtsc","key","1",path);
	}
	else 
	{
		writeIni("prtsc","key","0",path);
	}

	if( m_tab.GetCheck())
	{
		writeIni("tab","key","1",path);
	}

	else 
	{
		writeIni("tab","key","0",path);
	}

	if( m_esc.GetCheck())
	{
		writeIni("esc","key","1",path);
	}
	else 
	{
		writeIni("esc","key","0",path);
	}

	if( m_ctrl_a.GetCheck()) 
	{
		writeIni("ctrl_a","key","1",path);
	}
	else 
	{
		writeIni("ctrl_a","key","0",path);
	}

	if( m_ctrl_c.GetCheck())
	{
		writeIni("ctrl_c","key","1",path);
	}
	else 
	{
		writeIni("ctrl_c","key","0",path);
	}
 
	if( m_ctrl_d.GetCheck())
	{
		writeIni("ctrl_d","key","1",path);
	}
	else 
	{
		writeIni("ctrl_d","key","0",path);
	}

	if( m_ctrl_e.GetCheck())
	{
		writeIni("ctrl_e","key","1",path);
	}
	else 
	{
		writeIni("ctrl_e","key","0",path);
	}

	if( m_ctrl_f.GetCheck())
	{
		writeIni("ctrl_f","key","1",path);
	}
	else 
	{
        writeIni("ctrl_f","key","0",path);
	}
	if( m_ctrl_v.GetCheck())
	{
		writeIni("ctrl_v","key","1",path);
	}
    else
	{
		writeIni("ctrl_v","key","0",path);
	}

	if( m_ctrl_z.GetCheck())
	{
		writeIni("ctrl_z","key","1",path);
	}
	else
	{
        writeIni("ctrl_z","key","0",path);
	}


    if( m_ctrl_x.GetCheck())
	{
         writeIni("ctrl_x","key","1",path);
	}
	else
	{
        writeIni("ctrl_x","key","1",path);
	}
  

}

void CTesthookDllDlg::OnBUTTONDefault() 
{
	// TODO: Add your control notification handler code here
	CFileFind finder;
	BOOL bFind = finder.FindFile(path);
	if (!bFind)
	{
		_mkdir(pathName);
	}

	m_taskmgr.SetCheck(TRUE);
	m_delete.SetCheck(TRUE);
	m_prtsc.SetCheck(TRUE);
	m_tab.SetCheck(TRUE);
	m_esc.SetCheck(TRUE);
	m_ctrl_a.SetCheck(TRUE); 
	m_ctrl_c.SetCheck(TRUE);
	m_ctrl_d.SetCheck(TRUE);
	m_ctrl_e.SetCheck(TRUE);
	m_ctrl_f.SetCheck(TRUE);
	m_ctrl_v.SetCheck(TRUE);
	m_ctrl_z.SetCheck(TRUE);
	m_ctrl_x.SetCheck(TRUE);

	writeIni("taskmgr","key","1",path);
	writeIni("delete","key","1",path);
	writeIni("prtsc","key","1",path);
	writeIni("tab","key","1",path);
	writeIni("esc","key","1",path);
	writeIni("ctrl_a","key","1",path);
	writeIni("ctrl_c","key","1",path);
	writeIni("ctrl_d","key","1",path);
	writeIni("ctrl_e","key","1",path);
	writeIni("ctrl_f","key","1",path);
	writeIni("ctrl_v","key","1",path);
	writeIni("ctrl_z","key","1",path);
	writeIni("ctrl_x","key","1",path);

    MessageBox("设置成功!","",MB_ICONQUESTION);

}


void CTesthookDllDlg::OnButtonAction() 
{
	// TODO: Add your control notification handler code here
	SetWindowPos(&wndTop,0,0,0,0,NULL);

}

void CTesthookDllDlg::EixtProc()
{
	HANDLE handle; //定义CreateToolhelp32Snapshot系统快照句柄 
	handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);//获得系统快照句柄 	
	PROCESSENTRY32 info; //定义PROCESSENTRY32结构字指 	
	info.dwSize=sizeof(PROCESSENTRY32); 
	KKillByName(handle,info,"HotKeyMgr.exe");
 
	Sleep(200);
 
 
}


bool CTesthookDllDlg::KIfequalStrAndProcess(char *szStr ,char *szProcess)
{
	int i;
	if (szStr==NULL && szProcess==NULL)
		return FALSE;
	for (i=0; ;i++)
	{
		if (szStr[i]=='\0' && (szProcess[i]=='.' || szProcess[i]=='\0'))
			return TRUE;
		if (szStr[i]==szProcess[i])
			continue;
		if (((szStr[i]>=65 && szStr[i]<=90) || (szStr[i]>=97 && szStr[i]<=122)) &&
			((szProcess[i]>=65 && szProcess[i]<=90) || (szProcess[i]>=97 && szProcess[i]<=122)))
		{
			if (szStr[i]-32==szProcess[i] || szStr[i]==szProcess[i]-32) 
				continue;
			else
				return FALSE;
		}
		else
			return FALSE;
	}
}

BOOL CTesthookDllDlg::KTerminateProcessID(DWORD dwID)
{
	BOOL bRet = FALSE;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwID);
	if(hProcess!=NULL)
	{
		bRet = TerminateProcess(hProcess, 0);
	}
	CloseHandle(hProcess);
	return bRet;
}



int CTesthookDllDlg::KKillByName(HANDLE hProcessSnap, PROCESSENTRY32 pe32, char *szStr)
{
	BOOL bMore = Process32First(hProcessSnap,&pe32);
	while (bMore)
	{
		if (KIfequalStrAndProcess(szStr, pe32.szExeFile)==TRUE)
		{
			if(KTerminateProcessID(pe32.th32ProcessID)==FALSE)
				return ERR_UNABLE_TO_KILL;
			else 
				return TRUE;
		}
		bMore = Process32Next(hProcessSnap, &pe32);
	}
	
	return ERR_UNKNOWN_PROCESS;
}


LRESULT CTesthookDllDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	 
	if(message == WM_NCPAINT ) 
	{
		if(!b_show)
			ShowWindow(SW_HIDE);
		else
			ShowWindow(SW_SHOW);
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}
