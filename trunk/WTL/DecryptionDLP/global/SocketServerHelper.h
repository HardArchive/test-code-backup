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
			printf(" ����˿ڰ�ʧ��\r\n"/*, GetLastError*/);
			::MessageBox(NULL, _T("����˿ڰ�ʧ�ܣ�����"), _T("��ʾ"), MB_OK);
			return false;
		}

		if (!m_clsSocket.Listen(5))
		{
			printf(" ����˽������ģʽʧ��\r\n"/*, GetLastError*/);
			TRACE(_T("Server - ����˽������ģʽʧ��\r\n"));
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

		TRACE(_T("Server - ����˽������ӳɹ�!!!\r\n"));
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