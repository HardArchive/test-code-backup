// netdetective_server.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "../global/IOCPServerHelper.h"
#include "../global/file.h"


int _tmain(int argc, _TCHAR* argv[])
{
	CIOCPServerHelper clsIOCPServerHelper;
	clsIOCPServerHelper.Start(_T("0.0.0.0"), 6666);

	char chInput = 0;
	const char  ch = 'q';
	printf("�����ַ�q�˳�!!!!\r\n");
	scanf("%c", &chInput);
	system("cls");  //����
	while(ch != chInput)
	{
		printf("��������ַ�:%c\r\n", chInput);
		Sleep(1000);
		system("cls");  //����
		printf("�����ַ�q�˳�!!!!\r\n");
		scanf("%c", &chInput);		
	}
	
	clsIOCPServerHelper.Stop();
	return 0;
}

