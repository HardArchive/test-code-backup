// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <stdio.h>
//#include <initsock.h>
//#include <comm_protocol.h>
//#include <Windows.h>

//CInitSock initSock;		// 初始化Winsock库


#include "Socket.h"
//using namespace QNA;

int _tmain(int argc, _TCHAR* argv[])
{
	int iNum = 0;
	TCHAR tszText[MAX_PATH] = {0};

	QNA::DPACKET stuDPacket = {0};
	SOCKET socketClient = NULL;
	QNA::CSocket ObjSocket(false);
	if (!ObjSocket.Bind(6000))
	{
		printf(" 服务端口绑定失败\r\n"/*, GetLastError*/);
		return 0;
	}

	if (!ObjSocket.Listen(5))
	{
		printf(" 服务端进入监听模式失败\r\n"/*, GetLastError*/);
		return 0;
	}

	while(true)
	{
		socketClient = ObjSocket.Accept();
		if(socketClient == INVALID_SOCKET)
		{
			printf("Failed accept()\r\n");
			continue;
		}
		iNum = 0;
		_tcscpy(tszText, "TCP Server Demo!");
		memcpy((PVOID)stuDPacket.byBuffer, (PVOID)tszText, strlen(tszText));
		// 向客户端发送数据
		if (1 == ObjSocket.Send(socketClient, (PBYTE)&stuDPacket, sizeof(stuDPacket)))
		{
			printf("新连接,向客户端发送数据：%s\r\n", (TCHAR*)stuDPacket.byBuffer);
		}
		while(true)
		{			
			ZeroMemory(tszText, sizeof(tszText));
			ZeroMemory(&stuDPacket, sizeof(stuDPacket));
			if (1 == ObjSocket.Recv(socketClient, (PBYTE)&stuDPacket, sizeof(stuDPacket)))
			{
				printf("接收内容：%s\r\n", (TCHAR*)stuDPacket.byBuffer);
				// 向客户端发送数据
				if (1 == ObjSocket.Send(socketClient, (PBYTE)&stuDPacket, sizeof(stuDPacket)))
				{
					printf("发送数据：%s\r\n", (TCHAR*)stuDPacket.byBuffer);
				}
				else
					printf("向客户端发送数据失败\r\n");
				
				strcpy(tszText, (TCHAR*)stuDPacket.byBuffer);
				if (!_tcscmp(tszText, _T("QNA")))	
				{
					printf("关闭与本客户端连接……\r\n");
					break;		
				}
			}
		}

		// 关闭同客户端的连接
		::closesocket(socketClient);
	}

	getchar();

	return 0;
}

