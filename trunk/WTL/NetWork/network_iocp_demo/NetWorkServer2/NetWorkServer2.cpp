// NetWorkServer2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "TestIOCPServer.h"
//#include "../common/com_module/socket/IocpServer.h"
//#include "../common/com_module/socket/SocketHelper.h"
//#include "../global/helper.h"
int _tmain(int argc, _TCHAR* argv[])
{
	CTestIOCPServer ojbTestIOCPServer;
	ojbTestIOCPServer.Start();
	Sleep(10*60*1000);
	
	return 0;
}

