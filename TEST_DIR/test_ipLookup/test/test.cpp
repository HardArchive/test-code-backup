// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//这里是用宽字节的 需包含IpLookUp里的四个文件
//#include "IpLookup.h"
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	TCHAR szPath[MAX_PATH] = {0};
//	CIpLookup objIPLookup;
//	objIPLookup.GetCurrentPath(szPath);
//
//	//target="_blank"\>(.+?)\</a\>\</td\>
//	objIPLookup.SetUrlRegex(_T("http://dns.aizhan.com/?q="), _T("target=\"_blank\"\>(.+?)\</a\>\</td\>"));
//	objIPLookup.StartIpLookup(_T("173.245.61.112"));
//
//	return 0;
//}

//这是用窄字节的 需包含IpLookUp_dll库
//#include "IpLookUp_dll.h" 
#include <IpLookUp_dll.h>
#include "IniUtil.h"

bool GetConfig(TCHAR* ptszUrl, TCHAR* ptszRegex)
{
	QNA::CIniUtil clsIniUtil;
	TCHAR tszIniPath[MAX_PATH] = {0};

	if (!(ptszUrl||ptszRegex))
	{
		return false;
	}
	if (!clsIniUtil.GetExePath(tszIniPath))
	{
		return false;
	}

	_stprintf_s(tszIniPath, MAX_PATH, _T("%s\\config.ini"), tszIniPath);

	if (!clsIniUtil.SetIniFilename(tszIniPath))
	{
		return false;
	}

	std::tstring strTem;

	//获取Url
	strTem = clsIniUtil.GetString(_T("SERVER"), _T("Url"));   //分发中心
	if (!strTem.length())
	{
		//TRACE(_T("QNA-GetServer 获取Url错误……"));
		return false;
	}

	_stprintf_s(ptszUrl, 128, _T("%s"), strTem.c_str());
	strTem.clear();

	//获取Regex
	strTem = clsIniUtil.GetString(_T("SERVER"), _T("Regex"));   //分发中心
	if (!strTem.length())
	{
		//QNA::TRACE(_T("QNA-GetServer 获取Regex错误……"));
		return false;
	}
	_stprintf_s(ptszRegex, 128, _T("%s"), strTem.c_str());
	strTem.clear();
	return true;
}

#pragma comment(lib "IpLookUp_dll.lib");
int _tmain(int argc, _TCHAR* argv[])
{
	char szUrl[MAX_PATH] = {0};
	char szRegex[MAX_PATH] = {0};
	vector<string> strVecDomain;
	char szDomain[128] = {0};  //域名最大长度为72字符

	if (!GetConfig(szUrl, szRegex))
	{
		return -1;
	}

	SetUrlRegexA(szUrl, szRegex);
	if (!StartIpLookupA("119.75.218.77"))
	{
		printf("开始反查出错\r\n");
	}

	//将反查出的域名添加至向量
	for (int i=0; i<GetLookupDomainTotalNum(); i++)
	{
		if (GetLookupDomainA(szDomain, i)>0)
		{
			strVecDomain.push_back(string(szDomain));
		}
		ZeroMemory(szDomain, sizeof(szDomain));
	}
	printf("Over\r\n");

	getchar();

	return 0;
}