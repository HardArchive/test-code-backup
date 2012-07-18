// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Ini.h"

using namespace QNA;
int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR szSection[MAX_PATH] = _T("AIZHAN");
	TCHAR szEntry[MAX_PATH] = _T("Regex");
	CIni objTem(_T("D:\\config.ini"));

	std::tstring str = objTem.GetString(szSection, szEntry);

	_tcscpy_s(szSection, _T("PB"));
	_tcscpy_s(szEntry, _T("Port"));

	int Port = objTem.GetInt(szSection, szEntry);
	int Port2 = objTem.GetInt(_T("DC"), _T("Port"));

	bool bRet = objTem.WriteBoolean(_T("DC"), _T("Bool"), FALSE);

	bool bRet2 = objTem.GetBoolean(_T("PB"), _T("Bool"));

	bRet = objTem.GetBoolean(_T("DC"), _T("Bool"));


	return 0;
}

