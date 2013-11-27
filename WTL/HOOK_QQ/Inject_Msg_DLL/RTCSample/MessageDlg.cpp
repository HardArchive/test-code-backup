// ------------------------------------------------------------------------------------
// Copyright ©2002 Intel Corporation
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
// MessageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "rtccore.h"
#include "AVDConf.h"
#include "MessageDlg.h"
#include "defines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg dialog


CMessageDlg::CMessageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMessageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMessageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMessageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMessageDlg)
	DDX_Control(pDX, IDC_MESSAGE, m_cMessage);
	DDX_Control(pDX, IDC_TEXT, m_cText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMessageDlg, CDialog)
	//{{AFX_MSG_MAP(CMessageDlg)
	ON_BN_CLICKED(IDC_SENDTEXT, OnSendtext)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMessageDlg message handlers

//////////////////////////////////////////////////////////////////////////////
// Method: DeliverMessage()
// Parameter: pParticipant		Participant interface handle
//			  bstrContentType	Type of content (not used in this demo version)
//			  bstrMessage		Message to display
// Return Value: S_OK if successful.
// Purpose: Display the message from local and remote.
//////////////////////////////////////////////////////////////////////////////
HRESULT CMessageDlg::DeliverMessage(IRTCParticipant *pParticipant, BSTR bstrContentType, 
									BSTR bstrMessage)
{
    HRESULT hr;
    BSTR bstrURI = NULL;

    // Get the URI of the incoming user
    hr = pParticipant->get_UserURI(&bstrURI);

    if (FAILED(hr))
    {
        // get_UserURI failed
        return hr;
    }

    // Display the incoming message
	char szBuf[256], szText[265];
	wcstombs ( szBuf, bstrURI, 256);
	SendDlgItemMessage (IDC_MESSAGE, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szBuf);
	
	wcstombs ( szBuf, bstrMessage, 256 );
	sprintf ( szText, "     %s", szBuf );
	SendDlgItemMessage (IDC_MESSAGE, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szText);

    if (FAILED(hr))
    {
        // DoDisplayMessage failed
        return hr;
    }

    if (FAILED(hr))
    {
        // DoDisplayTyping failed
        return hr;
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method: DeliverUserStatus()
// Parameter: pParticipant		Participant interface handle
//			  enStatus			Displays the status of the remote user (Typing or not).
// Return Value: None
// Purpose: Shows the remote users activitiy status.
//////////////////////////////////////////////////////////////////////////////
HRESULT CMessageDlg::DeliverUserStatus(IRTCParticipant *pParticipant, RTC_MESSAGING_USER_STATUS enStatus)
{
    HRESULT hr;
    BSTR bstrURI = NULL;

    // Get the URI of the incoming user
    hr = pParticipant->get_UserURI(&bstrURI);

    if (FAILED(hr))
    {
        // get_UserURI failed
        return hr;
    }

    // Display the typing status of the incoming user
	if (enStatus)
	{
		char szTyping[256], szBuf[256];

		wcstombs ( szBuf, bstrURI, 256 );
        wsprintf(szTyping, "%s is typing.", szBuf);

        SetDlgItemText ( IDC_STATUSTEXT, szTyping );
 	}

    if (FAILED(hr))
    {
        // DoDisplayTyping failed
        return hr;
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnSendtext()
// Parameter: None
// Return Value: None
// Purpose: Sends a message to the remote user.
//////////////////////////////////////////////////////////////////////////////
void CMessageDlg::OnSendtext() 
{
	LONG	lLength;
	BSTR	bstrMessage;
	WCHAR	szBuf[256];

	GetDlgItemTextW (m_hWnd, IDC_TEXT, szBuf, 256);
	if ((lLength = lstrlenW(szBuf)) > 0)
	{
		bstrMessage = SysAllocString ( szBuf );

		if (bstrMessage == NULL)
		{
			// Error allocating memory for string
			return;
		}

		DoDisplayMessage();

		//Send the message to participant
		m_pParentSession->SendMessage ( NULL, bstrMessage, 0 );

		SAFE_FREE_STRING (bstrMessage);
	}

}

//////////////////////////////////////////////////////////////////////////////
// Method: OnClear()
// Parameter: None
// Return Value: None
// Purpose: Clear the text screen.
//////////////////////////////////////////////////////////////////////////////
void CMessageDlg::OnClear() 
{
	m_cMessage.ResetContent ();
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnInitDialog()
// Parameter: None
// Return Value: None
// Purpose: Initialize the Messaging dialog (WM_INITDIALOG).
//////////////////////////////////////////////////////////////////////////////
BOOL CMessageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cText.LimitText ( 256 );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////////
// Method: SetParentClient()
// Parameter: pClient	Parent client handle
// Return Value: None
// Purpose: Get the parent client handle
//////////////////////////////////////////////////////////////////////////////
void CMessageDlg::SetParentClient(IRTCClient *pClient)
{
	m_pParentClient = pClient;
}

//////////////////////////////////////////////////////////////////////////////
// Method: SetSession()
// Parameter: pSession	Parent session handle
// Return Value: None
// Purpose: Get the parent session handle
//////////////////////////////////////////////////////////////////////////////
void CMessageDlg::SetSession(IRTCSession *pSession)
{
	m_pParentSession = pSession;
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnOK()
// Parameter: None
// Return Value: None
// Purpose: Terminate the Messaging session
//////////////////////////////////////////////////////////////////////////////
void CMessageDlg::OnOK() 
{
	HRESULT hr = m_pParentSession->Terminate(RTCTR_NORMAL);

	SAFE_RELEASE (m_pParentSession);

	DestroyWindow();
}

//////////////////////////////////////////////////////////////////////////////
// Method: DoDisplayMessage()
// Parameter: None
// Return Value: None
// Purpose: Displays the Message from local user in the Message window
//////////////////////////////////////////////////////////////////////////////
void CMessageDlg::DoDisplayMessage()
{
	BSTR bstrURI = NULL;
	char szBuf[256], LBBuf[265];

	HRESULT hr = m_pParentClient->get_LocalUserURI (&bstrURI);

	wcstombs(szBuf, bstrURI, 256);
	SendDlgItemMessage (IDC_MESSAGE, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)szBuf );

	GetDlgItemText (IDC_TEXT, szBuf, 256);
	wsprintf ( LBBuf, "       %s", szBuf );
	SendDlgItemMessage (IDC_MESSAGE, LB_INSERTSTRING, (WPARAM)-1, (LPARAM)LBBuf);

	m_cText.SetSel (0, -1);
	m_cText.ReplaceSel ("");
}

//////////////////////////////////////////////////////////////////////////////
// Method: OnCancel()
// Parameter: None
// Return Value: None
// Purpose: Just trapping this message.
//////////////////////////////////////////////////////////////////////////////
void CMessageDlg::OnCancel() 
{
	return;	
}
