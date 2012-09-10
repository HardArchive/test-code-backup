// regOCXDlg.cpp : implementation file
// 
//���ܣ��ؼ�ע�����Ĵ����࣬�ڴ˼���ע�ᣬע���ؼ��Լ���ʾ����ȹ���
// ���ߣ��쾰�� ��
//���ڣ�2001��7��8��
//
#include "stdafx.h"
#include "regOCX.h"
#include "regOCXDlg.h"

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
	CWBButton	m_OK;  //��Ϊλͼ��ť��
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
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
	DDX_Control(pDX, IDOK, m_OK);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegOCXDlg dialog

CRegOCXDlg::CRegOCXDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRegOCXDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegOCXDlg)
	m_FileName = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRegOCXDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegOCXDlg)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_UNREGOCX, m_UnRegOCX);
	DDX_Control(pDX, IDC_SHOWDESKTOP, m_ShowDesktop);
	DDX_Control(pDX, IDC_BROWSE, m_Browse);
	DDX_Control(pDX, IDC_ABOUT, m_About);
	DDX_Control(pDX, IDC_REGOCX, m_RegOCX);
	DDX_Text(pDX, IDC_FILENAME, m_FileName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRegOCXDlg, CDialog)
	//{{AFX_MSG_MAP(CRegOCXDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_REGOCX, OnRegocx)
	ON_BN_CLICKED(IDC_UNREGOCX, OnUnregocx)
	ON_BN_CLICKED(IDC_SHOWDESKTOP, OnShowdesktop)
	ON_BN_CLICKED(IDC_ABOUT, OnAbout)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegOCXDlg message handlers

