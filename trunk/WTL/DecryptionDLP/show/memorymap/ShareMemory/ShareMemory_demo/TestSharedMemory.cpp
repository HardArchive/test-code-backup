// TestSharedMemory.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "SharedMemory.h"

SharedMemory sm;
int _tmain(int argc, _TCHAR* argv[])
{
	sm.WriteSharedMemory("1","bonychen");
	sm.WriteSharedMemory("2","skyword");

	string szValue1;
	string szValue2;
	sm.ReadSharedMemory("1",szValue1);
	sm.ReadSharedMemory("2",szValue2);
	printf("%s\r\n",szValue1.c_str());
	printf("%s\r\n",szValue2.c_str());
	while(1)
	{
		::Sleep(20000);
	}
	return 0;
}

