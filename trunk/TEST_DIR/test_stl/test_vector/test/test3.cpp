// test.cpp : �������̨Ӧ�ó������ڵ㡣
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
		//�ҵ���
		printf("�ҵ��ˡ���");
	}
	else
	{
		//�Ҳ���
		printf("�Ҳ�������");
	}

	getchar();
	return 0;
} 