BOOL CRegOCXDlg::OnInitDialog()
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
	
	//��ʼ�����洰����Ϊ0
	m_nWndNum = 0;

	//ʹ�ó�����������ǰ��
	SetWindowPos(&wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);

	//��ʼ������λͼ��ť
	m_Browse.LoadBitmaps(IDB_BITMAP2,6, 1, 1, 1, 1 );
	m_Browse.SetFontColor(RGB(240,200,90));
	m_RegOCX.LoadBitmaps(IDB_BITMAP1,5, 5, 5, 5, 4 );
	m_UnRegOCX.LoadBitmaps(IDB_BITMAP1,5, 5, 5, 5, 4 );
	m_ShowDesktop.LoadBitmaps(IDB_BITMAP1,5, 5, 5, 5, 4 );
	m_About.LoadBitmaps(IDB_BITMAP1,5, 5, 5, 5, 4 );
	m_Cancel.LoadBitmaps(IDB_BITMAP1,5, 5, 5, 5, 4 );
	
   /*
	// ����һ����Բ����ָ��������
    CRect rcDialog;
    GetClientRect (rcDialog ); 
    m_rgn. CreateEllipticRgn (0 , 0 , rcDialog.Width( ) , rcDialog.Height ( ) );
    ::SetWindowRgn(GetSafeHwnd() , (HRGN)m_rgn ,TRUE);
   */
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRegOCXDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRegOCXDlg::OnPaint() 
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
     /* 
	   //���ڳ�ʼ���Ի���ʱ������Բ����ʾΪ��������
       CPaintDC dc(this); // device context for painting

       //draw ellipse with out any border
       dc.SelectStockObject (NULL_PEN);
       //get the RGB colour components of the sphere color
       COLORREF color= RGB( 0 , 0 , 255);
       BYTE byRed =GetRValue (color);
       BYTE byGreen = GetGValue (color);
       BYTE byBlue = GetBValue (color);
       // get the size of the view window
       CRect rect;
       GetClientRect (rect);
       // get minimun number of units
       int nUnits =min (rect.right , rect.bottom );
       //calculate he horiaontal and vertical step size
       float fltStepHorz = (float) rect.right /nUnits;
       float fltStepVert = (float) rect.bottom /nUnits;
       int nEllipse = nUnits/3; // calculate how many to draw
       int nIndex;
       // current ellipse that is being draw
       CBrush brush;
       // bursh used for ellipse fill color
       CBrush *pBrushOld; // previous brush that was selected into dc
       //draw ellipse , gradually moving towards upper-right corner
       for (nIndex = 0; nIndex < nEllipse ;nIndex++)
	   {
        //creat solid brush
        brush. CreateSolidBrush (RGB ( ( (nIndex*byRed ) /nEllipse ),( ( nIndex * byGreen ) /nEllipse ), ( (nIndex * byBlue)/nEllipse ) ) );
        //select brush into dc
        pBrushOld= dc .SelectObject (&brush);
        //draw ellipse
        dc.Ellipse ( (int) fltStepHorz *nIndex, (int) fltStepVert * nIndex ,rect. right -( (int) fltStepHorz * nIndex )+ 1,rect . bottom -( (int) fltStepVert * (nIndex *2) ) +1);
        //delete the brush
        brush.DeleteObject ( );
	   }
      */
     
     /*
       //���Ի�����ӱ���λͼ
       CPaintDC dc(this);
	   CRect rect;

	   GetClientRect(&rect);
	   CDC dcMem;
	   dcMem.CreateCompatibleDC (&dc);
	   CBitmap bmpBackground;
       bmpBackground.LoadBitmap (IDB_BKBITMAP);
	   BITMAP bitMap;
	   bmpBackground.GetBitmap (&bitMap);
	   CBitmap *pbmpOld=dcMem.SelectObject (&bmpBackground);
	   dc.StretchBlt (0,0,rect.Width (),rect.Height (),&dcMem,0,0,bitMap.bmWidth ,bitMap.bmHeight ,SRCCOPY);
      */
      
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRegOCXDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//��С����ԭ���������д򿪵Ĵ��壬����ʾ������
void CRegOCXDlg::OnShowdesktop() 
{
// Ѱ�������������е����д�������������ʾ�Ĵ洢������pWnd[]��
CWnd *pWnd[MAXNUM_RUNWINDOW], *pWndCur;
int nWndNum = 0;

  pWnd[nWndNum]= GetWindow(GW_HWNDFIRST);
  while (pWnd[nWndNum] != NULL)
  {
   pWndCur = pWnd[nWndNum];
   CString str;
   pWndCur->GetWindowText(str);
   if (pWndCur->IsWindowVisible() && ! pWndCur->IsIconic() &&! pWndCur->GetParent() &&pWndCur != this &&pWndCur != GetDesktopWindow() &&! str.IsEmpty() &&str != "Program Manager")
   {
    // �б����д�������һ
    nWndNum++;
    // Ϊ�˰�ȫ�������������Ӧ�����ܴ�����!!
    nWndNum = min(nWndNum, MAXNUM_RUNWINDOW-1);
   }
   pWnd[nWndNum] = pWndCur->GetWindow(GW_HWNDNEXT);
  }

  if (nWndNum)
  {
  // ���������е��������洰����С��
  for (int i=0; i<nWndNum;++i)
  {
   pWnd[i]->ShowWindow(SW_SHOWMINNOACTIVE);
   m_pWnd[i] = pWnd[i];
  }
  m_nWndNum = nWndNum;
  }
  else
  {
  // �������в�����С�������洰�廹ԭ��ʾ
  for (int i=m_nWndNum-1; i>=0; --i)
  {
   if (::IsWindow(m_pWnd[i]->m_hWnd) &&
   m_pWnd[i]->IsIconic())
   {
    m_pWnd[i]->ShowWindow(SW_RESTORE);
    m_pWnd[i]->UpdateWindow();
   }
  }
  }
  //���ؼ�ע�����Ĵ����ᵽ��ǰ
  SetForegroundWindow();
}

