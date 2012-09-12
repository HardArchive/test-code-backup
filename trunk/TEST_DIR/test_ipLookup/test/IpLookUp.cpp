
#include "stdafx.h"
#include "IpLookUp.h"
#include <winsock.h>
#pragma comment(lib, "ws2_32")

//������խ�ֽڵ� �����IpLookUp_dll��
#include "IpLookUp_dll.h"

#pragma comment(lib "IpLookUp_dll.lib");

int GetIniNum(TCHAR* ptszInSection , TCHAR* ptszInEntry, TCHAR* ptszInIniName = _T("config.ini"))
{
	int iNum = 0;
	QNA::CIniUtil clsIniUtil;
	std::tstring strTem = "";
	TCHAR tszIniPath[MAX_PATH] = {0};

	if (!(ptszInIniName&&ptszInSection&&ptszInEntry))
	{
		return -1;
	}

	if (!clsIniUtil.GetExePath(tszIniPath))
	{
		return -2;
	}

	_stprintf_s(tszIniPath, MAX_PATH, _T("%s\\%s"), tszIniPath, ptszInIniName);

	if (!clsIniUtil.SetIniFilename(tszIniPath))
	{
		return -3;
	}	

	iNum = clsIniUtil.GetInt(ptszInSection, ptszInEntry);
	return iNum;
}

bool GetIniString(TCHAR* ptszOutString, TCHAR* ptszInSection , TCHAR* ptszInEntry, TCHAR* ptszInIniName = _T("config.ini"))
{
	QNA::CIniUtil clsIniUtil;
	std::tstring strTem = "";
	TCHAR tszIniPath[MAX_PATH] = {0};

	if (!(ptszInIniName&&ptszInSection&&ptszInEntry&&ptszOutString))
	{
		return false;
	}

	if (!clsIniUtil.GetExePath(tszIniPath))
	{
		return false;
	}

	_stprintf_s(tszIniPath, MAX_PATH, _T("%s\\%s"), tszIniPath, ptszInIniName);

	if (!clsIniUtil.SetIniFilename(tszIniPath))
	{
		return false;
	}	

	strTem = clsIniUtil.GetString(ptszInSection, ptszInEntry);   
	if (!strTem.length())
	{
		return false;
	}

	_stprintf(ptszOutString, _T("%s"), strTem.c_str());
	strTem.clear();

	return true;

}


//�õ��跴����������Ϣ����
int GetConfigNum()
{
	return GetIniNum(_T("SERVERNUM"), _T("Num"));
}

//��Config.ini�ļ��л�ȡ�����������������ʽ
bool GetConfig(LOOKUPCONFIG& stuLookUpConfig, int iNum)
{
	TCHAR tszNumServer[16] = {0};

	_stprintf(tszNumServer, _T("SERVER%d"), iNum);
	if (!GetIniString(stuLookUpConfig.szUrl, tszNumServer, _T("Url")))
	{
		return false;
	}

	if (!GetIniString(stuLookUpConfig.szRegex, tszNumServer, _T("Regex")))
	{
		return false;
	}

	return true;
}

bool GetIpLookIP(TCHAR* ptszIP)
{

	if (!GetIniString(ptszIP, _T("ADDR"), _T("IP")))
	{
		return false;
	}

	return true;
}

