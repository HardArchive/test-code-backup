

#include "stdafx.h"
#include <stdio.h>
#include <Winsock2.h>
#include <windows.h>
#include <string>

#include "msg.pb.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libprotobuf.lib")

using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1,1);
	err = WSAStartup(wVersionRequested,&wsaData);
	if ( err != 0)
		return 0;

	if ( LOBYTE( wsaData.wVersion) != 1 || HIBYTE(wsaData.wVersion) != 1)
	{
		WSACleanup();
		return 0;
	}

	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(8000);

	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	printf("Bind OK...\n");

	listen(sockSrv,5);
	printf("Listen OK ...\n");

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	while(1)
	{
		SOCKET sockConn = accept(sockSrv,(SOCKADDR*)&addrClient,&len);
		printf("Receive data...\n");

		char recvBuf[500];
		memset(recvBuf,0,500);       
		recv(sockConn, recvBuf, 500, 0);

		test::Vo_CharacterInfo Info;
		Info.ParseFromString(string(recvBuf);

		closesocket(sockConn);
		printf("Close Socket...\n");
	}

	system("pause");
	return 0;
}