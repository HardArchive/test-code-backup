// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TestPublic.h"
#include <TraceUtil.h>


int _tmain(int argc, _TCHAR* argv[])
{
	int iRet = 0;
	//FileUtil();

	//TraceUtil();

	//IniUtil();

	////iRet = TestSocket("av360.cn", "202.91.240.190");
	////iRet = CheckIsEffective("av360.cn", "202.91.240.190");
	//RG::TRACE("TestSocket���ؽ��:%d\r\n", iRet);


	TestRegUtil();


	getchar();
	return 0;
}
