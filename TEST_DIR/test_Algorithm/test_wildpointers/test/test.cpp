// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

void f(int i)
{
	int iAddr = (int)&i;
	int iTem2 = (int)(&i - 1);
	int iTem = *(&i - 1);
	*(&i - 1) += 3;
	int iTem1 = *(&i - 1);
}

int main()
{
	register int i = 1;

	int iLen = sizeof(i);
	int iAddr = (int)&i;
	int iAddr1 = iAddr-1;
	int iaddr2 = iAddr1 + 3;
	iAddr1 += 3;
	f(i);
	++i;

	int iAddr3 = (int)&i;
	printf("%d\n", i);
}
