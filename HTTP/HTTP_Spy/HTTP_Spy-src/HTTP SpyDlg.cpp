// HTTP SpyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HTTP Spy.h"
#include "HTTP SpyDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

// Initialize static members
BOOL    CHTTPSpyDlg::SysCommandKey = false;
CString CHTTPSpyDlg::WindowTitle = "HTTP Spy 0.2";

const int USERAGENTS = 10;

const char USERAGENT[][100] = {
								"", 
								"Googlebot/2.1(+http://www.google.com/bot.html)",
								"msnbot/1.0 (+http://search.msn.com/msnbot.htm)",
								"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0)",
								"Mozilla/4.0 (compatible; MSIE 7.0b; Windows NT 6.0)",
								"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)",
								"Mozilla/4.0 (compatible; MSIE 5.5; Windows NT 4.0)",
								"Mozilla/4.0 (compatible; MSIE 5.0; Windows 98; DigExt)",
								"Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.7.6) Gecko/20050223 Firefox/1.0.1",
								"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0) Opera 7.54  [es-ES]"
							};


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
// CHTTPSpyDlg dialog

CHTTPSpyDlg::CHTTPSpyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHTTPSpyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHTTPSpyDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon			= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_httpParams	= new HttpParams;
}

void CHTTPSpyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHTTPSpyDlg)
	DDX_Control(pDX, IDC_COMBO_USRAGENT, m_comboAgent);
	DDX_Control(pDX, IDC_REQUEST_METHOD, m_Group_Request);
	DDX_Control(pDX, IDC_TAB_RESPONSE, m_TabCtrl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHTTPSpyDlg, CDialog)
	//{{AFX_MSG_MAP(CHTTPSpyDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_EDIT_PORT, OnChangeEditPort)
	ON_EN_CHANGE(IDC_EDIT_URL, OnChangeEditUrl)
	ON_BN_CLICKED(IDC_METHOD_HEAD, OnMethodHead)
    ON_BN_CLICKED(IDC_METHOD_GET, OnMethodGet)
	ON_BN_CLICKED(IDC_BUTTON_DOIT, OnMethodDoIt)
	ON_CBN_SELCHANGE(IDC_COMBO_USRAGENT, OnSelchangeComboUsragent)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_RESET_SETTING, OnResetSetting)
	ON_BN_CLICKED(IDC_RESET_RESULT, OnResetResult)
	ON_BN_CLICKED(IDC_DIRECT_INTERNET, OnDirectInternet)
	ON_BN_CLICKED(IDC_HTTP_PROXY, OnHttpProxy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTTPSpyDlg message handlers

BOOL CHTTPSpyDlg::OnInitDialog()
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
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	resetType	= SETTING;

	//Added for Tab control
	m_TabCtrl.InsertItem(0,"Response");
	m_TabCtrl.InsertItem(1,"Server Config"); 
	m_TabCtrl.InitDialogs();

	CButton *btn			= (CButton *)GetDlgItem(IDC_METHOD_HEAD);
	CButton *interConBtn	= (CButton *)GetDlgItem(IDC_DIRECT_INTERNET);
	CButton *resetBtn		= (CButton *)GetDlgItem(IDC_RESET_SETTING);

	// Port range 1 to 65535
	SendDlgItemMessage(IDC_EDIT_PROXY_PORT, EM_SETLIMITTEXT, 5,0);
	SendDlgItemMessage(IDC_EDIT_PORT, EM_SETLIMITTEXT, 5,0);

	if(btn==NULL || interConBtn==NULL || resetBtn==NULL) {
		AfxMessageBox("Couldn't get button Control");
		return FALSE;
	}
  
	//btn->SendMessage(BM_SETCHECK,1,0L);
	btn->SendMessage(BM_CLICK,0,0);
	interConBtn->SendMessage(BM_CLICK,0,0);
	resetBtn->SendMessage(BM_CLICK,0,0);

    CRect rect;
	m_comboAgent.GetWindowRect(&rect );
    rect.bottom = rect.top + 3 * m_comboAgent.GetItemHeight( -1 ) + rect.Height();
    m_comboAgent.SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height(), SWP_NOMOVE | SWP_NOZORDER);

	for(int i=0; i < USERAGENTS; i++)	{
			m_comboAgent.AddString(USERAGENT[i]);
	}

    m_comboAgent.SetCurSel(0);
	m_comboAgent.GetLBText(0, strUserAgent);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CHTTPSpyDlg::resetDialog(void)
{
    if(getResetType()==SETTING)
	{
		//	Reset User agent
		m_comboAgent.SetCurSel(0);
		m_comboAgent.GetLBText(0, strUserAgent);

		// Reset Radio button 
		CButton *headbtn		= (CButton *)GetDlgItem(IDC_METHOD_HEAD);
		CButton *cb				= (CButton *)GetDlgItem(IDC_CHECK_PORT);
		CButton *interConBtn	= (CButton *)GetDlgItem(IDC_DIRECT_INTERNET);
		CButton *resetBtn		= (CButton *)GetDlgItem(IDC_RESET_SETTING);
		CEdit	*editURLCtrl	= (CEdit *)GetDlgItem(IDC_EDIT_URL);

		if(headbtn == NULL || cb == NULL || interConBtn == NULL) {
			AfxMessageBox("Couldn't get button Control");
			return FALSE;
		}
  		headbtn->SendMessage(BM_CLICK,0,0);
		interConBtn->SendMessage(BM_CLICK,0,0);
		resetBtn->SendMessage(BM_CLICK,0,0);
		// Reset Port Check
		cb->SendMessage(BM_SETCHECK,0,0L);
		SetDlgItemText(IDC_EDIT_URL, "");
		SetDlgItemText(IDC_EDIT_PORT,"");
		SetDlgItemText(IDC_PROXY_IPADDRESS, "");
		SetDlgItemText(IDC_EDIT_PROXY_PORT, "");
		editURLCtrl->SetFocus();		
	}
	else {
		m_TabCtrl.setDialogText(0,"");
		m_TabCtrl.setDialogText(1,"");
	}
	return TRUE;
}

void CHTTPSpyDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		switch (nID)
		{
			case SC_CLOSE:
				CHTTPSpyDlg::SysCommandKey = true;
			case SC_HOTKEY:
			case SC_HSCROLL:
			case SC_KEYMENU:
			case SC_MAXIMIZE:
			case SC_MINIMIZE:
			case SC_MOUSEMENU:
			case SC_MOVE:
			case SC_NEXTWINDOW:
			case SC_PREVWINDOW:
			case SC_RESTORE:
			case SC_SCREENSAVE:
			case SC_SIZE:
			case SC_TASKLIST:
			case SC_VSCROLL:
			case 61587: // show the system menu
			case 61443: // allow window resizing by click/dragging the main window 
                    // borders
			;
		}
		CDialog::OnSysCommand(nID, lParam);
        return;
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHTTPSpyDlg::OnPaint() 
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

BOOL CHTTPSpyDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	    return CDialog::OnCommand(wParam, lParam);
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHTTPSpyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

long CHTTPSpyDlg::ScanRadioGroup(BOOL bGetIndex,int nIndex)const
{
	long nRadioNo=cFirstIndex-1;//-1;	//1 based indexing
	CRect rBounds;
	m_Group_Request.GetWindowRect(&rBounds);//bounds of the groupbox

	if(m_Group_Request.GetParent()!=NULL)
		{
		BOOL bContinue=TRUE;

		// walk all children in group
		for(CWnd* pWnd=m_Group_Request.GetParent()->GetWindow(GW_CHILD);bContinue && IsValidControl(pWnd);pWnd=pWnd->GetNextWindow())	
			{
			CRect rRadio;
			pWnd->GetWindowRect(&rRadio);

			//Count auto radio buttons, that are encompassed by the group box. 
			//Exclude the group box itself.
			//
			if(rRadio.IntersectRect(rRadio,rBounds) && pWnd->GetSafeHwnd()!=m_Group_Request.GetSafeHwnd())
				{
				
				if(CHTTPSpyDlg::IsAutoRadio(pWnd))
					{
					nRadioNo++;	//Increment index of radio

					if(bGetIndex)		// thus get active
						{
						if(pWnd->SendMessage(BM_GETCHECK,0,0L)!=0)//Have the active button
							bContinue=FALSE;	
						}
					else						// set the radio
						{
						pWnd->SendMessage(BM_SETCHECK,nRadioNo==nIndex,0L);
						break;
						}
					}
				}
			} 
		if(bContinue && bGetIndex)//Didn't find set radio
			{
			nRadioNo=-1;
			TRACE("CRadioGroup::DoRadioGroup: Didn't find set radio in group.");
			}
		}
	return nRadioNo;
}

void CHTTPSpyDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CHTTPSpyDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	  if(CHTTPSpyDlg::SysCommandKey == 1)
	  {
		CHTTPSpyDlg::SysCommandKey = 0;
	 	CDialog::OnOK();
	  }
	  delete m_httpParams;
	  m_httpParams = 0;
}

void CHTTPSpyDlg::OnChangeEditPort() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CButton *cb				= (CButton *)GetDlgItem(IDC_CHECK_PORT);
	CEdit *PortEditBtn = (CEdit *) GetDlgItem(IDC_EDIT_PORT);

	if(cb == NULL) {
		AfxMessageBox("Couldn't get button Control");
		return;
	}
	// If a Port is entered check the port
	if(PortEditBtn->GetWindowTextLength()>0 
		&& cb->GetCheck()==0) {
		cb->SetCheck(1);

	}
	else if(PortEditBtn->GetWindowTextLength() ==0 
		    && cb->GetCheck()==1) {
		cb->SetCheck(0);
	}
}

void CHTTPSpyDlg::OnChangeEditUrl() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString title;
    //GetWindowText(title);
	GetDlgItemText(IDC_EDIT_URL,title);
	title.TrimLeft();
	if(title.GetLength() > 0 )
	  this->SetWindowText(CHTTPSpyDlg::WindowTitle +" : " + title);
	else
	  this->SetWindowText(CHTTPSpyDlg::WindowTitle);
}

void CHTTPSpyDlg::OnMethodHead() 
{
	// TODO: Add your control notification handler code here
	setMethod(HEAD);
}

void CHTTPSpyDlg::OnMethodGet() 
{
	// TODO: Add your control notification handler code here
	setMethod(GET);
}

void CHTTPSpyDlg::OnMethodDoIt()
{
	// Launch a thread for HTTP processing
	AfxBeginThread(doHttpProcessing, this);
}

void CHTTPSpyDlg::OnSelchangeComboUsragent() 
{
	// TODO: Add your control notification handler code here
	int idx = m_comboAgent.GetCurSel();
	if( idx < 0 ) return;

	m_comboAgent.GetLBText( idx, strUserAgent);
}

/**
 * Thread function performing the main processing
 */
