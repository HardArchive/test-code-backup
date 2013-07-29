#pragma once

#include "Socket.h"
#include "TraceUtil.h"
using namespace RG;

class CSocketServerHelper
{
public:
	CSocketServerHelper():m_clsSocket(false)
	{
		m_SocketClient = NULL;
	}
	~CSocketServerHelper()
	{
		Close();
	}

public:
	bool Open(UINT uiPort = 6000)
	{
		if (!m_clsSocket.Bind(uiPort))
		{
			printf(" 服务端口绑定失败\r\n"/*, GetLastError*/);
			::MessageBox(NULL, _T("服务端口绑定失败！！！"), _T("提示"), MB_OK);
			return false;
		}

		if (!m_clsSocket.Listen(5))
		{
			printf(" 服务端进入监听模式失败\r\n"/*, GetLastError*/);
			TRACE(_T("Server - 服务端进入监听模式失败\r\n"));
			return false;
		}
		return true;
	}

	bool Accept()
	{
		m_SocketClient = m_clsSocket.Accept();
		if(INVALID_SOCKET == m_SocketClient)
		{
			printf("Failed accept()\r\n");
			TRACE(_T("Server - Failed accept()\r\n"));
			return false;
		}

		TRACE(_T("Server - 服务端接收连接成功!!!\r\n"));
		return true;
	}

	DWORD Recv(PBYTE pbRecvBuf, UINT uiBufLen)
	{
		return m_clsSocket.Recv(m_SocketClient, pbRecvBuf, uiBufLen);
	}

	DWORD Send(PBYTE pbRecvBuf, UINT uiBufLen)
	{
		return m_clsSocket.Send(m_SocketClient, pbRecvBuf, uiBufLen);
	}

	void Close()
	{
		m_clsSocket.Close();
		m_SocketClient = NULL;
	}

	bool CheckClientStatus()
	{
		return ((NULL != m_SocketClient)?true:false);
	}

private:
	SOCKET m_SocketClient;
	RG::CSocket m_clsSocket;
};