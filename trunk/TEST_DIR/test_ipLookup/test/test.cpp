// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "IpLookup.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TCHAR szPath[MAX_PATH] = {0};
	CIpLookup objIPLookup;
	objIPLookup.GetCurrentPath(szPath);

	//target="_blank"\>(.+?)\</a\>\</td\>
	objIPLookup.SetUrlRegex(_T("http://dns.aizhan.com/?q="), _T("target=\"_blank\"\>(.+?)\</a\>\</td\>"));
	objIPLookup.StartIpLookup(_T("173.245.61.112"));

	return 0;
}

