#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib
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





