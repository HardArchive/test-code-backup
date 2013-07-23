// MyPipe.h: ∂®“ÂCMyPipe¿‡.
//
#ifndef _MYPIPE_H__INCLUDED
#define _MYPIPE_H__INCLUDED

class CMyPipe  
{
public:
	CMyPipe();
	virtual ~CMyPipe();

public:
	static UINT ServerReadProc(LPVOID lpVoid); 
	void ServerCreateNamedPipe(CString strReply); 
	void ClientSend(CString strRequest);
	void ClientCreateFile();

	CString GetReply()   { return m_strReply;}
    CString GetRequest() { return m_strRequest;}

private:
	CWinThread *m_pThread;

	CString m_strReply;
	CString m_strRequest;
	HANDLE m_hPipe;
};

#endif