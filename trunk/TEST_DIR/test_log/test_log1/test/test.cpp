// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Log.h"
using namespace INTERVISION;
int _tmain(int argc, _TCHAR* argv[])
{
	CLog_T objLog;
	objLog.SetLogPath("D:\\test1");
	objLog.Userlog(1, "18688708175, 12345");
	return 0;
}

