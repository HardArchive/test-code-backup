// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <stdio.h>
#include <initsock.h>
#include <comm_protocol.h>
#include <Windows.h>
//CInitSock initSock;		// ��ʼ��Winsock��
#include <Socket.h>


int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR tszText[MAX_PATH] = {0};
	QNA::DPACKET stuDPacket = {0};
	QNA::CSocket objSocket;

	objSocket.Connect(_T("192.168.30.124"), 6000);

	while(true)
	{
		ZeroMemory(tszText, sizeof(tszText));
		ZeroMemory(&stuDPacket, sizeof(stuDPacket));
		if (1 == objSocket.Recv(objSocket.GetSocket(), (PBYTE)&stuDPacket, sizeof(stuDPacket)))
		{
			printf("��������:%s\r\n", (TCHAR*)stuDPacket.byBuffer);

			strcpy(tszText, (TCHAR*)stuDPacket.byBuffer);
			if (!_tcscmp(tszText, _T("QNA")))
				break;
		}
		
		ZeroMemory(tszText, sizeof(tszText));
		ZeroMemory(&stuDPacket, sizeof(stuDPacket));
		scanf("%s", tszText);
		memcpy((PVOID)stuDPacket.byBuffer, (PVOID)tszText, strlen(tszText));
		objSocket.Send(objSocket.GetSocket(), (PBYTE)&stuDPacket, sizeof(stuDPacket));
	}


	getchar();

	return 0;
}

