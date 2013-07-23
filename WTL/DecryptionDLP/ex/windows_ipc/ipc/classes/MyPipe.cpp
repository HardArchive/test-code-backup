// MyPipe.cpp: 实现CMyPipe类.
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
		// FILE_FLAG_WRITE_THROUGH 设定阻塞.
		NULL); 
	
	// 检查并判别命名管道文件是否被打开,如果没有被打开，终止程序. 
	if (m_hPipe == INVALID_HANDLE_VALUE)
		TRACE("Unable to create a named pipe. Error: %d\n",GetLastError());
}

//---------------------------------------------------------------------------
// 用于客户程序的函数.
void CMyPipe::ClientSend(CString strRequest) 
{
	m_strRequest=strRequest;
	char toSendtxt[80];

	// 反复发送消息直到程序终止.
	while(1)
	{
		TRACE("Sending...\n");
		
		DWORD dwNumBytesWritten,dwNumBytesRead;
		
		strcpy(toSendtxt,m_strRequest);
		
		// 向管道写入消息.
		if (!WriteFile(m_hPipe,toSendtxt, (DWORD)sizeof(toSendtxt),
			&dwNumBytesWritten,(LPOVERLAPPED) NULL))
		{
			// 如果向命名管道写入时出现错误,终止程序. 
			TRACE("Unable to write to named pipe. Error: %d\n",GetLastError());
			CloseHandle(m_hPipe);
		}
		else{
			
			ReadFile(m_hPipe,toSendtxt, (DWORD)sizeof(toSendtxt),
				&dwNumBytesRead,(LPOVERLAPPED) NULL);
			m_strReply=toSendtxt;
			break;
		}
		// 在再次发送消息前等待.
        Sleep(4800);
	} 
}

//---------------------------------------------------------------------------
// 用于服务程序的函数.
void CMyPipe::ServerCreateNamedPipe(CString strReply) 
{	
	m_strReply=strReply;
	LPCSTR szPipeName=TEXT("\\\\.\\pipe\\ssnp\\"); 
	
	// 为接收消息创建命名管道.
	m_hPipe=CreateNamedPipe(szPipeName, 
		PIPE_ACCESS_DUPLEX|FILE_FLAG_WRITE_THROUGH, 
		// 阻塞模式. 
		PIPE_WAIT|PIPE_TYPE_BYTE, 
		PIPE_UNLIMITED_INSTANCES,
		128,128,NULL,NULL); 
	
	// 检查是否命名管道被创建.
	if (m_hPipe == INVALID_HANDLE_VALUE)
	{
		TRACE("Unable to create a named pipe.\n");
		return;
	}
	
	m_pThread=AfxBeginThread(ServerReadProc, this); // 启动线程.
}

//---------------------------------------------------------------------------
UINT CMyPipe::ServerReadProc(LPVOID lpVoid) 
{
	DWORD dwNumBytesRead,dwNumBytesWrite;
	char toDisptxt[80];
	
	// 允许客户连接命名管道,如果不成功，终止.
	TRACE("Waiting for connection... \n");
	
	CMyPipe *Parent= (CMyPipe*)lpVoid;
	
	if(!ConnectNamedPipe(Parent->m_hPipe, (LPOVERLAPPED) NULL))
	{
		TRACE("Unable to connect a named pipe. Error: %d\n",GetLastError());
		CloseHandle(Parent->m_hPipe);
		return 1;
	}
	
	// 反复检查消息直到程序终止.
	while(1)
	{
		// 读取消息并检查读取数据是否成功. 
		if (!ReadFile(Parent->m_hPipe, toDisptxt,sizeof(toDisptxt),
			&dwNumBytesRead, (LPOVERLAPPED) NULL))
		{
			TRACE("Unable to read from named pipe. Error: %d\n" ,GetLastError());
			CloseHandle(Parent->m_hPipe);
			return 1;
		}
		else
		{
			// 保存接收的字符串.
			Parent->m_strRequest=toDisptxt;
			strcpy(toDisptxt,Parent->m_strReply);
	
			// 写回一个字符串.
			WriteFile(Parent->m_hPipe, toDisptxt,sizeof(toDisptxt),
			    	&dwNumBytesWrite, (LPOVERLAPPED) NULL);
		}
	}

	return 0;
}