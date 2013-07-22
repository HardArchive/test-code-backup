//////////////////////////////////////////////////////////////////////////////////////////////////
//																								
// IPCWorkshop																					
//																								
// Environment:     Visual C++ 6.0, Windows 2000												
//                                                                            
// Notes:	Class CNamedPipe. Encapsultaed over NamedPipe Win32 API. 
//			A named pipe is a named, one-way or duplex pipe for communication between the pipe 
//			server and one or more pipe clients. Named pipes are like telephone calls: you talk 
//			only to one party, but you know that the message is being received.
//			In this class the server is capable of having an single client and accepts data from
//			the client.
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __NAMEDPIPE_H__
#define __NAMEDPIPE_H__

class CNamedPipe 
{
public:
	CNamedPipe();
	~CNamedPipe();

	// attributes
	void SetSize(int);
	int GetSize() const;
	LPARAM GetLparam() const;
	bool GetExitThread() const;
	UINT GetTransferMessage() const;
	HANDLE GetInputPipeHandle() const;	

	// operations
	bool Initialize(bool,LPCTSTR,LPCTSTR);
	void Listen( UINT, LPARAM);
	bool Write(char*);	
	
private:
	int m_nSize;
	bool m_nIsServer;
	bool m_nExitThread;
	CString m_strServerName;
	CString m_strPipeName;	
	HANDLE m_hOutputPipeHandle;
	HANDLE m_hInputPipeHandle;
	LPARAM m_nwindowProc;
	static UINT m_nTransferMessage;
};


////////////////////////////////////////////////////////////////////////////////
// Inline Functions															  //
////////////////////////////////////////////////////////////////////////////////
inline int CNamedPipe::GetSize() const
{
	return m_nSize;
}

inline void CNamedPipe::SetSize( int pSize )
{
	m_nSize = pSize;
}

inline HANDLE CNamedPipe::GetInputPipeHandle() const
{
	return m_hInputPipeHandle;
}

inline UINT CNamedPipe::GetTransferMessage() const
{
	return m_nTransferMessage;
}

inline LPARAM CNamedPipe::GetLparam() const
{
	return m_nwindowProc;
}

inline bool CNamedPipe::GetExitThread() const
{
	return m_nExitThread;
}

#endif //__NAMEDPIPE_H__