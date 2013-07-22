#include "stdafx.h"
#include "NamedPipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PIPE_TIMEOUT (120*1000)

UINT CNamedPipe::m_nTransferMessage = NULL;

////////////////////////////////////////////////////////////////////////////////
// Constructor / Destructor													  //
////////////////////////////////////////////////////////////////////////////////
CNamedPipe::CNamedPipe()
{
	m_nSize = 0;
	m_strServerName = _T("");
	m_strPipeName = _T("");	
	m_hOutputPipeHandle = NULL;
	m_hInputPipeHandle = NULL;
	m_nwindowProc = NULL;
	m_nExitThread = false;
}


CNamedPipe::~CNamedPipe()
{
	// When the object life ends, exit the thread.
	m_nExitThread = true;
	if( m_nIsServer)
		CloseHandle(m_hInputPipeHandle);
	else
		CloseHandle(m_hOutputPipeHandle);
}


////////////////////////////////////////////////////////////////////////////////
// worker thread handler function                                             //
////////////////////////////////////////////////////////////////////////////////
UINT ListenerProc( LPVOID pNamedPipe )
{
	CNamedPipe* This = (CNamedPipe*)pNamedPipe;
	if( This == NULL )
		return 0;
	int size = This->GetSize();

	// assign the size of the data to be read.
	ASSERT( size != 0 );
	ASSERT( This->GetTransferMessage() != NULL);
	char* buff = new char[size];
	DWORD dataRead;
	// This is an infinite loop and breaks when the application ends.
	while(1) {
		ReadFile( This->GetInputPipeHandle(), buff, size, &dataRead, NULL);
		if( dataRead > 0) {
			// acts as a post thread message
			::PostMessage( HWND_BROADCAST, This->GetTransferMessage(), (WPARAM)buff, This->GetLparam());
		}
		if( This->GetExitThread() == true)
			break;
	}
	if( buff )
		delete[] buff, buff = 0;
	return 1;
}


////////////////////////////////////////////////////////////////////////////////
// Operations                                                                 //
////////////////////////////////////////////////////////////////////////////////
bool CNamedPipe::Initialize(bool pIsServer, LPCTSTR pServerName, LPCTSTR pPipeName)
{
	if( pIsServer == true ) {
		m_strPipeName.Format("\\\\.\\pipe\\%s", pPipeName);
		// If the security attributes is NULL, then it will not be connected 
		// across the network. Therefore fill the security attributes. Allow anyone to be connected to our server. 
		// This concept is called Impersonation.
		SECURITY_ATTRIBUTES saPipe;
		saPipe.lpSecurityDescriptor = (PSECURITY_DESCRIPTOR) malloc(SECURITY_DESCRIPTOR_MIN_LENGTH);
		InitializeSecurityDescriptor(saPipe.lpSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION);
		// Access Control list is asiigned as NULL inorder to allow all access to the object.
		SetSecurityDescriptorDacl(saPipe.lpSecurityDescriptor, TRUE, (PACL) NULL,FALSE);
		saPipe.nLength = sizeof(saPipe);
		saPipe.bInheritHandle = TRUE;
		m_hInputPipeHandle = CreateNamedPipe(   m_strPipeName,				// pipe name
												PIPE_ACCESS_INBOUND,        // client to the server
												PIPE_WAIT,					// Blocking mode is enabled
												1,					        // support given to only one client
												0,							// output buffer size
												0,							// input buffer size
												PIPE_TIMEOUT,			    // time-out interval
												&saPipe						// SD
											);
		if( !m_hInputPipeHandle ) {
			TRACE("Unable to create named pipe. Error is %d", GetLastError() );
			return false;
		}			
	}
	else {
		m_strPipeName.Format("\\\\%s\\pipe\\%s", pServerName, pPipeName);
		m_hOutputPipeHandle = CreateFile(	  m_strPipeName,				// file name
											  GENERIC_WRITE,				// Only Write Permission
											  0,							// cannot be shared
											  NULL,							// SD
											  OPEN_EXISTING,				// Opens the file. fails if file doesn't exist
											  FILE_ATTRIBUTE_NORMAL,		// file attributes
											  NULL							// handle to template file
										);
		if( m_hOutputPipeHandle == INVALID_HANDLE_VALUE) {
			TRACE("Unable to create named pipe. Error is %d", GetLastError() );
			return false;
		}
	}
	return true;
}


bool CNamedPipe::Write(char* pData)
{
	DWORD sizeWritten;
	WriteFile( m_hOutputPipeHandle, pData, m_nSize, &sizeWritten, NULL);
	if( sizeWritten == 0 ) {
		TRACE(" Unable to write to the Named pipe. Error is %d", GetLastError() );
		return false;
	}
	return true;
}


void CNamedPipe::Listen( UINT pOnReceiveId, LPARAM pParam)
{
	m_nTransferMessage = pOnReceiveId;
	m_nwindowProc = pParam;
	// If server, then create a worker thread which will infinitely read the pipe.
	AfxBeginThread( ListenerProc, this);
}