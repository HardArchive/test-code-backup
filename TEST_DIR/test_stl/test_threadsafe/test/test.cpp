// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "map.h"
//#include "Queue.h"
#include "safemap.h"
int _tmain(int argc, _TCHAR* argv[])
{
	//RG::SAFESTL::CMap<int, DWORD> map1;
	//RG::UTIL::queue<int> clsQueue;
	RG::SAFESTL::CMap<int, DWORD> clsMap;

	clsMap.Lock();
	clsMap.size();
	clsMap.Unlock();
	RG::SAFESTL::CQueue<string> clsQueString;

	clsQueString.Lock();
	clsQueString.size();
	clsQueString.push(string("sadgbadfb"));
	clsQueString.size();
	clsQueString.Unlock();
	return 0;
}

