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

	//��ȡUrl
	strTem = clsIniUtil.GetString(_T("SERVER"), _T("Url"));   //�ַ�����
	if (!strTem.length())
	{
		//TRACE(_T("QNA-GetServer ��ȡUrl���󡭡�"));
		return false;
	}

	_stprintf_s(ptszUrl, 128, _T("%s"), strTem.c_str());
	strTem.clear();

	//��ȡRegex
	strTem = clsIniUtil.GetString(_T("SERVER"), _T("Regex"));   //�ַ�����
	if (!strTem.length())
	{
		//QNA::TRACE(_T("QNA-GetServer ��ȡRegex���󡭡�"));
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
	char szDomain[128] = {0};  //������󳤶�Ϊ72�ַ�

	if (!GetConfig(szUrl, szRegex))
	{
		return -1;
	}

	SetUrlRegexA(szUrl, szRegex);
	if (!StartIpLookupA("119.75.218.77"))
	{
		printf("��ʼ�������\r\n");
	}

	//����������������������
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