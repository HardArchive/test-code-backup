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

	char szUrl[MAX_PATH] = {"http://www.114best.com/ip/114.aspx?w="};
	char szRegex[MAX_PATH] = {"\</span\>\<a target=\"_blank\"\>(.+?)\</a\>&nbsp;&nbsp"};
	//target="_blank"\>(.+?)\</a\>\</td\>
	SetUrlRegexA(szUrl, szRegex);
	if (!StartIpLookupA("112.127.141.86"))
	{
		printf("��ʼ�������\r\n");
	}
	vector<string> strVecDomain;

	if (!GetLookupDomainA(strVecDomain))
	{
		printf("��ȡ�����������\r\n");
	}


	return 0;
}