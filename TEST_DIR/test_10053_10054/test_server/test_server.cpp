// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// 链接到WS2_32.lib

int _tmain(int argc, _TCHAR* argv[])
{

	while(true)
	{
		printf("进入while1 while2之前\r\n");
		while(true)
		{
			printf("进入while2 break 之前\r\n");
			break;  //break只能跳出一重循环
			printf("while2 break 之后\r\n");			
		}
		printf("while1 while2之后\r\n");
		break;
		printf("while1 break 之后\r\n");		
	}

	WORD    VersionRequested;
	WSADATA WsaData;

	VersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(VersionRequested, &WsaData))
	{
		printf("加载socket库失败！\n");
		return -1;
	}

	// 监听来自浏览器的请求
	SOCKET SockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN AddrClient;
	AddrClient.sin_family      = AF_INET;
	AddrClient.sin_addr.s_addr = INADDR_ANY;
	AddrClient.sin_port        = htons(9999);

	if (SockServer == INVALID_SOCKET)
	{
		printf("socket初始化失败！\n");
		closesocket(SockServer);
		WSACleanup();
		return -1;
	}

	if (bind(SockServer, (sockaddr*)&AddrClient, sizeof(AddrClient)) != 0)
	{
		printf("socket绑定失败！\n");
		closesocket(SockServer);
		WSACleanup();
		return -1;
	}

	if (listen(SockServer, 10) != 0)
	{
		printf("socket监听失败！\n");
		closesocket(SockServer);
		WSACleanup();
		return -1;
	}

	while (1)
	{
		SOCKET NewSocket = accept(SockServer, NULL, NULL);
		printf("accept  Err:%d; NewSocket:%d\r\n ", WSAGetLastError(), NewSocket);
		if (INVALID_SOCKET == NewSocket)
		{
			closesocket(NewSocket);
			continue;
		}
		else
		{
			struct linger so_linger;
			so_linger.l_onoff  = 1;
			so_linger.l_linger = 30;
			setsockopt(NewSocket ,SOL_SOCKET, SO_LINGER, (const char*)&so_linger, sizeof(so_linger)); 
		}

		char buff[1024];
		int rs = recv(NewSocket, buff, sizeof(buff), 0);
		printf("recv:%s, Err:%d; rs:%d\r\n ", buff, WSAGetLastError(), rs);

		char temp[] = "hello, client";
		rs = send(NewSocket, temp, sizeof(temp) + 1, 0);
		printf("send:%s, Err:%d; rs:%d\r\n ", temp, WSAGetLastError(), rs);

		closesocket(NewSocket);
	}

	system("pause");
	return 0;
}

