//---------------------------------------------------------------------------
//
//	"PipeListener.h"
//
//  Listen to one named pipe
//
//	by Holger Kloos
//
//---------------------------------------------------------------------------



#ifndef __PIPE_LISTENER__
#define __PIPE_LISTENER__


class CPipeListener;

interface IPipeDataDest
{
	virtual void OnConnectingPipe() = 0;
	virtual void OnDisConnectingPipe(CPipeListener* pReader) = 0;

	virtual void OnIncomingData(const char* pStr, DWORD nSize) = 0;
};



class CPipeListener
{
public:
	CPipeListener(IPipeDataDest* pDest);
	~CPipeListener();

	bool StartReader();

private:
	static DWORD WINAPI ThredFkt(LPVOID lpParameter);
	DWORD ReadPipe();
	
	DWORD	m_nID;
	HANDLE	m_hThread;

	IPipeDataDest* m_pDest;
};


#endif