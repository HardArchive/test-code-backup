// netdetective_server.cpp : 定义控制台应用程序的入口点。
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
	printf("输入字符q退出!!!!\r\n");
	scanf("%c", &chInput);
	system("cls");  //清屏
	while(ch != chInput)
	{
		printf("输入错误字符:%c\r\n", chInput);
		Sleep(1000);
		system("cls");  //清屏
		printf("输入字符q退出!!!!\r\n");
		scanf("%c", &chInput);		
	}
	
	clsIOCPServerHelper.Stop();
	return 0;
}

