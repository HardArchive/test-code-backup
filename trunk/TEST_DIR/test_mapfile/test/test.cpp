// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"
//
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include "stdafx.h"


void errorFun(int * p)
{
	*p=1;
}

int _tmain(int argc, _TCHAR* argv[])
{

	int * p=NULL;
	errorFun(p);
	return 0;
}
