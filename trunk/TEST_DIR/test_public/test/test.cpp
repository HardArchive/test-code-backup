// test.cpp : 定义控制台应用程序的入口点。
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
	//RG::TRACE("TestSocket返回结果:%d\r\n", iRet);


	TestRegUtil();


	getchar();
	return 0;
}
