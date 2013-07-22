#include "stdafx.h"
#include "DerivedSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


UINT CDerivedSocket::m_nTransferMessage = NULL;

////////////////////////////////////////////////////////////////////////////////
// Constructor / Destructor													  //
////////////////////////////////////////////////////////////////////////////////
CDerivedSocket::CDerivedSocket()
{
	m_nDataSize = 0;
}


CDerivedSocket::~CDerivedSocket()
{
}


////////////////////////////////////////////////////////////////////////////////
// Operations                                                                 //
////////////////////////////////////////////////////////////////////////////////
void CDerivedSocket::Initialize(bool pIsServer, LPCTSTR pHostAddress, UINT pHostPort)
{
	AfxSocketInit();
	// Creating a stream socket.
	// socket server
	if( pIsServer) {
		if( Create(pHostPort) == 0)
			TRACE("Unable to create a socket. Error is %d", GetLastError());		
	}
	else{	// socket client
		if( Create() == 0)
			TRACE("Unable to create a socket. Error is %d", GetLastError());
		if( Connect(pHostAddress, pHostPort) == 0)
			TRACE("Unable to connect to a socket. Error is %d", GetLastError());
	}
}

// Overrided Notification Function
void CDerivedSocket::OnReceive(int nErrorCode)
{
	CSocket::OnReceive(nErrorCode);
	ASSERT( m_nDataSize != 0 );
	char* buff = new char[m_nDataSize];
	Receive(buff, m_nDataSize);
	// Notify that the message is received.
	::PostMessage( HWND_BROADCAST, m_nTransferMessage, (WPARAM)buff, m_nLParam);
}

// Overrided Notification Function
void CDerivedSocket::OnAccept(int nErrorCode)
{
	CSocket::OnAccept(nErrorCode);
	CDerivedSocket* This = new CDerivedSocket();
	This->m_nDataSize = m_nDataSize;
	This->m_nLParam = m_nLParam;
	Accept(*This);	
}

// Collect the RegisterWindow message Id, inorder to notify
void CDerivedSocket::Listen(UINT pOnReciveId, LPARAM pParam)
{
	m_nLParam = pParam;
	m_nTransferMessage = pOnReciveId;
	CSocket::Listen();
}

// Sends data to the server
bool CDerivedSocket::Send(char* pData, int pLength)
{
	int bytesSent = 0;
	bytesSent = CSocket::Send( pData, pLength);
	if( bytesSent <= 0 ) {
		TRACE("Unable to send data. Error is %d", GetLastError());
		return false;
	}
	return true;		
}