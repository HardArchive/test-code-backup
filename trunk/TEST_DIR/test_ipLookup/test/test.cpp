// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//�������ÿ��ֽڵ� �����IpLookUp����ĸ��ļ�
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

//������խ�ֽڵ� �����IpLookUp_dll��
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