// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdio.h>
#include <wchar.h>
#include <tchar.h>
#include<typeinfo>



typedef struct TEST_ND
{
	int a;
	char b;
	wchar_t c;
	long d;
}TestND, *pTestND;

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 4;
	
	if(typeid(a) == typeid(int))
	{
		printf("%s\n", "type is same");
	}



	TestND stuTestND = {0};

	pTestND pstuTemp = NULL;

	if(typeid(pstuTemp) == typeid(pTestND))
	{
		printf("%s\n", "type is same");
	}
	return 0;
}

