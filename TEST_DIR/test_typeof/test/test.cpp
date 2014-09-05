// test.cpp : 定义控制台应用程序的入口点。
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
}TestND, *PTestND;

#define get(a) ({item = a; 1;})

int Getabc()
{
	int i = 5;
	return 5;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int a = 4;
	
	if(typeid(a) == typeid(int))
	{
		printf("%s\n", "type is same");
	}



	TestND stuTestND = {0};

	PTestND pstuTemp = NULL;

	if(typeid(*pstuTemp) == typeid(TestND))
	{
		printf("%s\n", "type is same");
	}

	printf("  123");  printf("   321\r\n");

	int item = 0;

	//bool bFlag = get(3);

	int i= 0, j = 3;
	printf("%d,%d, %d\r\n", i++, i++, i);   //1,0,2

	i= 0;
	printf("%d,%d\r\n", ++i, ++i);           //2,2


	i = 0;
	printf("%d,%d,%d,%d", Getabc(), i, i++, i);

	for(int j=i; (j>2)&&(j<50);j++)
	{
		printf("%d\r\n", j);
	}



	//typeid(*pstuTestND) temp ;
	return 0;
}

