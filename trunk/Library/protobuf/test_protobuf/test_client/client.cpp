
// Client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "msg.pb.h"
#include <stdio.h>
#include <windows.h>
#include <fstream>
#include <string>

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libprotobuf.lib")

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(1,1);

	err = WSAStartup( wVersionRequested, &wsaData);
	if ( err != 0)
	{
		return 0;
	}
	if ( LOBYTE(wsaData.wVersion)!=1 || HIBYTE(wsaData.wVersion != 1))
	{
		WSACleanup();
		return 0;
	}

	SOCKET sockClient = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8000);

	connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	printf("Connect Successful...\n");

	test::Vo_CharacterInfo Info;
	Info.set_charid(123);

	Info.set_charname("name");


	std::string info;
	Info.SerializeToString(&info);


	send(sockClient, info.data(), info.size(),0);
	printf("Send Successful...\n");


	closesocket(sockClient);
	WSACleanup();

	system("pause");
	return 0;
}