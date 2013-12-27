#include "StdAfx.h"
#include "Communication.h"

namespace Communicate
{
	CCommunication::CCommunication(SOCKET WorkSocket, DWORD nTimeOut, PCHAR pbBuffer, DWORD nBufferSize)
	{
		m_nBufferSize = nBufferSize;//�������sizeof(MSGHEAD)
		m_pbBuffer = pbBuffer;
		m_nTimeOut = nTimeOut;
		m_WorkSocket = WorkSocket;
	}

	CCommunication::CCommunication(DWORD nTimeOut = 60*5, BYTE minorVer = 2, BYTE majorVer = 2)
	{
		// ��ʼ��WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(minorVer, majorVer);
		if(::WSAStartup(sockVersion, &wsaData) != 0)
		{
			exit(0);
		}
		m_nBufferSize = 0;//�������sizeof(MSGHEAD)
		m_pbBuffer = NULL;
		m_nTimeOut = nTimeOut;
		m_WorkSocket = NULL;
	}

	CCommunication::~CCommunication()
	{
		CloseSocket();
	}

	bool CCommunication::GetBuffer(PCHAR& pbBuffer,DWORD& nBufferSize)
	{
		if (m_pbBuffer)
		{
			pbBuffer = m_pbBuffer;
			nBufferSize = m_nBufferSize;
			return true;
		}
		return false;
	}

	bool CCommunication::SetBuffer(PCHAR pbBuffer,DWORD nBufferSize)
	{
		//if (m_pbBuffer)
		{
			m_pbBuffer = pbBuffer;
			m_nBufferSize = nBufferSize;
			return true;
		}
		return false;
	}

	bool CCommunication::IsConnected()
	{
		HANDLE hEvent = WSACreateEvent();
		bool bRet = true;
		try
		{
			if (WSA_INVALID_EVENT == hEvent)
				throw("");

			if (SOCKET_ERROR == WSAEventSelect(m_WorkSocket, hEvent, FD_CLOSE)) 
				throw("");

			if (m_WorkSocket && hEvent) 
			{
				if (WAIT_OBJECT_0 == WSAWaitForMultipleEvents(1, &hEvent, false, 80, false)) 
				{
					WSAResetEvent(hEvent);
					CloseSocket();
					throw("");
				}
			}
			else 
				throw("");
		}
		catch (TCHAR* tszErr)
		{
			::OutputDebugString(tszErr);
			bRet = false;
		}
		if (hEvent)
			WSACloseEvent(hEvent);
		return bRet;
	}

	// ��������
	bool CCommunication::PendingReceive(DWORD dwlen)
	{
		ZeroMemory(m_pbBuffer, m_nBufferSize);

		WSABUF			wsabuf;
		WSAOVERLAPPED	over;
		DWORD			dwRecv = 0;
		DWORD           dwTotalRecv = 0;
		DWORD			dwFlags;
		DWORD           dwRet;
		BOOL			fPending;
		int				nRet;

		ZeroMemory(&over, sizeof(WSAOVERLAPPED));
		wsabuf.buf  = m_pbBuffer;
		wsabuf.len  = dwlen;
		over.hEvent = WSACreateEvent();
		if(over.hEvent == WSA_INVALID_EVENT)
		{
			CloseSocket();
			return false;
		}

		while(1)
		{
			dwFlags = 0;
			fPending = FALSE;
			// ��������
			nRet = WSARecv(m_WorkSocket, &wsabuf, 1, &dwRecv,	&dwFlags, &over, NULL);
  			if(nRet != 0)
			{
				if( WSAGetLastError() != WSA_IO_PENDING)
				{					
					WSACloseEvent(over.hEvent);
					CloseSocket();
					return false;
				}
				else
				{
					fPending = TRUE;
				}
			}
			else
			{
				fPending = TRUE;
			}

			// ����I/Oδ����
			if(fPending)
			{
				// �ȴ�����I/O�������
				dwRet = WSAWaitForMultipleEvents(1, &(over.hEvent), FALSE, m_nTimeOut*1000, FALSE);
				if(dwRet==WSA_WAIT_FAILED || dwRet==WAIT_TIMEOUT)
				{
					WSACloseEvent(over.hEvent);
					CloseSocket();
					return false;
				}

				WSAResetEvent(over.hEvent);

				// �ж��ص������Ƿ�ɹ�
				if(!WSAGetOverlappedResult(m_WorkSocket, &over, &dwRecv, FALSE, &dwFlags))
				{
					WSACloseEvent(over.hEvent);
					CloseSocket();
					return false;
				}
				else
				{
					if(dwRecv == 0)
					{
						WSACloseEvent(over.hEvent);
						CloseSocket();
						return false;
					}

					// ����������ֽ���
					dwTotalRecv += dwRecv;
					wsabuf.len -= dwRecv;

					if(wsabuf.len <= 0)
					{
						break;
					}
					else
					{
						wsabuf.buf += dwRecv;
					}
				}
			}
		}//while(1) ����δ����

		if(over.hEvent)
		{
			WSACloseEvent(over.hEvent);
		}
		return true;
	}

