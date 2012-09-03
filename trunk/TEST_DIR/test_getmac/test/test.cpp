// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

// 头文件包含
#include "stdafx.h"

#include <iostream>
using namespace std;

#include "GetMACAddress.h"
// 程序入口
int _tmain(int argc, _TCHAR* argv[])
{
	cout<<"1****************************************************"<<endl;
	GetMACAddress_1();
	//getchar();
	cout<<"2****************************************************"<<endl;
	GetMACAddress_2();  //2003下应该用不了  改天去XP下试试
	cout<<"3****************************************************"<<endl;
	GetMACAddress_3();
	cout<<"4****************************************************"<<endl;
	GetMACAddress_4();
	getchar();
	return 0;
}
