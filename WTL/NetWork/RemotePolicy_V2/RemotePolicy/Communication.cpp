#include "StdAfx.h"
#include "Communication.h"

namespace Communicate
{
	CCommunication::CCommunication(SOCKET WorkSocket,DWORD nTimeOut,PCHAR pbBuffer,DWORD nBufferSize)
	{
		m_nBufferSize = nBufferSize;//必须大于sizeof(MSGHEAD)
		m_pbBuffer = pbBuffer;
		m_nTimeOut = nTimeOut;
		m_WorkSocket = WorkSocket;
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
		if (m_pbBuffer)
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
		catch (char* szErr)
		{
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

	bool CCommunication::Connect(CString sServerIP,DWORD nPort)
	{
		CAsyncSocket asConnect;

		if(!AfxSocketInit())
			return false;

		if(!asConnect.Create(0, SOCK_STREAM, NULL))
			return false;

		DWORD dwFlag = 0;

		ioctlsocket(asConnect.m_hSocket, FIONBIO, &dwFlag);

		if(!asConnect.Connect(sServerIP, nPort))
			return false;

		dwFlag=1;
		ioctlsocket(asConnect.m_hSocket, FIONBIO, &dwFlag);

		m_WorkSocket = asConnect.Detach();
		return true;
	}
}