/******************************************************************************* 
1�� Function Name �� DNSVerify
2�� Description   �� DNSa��֤ ������IP�Ƿ��Ӧ
3�� Parameters    �� char* pszDomain, in ��֤����
char* pszIp, in ��֤IP
4�� Return Value  �� ���ڷ���1�� �������󷵻�-1�� �����ڷ���-2;
5�� Dynamic memory�� ��
6��	Author        �� QNA 
7�� Created       �� 2012-8-13 10:47:33
8�� Remark        �� 
********************************************************************************/ 
int DNSVerify(char* pszDomain, char* pszIp)
{
	int iNum = 0;
	DWORD dwError = 0;
	int WSA_return = 0;
	char szIP[32] = {0};
	WSADATA WSAData = {0};
	HOSTENT *host_entry = NULL;   //�ṹָ��
	struct in_addr addr = {0};

	if (!(pszDomain && pszIp))
	{
		printf("IP(%s)����(%s)��������\r\n", pszIp, pszDomain);
		return -1;
	}

	/*******************************************************************
	ʹ��Socket�ĳ�����ʹ��Socket֮ǰ�������WSAStartup������
	�ú����ĵ�һ������ָ����������ʹ�õ�Socket�汾��
	���и�λ�ֽ�ָ�����汾����λ�ֽ�ָ�����汾��
	����ϵͳ���õڶ����������������Socket�İ汾��Ϣ��
	��һ��Ӧ�ó������WSAStartup����ʱ������ϵͳ���������Socket�汾��������Ӧ��Socket�⣬
	Ȼ����ҵ���Socket�⵽��Ӧ�ó����С�
	�Ժ�Ӧ�ó���Ϳ��Ե����������Socket���е�����Socket�����ˡ�
	�ú���ִ�гɹ��󷵻�0��
	*****************************************************************/
	WSA_return = WSAStartup(MAKEWORD(2, 2), &WSAData);

	if (0 != WSA_return) 
	{
		printf("IP(%s)����(%s)�����׽��ְ汾ʧ��\r\n", pszIp, pszDomain);
		return -2;
	}

	/* ��Ҫ������������������ */
	host_entry = gethostbyname(pszDomain);

	if (NULL == host_entry)
	{
		dwError = WSAGetLastError();
		if (dwError != 0) 
		{
			if (dwError == WSAHOST_NOT_FOUND) 
				printf("IP(%s)����(%s)gethostbyname-Host not found\r\n", pszIp, pszDomain);
			else if (dwError == WSANO_DATA) 
				printf("IP(%s)����(%s)gethostbyname-No data record found\r\n", pszIp, pszDomain);
			else 
				printf("IP(%s)����(%s)gethostbyname-Function failed with error: %ld\r\n", pszIp, pszDomain, dwError);
		}
		return -3;
	}

	//����1
	//while(host_entry->h_addr_list[iNum])
	//{
	//	addr.s_addr = *(u_long *) host_entry->h_addr_list[iNum];
	//	printf("����IP��ַ: %d\r\n", iNum+1);
	//	printf("\t%s\r\n", inet_ntoa(addr));
	//	iNum++;
	//}	
	//����2
	while(host_entry->h_addr_list[iNum])
	{
		//printf("����IP��ַ: %d\r\n", iNum+1);
		sprintf_s(szIP, 32, "%d.%d.%d.%d",
			(host_entry->h_addr_list[iNum][0]&0x00ff),
			(host_entry->h_addr_list[iNum][1]&0x00ff),
			(host_entry->h_addr_list[iNum][2]&0x00ff),
			(host_entry->h_addr_list[iNum][3]&0x00ff));
		if (!strcmp(szIP, pszIp))
		{
			printf("IP(%s)����(%s)����ɹ�\r\n", pszIp, pszDomain);
			return 1;
		}
		iNum++;
	}	

	WSACleanup();	
	printf("IP(%s)����(%s)����ʧ�ܡ�û�ж�Ӧ��ϵ\r\n", pszIp, pszDomain);
	return 2;
}

int IpLook()
{
	int iNum = 0;
	int iRet = 0;
	TCHAR tszIP[32] = {0};
	char szDomain[128] = {0};  //������󳤶�Ϊ72�ַ�
	vector<string> strVecDomain;
	LOOKUPCONFIG stuLookUpConfig = {0};
	//vector<LOOKUPCONFIG> vecstuLookUpConfig;      //��������ѯ��������Ϣ  num+url+regex

	if (!GetIpLookIP(tszIP))
	{
		printf("��ȡIP��ַ����\r\n");
		Sleep(1000);
		return -1;
	}
	iNum = GetConfigNum();       //ȡ���������������

	for (int i=0; i<iNum; i++)
	{
		printf("��%d�η���\r\n", i+1);
		ZeroMemory(&stuLookUpConfig, sizeof(LOOKUPCONFIG));
		if (!GetConfig(stuLookUpConfig, i))
		{
			printf("��ȡIP��ַ����\r\n");
			Sleep(1000);
			return -2;
		}
		SetUrlRegexA(stuLookUpConfig.szUrl, stuLookUpConfig.szRegex);
		if (!StartIpLookupA(tszIP))
		{
			printf("��ʼ�������\r\n");
		}

		//����������������������
		for (int j=0; j<GetLookupDomainTotalNum(); j++)
		{
			if (GetLookupDomainA(szDomain, j)>0)
			{				
				strVecDomain.push_back(string(szDomain));
			}
			iRet = DNSVerify(szDomain, tszIP);	
			//printf("������:%d - ", iRet);
			ZeroMemory(szDomain, sizeof(szDomain));
		}
		printf("Over\r\n");
	}

	printf("All Over\r\n");

}
