#include "stdafx.h"
#include "RecvData.h"
#include "DataHelper.h"


BOOL CRecvData::Init()
{
	if(!CDataHelper::InitConnection())
	{
		return FALSE;
	}
	WSAData wsdata;
	if(WSAStartup(MAKEWORD(2,2),&wsdata))
	{
		printf("WSAStartup failure! \n");
		return FALSE;
	}

	//create socket
	m_sListening=socket(AF_INET,SOCK_STREAM,0);
	if(INVALID_SOCKET==m_sListening)
	{
		printf("socket error!\n");
		return FALSE;
	}

	//获取端口号
	char szTCPPort[16]={0};
	CDataHelper::GetRegConfig(TEXT("TcpPort"),szTCPPort);

	if(strlen(szTCPPort)==0)
	{
		::OutputDebugString(_T("400Phone服务 从注册表获取端口号参数失败!\r\n"));
		return FALSE;
	}
	int port=atoi(szTCPPort);

	struct sockaddr_in serverAddr;
 	serverAddr.sin_family=AF_INET;
	serverAddr.sin_port=htons(port);
	serverAddr.sin_addr.s_addr=0;

	//bind socket
	if(SOCKET_ERROR==::bind(m_sListening,(sockaddr *)&serverAddr,sizeof(sockaddr_in)))
	{
		printf("bind socket error!\n");
		closesocket(m_sListening);
		m_sListening=INVALID_SOCKET;
		return FALSE;
	}
	//listening socket
	if(SOCKET_ERROR==::listen(m_sListening,10))
	{
		printf("listen socket error!\n");
		closesocket(m_sListening);
		m_sListening=INVALID_SOCKET;
		return FALSE;
	}
	return TRUE;
}

void CRecvData::UnInit()
{
	if(m_sListening!=INVALID_SOCKET)
	{
		::closesocket(m_sListening);
	}
	WSACleanup();
	CDataHelper::ExitConnection();
}

void CRecvData::Start()
{
	fd_set fdSet;
	FD_ZERO(&fdSet);
	FD_SET(m_sListening,&fdSet);
	timeval tv={2,0};

	while(b_IsContinue)
	{
		fd_set fdCurSet=fdSet;
		if(select(0,&fdCurSet,NULL,NULL,&tv)<=0)
		{
			continue;
		}
		for(int i=0;i<fdCurSet.fd_count;i++)
		{
			SOCKET sCurSocket=fdCurSet.fd_array[i];
			if(sCurSocket==m_sListening)
			{
				SOCKET sClient=::accept(sCurSocket,NULL,NULL);
				if(INVALID_SOCKET !=sClient)
				{
					FD_SET(sClient,&fdSet);
				}
			}
			else
			{
				//receive data from client socket
				FileInfo fi;
				if(recv(sCurSocket,(char *)&fi,sizeof(FileInfo),0)<=0)
				{
					printf("socket quit\n");
					FD_CLR(sCurSocket,&fdSet);
					::closesocket(sCurSocket);
					continue;
				}

				printf("Get 400 Phone Pwd Phone:%s  Pwd:%s Pwd Len:%d \n",fi.PhoneNum, fi.Pwd, strlen(fi.Pwd));
				char cRet = CDataHelper::AddPhoneAndPwd(fi.PhoneNum,fi.Pwd)?1:0;

				::send(sCurSocket,&cRet,sizeof(char),0);

				FD_CLR(sCurSocket,&fdSet);
				::closesocket(sCurSocket);
			}
		}		
	}
}