	// ��������
	bool CCommunication::PendingSend(DWORD dwlen)
	{
		WSABUF			wsabuf;
		WSAOVERLAPPED	over;
		DWORD			dwSend;
		DWORD           dwTotalSend = 0;
		DWORD			dwFlags;
		DWORD			dwRet;
		BOOL			fPending;

		ZeroMemory(&over, sizeof(WSAOVERLAPPED));
		wsabuf.buf  = m_pbBuffer;
		wsabuf.len  = dwlen;
		over.hEvent = WSACreateEvent();
		if (over.hEvent == WSA_INVALID_EVENT)
		{
#ifdef _ZZYDEBUG
					GC_TRACE("Error5");
#endif
			CloseSocket();
			return false;
		}

		while(1)
		{
			fPending = FALSE;
			// ��������
			int nRet = WSASend(m_WorkSocket, &wsabuf, 1, &dwSend, 0, &over, NULL);
			if (nRet != 0)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
#ifdef _ZZYDEBUG
					GC_TRACE("Error4:%d",WSAGetLastError());
#endif
					WSACloseEvent(over.hEvent);
					CloseSocket();
					return false;
				}
				else
				{
					fPending = TRUE;
				}
			}
			else
			{
				fPending = TRUE;
			}

			// ����I/O����δ����
			if (fPending)
			{
				// �ȴ�����I/O��������
				dwRet = WSAWaitForMultipleEvents(1, &(over.hEvent), FALSE, m_nTimeOut*1000, FALSE);
				if (dwRet==WSA_WAIT_FAILED || dwRet==WAIT_TIMEOUT)
				{
#ifdef _ZZYDEBUG
					if (dwRet==WSA_WAIT_FAILED)
						GC_TRACE("Error1");
					else
						GC_TRACE("Error6");

#endif
					WSACloseEvent(over.hEvent);
					CloseSocket();
					return false;
				}

				WSAResetEvent(over.hEvent);

				// �ж��ص������Ƿ�ɹ�
				if (!WSAGetOverlappedResult(m_WorkSocket,	&over, &dwSend,	FALSE, &dwFlags))
				{
#ifdef _ZZYDEBUG
					GC_TRACE("Error2");
#endif
					WSACloseEvent(over.hEvent);
					CloseSocket();
					return false;
				}
				else
				{
					if (dwSend == 0)
					{
#ifdef _ZZYDEBUG
					GC_TRACE("Error3");
#endif
						WSACloseEvent(over.hEvent);
						CloseSocket();
						return false;
					}

					// ����ʣ�෢���ֽ���
					wsabuf.len -= dwSend;
					dwTotalSend += dwSend;

					if(wsabuf.len <= 0)
					{
						break;
					}
					else
					{
						wsabuf.buf += dwSend;
					}
				}
			}
		}//while (1) ����δ����

		// �������
		if(over.hEvent)
		{
			WSACloseEvent(over.hEvent);
		}
		return true;
	}

	// ���ݹر����ӣ��ɹ�����TRUE�����򷵻�FALSE
	bool CCommunication::CloseSocket()
	{
		if(m_WorkSocket != NULL)
		{
			shutdown(m_WorkSocket, SD_BOTH);
			closesocket(m_WorkSocket);
		}
		m_WorkSocket = NULL;
/*		if (!m_WorkSocket)
			return false;

		WSAEVENT evtSent;
		evtSent = WSACreateEvent();
		if(evtSent == WSA_INVALID_EVENT)
		{
			return false;
		}

		if(WSAEventSelect(m_WorkSocket,evtSent,FD_CLOSE) == SOCKET_ERROR)
		{
			closesocket(m_WorkSocket);	
			return false;
		}

		shutdown(m_WorkSocket, SD_SEND);

		// �ȴ��ͻ��˹ر�SOCKET
		if(WaitForSingleObject(evtSent, m_nTimeOut*1000) != WAIT_OBJECT_0)
		{
			 //��ʱ��ǿ�йر�SOCKET
			closesocket(m_WorkSocket);
			WSACloseEvent(evtSent);
			return false;
		}

		shutdown(m_WorkSocket, SD_BOTH);
		closesocket(m_WorkSocket);

		WSACloseEvent(evtSent);
*/
		return true;
	}

	//bool CCommunication::Connect(string sServerIP,DWORD nPort)
	//{
	//	CAsyncSocket asConnect;

	//	if(!AfxSocketInit())
	//		return false;

	//	if(!asConnect.Create(0, SOCK_STREAM, NULL))
	//		return false;

	//	DWORD dwFlag = 0;

	//	ioctlsocket(asConnect.m_hSocket, FIONBIO, &dwFlag);

	//	if(!asConnect.Connect(sServerIP, nPort))
	//		return false;

	//	dwFlag=1;
	//	ioctlsocket(asConnect.m_hSocket, FIONBIO, &dwFlag);

	//	m_WorkSocket = asConnect.Detach();
	//	return true;
	//}

	//���ӷ�����  ���ǿͻ����õĺ���  dwIPValueΪIPֵ uiPort�˿�
	//1 �ɹ� -1 ���ó��� -2 Socket ����ʧ�� -3 ���÷��ͳ�ʱʧ�� -4 ���ý��ճ�ʱʧ�� -5 ���ӳ�ʱ -6 ���ӳ���
	int CCommunication::Connect(DWORD dwIPValue, UINT uiPort)
	{
		FD_SET	    FS_ReadSocket;
		int         iLen = sizeof(int);
		DWORD       dwError = 0;
		DWORD       dwFlag = 1;          //dwFlag 1 ���������ģʽ Ϊ0��ֹ������ģʽ
		TIMEVAL	    timeout;             //��ʱ΢��
		SOCKADDR_IN	addr = {0};          //TCP  ���ݽṹ
		int iNetTimeout = 5*60;          //��ʱʱ��5��

		//�����׽��� AF_INET��ַ��    SOCK_STREAM(TCP)ʹ�õ�Э��  0�ض��ļ���Э����ص�Э��
		m_WorkSocket = socket(AF_INET,SOCK_STREAM, 0);
		if (m_WorkSocket == INVALID_SOCKET ) 
		{
			//TRACE(_T("Socket ����ʧ�ܣ�Exit!"));
			return -2;
		}
		//�����׽ӿ�   dwFlag 1 ���������ģʽ Ϊ0��ֹ������ģʽ
		::ioctlsocket(m_WorkSocket, FIONBIO, &dwFlag);

		//���÷��� ���ճ�ʱ
		if (::setsockopt(m_WorkSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&iNetTimeout,sizeof(int)) == SOCKET_ERROR)
			return -3;
		if (::setsockopt(m_WorkSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&iNetTimeout,sizeof(int)) == SOCKET_ERROR)
			return -4;


		addr.sin_family	= AF_INET;                               //ָ��Э���壬��socket�����ֻ����AF_INET 			
		addr.sin_addr.s_addr = dwIPValue;                //�洢IP��ַ
		addr.sin_port =	htons(uiPort);                           //�˿ںű���Ϳͻ������Ķ˿ں�һ��

		//����socket����
		if (SOCKET_ERROR == connect(m_WorkSocket, (sockaddr*)&addr, sizeof(addr)))
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				timeout.tv_sec = 15;        //���ӳ�ʱ5��
				timeout.tv_usec = 0;
				FD_ZERO(&FS_ReadSocket);            //��ʼ���׽���
				FD_SET(m_WorkSocket, &FS_ReadSocket);     //����stdout
				//���Բ���   �ȴ��ɶ����׽��ֽӿ�    �ȴ���д���׽��ֽӿ�   �ȴ���������׽ӿ�   ���ȴ�ʱ��
				dwFlag = select(0, NULL, &FS_ReadSocket, NULL, &timeout);  //ȷ��һ�������׽ӿڵ�״̬

				if(dwFlag > 0) 
				{ 
					getsockopt(m_WorkSocket, SOL_SOCKET, SO_ERROR, (char*)&dwError, &iLen); 
					if(dwError != 0) 
						return -6;
				} 
				if(!dwFlag) return -5;                   //���ӳ�ʱ
				if (dwFlag == SOCKET_ERROR) return -6;   //���ӳ��� 
			}
			else
			{
				return -6;  //�����쳣
			}
		}

		dwFlag = 0;
		::ioctlsocket(m_WorkSocket, FIONBIO, &dwFlag); 
		return 1;
	}

	//���ӷ�����  ���ǿͻ����õĺ���  IP��ַΪ�ַ���
	int CCommunication::Connect(char* pszIP, UINT uiPort)
	{
		return Connect(inet_addr(pszIP), uiPort);
	}
}

