// test.cpp : 定义控制台应用程序的入口点。
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

////c++ 在win32程序中打开控制台 并重定向输出
//
//#ifdef _DEBUG       // Release版禁用  
////AllocConsole为调用进程分配一个新的控制台。 如果函数成功，则返回值为非零值。如果该函数失败，则返回值为零。会设置GetLastError 
//AllocConsole();                                      //打开控制台窗口以显示调试信息   
////SetConsoleTitle设置控制台窗口的标题  GetConsoleTitle函数用于获取当前控制台窗口的标题
//SetConsoleTitleA("Debug Win Output");                //设置标题  
//
////GetStdHandle用于从一个特定的标准设备（标准输入、标准输出或标准错误）中取得一个句柄。
//HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);      //获取控制台输出句柄  
//
//INT hCrt = _open_osfhandle((INT)hCon, _O_TEXT);     //转化为C文件描述符  
//FILE * hf = _fdopen( hCrt, "w" );                   //转化为C文件流  
//setvbuf( hf, NULL, _IONBF, 0);                      //无缓冲  
//*stdout = *hf;                                      //重定向标准输出  
//#endif  

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	ULONG ulCurrPercent = 0;

	printf("\n");
	printf("\r当前程序执行进度：%2d%%        这里是空格  \r\n ", ulCurrPercent);

	for (ulCurrPercent = 0; ulCurrPercent <= 100; ulCurrPercent++)
	{
		Sleep(50);
		printf("\r进度：%2d%", ulCurrPercent);   //\n是另起一行，\r的话回到本行的开头，如果继续输入的话会把先前的覆盖掉
	}

	printf("\n");
	printf("\r当前程序执行进度：%2d%%        这里是空格  \r\n ", ulCurrPercent);


	return nRetCode;
}


