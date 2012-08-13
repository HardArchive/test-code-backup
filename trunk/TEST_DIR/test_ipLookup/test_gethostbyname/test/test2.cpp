
/* =====================================*
 * get_host.cpp
 * get host by name and by address
 * local machine may use ipconfig /all
 * L_o_o_n_i_e 2008-Jun-16
 * =====================================*/ 
#include "stdafx.h"
//#include <stdio.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdio.h>
//#pragma comment (lib, "wsock32.lib")
////#pragma comment(lib, "Ws2_32.lib")

//#include <windows.h>
//#include <iostream>
#include <winsock.h>
#pragma comment(lib, "ws2_32")

/******************************************************************************* 
1、 Function Name ： DNSVerify
2、 Description   ： DNSa验证 域名和IP是否对应
3、 Parameters    ： char* pszDomain, in 验证域名
					 char* pszIp, in 验证IP
4、 Return Value  ： 存在返回1， 参数错误返回-1， 不存在返回-2;
5、 Dynamic memory： 无
6、 Author        ： QNA 
7、 Created       ： 2012-8-13 10:47:33
8、 Remark        ： 
********************************************************************************/ 
int DNSVerify(char* pszDomain, char* pszIp)
{
	int WSA_return = 0;
	char szIP[32] = {0};
	WSADATA WSAData = {0};
	HOSTENT *host_entry = NULL;   //结构指针
	struct in_addr addr = {0};

	if (!(pszDomain && pszIp))
	{
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
	
	if(WSA_return==0)
	{
		/* 即要解析的域名或主机名 */
		host_entry=gethostbyname(pszDomain);
		printf("%s\n", pszDomain);
		if(host_entry!=0)
		{
			int iNum = 0;
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
					return 1;
				}
				iNum++;
			}	
		}
	}
	WSACleanup();
	return -2;
}

int main()
{
	int iRet = 0;
	char szDomain[64] = {0};
	char szIP[32] = {0};

	while(TRUE)
	{
		printf("请输入域名(输入QNA退出):\r\n");
		scanf("%s", szDomain);
		if (!strcmp(szDomain, "QNA"))
		{
			break;
		}
		printf("请输入IP(输入QNA退出):\r\n");
		scanf("%s", szIP);

		if (!strcmp(szIP, "QNA"))
		{
			break;
		}

		iRet = DNSVerify(szDomain, szIP);
		if (-1 == iRet)
		{
			printf("验证失败,参数错误;域名:%s,IP%s\r\n", szDomain, szIP);
		}
		if (-2 == iRet)
		{
			printf("验证成功,IP与域名不存在对应关系;域名:%s,IP:%s\r\n", szDomain, szIP);
		}
		if (1 == iRet)
		{
			printf("验证成功——IP与域名存在对应关系;域名:%s,IP:%s\r\n", szDomain, szIP);
		}
	}


	getchar();
	return 1;
}

