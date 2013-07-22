//---------------------------------------------------------------------------
//
//	"PipeListener.cpp"
//
//  Listen to one named pipe
//
//	by Holger Kloos
//
//---------------------------------------------------------------------------



#include "stdafx.h"
#include "PipeListener.h"


const char* sPipeName = "\\\\.\\pipe\\HolgersTracer";




CPipeListener::CPipeListener(IPipeDataDest* pDest)
{
	m_nID = 0;
	m_hThread = NULL;
	m_pDest = pDest;
}

CPipeListener::~CPipeListener()
{
	if (m_hThread)
		CloseHandle(m_hThread);
}

DWORD CPipeListener::ReadPipe()
{
	HANDLE hPipe = CreateNamedPipe(sPipeName, PIPE_ACCESS_INBOUND, PIPE_TYPE_BYTE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 0, 0, 200, NULL);
	if (!ConnectNamedPipe(hPipe, NULL))
		return FALSE;

	if (m_pDest)
		m_pDest->OnConnectingPipe();

	DWORD nReaded = 0;
	char buffer[512];
	while (ReadFile(hPipe, buffer, sizeof(buffer)-1, &nReaded, NULL))
	{
		buffer[nReaded] = 0;
		if (m_pDest)
			m_pDest->OnIncomingData(buffer, nReaded);
	}
	return 0;
}

DWORD WINAPI CPipeListener::ThredFkt(LPVOID lpParameter)
{
	CPipeListener* pThis = (CPipeListener*)lpParameter;

	DWORD nResult = pThis->ReadPipe(); 

	IPipeDataDest* pPipeDataDest = pThis->m_pDest;
	if (pPipeDataDest)
		pPipeDataDest->OnDisConnectingPipe(pThis);

	return nResult;
}

bool CPipeListener::StartReader()
{
	m_hThread = CreateThread(NULL, 0, ThredFkt, this, 0, &m_nID);
	return m_hThread != NULL;
}