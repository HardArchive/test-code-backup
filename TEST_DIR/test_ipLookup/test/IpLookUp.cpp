
#include "stdafx.h"
#include "IpLookUp.h"
#include <winsock.h>
#pragma comment(lib, "ws2_32")

//这是用窄字节的 需包含IpLookUp_dll库
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


//得到需反正的配置信息数量
int GetConfigNum()
{
	return GetIniNum(_T("SERVERNUM"), _T("Num"));
}

//从Config.ini文件中获取反查服务器和正则表达式
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
1、 Function Name ： DNSVerify
2、 Description   ： DNSa验证 域名和IP是否对应
3、 Parameters    ： char* pszDomain, in 验证域名
char* pszIp, in 验证IP
4、 Return Value  ： 存在返回1， 参数错误返回-1， 不存在返回-2;
5、 Dynamic memory： 无
6、	Author        ： QNA 
7、 Created       ： 2012-8-13 10:47:33
8、 Remark        ： 
********************************************************************************/ 
int DNSVerify(char* pszDomain, char* pszIp)
{
	int iNum = 0;
	DWORD dwError = 0;
	int WSA_return = 0;
	char szIP[32] = {0};
	WSADATA WSAData = {0};
	HOSTENT *host_entry = NULL;   //结构指针
	struct in_addr addr = {0};

	if (!(pszDomain && pszIp))
	{
		printf("IP(%s)域名(%s)参数错误\r\n", pszIp, pszDomain);
		return -1;
	}

	/*******************************************************************
	使用Socket的程序在使用Socket之前必须调用WSAStartup函数。
	该函数的第一个参数指明程序请求使用的Socket版本，
	其中高位字节指明副版本、低位字节指明主版本；
	操作系统利用第二个参数返回请求的Socket的版本信息。
	当一个应用程序调用WSAStartup函数时，操作系统根据请求的Socket版本来搜索相应的Socket库，
	然后绑定找到的Socket库到该应用程序中。
	以后应用程序就可以调用所请求的Socket库中的其它Socket函数了。
	该函数执行成功后返回0。
	*****************************************************************/
	WSA_return = WSAStartup(MAKEWORD(2, 2), &WSAData);

	if (0 != WSA_return) 
	{
		printf("IP(%s)域名(%s)请求套接字版本失败\r\n", pszIp, pszDomain);
		return -2;
	}

	/* 即要解析的域名或主机名 */
	host_entry = gethostbyname(pszDomain);

	if (NULL == host_entry)
	{
		dwError = WSAGetLastError();
		if (dwError != 0) 
		{
			if (dwError == WSAHOST_NOT_FOUND) 
				printf("IP(%s)域名(%s)gethostbyname-Host not found\r\n", pszIp, pszDomain);
			else if (dwError == WSANO_DATA) 
				printf("IP(%s)域名(%s)gethostbyname-No data record found\r\n", pszIp, pszDomain);
			else 
				printf("IP(%s)域名(%s)gethostbyname-Function failed with error: %ld\r\n", pszIp, pszDomain, dwError);
		}
		return -3;
	}

	//方法1
	//while(host_entry->h_addr_list[iNum])
	//{
	//	addr.s_addr = *(u_long *) host_entry->h_addr_list[iNum];
	//	printf("解析IP地址: %d\r\n", iNum+1);
	//	printf("\t%s\r\n", inet_ntoa(addr));
	//	iNum++;
	//}	
	//方法2
	while(host_entry->h_addr_list[iNum])
	{
		//printf("解析IP地址: %d\r\n", iNum+1);
		sprintf_s(szIP, 32, "%d.%d.%d.%d",
			(host_entry->h_addr_list[iNum][0]&0x00ff),
			(host_entry->h_addr_list[iNum][1]&0x00ff),
			(host_entry->h_addr_list[iNum][2]&0x00ff),
			(host_entry->h_addr_list[iNum][3]&0x00ff));
		if (!strcmp(szIP, pszIp))
		{
			printf("IP(%s)域名(%s)较验成功\r\n", pszIp, pszDomain);
			return 1;
		}
		iNum++;
	}	

	WSACleanup();	
	printf("IP(%s)域名(%s)较验失败…没有对应关系\r\n", pszIp, pszDomain);
	return 2;
}

int IpLook()
{
	int iNum = 0;
	int iRet = 0;
	TCHAR tszIP[32] = {0};
	char szDomain[128] = {0};  //域名最大长度为72字符
	vector<string> strVecDomain;
	LOOKUPCONFIG stuLookUpConfig = {0};
	//vector<LOOKUPCONFIG> vecstuLookUpConfig;      //第三方查询服务器信息  num+url+regex

	if (!GetIpLookIP(tszIP))
	{
		printf("获取IP地址出错\r\n");
		Sleep(1000);
		return -1;
	}
	iNum = GetConfigNum();       //取到反查服务器数量

	for (int i=0; i<iNum; i++)
	{
		printf("第%d次反查\r\n", i+1);
		ZeroMemory(&stuLookUpConfig, sizeof(LOOKUPCONFIG));
		if (!GetConfig(stuLookUpConfig, i))
		{
			printf("获取IP地址出错\r\n");
			Sleep(1000);
			return -2;
		}
		SetUrlRegexA(stuLookUpConfig.szUrl, stuLookUpConfig.szRegex);
		if (!StartIpLookupA(tszIP))
		{
			printf("开始反查出错\r\n");
		}

		//将反查出的域名添加至向量
		for (int j=0; j<GetLookupDomainTotalNum(); j++)
		{
			if (GetLookupDomainA(szDomain, j)>0)
			{				
				strVecDomain.push_back(string(szDomain));
			}
			iRet = DNSVerify(szDomain, tszIP);	
			//printf("较验结果:%d - ", iRet);
			ZeroMemory(szDomain, sizeof(szDomain));
		}
		printf("Over\r\n");
	}

	printf("All Over\r\n");

}
