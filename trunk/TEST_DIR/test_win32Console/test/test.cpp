// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
using namespace std;

typedef unsigned char UCHAR;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
#include <WinCon.h>

////c++ ��win32�����д򿪿���̨ ���ض������
//
//#ifdef _DEBUG       // Release�����  
////AllocConsoleΪ���ý��̷���һ���µĿ���̨�� ��������ɹ����򷵻�ֵΪ����ֵ������ú���ʧ�ܣ��򷵻�ֵΪ�㡣������GetLastError 
//AllocConsole();                                      //�򿪿���̨��������ʾ������Ϣ   
////SetConsoleTitle���ÿ���̨���ڵı���  GetConsoleTitle�������ڻ�ȡ��ǰ����̨���ڵı���
//SetConsoleTitleA("Debug Win Output");                //���ñ���  
//
////GetStdHandle���ڴ�һ���ض��ı�׼�豸����׼���롢��׼������׼������ȡ��һ�������
//HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);      //��ȡ����̨������  
//
//INT hCrt = _open_osfhandle((INT)hCon, _O_TEXT);     //ת��ΪC�ļ�������  
//FILE * hf = _fdopen( hCrt, "w" );                   //ת��ΪC�ļ���  
//setvbuf( hf, NULL, _IONBF, 0);                      //�޻���  
//*stdout = *hf;                                      //�ض����׼���  
//#endif  

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	ULONG ulCurrPercent = 0;

	printf("\n");
	printf("\r��ǰ����ִ�н��ȣ�%2d%%        �����ǿո�  \r\n ", ulCurrPercent);

	for (ulCurrPercent = 0; ulCurrPercent <= 100; ulCurrPercent++)
	{
		Sleep(50);
		printf("\r���ȣ�%2d%", ulCurrPercent);   //\n������һ�У�\r�Ļ��ص����еĿ�ͷ�������������Ļ������ǰ�ĸ��ǵ�
	}

	printf("\n");
	printf("\r��ǰ����ִ�н��ȣ�%2d%%        �����ǿո�  \r\n ", ulCurrPercent);


	return nRetCode;
}


