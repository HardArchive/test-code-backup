#pragma once
#include "stdafx.h"

#include "IniUtil.h"
#include <vector>
using namespace std;

//����������Ϣ
typedef struct LOOKUP_CONFIG
{
	char szUrl[128];        //������ַ eg��http://dns.aizhan.com/?q=  //������󳤶�Ϊ72�ַ�
	char szRegex[128];      //����������ʽ��eg��target=\"_blank\"\>(.+?)\</a\>\</td\>
	DWORD dwTimeInterval;   //�����������վʱ����  ��λ����
	DWORD dwPriority;       //�������ȼ� ֵ��1��������վ����
}LOOKUPCONFIG, *PLOOKUPCONFIG;

//�õ��跴����������Ϣ����
int GetConfigNum();
//��ȡ����������Ϣ
bool GetConfig(LOOKUPCONFIG& stuLookUpConfig, int iNum);
//��ȡ�跴���IP
bool GetIpLookIP(TCHAR* ptszIP);


int DNSVerify(char* pszDomain, char* pszIp);  //IP��������

int IpLook();        //IP����