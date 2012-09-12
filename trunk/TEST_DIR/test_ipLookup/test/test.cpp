// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "IpLookUp.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int iRet = IpLook();

	int iRet1 = DNSVerify("117.135.133.82", "117.135.133.74");

	getchar();

	return 0;
}