// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib

int _tmain(int argc, _TCHAR* argv[])
{

	while(true)
	{
		printf("����while1 while2֮ǰ\r\n");
		while(true)
		{
			printf("����while2 break ֮ǰ\r\n");
			break;  //breakֻ������һ��ѭ��
			printf("while2 break ֮��\r\n");			
		}
		printf("while1 while2֮��\r\n");
		break;
		printf("while1 break ֮��\r\n");		
	}

	WORD    VersionRequested;
	WSADATA WsaData;

	VersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(VersionRequested, &WsaData))
	{
		printf("����socket��ʧ�ܣ�\n");
		return -1;
	}

	// �������������������
	SOCKET SockServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN AddrClient;
	AddrClient.sin_family      = AF_INET;
	AddrClient.sin_addr.s_addr = INADDR_ANY;
	AddrClient.sin_port        = htons(9999);

	if (SockServer == INVALID_SOCKET)
	{
		printf("socket��ʼ��ʧ�ܣ�\n");
		closesocket(SockServer);
		WSACleanup();
		return -1;
	}

	if (bind(SockServer, (sockaddr*)&AddrClient, sizeof(AddrClient)) != 0)
	{
		printf("socket��ʧ�ܣ�\n");
		closesocket(SockServer);
		WSACleanup();
		return -1;
	}

	if (listen(SockServer, 10) != 0)
	{
		printf("socket����ʧ�ܣ�\n");
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

