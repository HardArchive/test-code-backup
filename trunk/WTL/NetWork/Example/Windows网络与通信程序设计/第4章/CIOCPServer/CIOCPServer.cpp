// CIOCPServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "IOCPServer.h"

CIOCPServer m_server;
int _tmain(int argc, _TCHAR* argv[])
{
	m_server.Start();
	return 0;
}

