// PhoneServer.cpp : �������̨Ӧ�ó������ڵ㡣
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