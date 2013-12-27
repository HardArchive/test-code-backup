// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <winsock2.h>
#pragma comment(lib, "WS2_32")	// ���ӵ�WS2_32.lib

int _tmain(int argc, _TCHAR* argv[])
{

	WORD    VersionRequested;
	WSADATA WsaData;

	VersionRequested = MAKEWORD(2, 2);

	if (WSAStartup(VersionRequested, &WsaData))
		return -1;

	SOCKET SocketServer  = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	SOCKADDR_IN AddrServer;
	AddrServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	AddrServer.sin_port             = htons(9999);
	AddrServer.sin_family           = AF_INET;

	// ���ӷ�����
	if (0 != connect(SocketServer, (SOCKADDR *)&AddrServer, sizeof(SOCKADDR)))
	{
		closesocket(SocketServer);
		return false;
	}

	int err = 0;
	char temp[] = "hello, server";
	int rs = send(SocketServer, temp, sizeof(temp) + 1, 0);
	printf("send:%s, Err:%d; rs:%d\r\n ", temp, WSAGetLastError(), rs);

	char buff[1024] = {0};
	rs = recv(SocketServer, buff, sizeof(buff), 0);
	printf("recv:%s, Err:%d; rs:%d\r\n ", buff, WSAGetLastError(), rs);

	// ������2��������ע�͵��������recv��������ķ���0�������ע�͵���recv�ͻ᷵��-1��
	// ���õ�10053�������
	rs = send(SocketServer, temp, sizeof(temp) + 1, 0);
	err = WSAGetLastError();
	printf("2send:%s, Err:%d; rs:%d\r\n ", buff, err, rs);

	rs = recv(SocketServer, buff, sizeof(buff), 0);
	err = WSAGetLastError();
	printf("2recv:%s, Err:%d; rs:%d\r\n ", buff, err, rs);

	system("pause");
	return 0;

	return 0;
}

