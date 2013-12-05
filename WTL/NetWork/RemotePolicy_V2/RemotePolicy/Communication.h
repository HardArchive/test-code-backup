#pragma once
#include "afx.h"
#include "afxsock.h"

#ifndef _COMMUNICATION_H_
#define _COMMUNICATION_H_

namespace Communicate
{
	class CCommunication :	public CObject								//Socket Tcp协议封装类
	{
	public:
		CCommunication(SOCKET WorkSocket,DWORD nTimeOut ,PCHAR pbBuffer, DWORD nBufferSize);
		~CCommunication();
		bool Connect(CString sServerIP,DWORD nPort);	// 连接服务器
		bool PendingReceive(DWORD dwlen);								// 数据接收实现函数
		bool PendingSend(DWORD dwlen);									// 数据发送实现函数
		bool CloseSocket();												// 关闭套接字
		bool IsConnected();

		bool GetBuffer(PCHAR& pbBuffer,DWORD& nBufferSize);
		bool SetBuffer(PCHAR pbBuffer,DWORD nBufferSize);

	protected:
		DWORD		m_nBufferSize;		//缓冲长度
		PCHAR		m_pbBuffer;			//缓冲区
		DWORD		m_nTimeOut;
		SOCKET		m_WorkSocket;

	};
}
using namespace Communicate;
#endif // _COMMUNICATION_H_