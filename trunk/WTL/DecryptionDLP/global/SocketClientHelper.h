#pragma once
#include "TraceUtil.h"
//#include <string>

#include "Socket.h"


class CSocketClientHelper
{
public:
	CSocketClientHelper()
	{
		m_bConnectFlag = false;
	}
	~CSocketClientHelper()
	{
		Close();
	}

public:
	bool Open(UINT uiPort = 6000)
	{
		
		if (1 != m_clsSocket.Connect(_T("127.0.0.1"), uiPort))
		{
			::MessageBox(NULL, _T("���ӷ�����ʧ��"), _T("��Ϣ��ʾ"), MB_OK);
			return false;
		}
		m_bConnectFlag = true;
		return true;
	}


	DWORD Recv(PBYTE pbRecvBuf, UINT uiBufLen)
	{
		if (!m_bConnectFlag)
		{
			Open();
		}
		return m_clsSocket.Recv(m_clsSocket.GetSocket(), pbRecvBuf, uiBufLen);
	}

	DWORD Send(PBYTE pbRecvBuf, UINT uiBufLen)
	{
		if (!m_bConnectFlag)
		{
			Open();
		}
		return m_clsSocket.Send(m_clsSocket.GetSocket(), pbRecvBuf, uiBufLen);
	}

	void Close()
	{
		m_clsSocket.Close();
		m_bConnectFlag = false;
	}

	bool CheckConnect()
	{
		if (!m_bConnectFlag)
		{
			return Open();
		}
		return false;
	}

private:
	bool m_bConnectFlag;
	RG::CSocket m_clsSocket;
};