//��ʾ���ڶԻ���
void CRegOCXDlg::OnAbout() 
{
	CAboutDlg dlgAbout;
		dlgAbout.DoModal();
}

//����ѡ�ؼ�����ϵͳע��
void CRegOCXDlg::OnRegocx() 
{
  //ActiveX�ؼ���·�����ļ���
  LPCTSTR pszDllName;

  //��ѡ�ؼ����Ƿ�Ϊ��
  if(m_FileName!="")
    pszDllName=m_FileName;
  else
  {
	MessageBox("����ѡ������Ҫע��Ŀؼ����ƣ�","��ʾ",MB_OK);
	return;
  }

  //װ��ActiveX�ؼ�
  HINSTANCE hLib = LoadLibrary(pszDllName);
  if (hLib < (HINSTANCE)HINSTANCE_ERROR)
  {
       MessageBox("��������ÿؼ��ļ�!","����",MB_OK);
       return;
  }
  //��ȡע�ắ��DllRegisterServer��ַ
  FARPROC lpDllEntryPoint; 
  lpDllEntryPoint = GetProcAddress(hLib,_T("DllRegisterServer"));
  //����ע�ắ��DllRegisterServer
  if(lpDllEntryPoint!=NULL)
  {
       if(FAILED((*lpDllEntryPoint)()))
       {
            MessageBox("�ؼ�ע��ʧ�ܣ�","����",MB_OK);
            FreeLibrary(hLib);
            return;
       }
       MessageBox("�ؼ�ע��ɹ�","��ʾ",MB_OK);
  }
  else
  MessageBox("�ؼ�ע��ʧ�ܣ�","����",MB_OK);
	
}

//����ע��ؼ�����ϵͳע��
void CRegOCXDlg::OnUnregocx() 
{
 //ActiveX�ؼ���·�����ļ���
  LPCTSTR pszDllName;

  //��ѡ�ؼ����Ƿ�Ϊ��
  if(m_FileName!="")
    pszDllName=m_FileName;
  else
  {
	MessageBox("����ѡ������Ҫע���Ŀؼ����ƣ�","��ʾ",MB_OK);
	return;
  }

  //װ��ActiveX�ؼ�
  HINSTANCE hLib = LoadLibrary(pszDllName);
  if (hLib < (HINSTANCE)HINSTANCE_ERROR)
  {
       MessageBox("��������ÿؼ��ļ�!","����",MB_OK);
       return;
  }
  //��ȡע������DllUnregisterServerr��ַ
  FARPROC lpDllEntryPoint; 
  lpDllEntryPoint = GetProcAddress(hLib,_T("DllUnregisterServer"));
  //����ע������DllUnRegisterServer
  if(lpDllEntryPoint!=NULL)
  {
       if(FAILED((*lpDllEntryPoint)()))
       {
            MessageBox("�ؼ�ע��ʧ�ܣ�","����",MB_OK);
            FreeLibrary(hLib);
            return;
       }
       MessageBox("�ؼ�ע���ɹ�","��ʾ",MB_OK);
  }
  else
  MessageBox("�ؼ�ע��ʧ�ܣ�","����",MB_OK);	
}

//������ҳ�Ҫע���ע���Ŀؼ��ļ�
void CRegOCXDlg::OnBrowse() 
{

   CFileDialog fileDialog( TRUE,"*.OCX",NULL,NULL,"�ؼ��ļ�(*.ocx)|*.ocx|�����ļ�(*.*)|*.*||");
	if (fileDialog.DoModal() == IDOK) {
		m_FileName=fileDialog.GetPathName();	
	
	    UpdateData(FALSE); //�ѱ���ֵ���µ��ؼ���
	}	
}

void CRegOCXDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	CDialog::OnLButtonDown(nFlags, point);

	//ʵ�����ζԻ�����ޱ����϶�
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
}


BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//����λͼ��ť
	m_OK.LoadBitmaps(IDB_BITMAP1,5, 5, 5, 5, 4 );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
