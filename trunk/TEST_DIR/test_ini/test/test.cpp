// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "Ini.h"
//
//using namespace QNA;
//int _tmain(int argc, _TCHAR* argv[])
//{
//	TCHAR szSection[MAX_PATH] = _T("AIZHAN");
//	TCHAR szEntry[MAX_PATH] = _T("Regex");
//	CIni objTem(_T("D:\\config.ini"));
//
//	std::tstring str = objTem.GetString(szSection, szEntry);
//	_tcscpy_s(szSection, _T("PB"));
//	_tcscpy_s(szEntry, _T("Port"));
//
//	int Port = objTem.GetInt(szSection, szEntry);
//	int Port2 = objTem.GetInt(_T("DC"), _T("Port"));
//	bool bRet = objTem.WriteBoolean(_T("DC"), _T("Bool"), FALSE);
//	bool bRet2 = objTem.GetBoolean(_T("PB"), _T("Bool"));
//	bRet = objTem.GetBoolean(_T("DC"), _T("Bool"));
//	return 0;
//}

#include "Config.h"

int _tmain(int argc, _TCHAR* argv[])
{
	CConfig clsConfig;
	clsConfig.Init();

	SERVER stuDCServer = {0};

	if (!clsConfig.GetServer(stuDCServer, true))
	{
		return 1;
	}

	SERVER stuPBServer = {0};

	if (!clsConfig.GetServer(stuPBServer, false))
	{
		return 1;
	}

	QUERYINFO stuQueryInfo1 = {0};
	QUERYINFO stuQueryInfo2 = {0};
	QUERYINFO stuQueryInfo3 = {0};
	QUERYINFO stuQueryInfo4 = {0};
	QUERYINFO stuQueryInfo5 = {0};
	int iRet1 = clsConfig.GetWebQueryInfo(stuQueryInfo1);
	stuQueryInfo2 = stuQueryInfo1;
	int iRet2 = clsConfig.GetWebQueryInfo(stuQueryInfo2);
	stuQueryInfo3 = stuQueryInfo2;
	int iRet3 = clsConfig.GetWebQueryInfo(stuQueryInfo3);
	stuQueryInfo4 = stuQueryInfo3;
	int iRet4 = clsConfig.GetWebQueryInfo(stuQueryInfo4);
	stuQueryInfo5 = stuQueryInfo4;
	int iRet5 = clsConfig.GetWebQueryInfo(stuQueryInfo5);
	if (!iRet5)
	{
		bool bRet1 = clsConfig.ResetQuery(stuQueryInfo5);
	}



	return 0;
}
