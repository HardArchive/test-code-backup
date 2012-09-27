// test_IconvLib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iconvpp\iconvpp.h"
using namespace Iconvpp;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string str;
	str= Utf8Convert::from_wstring(L"This is a test string~");
	printf("%s",str.c_str());
	return 0; 
	return 0;
}

