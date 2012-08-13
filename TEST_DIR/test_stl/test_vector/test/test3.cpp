// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

string strTem;
bool comp(string str)
{
	if (!str.compare(strTem))
	{
		return false;
	}
	return true;
}
int main3() 
{
	string strDomian;
	TCHAR tszDomain[64] = {_T("www.9cpp.com")};
	vector<string> strVecDomain;

	for (int i=0; i<15; i++)
	{	
		//strDomian + = '0'+i;
		_tcscpy(tszDomain, _T("www.9cpp.com"));
		_stprintf(tszDomain, _T("%s-%d"), tszDomain, i);
		strDomian = tszDomain;
		strVecDomain.push_back(strDomian);
	}
	strTem = tszDomain;

	vector<string>::iterator iter = find_if(strVecDomain.begin(), strVecDomain.end(), &comp);
	if(iter!=strVecDomain.end())
	{
		//找到了
		printf("找到了……");
	}
	else
	{
		//找不到
		printf("找不到……");
	}

	getchar();
	return 0;
} 