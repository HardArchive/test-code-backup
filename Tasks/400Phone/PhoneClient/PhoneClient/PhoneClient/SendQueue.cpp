#include "stdafx.h"
#include "SendQueue.h"
#include <winsock2.h>
#include "FileSearch.h"
void CSendQueue::AddFileInfo(FileInfo &fiInfo)
{
	::EnterCriticalSection(&m_cs);
	m_List.push(fiInfo);
	::SetEvent(m_hEvent);
	::LeaveCriticalSection(&m_cs);
}
int CSendQueue::DoWork()
{
	while(!this->m_bQuit)
	{
		if(WAIT_OBJECT_0==::WaitForSingleObject(m_hEvent,400))
		{
			::EnterCriticalSection(&m_cs);
			while(!m_List.empty())
			{
				FileInfo fiInfo=m_List.front();
				m_List.pop();
				
				//能过TCP转发给服务器
				if(!SendFileInfo(fiInfo))
				{
					if(fiInfo.ErrTimes <10)
					{
						fiInfo.ErrTimes++;
						m_List.push(fiInfo);
					}
					else
					{
						if(!CTool::SaveResult(fiInfo.iNum, "4000"))
							printf("create GOTOBOX file error!\n");
					}
					::LeaveCriticalSection(&m_cs);
					::Sleep(1000);
					::EnterCriticalSection(&m_cs);
				}
				else
				{
					if(!CTool::SaveResult(fiInfo.iNum, "3000"))
						printf("create GOTOBOX file error!\n");
				}
			}
			::LeaveCriticalSection(&m_cs);
			::ResetEvent(m_hEvent);
			::Sleep(100);
		}
	}
	return 0;
}
BOOL CSendQueue::SendFileInfo(FileInfo &fiInfo)
{
	printf("Get Pwd Phone:%s  Pwd:%s PwdLen:%d ErrTimes:%d\n",fiInfo.PhoneNum,fiInfo.Pwd,strlen(fiInfo.Pwd), fiInfo.ErrTimes);
	SOCKET fSocket=socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET==fSocket)
	{
		printf("Create socekt error Num:%d\r\n", WSAGetLastError());
		return FALSE;
	}

	struct sockaddr_in serverAddr;
 	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(8880);
	//serverAddr.sin_addr.s_addr=::inet_addr("192.168.30.86");
	serverAddr.sin_addr.s_addr=::inet_addr(m_szServerIP);
	if(SOCKET_ERROR==::connect(fSocket,(sockaddr *)&serverAddr,sizeof(sockaddr_in)))
	{
		printf("Connect server error Num:%d\r\n", WSAGetLastError());
		closesocket(fSocket);
		return FALSE;
	}
	int iSendLen=sizeof(FileInfo);
	while(iSendLen>0)
	{
		int iRet=::send(fSocket,(char *)&fiInfo,iSendLen,0);
		if(SOCKET_ERROR==iRet)
		{
			closesocket(fSocket);
			printf("Send data error Num:%d\r\n", WSAGetLastError());
			return FALSE;
		}
		else
		{
			iSendLen -=iRet;
		}
	}
	char cRet=0;
	if(SOCKET_ERROR==::recv(fSocket,&cRet,sizeof(char),0))
	{
		closesocket(fSocket);
		printf("Recv data error Num:%d\r\n", WSAGetLastError());
		return FALSE;
	}

	
	return cRet >0;
}