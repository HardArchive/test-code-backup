// RGWinService.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	//SetPriorityClass 中参数，分别对应 5 种进程优先权
	//	NORMAL_PRIORITY_CLASS    //标准
	//	IDLE_PRIORITY_CLASS  //空闲(低)
	//	HIGH_PRIORITY_CLASS  //高
	//	REALTIME_PRIORITY_CLASS  //实时
	//	BELOW_NORMAL_PRIORITY_CLASS  //低于标准
	//	ABOVE_NORMAL_PRIORITY_CLASS  //高于标准
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);


	Sleep(1000);
	return 0;
}

