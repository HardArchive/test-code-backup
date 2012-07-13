// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include "DownLoad.h"
using namespace QNA;

int _tmain(int argc, _TCHAR* argv[])
{
	//http://dns.aizhan.com/?q=www.youboy.com
	int iRet = 0;
	TCHAR tszUrl[MAX_PATH] = {0};
	TCHAR tszPath[MAX_PATH] = {0};
	TCHAR tszTem[MAX_PATH] = {0};
	CDownLoad CObjDL;	




	while(true)
	{
		std::cout << _T("输入QNA，退出程序！！！\r")<<std::endl;
		std::cout << _T("请输入要下载的网址：");

		std::cin >> tszUrl;
		if (!_tcscmp(tszUrl, _T("QNA")))
		{
			break;
		}
		std::cout << std::endl;
		std::cout << _T("请输入要保存的文件名：");
		std::cin >> tszTem;
		if (!_tcscmp(tszTem, _T("QNA")))
		{
			break;
		}
		_stprintf(tszPath, _T("D:\\WinPath\\desktop\\test\\%s.html"), tszTem);
		std::cout << tszPath << std::endl;

		iRet = CObjDL.DownLoadFile(tszUrl, tszPath);
		if (1 != iRet)
		{
			std::cout << _T("下载文件失败……") << std::endl;
		}
		else
		{
			std::cout << _T("下载文件成功！！！！！") << std::endl;
			std::cout << _T("文件路径:") << tszPath << std::endl << std::endl;
		}

		ZeroMemory(tszUrl, MAX_PATH*sizeof(TCHAR));
		ZeroMemory(tszPath, MAX_PATH*sizeof(TCHAR));
		ZeroMemory(tszTem, MAX_PATH*sizeof(TCHAR));
	}


	 //std::cin >> tszUrl;
	 //std::cout<<std::endl;
	 //std::cout<<_T("请输入要保存的文件名：");

	 //iRet = CObjDL.DownLoadFile(tszUrl, tszPath);

	 //ZeroMemory(tszUrl, MAX_PATH*sizeof(TCHAR));
	 //ZeroMemory(tszPath, MAX_PATH*sizeof(TCHAR));


	return 0;
}

