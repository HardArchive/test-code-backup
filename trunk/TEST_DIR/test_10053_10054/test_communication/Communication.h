#pragma once
//#include "afx.h"
//#include "afxsock.h"

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_
	
//#include "initsock.h"
#include <string>
using namespace std;
#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib

namespace Communicate
{
	class CCommunication /*:	public CObject	*/							//Socket TcpЭ���װ��
	{
	public:
		CCommunication(SOCKET WorkSocket, DWORD nTimeOut, PCHAR pbBuffer, DWORD nBufferSize);
		CCommunication(DWORD nTimeOut, BYTE minorVer, BYTE majorVer);
		~CCommunication();
		//bool Connect(string sServerIP,DWORD nPort);	// ���ӷ�����
		int Connect(char* pszIP, UINT uiPort);
		int Connect(DWORD dwIPValue, UINT uiPort);
		bool PendingReceive(DWORD dwlen);								// ���ݽ���ʵ�ֺ���
		bool PendingSend(DWORD dwlen);									// ���ݷ���ʵ�ֺ���
		bool CloseSocket();												// �ر��׽���
		bool IsConnected();

		bool GetBuffer(PCHAR& pbBuffer,DWORD& nBufferSize);
		bool SetBuffer(PCHAR pbBuffer,DWORD nBufferSize);

	protected:
		DWORD		m_nBufferSize;		//���峤��
		PCHAR		m_pbBuffer;			//������
		DWORD		m_nTimeOut;
		SOCKET		m_WorkSocket;

	};
}
using namespace Communicate;
#endif // _COMMUNICATION_H_