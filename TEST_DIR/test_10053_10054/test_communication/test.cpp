// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Communication.h"
int _tmain(int argc, _TCHAR* argv[])
{
	Communicate::CCommunication clsCCommunication((DWORD)2*60, 2, 2);
	clsCCommunication.Connect("114.255.5.53", 5555);
	bool bRet = false;
	char szBuffer[] = {"dga6d5fg4df65h4rgs65h4wg654adf6g54fd6g4adf65gfd"};
	bRet = clsCCommunication.SetBuffer(szBuffer, strlen(szBuffer));
	bRet = clsCCommunication.PendingSend(10);

	bRet = clsCCommunication.PendingReceive(5);

	clsCCommunication.CloseSocket();

	return 0;
}

