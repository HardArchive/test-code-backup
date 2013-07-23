// MyPipe.cpp: ʵ��CMyPipe��.
//
#include "stdafx.h"
#include "MyPipe.h"

//---------------------------------------------------------------------------
CMyPipe::CMyPipe()
{
	m_pThread=NULL;
}

//---------------------------------------------------------------------------
CMyPipe::~CMyPipe()
{
	if(m_pThread){ 
		if(TerminateThread(m_pThread->m_hThread,0))
		{
			if(m_pThread) 
				delete	m_pThread;
			m_pThread = NULL;
		}
	}
}

//---------------------------------------------------------------------------
void CMyPipe::ClientCreateFile()
{
	LPCSTR szPipeName=TEXT("\\\\.\\pipe\\ssnp\\"); 
	
	m_hPipe=CreateFile(szPipeName,
		GENERIC_READ|GENERIC_WRITE, 
		FILE_SHARE_READ| FILE_SHARE_WRITE,NULL, OPEN_EXISTING, 
        FILE_ATTRIBUTE_ARCHIVE|FILE_FLAG_WRITE_THROUGH,
		// FILE_FLAG_WRITE_THROUGH �趨����.
		NULL); 
	
	// ��鲢�б������ܵ��ļ��Ƿ񱻴�,���û�б��򿪣���ֹ����. 
	if (m_hPipe == INVALID_HANDLE_VALUE)
		TRACE("Unable to create a named pipe. Error: %d\n",GetLastError());
}

//---------------------------------------------------------------------------
// ���ڿͻ�����ĺ���.
void CMyPipe::ClientSend(CString strRequest) 
{
	m_strRequest=strRequest;
	char toSendtxt[80];

	// ����������Ϣֱ��������ֹ.
	while(1)
	{
		TRACE("Sending...\n");
		
		DWORD dwNumBytesWritten,dwNumBytesRead;
		
		strcpy(toSendtxt,m_strRequest);
		
		// ��ܵ�д����Ϣ.
		if (!WriteFile(m_hPipe,toSendtxt, (DWORD)sizeof(toSendtxt),
			&dwNumBytesWritten,(LPOVERLAPPED) NULL))
		{
			// ����������ܵ�д��ʱ���ִ���,��ֹ����. 
			TRACE("Unable to write to named pipe. Error: %d\n",GetLastError());
			CloseHandle(m_hPipe);
		}
		else{
			
			ReadFile(m_hPipe,toSendtxt, (DWORD)sizeof(toSendtxt),
				&dwNumBytesRead,(LPOVERLAPPED) NULL);
			m_strReply=toSendtxt;
			break;
		}
		// ���ٴη�����Ϣǰ�ȴ�.
        Sleep(4800);
	} 
}

//---------------------------------------------------------------------------
// ���ڷ������ĺ���.
void CMyPipe::ServerCreateNamedPipe(CString strReply) 
{	
	m_strReply=strReply;
	LPCSTR szPipeName=TEXT("\\\\.\\pipe\\ssnp\\"); 
	
	// Ϊ������Ϣ���������ܵ�.
	m_hPipe=CreateNamedPipe(szPipeName, 
		PIPE_ACCESS_DUPLEX|FILE_FLAG_WRITE_THROUGH, 
		// ����ģʽ. 
		PIPE_WAIT|PIPE_TYPE_BYTE, 
		PIPE_UNLIMITED_INSTANCES,
		128,128,NULL,NULL); 
	
	// ����Ƿ������ܵ�������.
	if (m_hPipe == INVALID_HANDLE_VALUE)
	{
		TRACE("Unable to create a named pipe.\n");
		return;
	}
	
	m_pThread=AfxBeginThread(ServerReadProc, this); // �����߳�.
}

//---------------------------------------------------------------------------
UINT CMyPipe::ServerReadProc(LPVOID lpVoid) 
{
	DWORD dwNumBytesRead,dwNumBytesWrite;
	char toDisptxt[80];
	
	// ����ͻ����������ܵ�,������ɹ�����ֹ.
	TRACE("Waiting for connection... \n");
	
	CMyPipe *Parent= (CMyPipe*)lpVoid;
	
	if(!ConnectNamedPipe(Parent->m_hPipe, (LPOVERLAPPED) NULL))
	{
		TRACE("Unable to connect a named pipe. Error: %d\n",GetLastError());
		CloseHandle(Parent->m_hPipe);
		return 1;
	}
	
	// ���������Ϣֱ��������ֹ.
	while(1)
	{
		// ��ȡ��Ϣ������ȡ�����Ƿ�ɹ�. 
		if (!ReadFile(Parent->m_hPipe, toDisptxt,sizeof(toDisptxt),
			&dwNumBytesRead, (LPOVERLAPPED) NULL))
		{
			TRACE("Unable to read from named pipe. Error: %d\n" ,GetLastError());
			CloseHandle(Parent->m_hPipe);
			return 1;
		}
		else
		{
			// ������յ��ַ���.
			Parent->m_strRequest=toDisptxt;
			strcpy(toDisptxt,Parent->m_strReply);
	
			// д��һ���ַ���.
			WriteFile(Parent->m_hPipe, toDisptxt,sizeof(toDisptxt),
			    	&dwNumBytesWrite, (LPOVERLAPPED) NULL);
		}
	}

	return 0;
}