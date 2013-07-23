// DealSocket.h: ����CDealSocket ��.
//
#ifndef _DEALSOCKET_H__INCLUDED
#define _DEALSOCKET_H__INCLUDED
#include <afxsock.h>

#define MAX_RECV_LEN           100   // ÿ�ν�������ַ�������.
#define MAX_PENDING_CONNECTS   4     // �ȴ����еĳ���.

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
