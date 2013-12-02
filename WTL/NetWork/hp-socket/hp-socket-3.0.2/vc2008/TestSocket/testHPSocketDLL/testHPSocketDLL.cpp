// testHPSocketDLL.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "TestIOCPServer.h"
CTestIOCPServer ojbTestIOCPServer;


void testHPSocketDLL()
{
	ojbTestIOCPServer.Start();
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	testHPSocketDLL();
	_tprintf(_T("testHPSocketDLL()!!!!!\r\n"));
	Sleep(15*2*1000);

	ojbTestIOCPServer.Stop();
	_tprintf(_T("Stop()!!!!!\r\n"));
	Sleep(5*1000);
	return 0;
}

