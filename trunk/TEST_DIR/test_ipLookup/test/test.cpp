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
#pragma comment(lib "IpLookUp_dll.lib");
int _tmain(int argc, _TCHAR* argv[])
{
	char szPath[MAX_PATH] = {0};
	GetCurrentPathA(szPath);

	//target="_blank"\>(.+?)\</a\>\</td\>
	SetUrlRegexA("http://dns.aizhan.com/?q=", "target=\"_blank\"\>(.+?)\</a\>\</td\>");
	StartIpLookupA("119.75.218.77");
	vector<string> strVecDomain;

	GetLookupDomainA(strVecDomain);

	return 0;
}