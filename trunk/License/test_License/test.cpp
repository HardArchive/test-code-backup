// test.cpp : �������̨Ӧ�ó������ڵ㡣
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
	printf("������Ϊ��%s\r\n", szMachineCode);
	printf("���к�Ϊ��%s\r\n", szSerial);

	printf("��ȡ���Ϊ��b1:%d; b2:%d, b3:%d\r\n", b1, b2, b3);

	getchar();

	return 0;
}

