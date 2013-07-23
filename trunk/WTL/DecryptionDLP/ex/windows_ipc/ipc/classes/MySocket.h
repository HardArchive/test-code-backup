// MySocket.h: ∂®“ÂCMySocket¿‡.
//
#ifndef _MYSOCKET_H__INCLUDED
#define _MYSOCKET_H__INCLUDED

#include "DealSocket.h"

class CMySocket  
{
public:
	CMySocket();
	virtual ~CMySocket();

public:
   	static UINT ServerReadProc(LPVOID lpVoid); 
	void SocketServer(int nPort,LPCSTR szReply);
	void SocketClient(LPCSTR szHost, int nPort,LPCSTR szRequest);
    CString GetReply() { return m_strReply;}
    CString GetRequest() { return m_strRequest;}

private:
	CString m_strReply;
	CString m_strRequest;
	CWinThread *m_pThread;
	int m_nPort;
};

#endif