UINT  CHTTPSpyDlg::doHttpProcessing(LPVOID pParam)
{
	CHTTPSpyDlg* pDlg			= (CHTTPSpyDlg*)pParam;
	CButton *chkBtn				= 0;
	CString strURL				= "";
	CString	strProxyIP			= "";
	CIPAddressCtrl *addrCtrl	= 0;
	unsigned int portNo			= 0;
	unsigned int proxyPortNo	= 0;
	int httpResStatus			= -1;
	CString strHttpResp			= "";
	CButton *pDoIt				= 0;

	pDlg->GetDlgItemText(IDC_EDIT_URL,strURL);
   	strURL.TrimLeft();
	strURL.TrimRight();
	
	// The URL should be valid to continue further
	if (strURL.GetLength() <= 0) return -1;  
		
	pDoIt = (CButton *)pDlg->GetDlgItem(IDC_BUTTON_DOIT);
	pDoIt->EnableWindow(false);

	pDlg->m_httpParams->setURI(strURL);
	pDlg->m_httpParams->setUserAgent(pDlg->strUserAgent);

	chkBtn = (CButton *) pDlg->GetDlgItem(IDC_CHECK_PORT);
	
	if(chkBtn->GetCheck()==1)
		portNo	= pDlg->GetDlgItemInt(IDC_EDIT_PORT);
	else
		portNo	= 80;

	pDlg->m_httpParams->setHttpPort(portNo);

	/**
	 * Check that proxy information is given properly
	 */
	if(pDlg->m_httpParams->getConnType() == THRU_PROXY)
	{
		addrCtrl = (CIPAddressCtrl *) pDlg->GetDlgItem(IDC_PROXY_IPADDRESS);
		if(addrCtrl->SendMessage(IPM_ISBLANK,0,0)	>	0)
		{
			AfxMessageBox("Please enter a valid HTTP proxy IP Address");
			addrCtrl->SetFocus();
			pDoIt->EnableWindow(true);
			return -1;
		}
		proxyPortNo = pDlg->GetDlgItemInt(IDC_EDIT_PROXY_PORT);
		if(proxyPortNo	<=	0)
		{
			AfxMessageBox("Please enter a valid HTTP proxy port");
			pDoIt->EnableWindow(true);
			return -1;
		}
		pDlg->m_httpParams->setProxyPort(proxyPortNo);

		pDlg->GetDlgItemText(IDC_PROXY_IPADDRESS, strProxyIP);
		/** If some octets are not filled then the IP Address 
		 * Ctrl will implicitly insert a 0. This needs to be 
		 * updated for the viewer display.
		 */
		pDlg->m_httpParams->setProxyIP(strProxyIP);
		pDlg->SetDlgItemText(IDC_PROXY_IPADDRESS, strProxyIP);
	}

	/**
	 * Connect at the specified port no of the host.
	 * The Host should allow connection at its port.
	 * After establishing the connection, send an HTTP request
	 */

	HttpSocket httpSock(pDlg->m_httpParams->getHttpProxyObj());

	if(portNo != 0)
		httpResStatus = httpSock.sendHttpRequest(pDlg->m_httpParams);
	else
	{
		//If the port no entered is out of integer range
		AfxMessageBox("Please enter a valid HTTP port");
		CEdit *portEdit = (CEdit *)pDlg->GetDlgItem(IDC_EDIT_PORT);
		portEdit->GetFocus();
		pDoIt->EnableWindow(true);
		return -1;
	}

	if(httpResStatus != 0) 
	{
		AfxMessageBox(httpSock.getErrorDesc());
		pDoIt->EnableWindow(true);
		return -1;
	}

	// Update the used HTTP port no for display
	pDlg->SetDlgItemInt(IDC_EDIT_PORT, httpSock.getPortNo(), true);

	// Read the Http response
	strHttpResp = httpSock.getHttpResponse();

    int start = 0;
    CString strRespFind  = "\r\nServer:";
    CString serverConfig = "";

	if(httpSock.getHeaderInfo("Server", serverConfig) != 0)
		serverConfig = "No Server Configuration information avaliable";
	
	pDlg->m_TabCtrl.setDialogText(0,strHttpResp);
	pDlg->m_TabCtrl.setDialogText(1,serverConfig);
	pDoIt->EnableWindow(true);
	pDlg	= 0;
	pDoIt	= 0;
	return 0;
}

void CHTTPSpyDlg::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	resetDialog();	
}

void CHTTPSpyDlg::OnResetSetting() 
{
	// TODO: Add your control notification handler code here
	resetType = SETTING;
	
}

void CHTTPSpyDlg::OnResetResult() 
{
	// TODO: Add your control notification handler code here
	resetType = RESULT;	
}

void CHTTPSpyDlg::OnDirectInternet() 
{
	// TODO: Add your control notification handler code here
	setConnType(DIRECT);
	CIPAddressCtrl *cipCtrlObj = (CIPAddressCtrl *)GetDlgItem(IDC_PROXY_IPADDRESS);
	cipCtrlObj->EnableWindow(false);
	CEdit *cProxyPortBtn = (CEdit *) GetDlgItem(IDC_EDIT_PROXY_PORT);
	cProxyPortBtn->EnableWindow(false);
}

void CHTTPSpyDlg::OnHttpProxy() 
{
	// TODO: Add your control notification handler code here
	setConnType(THRU_PROXY);	
	CIPAddressCtrl *cipCtrlObj = (CIPAddressCtrl *)GetDlgItem(IDC_PROXY_IPADDRESS);
	cipCtrlObj->EnableWindow(true);
	CEdit *cProxyPortBtn = (CEdit *) GetDlgItem(IDC_EDIT_PROXY_PORT);
	cProxyPortBtn->EnableWindow(true);
}
