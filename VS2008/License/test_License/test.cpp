// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../License/License.h"
#pragma comment(lib, "../debug/License.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	bool b1 = false, b2 = false, b3 = false;
	char szMachineCode[32] = {0};
	char szSerial[32] = {0};
	b1 = GetMachineCode(szMachineCode);

	strcpy(szSerial, "TIPTOP");

	b2 = GetSerial(szSerial);


	b3 = CheckSerial(szSerial);
	printf("机器码为：%s\r\n", szMachineCode);
	printf("序列号为：%s\r\n", szSerial);

	printf("获取结果为：b1:%d; b2:%d, b3:%d\r\n", b1, b2, b3);

	getchar();

	return 0;
}

