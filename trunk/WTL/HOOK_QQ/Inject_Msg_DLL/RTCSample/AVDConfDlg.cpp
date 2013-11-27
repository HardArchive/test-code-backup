// ------------------------------------------------------------------------------------
// Copyright ?002 Intel Corporation
// All Rights Reserved
// 
// Permission is granted to use, copy, distribute and prepare derivative works of this 
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  This software is provided "AS IS." 
//
// Intel specifically disclaims all warranties, express or implied, and all liability,
// including consequential and other indirect damages, for the use of this software, 
// including liability for infringement of any proprietary rights, and including the 
// warranties of merchantability and fitness for a particular purpose.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.
// ------------------------------------------------------------------------------------
//
// AVDConfDlg.cpp : implementation file
//
// Purpose: Main Dialog dialog handler module to handle all dialog messages.
//

#include "stdafx.h"
#include "rtccore.h"

#include "AVDConf.h"
#include "AVDConfDlg.h"
#include "MakeCallDlg.h"
#include "incomingdlg.h"

#include "defines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Global variable used to store destination SIP
BSTR	g_bstrAddr;

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
// CAVDConfDlg dialog

CAVDConfDlg::CAVDConfDlg(CWnd* pParent /*=NULL*/)
: CDialog(CAVDConfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAVDConfDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAVDConfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAVDConfDlg)
	DDX_Control(pDX, IDC_WHITEBOARD, m_cWhiteboard);
	DDX_Control(pDX, IDC_MESSAGING, m_cMessaging);
	DDX_Control(pDX, IDC_TUNING, m_cTuning);
	DDX_Control(pDX, IDC_MAKECALL, m_cMakeCall);
	DDX_Control(pDX, IDC_APPCALL, m_cAppCall);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAVDConfDlg, CDialog)
	//{{AFX_MSG_MAP(CAVDConfDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MAKECALL, OnMakecall)
	ON_BN_CLICKED(IDC_APPCALL, OnAppcall)
	ON_BN_CLICKED(IDC_TUNING, OnTuning)
	ON_BN_CLICKED(IDC_MESSAGING, OnMessaging)
	ON_BN_CLICKED(IDM_ABOUTBOX, OnAboutbox)
	ON_BN_CLICKED(IDC_WHITEBOARD, OnWhiteboard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAVDConfDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// Dialog initialization method created by the App Wizard.
// Windows Message: WM_INITDIALOG
BOOL CAVDConfDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize the RTC Client
	if (InitRTCClient() != S_OK)
	{
		// Disable all feature buttons if the initialization fails.
		MessageBox ( "Initialization Error" );
		m_cMakeCall.EnableWindow ( FALSE );
		m_cAppCall.EnableWindow ( FALSE );
		m_cMessaging.EnableWindow ( FALSE );
		m_cTuning.EnableWindow ( FALSE );
		m_cWhiteboard.EnableWindow ( FALSE );
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CAVDConfDlg::OnPaint() 
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
HCURSOR CAVDConfDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnMakeCall()
// Parameter: None
// Return Value: None
// Purpose: This method is called when the user clicks the AV Call button.
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnMakecall() 
{
	MakeCallDlg cDlg;

	// Display the SIP dialog box.
	if (cDlg.DoModal() == IDOK)
	{
		// Make a PC to PC call.
		MakeCall (RTCST_PC_TO_PC, g_bstrAddr);
	}
}


//////////////////////////////////////////////////////////////////////////////
// Method: OnMessaging()
// Parameter: None
// Return Value: None
// Purpose: This method is called when the user clicks the Messenging button
//			to start a Text Chat session
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnMessaging() 
{
	MakeCallDlg cDlg;

	if (cDlg.DoModal() == IDOK)
	{													 
		// Make a Instant Messaging Call.
		MakeCall (RTCST_IM, g_bstrAddr);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnAppcall()
// Parameter: None
// Return Value: None
// Purpose: This method is called when the user clicks the App Sharing button
//			to start a Application sharing session.
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnAppcall() 
{
	HRESULT	hr;
	VARIANT_BOOL	vbool = VARIANT_FALSE;

	// Check the state of T.120 applet
	m_pClient->get_IsT120AppletRunning(RTCTA_APPSHARING, &vbool);

	if (vbool == VARIANT_FALSE)
	{ // Start T120 Applet to start application sharing
		hr = m_pClient->StartT120Applet ( RTCTA_APPSHARING );
	}
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnWhiteboard()
// Parameter: None
// Return Value: None
// Purpose: This method is called when the user clicks the Whiteboard button
//			to start a Whiteboard sharing session
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnWhiteboard() 
{
	HRESULT	hr;
	VARIANT_BOOL	vbool = VARIANT_FALSE;

	// Check the state of T.120 applet
	m_pClient->get_IsT120AppletRunning(RTCTA_WHITEBOARD, &vbool);

	if (vbool == VARIANT_FALSE)
	{ // Start T120 Applet to start Whiteboard sharing
		hr = m_pClient->StartT120Applet ( RTCTA_WHITEBOARD );
	}
}

//////////////////////////////////////////////////////////////////////////////
// Method: MakeCall()
// Parameter: enType	Type of call.  See RTCCORE.H for types of Session
//			  bstrURI	Target Address (IP, Phone number, e-mail)
// Return Value: S_OK if successful
// Purpose: This method is will initiate either an AV Conferencing call or an
//			Instant Messaging call.
//////////////////////////////////////////////////////////////////////////////
HRESULT CAVDConfDlg::MakeCall(RTC_SESSION_TYPE enType, BSTR bstrURI)
{
	HRESULT hr;

	// Only Instant Messaging can have multiple sessions for other user participation.
	if (enType != RTCST_IM)
	{
		// Is there already an AV session? We can only
		// allow one at a time
		if ((m_AVDlg) && (m_AVDlg.GetState () != RTCSS_IDLE))
		{
			MessageBoxW (m_hWnd, L"An audio/video call is in progress!", NULL, MB_OK );
			return S_FALSE;
		}
	}

	// Create the Client session
	IRTCSession * pSession = NULL;

	hr = m_pClient->CreateSession(enType, NULL, NULL, 0, &pSession);

	if (FAILED(hr))
	{
		// CreateSession failed
		return hr;
	}

	// Add the participant to the session
	// calling AddParticipation with the target SIP will initiate the RTC
	// layer to call the participant.
	hr = pSession->AddParticipant(bstrURI, NULL, &m_Participant);

	if (FAILED(hr))
	{
		// AddParticipant failed					
		SAFE_RELEASE(pSession);
		return hr;
	}

	// Add the session to the session list
	// This will create the session window
	hr = AddSession(pSession, enType);

	if (FAILED(hr))
	{  
		// AddSession failed        
		return hr;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method: InitRTCClient()
// Parameter: None
// Return Value: S_OK if successful.
// Purpose: This method initiates the RTC layer.
//////////////////////////////////////////////////////////////////////////////
HRESULT CAVDConfDlg::InitRTCClient()
{
	// Create the RTC Client
	HRESULT hr;

	m_pClient = NULL;
	m_pEvents = NULL;

	// Initialize the COM library in the current thread.
	hr = CoInitialize(NULL);

	if (FAILED(hr))
	{
		return hr;
	}

	// Initialize the RTC COM object
	hr = CoCreateInstance (CLSID_RTCClient, NULL,
		CLSCTX_INPROC_SERVER, IID_IRTCClient,
		(LPVOID *)&m_pClient);

	if (FAILED(hr))
	{
		// Failed to load the RTC Client Interface.
		return hr;
	}

	// Initialize the client interface
	hr = m_pClient->Initialize();

	if (FAILED(hr))
	{
		// Unable to initialize the RTC Interface
		SAFE_RELEASE(m_pClient);
		return hr;
	}

	// Set the preferred media type we want to initiate for this application.
	// RTCMT_ALL will initialize Audio, Video, and T.120 layer.
	m_pClient->SetPreferredMediaTypes ( RTCMT_ALL, VARIANT_TRUE );

	// Set the event filter to listen for.
	// Using RTCEF_ALL will set for all events
	// For the sample application, we will demonstrate how to set the event listener for
	// a limited set of events.
	long lEventMask = RTCEF_SESSION_STATE_CHANGE |
		RTCEF_MESSAGING |
		RTCEF_MEDIA |
		RTCEF_INTENSITY |
		RTCEF_CLIENT;

	// Initialize the event filter this demo application want callback.
	hr = m_pClient->put_EventFilter( lEventMask );

	if ( FAILED(hr) )
	{
		// put_EventFilter failed
		SAFE_RELEASE(m_pClient);
		return hr;
	}

	// Create the event sink object
	m_pEvents = new CRTCEvents;

	if (!m_pEvents)
	{
		// Out of memory
		SAFE_RELEASE(m_pClient);
		return hr;
	}

	// initialize the event handler
	hr = m_pEvents->Advise( m_pClient, m_hWnd );

	if ( FAILED(hr) )
	{
		// Advise failed
		SAFE_RELEASE(m_pClient);
		return hr;
	}

	// Set the listen mode for RTC client
	// RTCLM_BOTH opens the standard SIP port 5060, as well as
	// a dynamic port.
	hr = m_pClient->put_ListenForIncomingSessions(RTCLM_BOTH); 

	//m_pClient->put_MaxBitrate(1000000L);
	//m_pClient->put_TemporalSpatialTradeOff(0);

	if ( FAILED(hr) )
	{
		// put_ListenMode failed
		SAFE_RELEASE(m_pClient);
		return hr;
	}

	// Initalize the program state machine to IDLE
	if (m_AVDlg)
		m_AVDlg.SetState (RTCSS_IDLE);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method: WindowProc()
// Purpose: Handles Windows procedural messages.
//////////////////////////////////////////////////////////////////////////////
LRESULT CAVDConfDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_RTC_EVENT:
		OnRTCEvent (message, wParam, lParam);
		break;
	}
	return CDialog::WindowProc(message, wParam, lParam);
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnRTCEvent()
// Parameter: message			Type of RTC Event to be process
//			  wParam, lParam	Extra messsage parameters
// Return Value: S_OK if successful.
// Purpose: This method process the RTC Events from the RTC layer.
//////////////////////////////////////////////////////////////////////////////
HRESULT CAVDConfDlg::OnRTCEvent(UINT message, WPARAM wParam, LPARAM lParam)
{
	IDispatch * pDisp = (IDispatch *)lParam;
	RTC_EVENT enEvent = (RTC_EVENT)wParam;
	HRESULT hr;

	// Based on the RTC_EVENT type, query for the 
	// appropriate event interface and call a helper
	// method to handle the event
	switch ( wParam )
	{
	case RTCE_SESSION_STATE_CHANGE:
		{
			IRTCSessionStateChangeEvent * pEvent = NULL;

			// Get the event handle associated with the current session.
			hr = pDisp->QueryInterface( IID_IRTCSessionStateChangeEvent,
				(void **)&pEvent );

			if (SUCCEEDED(hr))
			{
				OnRTCSessionStateChangeEvent(pEvent);
				SAFE_RELEASE(pEvent);
			}              
		}
		break;

	case RTCE_MESSAGING:
		{
			IRTCMessagingEvent * pEvent = NULL;

			// Get the event handle associated with the current session.
			hr = pDisp->QueryInterface( IID_IRTCMessagingEvent,
				(void **)&pEvent );

			if (SUCCEEDED(hr))
			{
				OnRTCMessagingEvent(pEvent);
				SAFE_RELEASE(pEvent);
			}              
		}
		break;

	case RTCE_MEDIA:
		{
			IRTCMediaEvent * pEvent = NULL;

			// Get the event handle associated with the current session.
			hr = pDisp->QueryInterface( IID_IRTCMediaEvent,
				(void **)&pEvent );

			if (SUCCEEDED(hr))
			{
				OnRTCMediaEvent(pEvent);
				SAFE_RELEASE(pEvent);
			}
		}
		break;

	case RTCE_INTENSITY:
		{
			IRTCIntensityEvent * pEvent = NULL;

			// Get the event handle associated with the current session.
			hr = pDisp->QueryInterface( IID_IRTCIntensityEvent,
				(void **)&pEvent );

			if (SUCCEEDED(hr))
			{
				OnRTCIntensityEvent(pEvent);
				SAFE_RELEASE(pEvent);
			}
		}
		break;

	case RTCE_CLIENT:
		{
			IRTCClientEvent * pEvent = NULL;

			// Get the event handle associated with the current session.
			hr = pDisp->QueryInterface( IID_IRTCClientEvent,
				(void **)&pEvent );

			if (SUCCEEDED(hr))
			{
				OnRTCClientEvent(pEvent);
				SAFE_RELEASE(pEvent);
			}
		}
		break;
	}

	// Release the event
	SAFE_RELEASE(pDisp);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnMessaging()
// Parameter: None
// Return Value: None
// Purpose: This method is called when the user clicks the Messenging button
//			to start a Text Chat session
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnTuning() 
{
	if (m_pClient)
		m_pClient->InvokeTuningWizard((OAHWND)m_hWnd);
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnRTCSessionStateChangeEvent()
// Parameter: pEvent		Pointer to the event information
// Return Value: None
// Purpose: This method handles all events that are associated to State changes
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnRTCSessionStateChangeEvent(IRTCSessionStateChangeEvent *pEvent)
{
	IRTCSession * pSession = NULL;
	RTC_SESSION_STATE enState;
	HRESULT hr;

	// Get the session state
	hr = pEvent->get_State(&enState);

	if (FAILED(hr))
	{
		// get_State failed
		return;
	}

	// Get a pointer to the current session.
	hr = pEvent->get_Session(&pSession);

	if (FAILED(hr))
	{
		// get_Session failed
		return;
	}

	switch ( enState )
	{
	case RTCSS_INCOMING:
		{
			// Handles RTC Event for an Incoming call request.

			// This is a new session
			RTC_SESSION_TYPE enType;

			hr = pSession->get_Type(&enType);

			if (FAILED(hr))
			{
				// get_Type failed
				SAFE_RELEASE(pSession);
				return;
			}            

			if (enType == RTCST_IM)
			{
				// This is an IM call
				m_pClient->PlayRing(RTCRT_MESSAGE, VARIANT_TRUE);
			}
			else            
			{
				// This is an AV call
				if ((m_AVDlg) && (m_AVDlg.GetState() != RTCSS_IDLE))
				{
					// If another AV call is in progress, then
					// we are already busy.
					pSession->Terminate(RTCTR_BUSY);

					SAFE_RELEASE(pSession);
					return;
				}

				// Get the participant object
				IRTCEnumParticipants * pEnum = NULL;
				IRTCParticipant * pParticipant = NULL;

				hr = pSession->EnumerateParticipants(&pEnum);

				if (FAILED(hr))
				{
					// EnumerateParticipants failed
					SAFE_RELEASE(pSession);
					return;
				}

				hr = pEnum->Next(1, &pParticipant, NULL);

				SAFE_RELEASE(pEnum);

				if (hr != S_OK)
				{
					// Next failed
					SAFE_RELEASE(pSession);
					return;
				}

				// Get the participant URI
				BSTR bstrURI = NULL;

				hr = pParticipant->get_UserURI(&bstrURI);

				if (FAILED(hr))
				{
					// get_UserURI failed
					SAFE_RELEASE(pSession);
					SAFE_RELEASE(pParticipant);
					return;
				}

				// Get the participant name
				BSTR bstrName = NULL;
				hr = pParticipant->get_Name(&bstrName);

				SAFE_RELEASE(pParticipant);

				if (FAILED(hr))
				{
					// get_PresentityURI failed
					SAFE_FREE_STRING(bstrURI);
					SAFE_RELEASE(pSession);
					return;
				}

				// Play a phone ring sound to alert the participant.
				m_pClient->PlayRing(RTCRT_PHONE, VARIANT_TRUE);

				// Show the session dialog
				BOOL fAccept;
				CIncomingDlg	cDlg;	   
				SESSION_DATA	pData;

				pData.bstrName = bstrName;
				pData.bstrURI = bstrURI;

				SetWindowLong ( m_hWnd, GWL_USERDATA, (LONG)&pData);

				// Show the Incoming phone request dialog.
				fAccept = (cDlg.DoModal() == IDOK);

				//Free the caller string
				SAFE_FREE_STRING(bstrURI);
				SAFE_FREE_STRING(bstrName);

				if (fAccept)
				{
					// Accept the session
					hr = pSession->Answer();

					if (FAILED(hr))
					{
						// Answer failed
						SAFE_RELEASE(pSession);
						return;
					}
				}
				else
				{
					// Reject the session
					pSession->Terminate(RTCTR_REJECT);

					SAFE_RELEASE(pSession);
					return;
				}
			}  

			// Add the session to the session list
			// This will create the session window
			hr = AddSession(pSession, enType);

			if (FAILED(hr))
			{
				// AddSession failed
				SAFE_RELEASE(pSession);
				return;
			}
		}        
		break;
	}

	// Deliver the session state to the session window
	if (m_AVDlg)
		m_AVDlg.SetState (enState);
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnRTCMediaEvent()
// Parameter: pEvent		Pointer to the event information
// Return Value: None
// Purpose: This method handles event during an Audio/Video conferencing call
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnRTCMediaEvent(IRTCMediaEvent *pEvent)
{
	long lMediaType;
	RTC_MEDIA_EVENT_TYPE enType;
	RTC_MEDIA_EVENT_REASON enReason;
	HRESULT hr;

	hr = pEvent->get_MediaType(&lMediaType);

	if (FAILED(hr))
	{
		// get_MediaType failed
		return;
	}

	hr = pEvent->get_EventType(&enType);			  
	if (FAILED(hr))
	{
		// get_EventType failed
		return;
	}

	hr = pEvent->get_EventReason(&enReason);

	if (FAILED(hr))
	{
		// get_EventReason failed
		return;
	}

	if ((m_AVDlg) && (m_AVDlg.GetState () != RTCSS_IDLE))
	{
		// Deliver the media state to the session window
		m_AVDlg.DeliverMedia(lMediaType, enType, enReason);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnRTCIntensityEvent()
// Parameter: pEvent		Pointer to the event information
// Return Value: None
// Purpose: This method handles event during an Audio/Video conferencing call
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnRTCIntensityEvent(IRTCIntensityEvent *pEvent)
{
	RTC_AUDIO_DEVICE enDevice;
	long lLevel, lMin, lMax;
	HRESULT hr;

	// Get the type of device (MIC or Speaker) that is being adjusted.
	hr = pEvent->get_Direction(&enDevice);

	if (FAILED(hr))
	{
		// get_Direction failed
		return;
	}

	hr = pEvent->get_Level(&lLevel);

	if (FAILED(hr))
	{
		// get_Level failed
		return;
	}

	hr = pEvent->get_Min(&lMin);

	if (FAILED(hr))
	{
		// get_Min failed
		return;
	}

	hr = pEvent->get_Max(&lMax);

	if (FAILED(hr))
	{
		// get_Max failed
		return;
	}

	// Normalize level to between zero and 100
	if ((lMax - lMin) == 0)
	{
		lLevel = 0;
	}
	else
	{
		lLevel = (lLevel - lMin) * 100 / (lMax - lMin);
	}

	if (m_AVDlg.GetState () != RTCSS_IDLE)
	{
		// Deliver the intensity state to the session window
		m_AVDlg.DeliverIntensity(enDevice, lLevel);
	}
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnRTCClientEvent()
// Parameter: pEvent		Pointer to the event information
// Return Value: None
// Purpose: Process RTCE_CLIENT event messages.
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnRTCClientEvent(IRTCClientEvent *pEvent)
{
	HRESULT hr;

	// Get the client event tyoe
	RTC_CLIENT_EVENT_TYPE enEventType;

	hr = pEvent->get_EventType(&enEventType);

	if (FAILED(hr))
	{
		// get_EventType failed
		return;
	}

	if ( enEventType == RTCCET_ASYNC_CLEANUP_DONE )
	{
		m_pClient->Shutdown();
		SAFE_RELEASE(m_pClient);
		// The RTC client has finished preparing for
		// shutdown. Destroy the window now.
		DestroyWindow();
	}
	else
	{
		if (m_AVDlg.GetState() != RTCSS_IDLE)
		{
			// Deliver the client state to the session window
			m_AVDlg.DeliverClient(enEventType);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
// Method: AddSession()
// Parameter: pSession		Session Handle
//			  enType		Type of Session to add to session list.
// Return Value: None
// Purpose: This method adds the current call to a session list.
//////////////////////////////////////////////////////////////////////////////
HRESULT CAVDConfDlg::AddSession(IRTCSession *pSession, RTC_SESSION_TYPE enType)
{
	// Is this an audio/video session?
	BOOL fAVSession = (enType != RTCST_IM);

	// Show the appropriate dialog box for either an AV Call or Messaging
	// call.
	if (fAVSession)
	{
		BOOL ret = m_AVDlg.Create (IDD_AVDLG,this);
		if (!ret)
		{
			MessageBox ( "Error Creating Dialog" );
		}
		m_AVDlg.ShowWindow (SW_SHOW);
		m_AVDlg.SetParentClient (m_pClient);
		m_AVDlg.SetSession (pSession);
	}
	else
	{
		BOOL ret = m_cMessageDlg.Create (IDD_MESSAGEDLG, this);
		if (!ret)
		{
			MessageBox ( "Error Creating Dialog" );
		}
		m_cMessageDlg.ShowWindow (SW_SHOW);
		m_cMessageDlg.SetParentClient ( m_pClient );
		m_cMessageDlg.SetSession (pSession);

	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
// Method: OnRTCMessagingEvent()
// Parameter: pEvent		Pointer to the event object.
// Return Value: None
// Purpose: This method processes all event messages relating to a Instant
//			Messaging event.
//////////////////////////////////////////////////////////////////////////////
HRESULT CAVDConfDlg::OnRTCMessagingEvent(IRTCMessagingEvent *pEvent)
{
	IRTCSession * pSession = NULL;
	//CRTCSession * pSessWindow = NULL;
	IRTCParticipant * pParticipant = NULL;
	RTC_MESSAGING_EVENT_TYPE enType;
	RTC_MESSAGING_USER_STATUS enStatus;
	BSTR bstrContentType = NULL;
	BSTR bstrMessage = NULL;
	HRESULT hr;

	hr = pEvent->get_Session(&pSession);

	if (FAILED(hr))
	{
		// get_Session failed
		return S_FALSE;
	}

	hr = pEvent->get_EventType(&enType);

	if (FAILED(hr))
	{
		// get_EventType failed
		return S_FALSE;
	}

	hr = pEvent->get_Participant(&pParticipant);

	if (FAILED(hr))
	{
		// get_Participant failed
		return S_FALSE;
	}

	if (enType == RTCMSET_MESSAGE)
	{
		hr = pEvent->get_MessageHeader(&bstrContentType);

		if (FAILED(hr))
		{
			// get_MessageHeader failed
			SAFE_RELEASE(pParticipant);
			return S_FALSE;
		}

		hr = pEvent->get_Message(&bstrMessage);

		if (FAILED(hr))
		{
			// get_Message failed
			SAFE_RELEASE(pParticipant);
			SAFE_FREE_STRING(bstrContentType);
			return S_FALSE;
		}

		// Deliver the message to the session window
		if (m_cMessageDlg)
			m_cMessageDlg.DeliverMessage(pParticipant, bstrContentType, bstrMessage);

		SAFE_FREE_STRING(bstrContentType);
		SAFE_FREE_STRING(bstrMessage);
	}
	else if (enType == RTCMSET_STATUS)
	{
		hr = pEvent->get_UserStatus(&enStatus);

		if (FAILED(hr))
		{
			// get_UserStatus failed
			return S_FALSE;
		}

		// Deliver the user status to the session window
		m_cMessageDlg.DeliverUserStatus(pParticipant, enStatus);
	}

	SAFE_RELEASE(pParticipant); 

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnOK()
// Parameter: None
// Return Value: None
// Purpose: Using the OK button to handle when the user shuts down the application
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnOK() 
{
	if (m_pClient)
	{
		m_pClient->StopT120Applets();

		m_pClient->PrepareForShutdown();

		if (m_pEvents)
		{
			m_pEvents->Unadvise( m_pClient);
		}
	}


	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnCancel()
// Parameter: None
// Return Value: None
// Purpose: Prevents the users from hitting the ESC button, or ALT+F4 to quit
//			the program.  Just trapping the IDCANCEL event.
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnCancel ()
{	// Don't process the ESC key or Cancel button.
	MessageBox ( "Please use the shutdown\nbutton to quit." );
	return;
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnAboutbox()
// Parameter: None
// Return Value: None
// Purpose: Shows the About box
//////////////////////////////////////////////////////////////////////////////
void CAVDConfDlg::OnAboutbox() 
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

