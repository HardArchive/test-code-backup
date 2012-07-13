// test.cpp : 定义控制台应用程序的入口点。
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

