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
// AVDlg.cpp : implementation file
//
// Purpose: Handles the Audio and Video window messages.
//

#include "stdafx.h"
#include "rtccore.h"
#include "AVDConf.h"
#include "AVDlg.h"
#include "defines.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAVDlg dialog


CAVDlg::CAVDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAVDlg::IDD, pParent)
{
	m_pParentClient = NULL;
	//{{AFX_DATA_INIT(CAVDlg)
	//}}AFX_DATA_INIT
}
								   

void CAVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAVDlg)
	DDX_Control(pDX, IDC_SPEAKERMUTE, m_cSpeakerMute);
	DDX_Control(pDX, IDC_SPEAKER_BAR, m_cSpeakerLevel);
	DDX_Control(pDX, IDC_MIC_BAR, m_cMicLevel);
	DDX_Control(pDX, IDC_SPEAKER_SLIDER, m_cSpeakerSlider);
	DDX_Control(pDX, IDC_MICMUTE, m_cMicMute);
	DDX_Control(pDX, IDC_SPEAKER_TEXT, m_cSpeakerText);
	DDX_Control(pDX, IDC_MIC_TEXT, m_cMicText);
	DDX_Control(pDX, IDC_MIC_SLIDER, m_cMicSlider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAVDlg, CDialog)
	//{{AFX_MSG_MAP(CAVDlg)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_SPEAKER_SLIDER, OnSpeakerSlider)
	ON_NOTIFY(NM_OUTOFMEMORY, IDC_MIC_SLIDER, OnMicSlider)
	ON_BN_CLICKED(IDC_SPEAKERMUTE, OnSpeakermute)
	ON_BN_CLICKED(IDC_MICMUTE, OnMicmute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAVDlg message handlers

//////////////////////////////////////////////////////////////////////////////
// Method:  OnOK()
// Purpose: When the user clicks the Terminate button (id'd with OK button), 
//			the call will be terminated if there is an active session.
/////////////////////////////////////////////////////////////////////////////
void CAVDlg::OnOK() 
{
	if (m_pSession)
	{
		HRESULT hr = m_pSession->Terminate(RTCTR_NORMAL);
	}

	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////////
// Method:  OnCancel()
// Purpose: This message is not processed.  Need to intercept the CANCEL message or
//			the message will be process by parent window.
/////////////////////////////////////////////////////////////////////////////
void CAVDlg::OnCancel ()
{
	return;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  OnSpeakerSlider()
// Purpose: Process the Speaker volume slider.
//////////////////////////////////////////////////////////////////////////////
void CAVDlg::OnSpeakerSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	long lPos = m_cSpeakerSlider.GetPos ();
	DoVolume (RTCAD_SPEAKER, lPos);
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  OnMicSlider()
// Purpose: Process the MIC gain slider.
/////////////////////////////////////////////////////////////////////////////
void CAVDlg::OnMicSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	long lPos = m_cMicSlider.GetPos ();
	DoVolume (RTCAD_MICROPHONE, lPos);
	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  OnMicmute()
// Purpose: Process the MIC mute button message.
/////////////////////////////////////////////////////////////////////////////
void CAVDlg::OnMicmute() 
{
	if (m_cMicMute.IsDlgButtonChecked (IDC_MICMUTE) != 0)
		m_cMicSlider.EnableWindow (FALSE);
	else
		m_cMicSlider.EnableWindow (TRUE);
	
	DoMute (RTCAD_MICROPHONE);
}

//////////////////////////////////////////////////////////////////////////////
// Method:  OnSpeakermute()
// Purpose: Process the Speaker mute button message.
/////////////////////////////////////////////////////////////////////////////
void CAVDlg::OnSpeakermute() 
{
	if (IsDlgButtonChecked (IDC_SPEAKERMUTE) != 0)
		m_cSpeakerSlider.EnableWindow (FALSE);
	else
		m_cSpeakerSlider.EnableWindow (TRUE);
	
	DoMute (RTCAD_SPEAKER);
}

//////////////////////////////////////////////////////////////////////////////
// Method:  SetParentClient()
// Purpose: Called by the Parent window to let the child reference any public 
//			parent methods.
/////////////////////////////////////////////////////////////////////////////
HRESULT CAVDlg::SetParentClient(IRTCClient *pClient)
{
	m_pParentClient = pClient;
	ShowAudio();
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  DoMute()
// Purpose: Process Audio mute message for the MIC or Speaker
/////////////////////////////////////////////////////////////////////////////
HRESULT CAVDlg::DoMute(RTC_AUDIO_DEVICE enDevice)
{
    IRTCClient * pClient = NULL;
    BOOL fMute = FALSE;	  
    HWND hWnd = NULL;
    HRESULT hr;

    // Check to make sure there is a valid call in progress.
	if (m_pParentClient == NULL)
		return -1;

    // Get current check state
	fMute = IsDlgButtonChecked ( (enDevice == RTCAD_SPEAKER) ? IDC_SPEAKERMUTE : IDC_MICMUTE);

    // Set the mute
    hr = m_pParentClient->put_AudioMuted(enDevice, fMute ? VARIANT_TRUE : VARIANT_FALSE);

    if (FAILED(hr))
    {
        // put_AudioMute failed
        return hr;
    }
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  DoVolume()
// Purpose: Process Audio volume level for speaker or mic
/////////////////////////////////////////////////////////////////////////////
HRESULT CAVDlg::DoVolume(RTC_AUDIO_DEVICE enDevice, long lVolume)
{
    HRESULT hr;

    // Check to make sure there is a valid call in progress.
	if (m_pParentClient == NULL)
		return -1;

    // Set the volume
    hr = m_pParentClient->put_Volume(enDevice, lVolume);

    if (FAILED(hr))
    {
        // put_Volume failed
        return hr;
    }

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  ShowAudio()
// Purpose: Process Audio volume level for speaker or mic
/////////////////////////////////////////////////////////////////////////////
HRESULT CAVDlg::ShowAudio()
{
    VARIANT_BOOL fMute = FALSE;
    long lVolume = 0;
    long lMediaCaps = 0;
    HRESULT hr;

    if (m_pParentClient == NULL)
		return -1;

    // Get the media capabilities
    hr = m_pParentClient->get_MediaCapabilities(&lMediaCaps);

    if (FAILED(hr))
    {
        // get_MediaCapabilities failed
        return hr;
    }

    if (lMediaCaps & RTCMT_AUDIO_RECEIVE)
    {
        // Show the speaker mute state
        hr = m_pParentClient->get_AudioMuted(RTCAD_SPEAKER, &fMute);

        if (FAILED(hr))
        {
            // get_AudioMuted failed
            return hr;
        }

        m_cSpeakerMute.SetCheck ( fMute ? BST_CHECKED : BST_UNCHECKED);

        // Show the speaker volume
        hr = m_pParentClient->get_Volume(RTCAD_SPEAKER, &lVolume);

        if (FAILED(hr))
        {
            // get_Volume failed
            return hr;
        }

		m_cSpeakerSlider.SetPos (lVolume);
    }

    if (lMediaCaps & RTCMT_AUDIO_SEND)
    {
  
        // Show the microphone mute state
        hr = m_pParentClient->get_AudioMuted(RTCAD_MICROPHONE, &fMute);

        if (FAILED(hr))
        {
            // get_AudioMuted failed
            return hr;
        }

        m_cMicMute.SetCheck ( fMute ? BST_CHECKED : BST_UNCHECKED );
   
        // Show the microphone volume
        hr = m_pParentClient->get_Volume(RTCAD_MICROPHONE, &lVolume);

        if (FAILED(hr))
        {
            // get_Volume failed
            //SAFE_RELEASE(pClient);
            return hr;
        }

		m_cSpeakerSlider.SetPos (lVolume);
    }
    return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  ShowVideo()
// Purpose: Process the Video window
/////////////////////////////////////////////////////////////////////////////
HRESULT CAVDlg::ShowVideo(RTC_VIDEO_DEVICE enDevice, BOOL fShow)
{
    IVideoWindow * pVid = NULL;
    long lMediaCaps = 0;
    HWND hWnd = NULL;
    HRESULT hr;

    if (m_pParentClient == NULL)
	{
		return -1;
	}

	// Get the media capabilities
    hr = m_pParentClient->get_MediaCapabilities(&lMediaCaps);

    if (FAILED(hr))
    {
        // get_MediaCapabilities failed
        return hr;
    }

    // Get the video interface for the video window
	hr = m_pParentClient->get_IVideoWindow(enDevice, &pVid);

    if (FAILED(hr))
    {
        // get_IVideoWindow failed
        return hr;
    }

    // Process the Preview Window here.
	if (enDevice == RTCVD_PREVIEW)
    {          
        // Determine whether to show the preview video
        fShow = fShow && (lMediaCaps & RTCMT_VIDEO_SEND);
        m_fShowPrev = fShow;
        hWnd = m_hPrevVideoParent;

        // Show or hide the preview video preview parent window
        ::ShowWindow(m_hPrevVideoParent, fShow ? SW_SHOW : SW_HIDE);

        // Set window region for receive video parent window        
        HRGN hRegion;

        if (fShow)
        {
            // Set the receive video region to make room for the preview
            // video picture-in-picture
            POINT rgPoints[6] =
                    { 0, 0,
                      0, m_lRecvHeight,
                      m_lRecvWidth - m_lPrevWidth, m_lRecvHeight,
                      m_lRecvWidth - m_lPrevWidth, m_lRecvHeight - m_lPrevHeight,
                      m_lRecvWidth, m_lRecvHeight - m_lPrevHeight,
                      m_lRecvWidth, 0
                    };

            hRegion = CreatePolygonRgn(rgPoints, 6, ALTERNATE);
        }
        else
        {
            // Set the receive video region to be rectangular
            hRegion = CreateRectRgn(0, 0, m_lRecvWidth, m_lRecvHeight);
        }

        ::SetWindowRgn(m_hRecvVideoParent, hRegion, TRUE);
    }
    else
    {
        // Determine whether to show the receive video        
        fShow = fShow && (lMediaCaps & RTCMT_VIDEO_RECEIVE);
        m_fShowRecv = fShow;
        hWnd = m_hRecvVideoParent;

        // Always show the video receive parent window
        ::ShowWindow(m_hRecvVideoParent, SW_SHOW);
    }

    if ( fShow == TRUE )
    {
        // Set the video window style
        pVid->put_WindowStyle( WS_CHILD |
                               WS_CLIPCHILDREN |
                               WS_CLIPSIBLINGS );
        
        // Set the parent window for the video window
        pVid->put_Owner( (OAHWND)hWnd );

        RECT rc;
        ::GetClientRect(hWnd, &rc );  
        
        // Position the video window
        pVid->SetWindowPosition(
            rc.left,
            rc.top,
            rc.right,
            rc.bottom
            );

        // Make the video window visible
        pVid->put_Visible(-1);        
    }       

    SAFE_RELEASE(pVid);    

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  DeliverMedia()
// Purpose: Process the recieved video and preview video.
/////////////////////////////////////////////////////////////////////////////
HRESULT CAVDlg::DeliverMedia(long lMediaType, RTC_MEDIA_EVENT_TYPE enType, 
							 RTC_MEDIA_EVENT_REASON enReason)
{
    RTC_VIDEO_DEVICE enVideo;
    BOOL fShow;
    HRESULT hr = S_OK;

    // Which media type changed?
    switch (lMediaType)
    {
    case RTCMT_VIDEO_SEND:
        enVideo = RTCVD_PREVIEW;
        break;

    case RTCMT_VIDEO_RECEIVE:
        enVideo = RTCVD_RECEIVE;
        break;

    case RTCMT_T120_SENDRECV:
		break;

	default:
        return S_OK;
    }

    // Did the media start or stop?
    switch (enType) 
    {
    case RTCMET_STARTED:
        fShow = TRUE;
        break;

    case RTCMET_STOPPED:
		::ShowWindow(m_hRecvVideoParent, SW_HIDE);
		::SetParent(m_hRecvVideoParent, NULL);

		::ShowWindow(m_hPrevVideoParent, SW_HIDE);
		::SetParent(m_hPrevVideoParent, NULL);

		SetState (RTCSS_IDLE);

		SAFE_RELEASE(m_pSession);

		DestroyWindow();
		return S_OK;

    case RTCMET_FAILED:
        fShow = FALSE;
        break;

    default:
        return S_OK;
    }

    // Show or hide the video as appropriate
    hr = ShowVideo(enVideo, fShow);

    return hr;

}

//////////////////////////////////////////////////////////////////////////////
// Method:  DeliverIntensity()
// Purpose: Process messages relating to the MIC and/or Speaker devices.
/////////////////////////////////////////////////////////////////////////////
HRESULT CAVDlg::DeliverIntensity(RTC_AUDIO_DEVICE enDevice, long lLevel)
{
    switch(enDevice)
    {
    case RTCAD_SPEAKER:
        // Set the position of the speaker progress bar
		m_cSpeakerLevel.SetPos (lLevel);
        break;

    case RTCAD_MICROPHONE:
        // Set the position of the microphone progress bar
		m_cMicLevel.SetPos (lLevel);
        break;
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  DeliverClient()
// Purpose: Process the audio changes to the audio or video devices.
/////////////////////////////////////////////////////////////////////////////
HRESULT CAVDlg::DeliverClient(RTC_CLIENT_EVENT_TYPE enEventType)
{
    switch(enEventType)
    {
    case RTCCET_VOLUME_CHANGE:
        // On volume change refresh the audio controls
        // so volume controls can be updated
        ShowAudio();
        break;

    case RTCCET_DEVICE_CHANGE:
        // On device change refresh the audio and video
        // controls as available devices may have changed
        ShowAudio();
        ShowVideo(RTCVD_PREVIEW, m_fShowPrev);
        ShowVideo(RTCVD_RECEIVE, m_fShowRecv);
        break;
    }

    return S_OK;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  OnInitDialog()
// Purpose: Process the WM_INITDIALOG message.
/////////////////////////////////////////////////////////////////////////////
BOOL CAVDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int OFFSETX = 10;
	int OFFSETY = 50;

    RECT rcWnd;
    const int SIZE_EDGE = 5;
    const int TEXT_HEIGHT = 16;
    const int LEVEL_HEIGHT = 10;
    const int VOL_HEIGHT = 20;   

    ::GetClientRect(m_hWnd, &rcWnd);    
	
	m_pParentClient = NULL;
    
	// Calculate video window size
    float fAspectRatio = (float)QCIF_CY_SIZE/(float)QCIF_CX_SIZE;
    m_lRecvWidth = rcWnd.right - rcWnd.left - 2*SIZE_EDGE;
    m_lRecvHeight = (long)(fAspectRatio * (float)m_lRecvWidth);
	
    m_lPrevWidth = QQCIF_CX_SIZE;
    m_lPrevHeight = QQCIF_CY_SIZE;

    // Resize the receive video parent window  
    RECT rcRecvVid;

    rcRecvVid.left = rcWnd.left + SIZE_EDGE; 
    rcRecvVid.right = rcWnd.left + m_lRecvWidth + SIZE_EDGE;
    rcRecvVid.top = rcWnd.top + SIZE_EDGE;
    rcRecvVid.bottom = rcWnd.top + m_lRecvHeight + SIZE_EDGE;
    
    // Resize the preview video parent window
    RECT rcPrevVid;

    rcPrevVid.left = rcRecvVid.right - m_lPrevWidth; 
    rcPrevVid.right = rcRecvVid.right;
    rcPrevVid.top = rcRecvVid.bottom - m_lPrevHeight;
    rcPrevVid.bottom = rcRecvVid.bottom;         

	m_hRecvVideoParent = NULL;
	m_hPrevVideoParent = NULL;

	// Set the Speaker and MIC Slider range
	m_cSpeakerSlider.SetRangeMax (0xffff);
	m_cMicSlider.SetRangeMax (0xffff);

	// Create the Video windows
    if (m_hRecvVideoParent == NULL)
    {
        // Create the receive video parent window
        m_hRecvVideoParent = CreateWindowExW(
            WS_EX_CLIENTEDGE,
            L"STATIC",
            NULL,
            WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			rcRecvVid.left, rcRecvVid.top,
			m_lRecvWidth, m_lRecvHeight,
            m_hWnd,
            NULL,
            GetModuleHandle(NULL),
            NULL);
    
        if ( !m_hRecvVideoParent )
        {
            // CreateWindowExW failed
            return -1;
        }
    }
    else
    {
        // Take ownership of the receive video parent window
        if (!::SetParent(m_hRecvVideoParent, m_hWnd))
        {
            // SetParent failed
            return -1;
        }
    }

	if (m_hPrevVideoParent == NULL)
    {
        // Create the preview video parent window
        m_hPrevVideoParent = CreateWindowExW(
            WS_EX_CLIENTEDGE,
            L"STATIC",
            NULL,
            WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			rcPrevVid.left, rcPrevVid.top,
			m_lPrevWidth, m_lPrevHeight,
            m_hWnd,
            NULL,
            GetModuleHandle(NULL),
            NULL);
    
        if ( !m_hPrevVideoParent )
        {
            // CreateWindowExW failed
            return -1;
        }
    }
    else
    {
        // Take ownership of the preview video parent window
        if (!::SetParent(m_hPrevVideoParent, m_hWnd))
        {
            // SetParent failed
            return -1;
        }
    }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//////////////////////////////////////////////////////////////////////////////
// Method:  SetSession()
// Purpose: Notify child window (or AV Window) the handle to the parent Session
//			handle.
/////////////////////////////////////////////////////////////////////////////
void CAVDlg::SetSession(IRTCSession *pSession)
{
	m_pSession = pSession;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  SetState()
// Purpose: Sets the state of the AV call.
/////////////////////////////////////////////////////////////////////////////
void CAVDlg::SetState(RTC_SESSION_STATE enState)
{
	m_rtcState = enState;
}

//////////////////////////////////////////////////////////////////////////////
// Method:  GetState()
// Purpose: Returns the state of the AV call.
/////////////////////////////////////////////////////////////////////////////
RTC_SESSION_STATE CAVDlg::GetState()
{
	return m_rtcState;
}
