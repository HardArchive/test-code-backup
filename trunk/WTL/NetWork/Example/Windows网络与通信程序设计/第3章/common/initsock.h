//////////////////////////////////////////////////////////
// initsock.h�ļ�
#include <malloc.h>
#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib

class CInitSocket
{
public:
	CInitSocket(LPWSADATA lpWSAData = NULL, BYTE minorVersion = 2, BYTE majorVersion = 2)
	{
		m_iResult = 0;
		LPWSADATA lpTemp = lpWSAData;
		if(!lpTemp)
		{
			lpTemp	= (LPWSADATA)_alloca(sizeof(WSADATA));
		}

		m_iResult	= ::WSAStartup(MAKEWORD(minorVersion, majorVersion), lpTemp);
	}

	~CInitSocket()
	{
		if(IsValid())
		{
			::WSACleanup();
		}
	}

	int		GetResult()	{return m_iResult;}
	BOOL	IsValid()	{return m_iResult == 0;}

private:
	int		m_iResult;
};