// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#define HTTP_INFO_HOST            0x00000001L      //标记host信息没有获取完成
#define HTTP_INFO_URL             0x00000002L      //标记URL信息没有获取完成
#define HTTP_INFO_COOKIE          0x00000004L      //标记Cookie信息没有获取完成


//& 按位与
//| 按位或
//^ 按位异或

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

