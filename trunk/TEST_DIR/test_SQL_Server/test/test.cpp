// test.cpp : �������̨Ӧ�ó������ڵ㡣
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

