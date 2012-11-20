#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// Á´½Óµ½WS2_32.lib
#include "httpsocket.h"

bool CheckIsEffective(const char* url);


class CMySock : public CHTTPSocket
{
	char szErrMessage[255];
public:
	void OnError();
	void OnResponse(); 
	bool IsEffective();
};





