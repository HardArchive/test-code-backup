// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <stdio.h>
//#include <initsock.h>
//#include <comm_protocol.h>
//#include <Windows.h>

//CInitSock initSock;		// ��ʼ��Winsock��


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
		printf(" ����˿ڰ�ʧ��\r\n"/*, GetLastError*/);
		return 0;
	}

	if (!ObjSocket.Listen(5))
	{
		printf(" ����˽������ģʽʧ��\r\n"/*, GetLastError*/);
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
		// ��ͻ��˷�������
		if (1 == ObjSocket.Send(socketClient, (PBYTE)&stuDPacket, sizeof(stuDPacket)))
		{
			printf("������,��ͻ��˷������ݣ�%s\r\n", (TCHAR*)stuDPacket.byBuffer);
		}
		while(true)
		{			
			ZeroMemory(tszText, sizeof(tszText));
			ZeroMemory(&stuDPacket, sizeof(stuDPacket));
			if (1 == ObjSocket.Recv(socketClient, (PBYTE)&stuDPacket, sizeof(stuDPacket)))
			{
				printf("�������ݣ�%s\r\n", (TCHAR*)stuDPacket.byBuffer);
				// ��ͻ��˷�������
				if (1 == ObjSocket.Send(socketClient, (PBYTE)&stuDPacket, sizeof(stuDPacket)))
				{
					printf("�������ݣ�%s\r\n", (TCHAR*)stuDPacket.byBuffer);
				}
				else
					printf("��ͻ��˷�������ʧ��\r\n");
				
				strcpy(tszText, (TCHAR*)stuDPacket.byBuffer);
				if (!_tcscmp(tszText, _T("QNA")))	
				{
					printf("�ر��뱾�ͻ������ӡ���\r\n");
					break;		
				}
			}
		}

		// �ر�ͬ�ͻ��˵�����
		::closesocket(socketClient);
	}

	getchar();

	return 0;
}

