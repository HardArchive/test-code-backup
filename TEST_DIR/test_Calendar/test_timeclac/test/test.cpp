// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DateTimeN.h"
using namespace Matr;
//#include <windows.h>
//#include <atlstr.h>
//#include <stdlib.h>
//#include <malloc.h>
//#include <memory.h>
//#include <tchar.h>
//#include <stdio.h>
#include <ATLComTime.h>
#define DateTimeToString(dt) COleDateTime(dt).Format("%Y-%m-%d %H:%M:%S")
#define DateToString(dt) COleDateTime(dt).Format("%Y-%m-%d")
#define CURRENTDATE COleDateTime::GetCurrentTime().Format("%Y%m%d")
#define CURRENTTIME COleDateTime::GetCurrentTime().Format("%H%M")
#define GETCURRENTTIME COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S")
int _tmain(int argc, _TCHAR* argv[])
{
	DATE dt = 41375.729167; 
	CString pTime = DateTimeToString(dt);
	DateTimeN obj1(2012, 04, 27, 15, 05);
	DateTimeN obj2(2012, 04, 26, 13, 05); //201203311822
	DateTimeN obj3 = obj1 -obj2;
	int iDay = obj3.hour();

	return 0;
}

