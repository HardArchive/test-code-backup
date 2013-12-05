#pragma once
#include "afx.h"
#include "afxsock.h"

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

namespace Communicate
{
	class CCommunication :	public CObject								//Socket TcpЭ���װ��
	{
	public:
		CCommunication(SOCKET WorkSocket,DWORD nTimeOut ,PCHAR pbBuffer, DWORD nBufferSize);
		~CCommunication();
		bool Connect(CString sServerIP,DWORD nPort);	// ���ӷ�����
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