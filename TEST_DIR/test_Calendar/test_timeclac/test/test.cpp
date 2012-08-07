// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DateTimeN.h"
using namespace Matr;
int _tmain1(int argc, _TCHAR* argv[])
{

	DateTimeN obj1(2012, 04, 27, 15, 05);
	DateTimeN obj2(2012, 04, 26, 13, 05); //201203311822
	DateTimeN obj3 = obj1 -obj2;
	int iDay = obj3.hour();
	return 0;
}

