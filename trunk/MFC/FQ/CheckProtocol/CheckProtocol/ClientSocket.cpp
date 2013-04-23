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
	//cout<<"�����Ѿ��Ƴ�!"<<endl;
}

bool CClientSocket::GetWinsockLibrary()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD( 2, 2 );

	//����SOCKET�汾��
	err = WSAStartup( wVersionRequested, &wsaData );          
	if ( err != 0 ) 
	{
		//cout<<"����SOCKET�汾��ʧ��!"<<endl;
		return false;
	}

	//�ж�SOCKET�汾���ͺ�
	if ( LOBYTE( wsaData.wVersion ) != 2 || HIBYTE( wsaData.wVersion ) != 2 )    
	{
		//cout<<"����SOCKET�汾��ʧ��!"<<endl;
		WSACleanup();													//�ر�SOCKET��
		return false; 
	}
	return (m_bWinsockLibrary = TRUE);
}

bool CClientSocket::LinkService(char *pStrIp /*= "127.0.0.1"*/,int nPort /*= 8888*/)
{
	if(!m_bWinsockLibrary)
	{
		//cout<<"��������汾��ʧ��!"<<endl;
	}

	m_hClientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_IP);
	if(INVALID_SOCKET == m_hClientSocket)
	{
		//cout<<"����SOCKETʧ��!"<<endl;
	}

	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port  = htons(nPort);
	sa.sin_addr.S_un.S_addr = inet_addr(pStrIp);

	if(SOCKET_ERROR == connect(m_hClientSocket,(LPSOCKADDR)(&sa),sizeof(sockaddr_in)))
	{
		int n = GetLastError();
		//cout<<"������������ʧ��!"<<"   "<<n<<endl;
		return false;
	}

	m_hOverRecvThreadEvent  = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(NULL == m_hOverRecvThreadEvent)
	{
		//cout<<"���������߳��¼�ʧ��!"<<endl;
		return false;
	}
	
	m_hRecvThread = CreateThread(NULL,0,RecvThread,this,0,0);
	if(NULL == m_hRecvThread)
	{
		//cout<<"���������߳�ʧ��!"<<endl;
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
		//cout<<"������Ϣʧ��!"<<endl;
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
			if(WSAECONNRESET == GetLastError())                         //Xt:�������Ͽ��������Ƴ��߳�
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
// 	int nDataHeadLen = sizeof(DataHead);                                //Xt: ��ͷ����                    
// 	PDataHead pDH = (PDataHead)pPack;                                   
// 	pDH->cDataType = cType;
// 	pDH->nDataLen  = nDataHeadLen+strlen(pSendData)+1;
// 	memcpy(pPack+nDataHeadLen,pSendData,strlen(pSendData)+1);
// 	memcpy(pPack+pDH->nDataLen,m_strPackEnd,m_nPackLen);	            //Xt: ĩβ������
// 	nSendDataLen = pDH->nDataLen + m_nPackLen;
// 	return true;
// }

// void  CClientSocket::ResolveRecvData(char *pPack)
// {
// 	int nDataHeadLen = sizeof(DataHead);
// 	DataHead *pDH    = (PDataHead)pPack;
//     char *pData      = pPack+nDataHeadLen;
// 	int  nDataLen    = (pDH->nDataLen)-nDataHeadLen;
// 	//cout <<"���յ�����ϢΪ:"<<pData<<endl;
// 
// 	if(0 == memcmp(pData+nDataLen,m_strPackEnd,m_nPackLen))
// 	{ 
// 		switch(pDH->cDataType)
// 		{
// 		case ('O'):
// 			{
// 				//cout<<"�յ�����: O"<<endl;
// 				SendPacket(pPack,'P',"YES",nDataLen);
// 				SendData(pPack,nDataLen);
// 				break;
// 			}
// 		case ('P'):
// 			{
// 				//cout<<"�յ�����: P"<<endl;
// 				SendPacket(pPack,'O',"KKKKK",nDataLen);
// 				SendData(pPack,nDataLen);
// 				break;
// 			}
// 		}
//  	}
// 	else
// 	{
// 		//cout<<"���մ�����·�������"<<endl;   
// 		SendPacket(pPack,'R',"�ط�",nDataLen);
// 		if(!SendData(pPack,nDataLen))
// 		{
// 			//cout<<"���·�������ʧ��!"<<endl; 
// 		}
// 	}
// }	

