// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
using namespace std;

void ShowMessage()
{
	cout<<"����������ѡ��:"<<endl;
	cout<<"\r1���ر���ʾ��"<<endl;
	cout<<"\r2���ر���ʾ����15s�Զ�����ʾ��"<<endl;
	cout<<"\r3���˳�"<<endl;
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
			//�ر���ʾ�� 
			::SendMessage(HWND_BROADCAST,   WM_SYSCOMMAND,   SC_MONITORPOWER,   1); 			
		}

		if ('2' == chInput)
		{
			//�ر���ʾ�� 
			::SendMessage(HWND_BROADCAST,   WM_SYSCOMMAND,   SC_MONITORPOWER,   1); 
			Sleep(15000);
			//����ʾ��
			::SendMessage(NULL,   WM_SYSCOMMAND,   SC_MONITORPOWER,   -1); 	 
		}

		if ('3' == chInput)
		{
			break;
		}
		chInput = 0;
		system("cls");  //����
	}






	return 0;
}

