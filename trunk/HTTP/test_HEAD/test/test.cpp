// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "http_Check.h"

int _tmain(int argc, _TCHAR* argv[])
{

	bool bFlag = CheckIsEffective("www.9cpp.com");
	if (bFlag)
	{
		printf("����\r\n");
	}
	else
	{
		printf("������\r\n");
	}

	bFlag = CheckIsEffective("9cpp.com");
	if (bFlag)
	{
		printf("����\r\n");
	}
	else
	{
		printf("������\r\n");
	}

	bFlag = CheckIsEffective("blog.sina.com.cn");
	if (bFlag)
	{
		printf("����\r\n");
	}
	else
	{
		printf("������\r\n");
	}
	return 0;
}

