// IPCWorkshopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IPCWorkshop.h"
#include "constants.h"
#include "IPCWorkshopDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT CIPCWorkshopDlg::m_nTransferMessage = RegisterWindowMessage(_T("{1DF71F66-5648-49dd-96A1-79F73A747956}") );
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
// CIPCWorkshopDlg dialog

CIPCWorkshopDlg::CIPCWorkshopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPCWorkshopDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIPCWorkshopDlg)
	m_strName = _T("");
	m_strPhone = _T("");
	m_strDOB = _T("");
	m_nEmpId = 0;
	m_strAddress = _T("");
	m_strServerName = ".";			// supports IPC on local machine
	m_strIPAddress = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);		
}

void CIPCWorkshopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIPCWorkshopDlg)
	DDX_Control(pDX, IDC_GB_PROCESS, m_ctrlProcess);
	DDX_Control(pDX, IDC_COMBO_IPCMECHANISMS, m_ctrlIPCMechanism);
	DDX_Text(pDX, IDC_NAME, m_strName);
	DDX_Text(pDX, IDC_PHONE, m_strPhone);
	DDX_Text(pDX, IDC_DOB, m_strDOB);
	DDX_Text(pDX, IDC_EMPID, m_nEmpId);
	DDX_Text(pDX, IDC_ADDRESS, m_strAddress);
	DDX_Text(pDX, IDC_SERVER, m_strServerName);
	DDX_Text(pDX, IDC_IPADDRESS, m_strIPAddress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIPCWorkshopDlg, CDialog)
	//{{AFX_MSG_MAP(CIPCWorkshopDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_SERVER, OnRadioServer)
	ON_BN_CLICKED(IDC_RADIO_CLIENT, OnRadioClient)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_RADIO_LOCAL, OnRadioLocal)
	ON_BN_CLICKED(IDC_RADIO_INTRANET, OnRadioIntranet)
	ON_BN_CLICKED(IDC_RADIO_INTERNET, OnRadioInternet)
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_CBN_SELCHANGE(IDC_COMBO_IPCMECHANISMS, OnSelchangeComboIpcmechanisms)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( m_nTransferMessage, OnMessageTransfer )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIPCWorkshopDlg message handlers

