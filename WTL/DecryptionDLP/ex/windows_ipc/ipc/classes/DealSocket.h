// DealSocket.h: 定义CDealSocket 类.
//
#ifndef _DEALSOCKET_H__INCLUDED
#define _DEALSOCKET_H__INCLUDED
#include <afxsock.h>

#define MAX_RECV_LEN           100   // 每次接收最大字符串长度.
#define MAX_PENDING_CONNECTS   4     // 等待队列的长度.

class CDealSocket  
{
public:
	CDealSocket();
	virtual ~CDealSocket();

public:
	SOCKET GetConnect(CString host ,int port);
	SOCKET Listening(int port);
	CString GetResponse(SOCKET hSock);
	CString GetLocalAddr();
};

extern CDealSocket dealsocket;

#endif
