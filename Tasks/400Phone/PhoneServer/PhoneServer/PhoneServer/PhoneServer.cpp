// PhoneServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "RecvData.h"
#pragma comment(lib,"ws2_32.lib")

#include "PhoneService.h"

CPhoneService service;
int _tmain(int argc, _TCHAR* argv[])
{
	service.InitInstance();
	service.ExitInstance();
	return 0;
}