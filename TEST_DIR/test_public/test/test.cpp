// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#define USES_RELEASE_TRACE
//#include "public/TraceUtil.h"

//#include "public/RegUtil.h"
#include "public/IniUtil.h"
//#include "public/PathUtil.h"
#include <iostream>

using namespace std;

//int TraceUtil();
int IniUtil()
{
	QNA::CIniUtil clsIniUtil;


	return 1;
}

//int PathUtil()
//{
//	TCHAR tszPath[MAX_PATH] = {0};
//	if (!QNA::GetExePath(tszPath))
//	{
//		return -1;
//	}
//
//	_tcscat_s(tszPath, _T("\\123"));
//	
//	if (!QNA::CreateMultipleDirectory(tszPath))
//	{
//		return -2;
//	}
//
//	_tcscpy_s(tszPath, _T("D:\\123"));
//
//	if (!QNA::CreateMultipleDirectory(tszPath))
//	{
//		return -2;
//	}
//	return 1;
//}

int _tmain(int argc, _TCHAR* argv[])
{
	//PathUtil();

	//TraceUtil();

	//IniUtil();

	return 0;
}
//
//int TraceUtil()
//{
//	QNA::TRACE(_T("你好QNA……%s\r\n"), _T("1"));
//	QNA::TRACE(_T("你好QNA……%s\r\n"), _T("2"));
//	QNA::TRACE(_T("你好QNA……%s\r\n"), _T("3"));
//	QNA::TRACE(_T("你好QNA……%s\r\n"), _T("4"));
//	QNA::TRACE(_T("你好QNA……%s\r\n"), _T("5"));
//	return 1;
//}
