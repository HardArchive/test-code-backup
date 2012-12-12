// test.cpp : 定义控制台应用程序的入口点。
//
//
//#include "stdafx.h"
//
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

// 头文件包含
#include "stdafx.h"
#include "SecurityTool.h"
#include "GetHostIP.h"


void outputuser();
// 程序入口
int _tmain(int argc, _TCHAR* argv[])
{
	outputuser();

	printf("\n方法1#####\r\n");
	GetHostIP_01();
	printf("\n方法2#####\r\n");
	GetHostIP_02();
	printf("\n方法3#####\r\n");
	GetHostIP_03();

	getchar();
	return 0;
}



void outputuser()
{
	CString strUser = "";
	CSecurityTool::GetCurrProcessUser(strUser);
	//cout<<"当前进程用户：   "<<strUser.<<endl;
	CSecurityTool::GetLogUserXP(strUser);
	//cout<<"XP登陆用户：     "<<strUser<<endl;
	CSecurityTool::GetLogUser2K(strUser);
	//cout<<"win2000登陆用户："<<strUser<<endl;
}