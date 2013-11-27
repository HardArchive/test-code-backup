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
// RTCEvents.cpp: implementation of the CRTCEvents class.
//				  Event Handler class that is registered with the RTC event callback
//				  mechnism.

#include "stdafx.h"
#include "AVDConf.h"
#include "RTCEvents.h"

#include "defines.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRTCEvents::CRTCEvents()
{
	m_dwRefCount = 0;
	m_dwCookie = 0;
	m_hWnd = NULL;
}

CRTCEvents::~CRTCEvents()
{

}

//////////////////////////////////////////////////////////////////////////////
// Method: AddRef()
// Parameter: None
// Return Value: Reference count
// Purpose: Keeps track of the number of connections to the event object.
//			Called from QueryInterface() method.
//////////////////////////////////////////////////////////////////////////////
ULONG STDMETHODCALLTYPE CRTCEvents::AddRef()
{
    m_dwRefCount++;
    return m_dwRefCount;
}

//////////////////////////////////////////////////////////////////////////////
// Method: Release()
// Parameter: None
// Return Value: Reference count
// Purpose: Releases connection to the event object and decrements the ref
//			count.  See SAFE_RELEASE() macro
//////////////////////////////////////////////////////////////////////////////
ULONG STDMETHODCALLTYPE CRTCEvents::Release()
{
    m_dwRefCount--;

    if ( 0 == m_dwRefCount)
    {
        delete this;
    }

    return m_dwRefCount;
}

//////////////////////////////////////////////////////////////////////////////
// Method: QueryInterface()
// Parameter: None
// Return Value: None
// Purpose: Called by RTC to create a connection point.
//////////////////////////////////////////////////////////////////////////////
HRESULT STDMETHODCALLTYPE CRTCEvents::QueryInterface(REFIID iid, void **ppvObject)
{
    if (iid == IID_IRTCEventNotification)
    {
        *ppvObject = (void *)this;
        AddRef();
        return S_OK;
    }

    if (iid == IID_IUnknown)
    {
        *ppvObject = (void *)this;
        AddRef();
        return S_OK;
    }
	return E_NOINTERFACE;
}

//////////////////////////////////////////////////////////////////////////////
// Method: Advise()
// Parameter: pClient	Client object to associate the connection point
//			  hWnd		Owner's Window handle 
// Return Value: S_OK if successful.
// Purpose: Called during initialization to create an event connection point.
//////////////////////////////////////////////////////////////////////////////
HRESULT CRTCEvents::Advise(IRTCClient *pClient, HWND hWnd)
{
	IConnectionPointContainer * pCPC = NULL;
	IConnectionPoint * pCP = NULL;
    HRESULT hr;

    // Find the connection point container
	hr = pClient->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);

	if (SUCCEEDED(hr))
    {
        // Find the connection point
		hr = pCPC->FindConnectionPoint(IID_IRTCEventNotification, &pCP);

        SAFE_RELEASE(pCPC);

        if (SUCCEEDED(hr))
        {
            // Advise the connection
		    hr = pCP->Advise(this, &m_dwCookie);

            SAFE_RELEASE(pCP);
        }
    }

    // Store the window handle of the application so we
    // can post messages to it when events are fired
    m_hWnd = hWnd;
	
	return hr;
}

//////////////////////////////////////////////////////////////////////////////
// Method: Unadvise()
// Parameter: pClient	Client object that owns the connection point.
// Return Value: S_OK if successful.
// Purpose: Releases the connection point to the event handler.
//////////////////////////////////////////////////////////////////////////////
HRESULT CRTCEvents::Unadvise(IRTCClient *pClient)
{
	IConnectionPointContainer * pCPC = NULL;
	IConnectionPoint * pCP = NULL;
    HRESULT hr;

    if (m_dwCookie)
    {
        // Find the connection point container
	    hr = pClient->QueryInterface(IID_IConnectionPointContainer, (void**)&pCPC);

	    if (SUCCEEDED(hr))
        {
            // Find the connection point
		    hr = pCPC->FindConnectionPoint(IID_IRTCEventNotification, &pCP);

            SAFE_RELEASE(pCPC);

            if (SUCCEEDED(hr))
            {
                // Unadvise this connection
		        hr = pCP->Unadvise(m_dwCookie);

                SAFE_RELEASE(pCP);
            }
        }
    }

	return hr;
}

//////////////////////////////////////////////////////////////////////////////
// Method: Event()
// Parameter: enEvent		Type of event from the RTC object
//			  pDisp			Dispatch handle	
// Return Value: S_OK if successful.
// Purpose: Event Message handler
//////////////////////////////////////////////////////////////////////////////
HRESULT STDMETHODCALLTYPE CRTCEvents::Event(RTC_EVENT enEvent, IDispatch *pDisp)
{
	// We will post a message containing the event to the
    // application window.
    
    // Add a reference to the event so we can hold onto it while
    // the event is in the message queue.
    pDisp->AddRef();

    // Post the message
    PostMessage( m_hWnd, WM_RTC_EVENT, (WPARAM)enEvent, (LPARAM)pDisp );

    return S_OK;
}
