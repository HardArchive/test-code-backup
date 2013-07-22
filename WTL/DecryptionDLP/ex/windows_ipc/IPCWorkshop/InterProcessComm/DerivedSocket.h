//////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                            
// IPCWorkshop							                                      
//                                                                            
// Environment:     Visual C++ 6.0, Windows 2000                              
//                                                                            
// Notes:	Class CDerivedSocket. Encapsultaed over CSocket. Creates both
//			socket server and socket client. Overrides only two methods 
//			just to receive an pointer from the client.  
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __DERIVEDSOCKET_H__
#define __DERIVEDSOCKET_H__

class CDerivedSocket : public CSocket
{
public:
	CDerivedSocket();
	~CDerivedSocket();

	// Attributes
	void SetDataSize(int);
	int GetDataSize() const;

	// Operations
	void Initialize(bool, LPCTSTR pHostAddress = "200.125.150.47", UINT pHostPort = 5000);
	void Listen(UINT, LPARAM);
	bool Send(char*, int);
	virtual void OnReceive( int nErrorCode );
	virtual void OnAccept( int nErrorCode );	

protected:
	int m_nDataSize;
	LPARAM m_nLParam;
	static UINT m_nTransferMessage;
};

////////////////////////////////////////////////////////////////////////////////
// Inline Functions															  //
////////////////////////////////////////////////////////////////////////////////
inline void CDerivedSocket::SetDataSize(int pDataSize)
{
	m_nDataSize = pDataSize;	
}

inline int CDerivedSocket::GetDataSize() const
{
	return m_nDataSize; 	
}

#endif //__DERIVEDSOCKET_H__
