#include "stdafx.h"
#include "Mailslot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MESSAGETIMEOUT 1000

UINT CMailslot::m_nTransferMessage = NULL;
////////////////////////////////////////////////////////////////////////////////
// Constructor / Destructor													  //
////////////////////////////////////////////////////////////////////////////////
CMailslot::CMailslot()
{
	m_nIsServer = false;
	m_hInputslot = NULL;
	m_hOutputslot = NULL;
	m_strMailslot = _T("");
	m_nSize = 0;
}

CMailslot::~CMailslot()
{
	if( m_nIsServer)
		if(m_hInputslot != NULL)
			CloseHandle( m_hInputslot);
	else
		if(m_hOutputslot != NULL)
			CloseHandle( m_hOutputslot);
}


////////////////////////////////////////////////////////////////////////////////
// worker thread handler function                                             //
////////////////////////////////////////////////////////////////////////////////
UINT SlotListen( LPVOID pMailslot )
{
	CMailslot* This = (CMailslot*)pMailslot;
	if( This == NULL )
		return 0;
	
	DWORD message, noofMessages;
	// assign the size of the data to be read.
	ASSERT( This->GetTransferMessage() != NULL);
	// This is an infinite loop and breaks when the application ends.
	while(1) {
		GetMailslotInfo( This->GetInputslot(), NULL, &message, &noofMessages, NULL );
		if( noofMessages > 0) {
			// The total messages will be reduced only at the end of read operation
			// Hence don't use post message operation.
			LRESULT returnVal = ::SendMessageTimeout(HWND_BROADCAST, This->GetTransferMessage(), 
				NULL, This->GetLparam(), SMTO_BLOCK, MESSAGETIMEOUT, NULL);	
			if( returnVal == 0)
				TRACE("Unable to send message. Error is %d", GetLastError()); 
		}	
	}	
	return 1;
}


////////////////////////////////////////////////////////////////////////////////
// Operations                                                                 //
////////////////////////////////////////////////////////////////////////////////
bool CMailslot::Initialize(bool pIsServer, LPCTSTR pMailslotName)
{
	m_nIsServer = pIsServer;

	// Mailslot server
	if( m_nIsServer ) {
		m_strMailslot.Format("\\\\.\\mailslot\\%s", pMailslotName);
		m_hInputslot = CreateMailslot(	m_strMailslot,              // mailslot name
							 			0,					        // message can be of any size
										MAILSLOT_WAIT_FOREVER,      // waits forever for a message
										NULL						// It will not be inherited
									 );
		if( m_hInputslot == INVALID_HANDLE_VALUE) {
			TRACE("Unable to create a mail slot");
			return false;
		}
	}
	else {	// Mailslot client
		m_strMailslot.Format("\\\\*\\mailslot\\%s", pMailslotName);
		m_hOutputslot = CreateFile(	m_strMailslot,					// file name
									GENERIC_WRITE,					// Only Write Permission
									0,								// cannot be shared
									NULL,							// SD
									OPEN_EXISTING,					// Opens the file. fails if file doesn't exist			
									FILE_ATTRIBUTE_NORMAL,			// file attributes
									NULL							// handle to template file
								  );
		if( m_hOutputslot == INVALID_HANDLE_VALUE) {
			TRACE("Unable to create a file");
			return false;
		}
	}
	return true;
}


bool CMailslot::Write(char* pData)
{
	DWORD sizeWritten;
	WriteFile( m_hOutputslot, pData, m_nSize, &sizeWritten, NULL);
	if( sizeWritten == 0 ) {
		TRACE(" Unable to write to the Mail slot. Error is %d", GetLastError() );
		return false;
	}
	return true;
}


void CMailslot::Read(char*& pBuffer)
{
	DWORD message, noofMessages, dataRead;
	// check for the total messages
	BOOL result = GetMailslotInfo(	m_hInputslot,
									NULL,
									&message,
									&noofMessages,
									NULL
								 );
	if( !result)
		TRACE("Unable to get mail slot information");
	if( noofMessages == MAILSLOT_NO_MESSAGE)
		TRACE("There are no messages in the mailslot");
	// Only the last message is taken.
	while( noofMessages != 0)
	{
		ReadFile( m_hInputslot, pBuffer, m_nSize, &dataRead, NULL);
		GetMailslotInfo(	m_hInputslot,
							NULL,
							&message,
							&noofMessages,
							NULL
						);
	}
}

// change the Id and the lparam to application specific.
void CMailslot::Listen(UINT pOnReceiveId, LPARAM pParam)
{
	m_nTransferMessage = pOnReceiveId;
	m_nwindowProc = pParam;
	AfxBeginThread( SlotListen, this);
}


