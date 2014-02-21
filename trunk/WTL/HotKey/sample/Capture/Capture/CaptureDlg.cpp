// CaptureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Capture.h"
#include "CaptureDlg.h"
#include <windowsx.h>
#pragma comment(lib,"hook.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define IDM_SHELL WM_USER+1
BOOL __declspec(dllexport)__stdcall  AddHotkey(HWND,UCHAR key,UCHAR mask);
BOOL __declspec(dllexport)__stdcall  DeleteHotkey(HWND,UCHAR key,UCHAR mask);
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
UCHAR Key_Table[]={0x78,0x79,0x7a,0x7b,0x6a,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39};
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
// CCaptureDlg dialog

CCaptureDlg::CCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCaptureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCaptureDlg)
	m_bControl = FALSE;
	m_bAlt = FALSE;
	m_bShift = FALSE;
	m_Path = _T("c:\\");
	m_Number = _T("0 picture captured.");
	nCount=0;
	bRegistered=FALSE;
	bTray=FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCaptureDlg)
	DDX_Control(pDX, IDC_KEY, m_Key);
	DDX_Check(pDX, IDC_CONTROL, m_bControl);
	DDX_Check(pDX, IDC_ALT, m_bAlt);
	DDX_Check(pDX, IDC_SHIFT, m_bShift);
	DDX_Text(pDX, IDC_PATH, m_Path);
	DDX_Text(pDX, IDC_NUMBER, m_Number);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCaptureDlg, CDialog)
	//{{AFX_MSG_MAP(CCaptureDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_BN_CLICKED(ID_CHANGE, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCaptureDlg message handlers

BOOL CCaptureDlg::OnInitDialog()
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
	m_Key.SetCurSel(0);	
	RegisterHotkey();
	CMenu* pMenu=GetSystemMenu(FALSE);
	pMenu->DeleteMenu(SC_MAXIMIZE,MF_BYCOMMAND);
	pMenu->DeleteMenu(SC_SIZE,MF_BYCOMMAND);
	pMenu->DeleteMenu(SC_RESTORE,MF_BYCOMMAND);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCaptureDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCaptureDlg::OnPaint() 
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
HCURSOR CCaptureDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCaptureDlg::OnCancel() 
{
	if(bTray)
		DeleteIcon();
	CDialog::OnCancel();
}

void CCaptureDlg::OnAbout() 
{
	CAboutDlg dlg;
	dlg.DoModal();
}

