// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#define HTTP_INFO_HOST            0x00000001L      //���host��Ϣû�л�ȡ���
#define HTTP_INFO_URL             0x00000002L      //���URL��Ϣû�л�ȡ���
#define HTTP_INFO_COOKIE          0x00000004L      //���Cookie��Ϣû�л�ȡ���


//& ��λ��
//| ��λ��
//^ ��λ���

int _tmain(int argc, _TCHAR* argv[])
{

	int iTem = HTTP_INFO_HOST & HTTP_INFO_URL & HTTP_INFO_COOKIE;
	int iTem2 = HTTP_INFO_HOST | HTTP_INFO_URL | HTTP_INFO_COOKIE;
	
	if (iTem2 & HTTP_INFO_HOST)
	{
		Sleep(1);
	}
	if (iTem & HTTP_INFO_HOST)
	{
		Sleep(1);
	}
	if (iTem2 | HTTP_INFO_HOST)
	{
		Sleep(1);
	}
	if (iTem | HTTP_INFO_HOST)
	{
		Sleep(1);
	}
	return 0;
}