BOOL CIPCWorkshopDlg::OnInitDialog()
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
	UpdateUI(false);
	m_ctrlIPCMechanism.SetCurSel(0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIPCWorkshopDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CIPCWorkshopDlg::OnPaint() 
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
HCURSOR CIPCWorkshopDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CIPCWorkshopDlg::UpdateUI(bool pEnable)
{
	GetDlgItem(IDC_RADIO_LOCAL)->EnableWindow(pEnable);
	GetDlgItem(IDC_RADIO_INTRANET)->EnableWindow(pEnable);
	GetDlgItem(IDC_RADIO_INTERNET)->EnableWindow(pEnable);
	GetDlgItem(IDC_COMBO_IPCMECHANISMS)->EnableWindow(pEnable);
	GetDlgItem(IDC_EMPID)->EnableWindow(pEnable);
	GetDlgItem(IDC_NAME)->EnableWindow(pEnable);
	GetDlgItem(IDC_ADDRESS)->EnableWindow(pEnable);
	GetDlgItem(IDC_PHONE)->EnableWindow(pEnable);
	GetDlgItem(IDC_DOB)->EnableWindow(pEnable);	
	GetDlgItem(IDC_SERVER)->EnableWindow(pEnable);
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(pEnable);
	GetDlgItem(IDC_CONNECT)->EnableWindow(pEnable);
	GetDlgItem(IDC_SEND)->EnableWindow(false);	
}


void CIPCWorkshopDlg::Refresh()
{
	m_strName = "";
	m_strDOB = "";
	m_strPhone = "";
	m_nEmpId = 0;
	m_strAddress = "";
	UpdateData(FALSE);
}


////////////////////////////////////////////////////////////////////////////////
// Server selected															  //	
////////////////////////////////////////////////////////////////////////////////
void CIPCWorkshopDlg::OnRadioServer() 
{
	GetDlgItem(IDC_RADIO_CLIENT)->EnableWindow(false);
	m_nIsServer = true;
	Refresh();
	UpdateUI(false);
	// Here the purpose of Server, is to receive data from its clients.
	// Hence all the communication medias are supported.
	// Initialize all the possible communication mediums.
	InitializeServer();	
}


////////////////////////////////////////////////////////////////////////////////
// Client selected															  //
////////////////////////////////////////////////////////////////////////////////
void CIPCWorkshopDlg::OnRadioClient() 
{	
	m_nIsServer = false;
	// client decides which communication to be used. 
	GetDlgItem(IDC_RADIO_SERVER)->EnableWindow(false);
	GetDlgItem(IDC_RADIO_LOCAL)->EnableWindow(true);
	GetDlgItem(IDC_RADIO_INTRANET)->EnableWindow(true);
	GetDlgItem(IDC_RADIO_INTERNET)->EnableWindow(true);
}


////////////////////////////////////////////////////////////////////////////////
// Local Machine selected													  //	
////////////////////////////////////////////////////////////////////////////////
void CIPCWorkshopDlg::OnRadioLocal() 
{
	m_ctrlIPCMechanism.ResetContent();
	m_ctrlIPCMechanism.SetCurSel(0);
	UpdateUI(true);
	// Local support is given for all inter process communication methods.
	m_ctrlIPCMechanism.AddString(CLIPBOARD);
	m_ctrlIPCMechanism.AddString(FILEMAPPING);
	m_ctrlIPCMechanism.AddString(MAILSLOTS);
	m_ctrlIPCMechanism.AddString(NAMEDPIPES);
	m_ctrlIPCMechanism.AddString(SOCKETS);
	GetDlgItem(IDC_SERVER)->EnableWindow(false);
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(false);
}


////////////////////////////////////////////////////////////////////////////////
// Intranet selected														  //
////////////////////////////////////////////////////////////////////////////////
void CIPCWorkshopDlg::OnRadioIntranet() 
{
	m_ctrlIPCMechanism.ResetContent();
	m_ctrlIPCMechanism.SetCurSel(0);
	UpdateUI(true);
	// Intranet Support given for MailSlots, NamedPipes and Sockets
	m_ctrlIPCMechanism.AddString(MAILSLOTS);
	m_ctrlIPCMechanism.AddString(NAMEDPIPES);
	m_ctrlIPCMechanism.AddString(SOCKETS);
}


////////////////////////////////////////////////////////////////////////////////
// Internet selected														  //
////////////////////////////////////////////////////////////////////////////////
void CIPCWorkshopDlg::OnRadioInternet() 
{
	m_ctrlIPCMechanism.ResetContent();
	m_ctrlIPCMechanism.SetCurSel(0);
	UpdateUI(true);
	// Internet support 
	m_ctrlIPCMechanism.AddString(SOCKETS);
}


////////////////////////////////////////////////////////////////////////////////
// IPC Mechanism selected													  //
////////////////////////////////////////////////////////////////////////////////
void CIPCWorkshopDlg::OnSelchangeComboIpcmechanisms() 
{
	CString selIPC = GetSelectedIPC();

	if(  selIPC == CLIPBOARD ) {								// Clipboard
		GetDlgItem(IDC_SERVER)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(false);
		GetDlgItem(IDC_CONNECT)->EnableWindow(false);
		GetDlgItem(IDC_SEND)->EnableWindow(true);
	}
	if( selIPC == FILEMAPPING) {								// File Mapping
		GetDlgItem(IDC_SERVER)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(false);
		GetDlgItem(IDC_CONNECT)->EnableWindow(true);
		GetDlgItem(IDC_SEND)->EnableWindow(false);
	}
	if( selIPC == MAILSLOTS) {									// Mailslots
		GetDlgItem(IDC_SERVER)->EnableWindow(true);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(false);
		GetDlgItem(IDC_CONNECT)->EnableWindow(true);
		GetDlgItem(IDC_SEND)->EnableWindow(false);
	}	
	if( selIPC == NAMEDPIPES) {									// Named Pipes
		GetDlgItem(IDC_SERVER)->EnableWindow(true);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(false);
		GetDlgItem(IDC_CONNECT)->EnableWindow(true);
		GetDlgItem(IDC_SEND)->EnableWindow(false);
	}	
	if( selIPC == SOCKETS) {									// Sockets
		GetDlgItem(IDC_SERVER)->EnableWindow(false);
		GetDlgItem(IDC_IPADDRESS)->EnableWindow(true);
		GetDlgItem(IDC_CONNECT)->EnableWindow(true);
		GetDlgItem(IDC_SEND)->EnableWindow(false);
	}		
}

////////////////////////////////////////////////////////////////////////////////
// Connect to the server through the selected IPC - client function			  //
////////////////////////////////////////////////////////////////////////////////
void CIPCWorkshopDlg::OnConnect() 
{
	UpdateData(TRUE);
	// Get the selected IP mechanism.
	CString selIPC = GetSelectedIPC();
	int dataSize = sizeof( EmployeeData);

	if( selIPC == FILEMAPPING) {	//FileMapping
		if( m_objFileMapping.Initialize( FILEMAPNAME, dataSize ) == false) {
			AfxMessageBox("FileMap Initialisation Failed");
			return;
		}
	}
	if( selIPC == MAILSLOTS) {
		if( m_objMailslot.Initialize(false, MAILSLOTNAME) == false) {
			AfxMessageBox("Mailslot creation failed");
			return;
		}
	}
	if( selIPC == NAMEDPIPES){
		// Take Local Mode, if the user doesn't specify any mode.
		if(m_strServerName.IsEmpty())
			m_strServerName = ".";
		m_objNamedPipe.SetSize(dataSize);	
		if( m_objNamedPipe.Initialize(false, m_strServerName, PIPENAME) == false ) {
			AfxMessageBox("NamedPipe creation failed");
			return;
		}
	}
	if( selIPC == SOCKETS) {
		if( !m_strIPAddress.IsEmpty() )
			m_objSocket.Initialize(false, m_strIPAddress);
		else
			m_objSocket.Initialize(false);		
	}
	AfxMessageBox("Successfully Connected");	
	GetDlgItem(IDC_SEND)->EnableWindow(true);
	GetDlgItem(IDC_CONNECT)->EnableWindow(false);
	GetDlgItem(IDC_SERVER)->EnableWindow(false);
	GetDlgItem(IDC_IPADDRESS)->EnableWindow(false);
}


////////////////////////////////////////////////////////////////////////////////
// This application passes a structure pointer to a server named EmployeeData // 	
// client function															  //
////////////////////////////////////////////////////////////////////////////////
void CIPCWorkshopDlg::OnSend() 
{
	UpdateData(TRUE);
	// Prepare the pointer for communication.
	EmployeeData* employeeData = new EmployeeData();
	employeeData->m_nEmployeeNo = m_nEmpId;
	lstrcpy( employeeData->m_strName, m_strName);
	lstrcpy( employeeData->m_strDateOfBirth, m_strDOB);
	lstrcpy( employeeData->m_strAddress, m_strAddress);
	lstrcpy( employeeData->m_strPhone, m_strPhone);
	int size = sizeof(EmployeeData);

	// Get the IPC name selected by the user.	
	CString selIPC = GetSelectedIPC();

	if( selIPC == CLIPBOARD) {													// Clipboard
		if( m_objClipboard.Write( (char*)employeeData, size ) == false) {
			AfxMessageBox("Write Operation Failed");
			return;
		}
		// Notify the server that the data is sent.
		// Since pointers cannot be sent across messages 
		// for different processes, we have to use IPC. 
		::PostMessage( HWND_BROADCAST, m_nTransferMessage, (WPARAM)	m_hWnd, 0 );
	}
	if( selIPC == FILEMAPPING) {												// File Mapping
		m_objFileMapping.Write( (char*)employeeData );		
		::PostMessage( HWND_BROADCAST, m_nTransferMessage, (WPARAM)	m_hWnd, 1 );
	}
	if( selIPC == MAILSLOTS) {													// Mail slots
		m_objMailslot.SetSize( size);
		if( m_objMailslot.Write( (char*)employeeData ) == false) {
			AfxMessageBox("Write Operation Failed");
			return;
		}
	}	
	if( selIPC == NAMEDPIPES) {													// Named Pipes
		if( m_objNamedPipe.Write( (char*)employeeData ) == false) {
			AfxMessageBox("Write Operation Failed");
			return;
		}
	}	
	if( selIPC == SOCKETS) {													// IP sockets
		if( m_objSocket.Send( (char*)employeeData, size) == false) {
			AfxMessageBox("Write Operation Failed");
			return;
		}
	}	
}


////////////////////////////////////////////////////////////////////////////////
// Initialize all IPC mechanisms in the server initially.					  // 	
// CPU time will be very high.												  //
////////////////////////////////////////////////////////////////////////////////
void CIPCWorkshopDlg::InitializeServer()
{
	// Clipboard
	m_objClipboard.Open();
	// FileMapping
	m_objFileMapping.Initialize( FILEMAPNAME, sizeof( EmployeeData) ); 
	// Mailslots
	m_objMailslot.Initialize(true, MAILSLOTNAME);
	m_objMailslot.Listen( m_nTransferMessage,MAILSLOTCONTROL); 
	m_objMailslot.SetSize( sizeof( EmployeeData));
	// NamedPipes
	m_objNamedPipe.Initialize(true, ".", PIPENAME);
	m_objNamedPipe.SetSize(sizeof( EmployeeData) );
	m_objNamedPipe.Listen(m_nTransferMessage, PIPECONTROL);
	// Sockets.	
	m_objSocket.Initialize(true, m_strIPAddress);	
	m_objSocket.Listen(m_nTransferMessage, SOCKETCONTROL);
	m_objSocket.SetDataSize(sizeof( EmployeeData));	
}


////////////////////////////////////////////////////////////////////////////////
// After client sends the message, it will notify the server to read the data //
// using message															  //
////////////////////////////////////////////////////////////////////////////////
LRESULT CIPCWorkshopDlg::OnMessageTransfer( WPARAM wparam, LPARAM lparam )
{
	//int check = m_ctrlProcess.GetCheck();
	if( m_nIsServer == false )
		return 1;
	AfxMessageBox( "Received");
	int size = sizeof( EmployeeData );
	char* buffer = new char[size];
	
	if( lparam == 0) {								// Clipboard.
		m_objClipboard.Read(buffer, size);
		SetVariables( buffer );	
	}
	else if( lparam == 1 ) {						// FileMapping.
		m_objFileMapping.Read( buffer );		
		SetVariables( buffer );	
	}
	else if( lparam == 2) {							// Mailslots.
		m_objMailslot.SetSize( size);
		m_objMailslot.Read(	buffer);
		SetVariables( buffer );			
	}
	else if( lparam == 3) {							// NamedPipes.
		SetVariables( (char*)wparam );	
	}
	else if( lparam == 4) {							// IPSockets.
		SetVariables( (char*)wparam );
	}
	if( buffer )
		delete[] buffer, buffer = 0; 

	return 1;
}


////////////////////////////////////////////////////////////////////////////////
// updates from the structure to UI											  //
////////////////////////////////////////////////////////////////////////////////
void CIPCWorkshopDlg::SetVariables(char* pBuffer)
{
	EmployeeData* employeeData = new EmployeeData();
	int size = sizeof(EmployeeData);
	memcpy( employeeData, pBuffer, size );
	m_strName = employeeData->m_strName;
	m_strDOB = employeeData->m_strDateOfBirth;
	m_strPhone = employeeData->m_strPhone;
	m_nEmpId = employeeData->m_nEmployeeNo;
	m_strAddress = employeeData->m_strAddress;
	UpdateData(FALSE);
	if( employeeData )
		delete employeeData, employeeData = 0;
}


////////////////////////////////////////////////////////////////////////////////
// Get the selected IPC mechanism											  //
////////////////////////////////////////////////////////////////////////////////
CString CIPCWorkshopDlg::GetSelectedIPC()
{
	CString selIPC;
	int selectedIndex = m_ctrlIPCMechanism.GetCurSel();	
	int selLength = m_ctrlIPCMechanism.GetLBTextLen(selectedIndex);
	m_ctrlIPCMechanism.GetLBText(selectedIndex, selIPC.GetBuffer(selLength));
	selIPC.ReleaseBuffer();
	return selIPC;
}
