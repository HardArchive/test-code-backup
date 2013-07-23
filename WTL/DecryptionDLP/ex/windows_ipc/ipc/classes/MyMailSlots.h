// MyMailSlots.h: ∂®“ÂCMyMailSlots¿‡.
//
#ifndef _MYMAILSLOTS_H__INCLUDED
#define _MYMAILSLOTS_H__INCLUDED

class CMyMailSlots  
{
public:
	CMyMailSlots();
	virtual ~CMyMailSlots();

public:
	static UINT ServerReadProc(LPVOID lpVoid); 
	void ServerCreateMailslot();
	void ClientCreateFile(); 
	void ClientSend(CString strRequest);

//    CString GetReply() { return m_strReply;}
    CString GetRequest() { return m_strRequest;}

private:
//	CString m_strReply;
	CString m_strRequest;
	HANDLE m_hSMS_Slot;
	CWinThread *m_pThread;
};

#endif
