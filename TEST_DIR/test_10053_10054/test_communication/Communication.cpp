#include "StdAfx.h"
#include "Communication.h"

namespace Communicate
{
	CCommunication::CCommunication(SOCKET WorkSocket, DWORD nTimeOut, PCHAR pbBuffer, DWORD nBufferSize)
	{
		m_nBufferSize = nBufferSize;//必须大于sizeof(MSGHEAD)
		m_pbBuffer = pbBuffer;
		m_nTimeOut = nTimeOut;
		m_WorkSocket = WorkSocket;
	}

	CCommunication::CCommunication(DWORD nTimeOut = 60*5, BYTE minorVer = 2, BYTE majorVer = 2)
	{
		// 初始化WS2_32.dll
		WSADATA wsaData;
		WORD sockVersion = MAKEWORD(minorVer, majorVer);
		if(::WSAStartup(sockVersion, &wsaData) != 0)
		{
			exit(0);
		}
		m_nBufferSize = 0;//必须大于sizeof(MSGHEAD)
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

	// 接收数据
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
			// 接收数据
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

			// 接收I/O未结束
			if(fPending)
			{
				// 等待接收I/O操作完成
				dwRet = WSAWaitForMultipleEvents(1, &(over.hEvent), FALSE, m_nTimeOut*1000, FALSE);
				if(dwRet==WSA_WAIT_FAILED || dwRet==WAIT_TIMEOUT)
				{
					WSACloseEvent(over.hEvent);
					CloseSocket();
					return false;
				}

				WSAResetEvent(over.hEvent);

				// 判断重叠操作是否成功
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

					// 计算接收总字节数
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
		}//while(1) 接收未结束

		if(over.hEvent)
		{
			WSACloseEvent(over.hEvent);
		}
		return true;
	}

	// 发送数据
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
			// 发送数据
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

			// 发送I/O操作未结束
			if (fPending)
			{
				// 等待发送I/O操作接束
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

				// 判断重叠操作是否成功
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

					// 计算剩余发送字节数
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
		}//while (1) 发送未结束

		// 发送完成
		if(over.hEvent)
		{
			WSACloseEvent(over.hEvent);
		}
		return true;
	}

	// 从容关闭连接，成功返回TRUE，否则返回FALSE
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

		// 等待客户端关闭SOCKET
		if(WaitForSingleObject(evtSent, m_nTimeOut*1000) != WAIT_OBJECT_0)
		{
			 //超时，强行关闭SOCKET
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

	//连接服务器  这是客户端用的函数  dwIPValue为IP值 uiPort端口
	//1 成功 -1 调用出错 -2 Socket 创建失败 -3 设置发送超时失败 -4 设置接收超时失败 -5 连接超时 -6 连接出错
	int CCommunication::Connect(DWORD dwIPValue, UINT uiPort)
	{
		FD_SET	    FS_ReadSocket;
		int         iLen = sizeof(int);
		DWORD       dwError = 0;
		DWORD       dwFlag = 1;          //dwFlag 1 允许非阻塞模式 为0禁止非阻塞模式
		TIMEVAL	    timeout;             //定时微妙
		SOCKADDR_IN	addr = {0};          //TCP  数据结构
		int iNetTimeout = 5*60;          //超时时间5秒

		//创建套接字 AF_INET地址族    SOCK_STREAM(TCP)使用的协议  0特定的家族协议相关的协议
		m_WorkSocket = socket(AF_INET,SOCK_STREAM, 0);
		if (m_WorkSocket == INVALID_SOCKET ) 
		{
			//TRACE(_T("Socket 创建失败，Exit!"));
			return -2;
		}
		//控制套接口   dwFlag 1 允许非阻塞模式 为0禁止非阻塞模式
		::ioctlsocket(m_WorkSocket, FIONBIO, &dwFlag);

		//设置发送 接收超时
		if (::setsockopt(m_WorkSocket,SOL_SOCKET,SO_SNDTIMEO,(char *)&iNetTimeout,sizeof(int)) == SOCKET_ERROR)
			return -3;
		if (::setsockopt(m_WorkSocket,SOL_SOCKET,SO_RCVTIMEO,(char *)&iNetTimeout,sizeof(int)) == SOCKET_ERROR)
			return -4;


		addr.sin_family	= AF_INET;                               //指代协议族，在socket编程中只能是AF_INET 			
		addr.sin_addr.s_addr = dwIPValue;                //存储IP地址
		addr.sin_port =	htons(uiPort);                           //端口号必须和客户发往的端口号一致

		//建立socket连接
		if (SOCKET_ERROR == connect(m_WorkSocket, (sockaddr*)&addr, sizeof(addr)))
		{
			if (WSAGetLastError() == WSAEWOULDBLOCK)
			{
				timeout.tv_sec = 15;        //连接超时5秒
				timeout.tv_usec = 0;
				FD_ZERO(&FS_ReadSocket);            //初始化套接字
				FD_SET(m_WorkSocket, &FS_ReadSocket);     //设置stdout
				//忽略参数   等待可读的套接字接口    等待可写的套接字接口   等待错误检查的套接口   最多等待时间
				dwFlag = select(0, NULL, &FS_ReadSocket, NULL, &timeout);  //确定一个或多个套接口的状态

				if(dwFlag > 0) 
				{ 
					getsockopt(m_WorkSocket, SOL_SOCKET, SO_ERROR, (char*)&dwError, &iLen); 
					if(dwError != 0) 
						return -6;
				} 
				if(!dwFlag) return -5;                   //连接超时
				if (dwFlag == SOCKET_ERROR) return -6;   //连接出错 
			}
			else
			{
				return -6;  //连接异常
			}
		}

		dwFlag = 0;
		::ioctlsocket(m_WorkSocket, FIONBIO, &dwFlag); 
		return 1;
	}

	//连接服务器  这是客户端用的函数  IP地址为字符串
	int CCommunication::Connect(char* pszIP, UINT uiPort)
	{
		return Connect(inet_addr(pszIP), uiPort);
	}
}

