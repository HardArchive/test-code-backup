// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include "MyDiskInfo.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CMyDiskInfo objMyDiskInfo;
	char szModelNumber[MAX_PATH] = {0};
	char szSerialNumber[MAX_PATH] = {0};
	objMyDiskInfo.GetDiskInfo();
	strcpy(szModelNumber, objMyDiskInfo.szModelNumber);
	strcpy(szSerialNumber, objMyDiskInfo.szSerialNumber);
	return 0;
}