void CCaptureDlg::OnBrowse() 
{
	CString str;
	BROWSEINFO bi;
	char name[MAX_PATH];
	ZeroMemory(&bi,sizeof(BROWSEINFO));
	bi.hwndOwner=GetSafeHwnd();
	bi.pszDisplayName=name;
	bi.lpszTitle="Select folder";
	bi.ulFlags=BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST idl=SHBrowseForFolder(&bi);
	if(idl==NULL)
		return;
	SHGetPathFromIDList(idl,str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	m_Path=str;
	if(str.GetAt(str.GetLength()-1)!='\\')
		m_Path+="\\";
	UpdateData(FALSE);
}

void CCaptureDlg::SaveBmp()
{
	CDC dc;
	dc.CreateDC("DISPLAY",NULL,NULL,NULL);
	CBitmap bm;
	int Width=GetSystemMetrics(SM_CXSCREEN);
	int Height=GetSystemMetrics(SM_CYSCREEN);
	bm.CreateCompatibleBitmap(&dc,Width,Height);
	CDC tdc;
	tdc.CreateCompatibleDC(&dc);
	CBitmap*pOld=tdc.SelectObject(&bm);
	tdc.BitBlt(0,0,Width,Height,&dc,0,0,SRCCOPY);
	tdc.SelectObject(pOld);
	BITMAP btm;
	bm.GetBitmap(&btm);
	DWORD size=btm.bmWidthBytes*btm.bmHeight;
	LPSTR lpData=(LPSTR)GlobalAllocPtr(GPTR,size);
/////////////////////////////////////////////
	BITMAPINFOHEADER bih;
	bih.biBitCount=btm.bmBitsPixel;
	bih.biClrImportant=0;
	bih.biClrUsed=0;
	bih.biCompression=0;
	bih.biHeight=btm.bmHeight;
	bih.biPlanes=1;
	bih.biSize=sizeof(BITMAPINFOHEADER);
	bih.biSizeImage=size;
	bih.biWidth=btm.bmWidth;
	bih.biXPelsPerMeter=0;
	bih.biYPelsPerMeter=0;
///////////////////////////////////
	GetDIBits(dc,bm,0,bih.biHeight,lpData,(BITMAPINFO*)&bih,DIB_RGB_COLORS);
//	bm.GetBitmapBits(size,lpData);	//此函数在处理5-5-5模式的16位色下会出现颜色混乱
//////////////////////////////
	static int filecount=0;
	CString name;
	name.Format("pict%04d.bmp",filecount++);
	name=m_Path+name;
	BITMAPFILEHEADER bfh;
	bfh.bfReserved1=bfh.bfReserved2=0;
	bfh.bfType=((WORD)('M'<< 8)|'B');
	bfh.bfSize=54+size;
	bfh.bfOffBits=54;
	CFile bf;
	if(bf.Open(name,CFile::modeCreate|CFile::modeWrite)){
		bf.WriteHuge(&bfh,sizeof(BITMAPFILEHEADER));
		bf.WriteHuge(&bih,sizeof(BITMAPINFOHEADER));
		bf.WriteHuge(lpData,size);
		bf.Close();
		nCount++;
	}
	GlobalFreePtr(lpData);
	if(nCount==1)
		m_Number.Format("%d picture captured.",nCount);
	else
		m_Number.Format("%d pictures captured.",nCount);
	UpdateData(FALSE);
}

BOOL CCaptureDlg::PreTranslateMessage(MSG* pMsg) 
{
    if(pMsg -> message == WM_KEYDOWN)
	{
        if(pMsg -> wParam == VK_ESCAPE)
			return TRUE;
		if(pMsg -> wParam == VK_RETURN)
			return TRUE;
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CCaptureDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message==WM_HOTKEY&&lParam==WM_KEYDOWN){
		SaveBmp();
		return FALSE;
	}
	if(message==IDM_SHELL&&lParam==WM_RBUTTONUP){
		CMenu pop;
		pop.LoadMenu(MAKEINTRESOURCE(IDR_MENU1));
		CMenu*pMenu=pop.GetSubMenu(0);
		pMenu->SetDefaultItem(ID_EXITICON);
		CPoint pt;
		GetCursorPos(&pt);
		int id=pMenu->TrackPopupMenu(TPM_RIGHTALIGN|TPM_NONOTIFY|TPM_RETURNCMD|TPM_LEFTBUTTON,
			pt.x,pt.y,this);
		if(id==ID_EXITICON)
			DeleteIcon();
		else if(id==ID_EXIT)
			OnCancel();
		return FALSE;
	}
	LRESULT res= CDialog::WindowProc(message, wParam, lParam);
	if(message==WM_SYSCOMMAND&&wParam==SC_MINIMIZE)
		AddIcon();
	return res;
}

void CCaptureDlg::AddIcon()
{
	NOTIFYICONDATA data;
	data.cbSize=sizeof(NOTIFYICONDATA);
	CString tip;
	tip.LoadString(IDS_ICONTIP);
	data.hIcon=GetIcon(0);
	data.hWnd=GetSafeHwnd();
	strcpy(data.szTip,tip);
	data.uCallbackMessage=IDM_SHELL;
	data.uFlags=NIF_ICON|NIF_MESSAGE |NIF_TIP ;
	data.uID=98;
	Shell_NotifyIcon(NIM_ADD,&data);
	ShowWindow(SW_HIDE);
	bTray=TRUE;
}

void CCaptureDlg::DeleteIcon()
{
	NOTIFYICONDATA data;
	data.cbSize=sizeof(NOTIFYICONDATA);
	data.hWnd=GetSafeHwnd();
	data.uID=98;
	Shell_NotifyIcon(NIM_DELETE,&data);
	ShowWindow(SW_SHOW);
	SetForegroundWindow();
	ShowWindow(SW_SHOWNORMAL);
	bTray=FALSE;
}

void CCaptureDlg::OnChange() 
{
	RegisterHotkey();
}

BOOL CCaptureDlg::RegisterHotkey()
{
	UpdateData();
	UCHAR mask=0;
	UCHAR key=0;
	if(m_bControl)
		mask|=4;
	if(m_bAlt)
		mask|=2;
	if(m_bShift)
		mask|=1;
	key=Key_Table[m_Key.GetCurSel()];
	if(bRegistered){
		DeleteHotkey(GetSafeHwnd(),cKey,cMask);
		bRegistered=FALSE;
	}
	cMask=mask;
	cKey=key;
	bRegistered=AddHotkey(GetSafeHwnd(),cKey,cMask);
	return bRegistered;
}
