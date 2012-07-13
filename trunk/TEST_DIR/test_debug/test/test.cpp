// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "vld.h"
void f()
{
	int *p = new int(0x12345678);
	printf("p=%08x, ", p);
}


int _tmain(int argc, _TCHAR* argv[])
{
	f();

	return 0;
}

