#include "stdafx.h"
#include "ClientSocket.h"

//char   *CClientSocket::m_pStrData = NULL; 

CClientSocket::CClientSocket(void)
{
	m_hClientSocket         = NULL;
	m_bWinsockLibrary       = false;
	m_bQuieRecvThread       = false;
	m_hRecvThread		    = NULL;
	m_hOverRecvThreadEvent  = NULL;

	memset(m_pStrData,0,sizeof(m_pStrData));// = new char [RECV_DATA];
	GetWinsockLibrary();
}

CClientSocket::~CClientSocket(void)
{
	CloseResources();
}

void CClientSocket::CloseResources()
{
	if(m_bWinsockLibrary)
	{
		WSACleanup();
	}

	if(m_hClientSocket)
	{
		closesocket(m_hClientSocket);
		m_hClientSocket = NULL;
	}

	if(m_hOverRecvThreadEvent)
	{
		SetEvent(m_hOverRecvThreadEvent);
		Sleep(100);
		CloseHandle(m_hOverRecvThreadEvent);
		m_hOverRecvThreadEvent = NULL;
	}

	if(m_hRecvThread)
	{
		if(!m_bQuieRecvThread)
		{
			TerminateThread(m_hRecvThread,0);
		}
		CloseHandle(m_hRecvThread);
	}
	//cout<<"服务已经推出!"<<endl;
}

bool CClientSocket::GetWinsockLibrary()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );

	//创建SOCKET版本库
	err = WSAStartup( wVersionRequested, &wsaData );          
	if ( err != 0 ) 
	{
		//cout<<"创建SOCKET版本库失败!"<<endl;
		return false;
	}

	//判断SOCKET版本库型号
	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 )    
	{
		//cout<<"创建SOCKET版本库失败!"<<endl;
		WSACleanup();													//关闭SOCKET库
		return false; 
	}
	return (m_bWinsockLibrary = TRUE);
}

bool CClientSocket::LinkService(char *pStrIp /*= "127.0.0.1"*/,int nPort /*= 8888*/)
{
	if(!m_bWinsockLibrary)
	{
		//cout<<"开启网络版本库失败!"<<endl;
	}

	m_hClientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	if(INVALID_SOCKET == m_hClientSocket)
	{
		//cout<<"创建SOCKET失败!"<<endl;
	}

	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port  = htons(nPort);
	sa.sin_addr.S_un.S_addr = inet_addr(pStrIp);

	if(SOCKET_ERROR == connect(m_hClientSocket,(LPSOCKADDR)(&sa),sizeof(sockaddr_in)))
	{
		int n = GetLastError();
		//cout<<"创建网络连接失败!"<<"   "<<n<<endl;
		return false;
	}

	m_hOverRecvThreadEvent  = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(NULL == m_hOverRecvThreadEvent)
	{
		//cout<<"创建结束线程事件失败!"<<endl;
		return false;
	}
	
	m_hRecvThread = CreateThread(NULL,0,RecvThread,this,0,0);
	if(NULL == m_hRecvThread)
	{
		//cout<<"创建接收线程失败!"<<endl;
		return false;
	}

	Sleep(100);
	CloseHandle(m_hRecvThread);
	m_hRecvThread = NULL;
	return true;
}

bool CClientSocket::SendData(char *pData,int nSendDataLen)
{
	if(SOCKET_ERROR == send(m_hClientSocket,pData,nSendDataLen,0))
	{
		//cout<<"发送信息失败!"<<endl;
		return false;
	}
	return true;
}

unsigned long WINAPI CClientSocket::RecvThread(void *pParam)
{
	CClientSocket *pThis = (CClientSocket *)(pParam);

	while(1)
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(pThis->m_hOverRecvThreadEvent,0))
		{
			return 0;
		}

		if( SOCKET_ERROR != recv(pThis->m_hClientSocket,pThis->m_pStrData,RECV_DATA,0))
		{
			pThis->RecvData(pThis->m_pStrData);
		}
		else
		{
			if(WSAECONNRESET == GetLastError())                         //Xt:服务器断开连接则推出线程
				break;
		}

		if(pThis->m_pStrData[0] != '\0')
		{
			memset(pThis->m_pStrData,0,RECV_DATA);
		}
	}
	pThis->m_bQuieRecvThread = TRUE;
	return 1;
}

// bool CClientSocket::SendPacket(char *pPack,char cType,char *pSendData,int &nSendDataLen)
// {
// 	if(cType == '\0' || NULL == pPack || NULL == pSendData)
// 	{
// 		return false;
// 	}
// 	memset(pPack,0,100);
// 	int nDataHeadLen = sizeof(DataHead);                                //Xt: 包头长度                    
// 	PDataHead pDH = (PDataHead)pPack;                                   
// 	pDH->cDataType = cType;
// 	pDH->nDataLen  = nDataHeadLen+strlen(pSendData)+1;
// 	memcpy(pPack+nDataHeadLen,pSendData,strlen(pSendData)+1);
// 	memcpy(pPack+pDH->nDataLen,m_strPackEnd,m_nPackLen);	            //Xt: 末尾结束符
// 	nSendDataLen = pDH->nDataLen + m_nPackLen;
// 	return true;
// }

// void  CClientSocket::ResolveRecvData(char *pPack)
// {
// 	int nDataHeadLen = sizeof(DataHead);
// 	DataHead *pDH    = (PDataHead)pPack;
//     char *pData      = pPack+nDataHeadLen;
// 	int  nDataLen    = (pDH->nDataLen)-nDataHeadLen;
// 	//cout <<"接收到得信息为:"<<pData<<endl;
// 
// 	if(0 == memcmp(pData+nDataLen,m_strPackEnd,m_nPackLen))
// 	{ 
// 		switch(pDH->cDataType)
// 		{
// 		case ('O'):
// 			{
// 				//cout<<"收到类型: O"<<endl;
// 				SendPacket(pPack,'P',"YES",nDataLen);
// 				SendData(pPack,nDataLen);
// 				break;
// 			}
// 		case ('P'):
// 			{
// 				//cout<<"收到类型: P"<<endl;
// 				SendPacket(pPack,'O',"KKKKK",nDataLen);
// 				SendData(pPack,nDataLen);
// 				break;
// 			}
// 		}
//  	}
// 	else
// 	{
// 		//cout<<"接收错误从新发送请求"<<endl;   
// 		SendPacket(pPack,'R',"重发",nDataLen);
// 		if(!SendData(pPack,nDataLen))
// 		{
// 			//cout<<"重新发送请求失败!"<<endl; 
// 		}
// 	}
// }	

