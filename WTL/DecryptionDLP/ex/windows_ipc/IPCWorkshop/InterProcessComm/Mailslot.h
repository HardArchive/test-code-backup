//////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                            
// IPCWorkshop							                                      
//                                                                            
// Environment:     Visual C++ 6.0, Windows 2000                              
//                                                                            
// Notes:	Class CMailslot. Encapsultaed over Mailslot Win32 API. 
//			Any win32 application can store messages in a mailslot and the owner 
//			will retrieve messages that are stored there. Mailslot is a simple way 
//			for a process to broadcast messages to multiple processes using datagram. 
//			Hence there won't be any confirmation of the receipt. 
//			This is a mechanism for one way inter process communication.
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __MAILSLOT_H__
#define __MAILSLOT_H__

class CMailslot 
{
public:
	CMailslot();
	~CMailslot();

	// Attributes
	void SetSize(int);
	LPARAM GetLparam() const;
	HANDLE GetInputslot() const;
	UINT GetTransferMessage() const;

	// Operations
	bool Initialize(bool, LPCTSTR);
	void Listen(UINT, LPARAM);
	bool Write(char*);
	void Read(char*&);		

private:
	int m_nSize;
	bool m_nIsServer;
	HANDLE m_hInputslot;
	HANDLE m_hOutputslot;
	LPARAM m_nwindowProc;
	CString m_strMailslot;
	static UINT m_nTransferMessage;
};

////////////////////////////////////////////////////////////////////////////////
// Inline Functions															  //
////////////////////////////////////////////////////////////////////////////////
inline void CMailslot::SetSize( int pSize)
{
	m_nSize = pSize;
}

inline HANDLE CMailslot::GetInputslot() const
{
	return m_hInputslot;
}

inline UINT CMailslot::GetTransferMessage() const
{
	return m_nTransferMessage;
}

inline LPARAM CMailslot::GetLparam() const
{
	return m_nwindowProc;
}
#endif //__MAILSLOT_H__