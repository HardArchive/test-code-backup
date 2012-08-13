
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
1�� Function Name �� DNSVerify
2�� Description   �� DNSa��֤ ������IP�Ƿ��Ӧ
3�� Parameters    �� char* pszDomain, in ��֤����
					 char* pszIp, in ��֤IP
4�� Return Value  �� ���ڷ���1�� �������󷵻�-1�� �����ڷ���-2;
5�� Dynamic memory�� ��
6�� Author        �� QNA 
7�� Created       �� 2012-8-13 10:47:33
8�� Remark        �� 
********************************************************************************/ 
int DNSVerify(char* pszDomain, char* pszIp)
{
	int WSA_return = 0;
	char szIP[32] = {0};
	WSADATA WSAData = {0};
	HOSTENT *host_entry = NULL;   //�ṹָ��
	struct in_addr addr = {0};

	if (!(pszDomain && pszIp))
	{
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
	
	if(WSA_return==0)
	{
		/* ��Ҫ������������������ */
		host_entry=gethostbyname(pszDomain);
		printf("%s\n", pszDomain);
		if(host_entry!=0)
		{
			int iNum = 0;
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
		printf("����������(����QNA�˳�):\r\n");
		scanf("%s", szDomain);
		if (!strcmp(szDomain, "QNA"))
		{
			break;
		}
		printf("������IP(����QNA�˳�):\r\n");
		scanf("%s", szIP);

		if (!strcmp(szIP, "QNA"))
		{
			break;
		}

		iRet = DNSVerify(szDomain, szIP);
		if (-1 == iRet)
		{
			printf("��֤ʧ��,��������;����:%s,IP%s\r\n", szDomain, szIP);
		}
		if (-2 == iRet)
		{
			printf("��֤�ɹ�,IP�����������ڶ�Ӧ��ϵ;����:%s,IP:%s\r\n", szDomain, szIP);
		}
		if (1 == iRet)
		{
			printf("��֤�ɹ�����IP���������ڶ�Ӧ��ϵ;����:%s,IP:%s\r\n", szDomain, szIP);
		}
	}


	getchar();
	return 1;
}

