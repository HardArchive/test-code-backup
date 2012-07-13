// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
using namespace std;

void ShowMessage()
{
	cout<<"请输入数字选择:"<<endl;
	cout<<"\r1、关闭显示器"<<endl;
	cout<<"\r2、关闭显示器后15s自动打开显示器"<<endl;
	cout<<"\r3、退出"<<endl;
}


int _tmain(int argc, _TCHAR* argv[])
{
	char chInput = 0;
	while (true)
	{
		ShowMessage();
		chInput = getchar();
		getchar();
		if ('1' == chInput)
		{
			//关闭显示器 
			::SendMessage(HWND_BROADCAST,   WM_SYSCOMMAND,   SC_MONITORPOWER,   1); 			
		}

		if ('2' == chInput)
		{
			//关闭显示器 
			::SendMessage(HWND_BROADCAST,   WM_SYSCOMMAND,   SC_MONITORPOWER,   1); 
			Sleep(15000);
			//打开显示器
			::SendMessage(NULL,   WM_SYSCOMMAND,   SC_MONITORPOWER,   -1); 	 
		}

		if ('3' == chInput)
		{
			break;
		}
		chInput = 0;
		system("cls");  //清屏
	}






	return 0;
}

