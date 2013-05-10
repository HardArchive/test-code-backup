// test.cpp : 定义控制台应用程序的入口点。
//
//在一个字符串中检查是否有子字符串
#include "stdafx.h"

TCHAR g_ptTem1[] = _T("/ndbrowser/login.aspx?m=50E5\0149E9D0B5&g=&u=www.baidu.com&t=2&xi=C0A81E2250E549E9D0B5 ");
TCHAR g_ptTem2[] = _T("/ndbrowser/login.aspx?m=50E549E9D0B5&g=&u=www.baidu.com&t=2&xi=C0A81E2250E549E9D0B5 ");
TCHAR g_ptTem3[] = _T("/ndbrowser/login.aspx?m=50E549E9D0B5&g=&u=www.baidu.com&t=2&xi=C0A81E2250E549E9D0B5 ");
TCHAR g_ptTem4[] = _T("/ndbrowser/login.aspx?m=50E549E9D0B5&g=&u=www.baidu.com&t=2&xi=C0A81E2250E549E9D0B5 ");
TCHAR g_ptTem5[] = _T("/ndbrowser/login.aspx?m=50E549E9D0B5&g=&u=www.baidu.com&t=2&xi=C0A81E2250E549E9D0B5 ");

bool CheckSeparator(const TCHAR* ptInTem)
{
	while(*ptInTem)
	{
		if (1 == *ptInTem) return false;
		if (2 == *ptInTem) return false;
		if (3 == *ptInTem) return false;
		ptInTem++;
	}
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	bool bRet1 = CheckSeparator(g_ptTem5);
	g_ptTem5[10] = '\01';
	bool bRet2 = CheckSeparator(g_ptTem5);
	g_ptTem3[3] = '\02';
	bool bRet3 = CheckSeparator(g_ptTem3);
	g_ptTem4[3] = '\03';
	bool bRet4 = CheckSeparator(g_ptTem4);
	return 0;
}

