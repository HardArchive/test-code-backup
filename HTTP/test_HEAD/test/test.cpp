// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "http_Check.h"

int _tmain(int argc, _TCHAR* argv[])
{

	bool bFlag = CheckIsEffective("www.9cpp.com");
	if (bFlag)
	{
		printf("可行\r\n");
	}
	else
	{
		printf("不可行\r\n");
	}

	bFlag = CheckIsEffective("9cpp.com");
	if (bFlag)
	{
		printf("可行\r\n");
	}
	else
	{
		printf("不可行\r\n");
	}

	bFlag = CheckIsEffective("blog.sina.com.cn");
	if (bFlag)
	{
		printf("可行\r\n");
	}
	else
	{
		printf("不可行\r\n");
	}
	return 0;
}

