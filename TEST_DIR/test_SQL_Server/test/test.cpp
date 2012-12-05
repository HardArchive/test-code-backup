// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DataHelper.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CDataHelper clsDataHelper;
	clsDataHelper.InitConnection();
	clsDataHelper.GetRemoteControl();

	return 0;
}

