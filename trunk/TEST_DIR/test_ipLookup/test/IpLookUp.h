#pragma once
#include "stdafx.h"

#include "IniUtil.h"
#include <vector>
using namespace std;

//反查配置信息
typedef struct LOOKUP_CONFIG
{
	char szUrl[128];        //反查网址 eg：http://dns.aizhan.com/?q=  //域名最大长度为72字符
	char szRegex[128];      //反查正则表达式，eg：target=\"_blank\"\>(.+?)\</a\>\</td\>
	DWORD dwTimeInterval;   //反查第三方网站时间间隔  单位分钟
	DWORD dwPriority;       //访问优先级 值从1到反查网站总数
}LOOKUPCONFIG, *PLOOKUPCONFIG;

//得到需反正的配置信息数量
int GetConfigNum();
//获取反正配置信息
bool GetConfig(LOOKUPCONFIG& stuLookUpConfig, int iNum);
//获取需反查的IP
bool GetIpLookIP(TCHAR* ptszIP);


int DNSVerify(char* pszDomain, char* pszIp);  //IP域名较验

int IpLook();        //IP反查