#include "stdafx.h"
#include "GetHostIP.h"
#include <iostream>
using namespace std;
#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")


///函数作用,输出网卡信息
void output(PIP_ADAPTER_INFO pIpAdapterInfo)
{
	//可能有多网卡,因此通过循环去判断
	while (pIpAdapterInfo)
	{
		cout<<"网卡名称："<<pIpAdapterInfo->AdapterName<<endl;
		cout<<"网卡描述："<<pIpAdapterInfo->Description<<endl;
		cout<<"网卡MAC地址：";
		for (UINT i = 0; i < pIpAdapterInfo->AddressLength; i++)
		{
			if (i==pIpAdapterInfo->AddressLength-1)
			{
				printf("%02x\n", pIpAdapterInfo->Address[i]);
			}
			else
			{
				printf("%02x-", pIpAdapterInfo->Address[i]);
			}
		}
		cout<<"网卡IP地址如下："<<endl;
		//可能网卡有多IP,因此通过循环去判断
		IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
		do 
		{
			cout<<pIpAddrString->IpAddress.String<<endl;
			pIpAddrString=pIpAddrString->Next;
		} while (pIpAddrString);
		pIpAdapterInfo = pIpAdapterInfo->Next;
		cout<<"*****************************************************"<<endl;
	}
	return;
}
#include <WinSock2.h>
#pragma comment(lib, "WS2_32")
void GetHostIP_01()
{
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	ZeroMemory(pIpAdapterInfo, sizeof(IP_ADAPTER_INFO));
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}
	if (ERROR_SUCCESS==nRel)
	{
		//输出网卡信息
		output(pIpAdapterInfo);
	}
	//释放内存空间
	if (pIpAdapterInfo)
	{
		delete pIpAdapterInfo;
	}
}



int GetHostIP_02()
{
	WSAData wsaData;
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
	{
		system("pause");
		return 255;
	}
	char host_name[255];
	//获取本地主机名称
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR) {
		printf("Error %d when getting local host name.n", WSAGetLastError());
		return 1;
	}
	printf("Host name is: %s\n", host_name);
	//从主机名数据库中得到对应的“主机”
	struct hostent *phe = gethostbyname(host_name);
	if (phe == 0) {
		printf("Yow! Bad host lookup.");
		return 1;
	}
	//循环得出本地机器所有IP地址
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) {
		struct in_addr addr;
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
		printf("Address %d : %s\n" , i, inet_ntoa(addr));
	}
	WSACleanup();
}

#include <stdio.h>
#include <conio.h>
void GetHostIP_03()
{ 
	int i,j; 
	char ip[20]; 
	char temp[100]; 
	char ch='\0'; 
	FILE *fp; 
	system("ipconfig >d:\\myip.txt"); 
	if ((fp=fopen("d:\\myip.txt","r"))==NULL) 
	{ 
		printf("the file can not open:\nPress any key to exit:"); 
		getch(); 
		exit(1); 
	} 
	for (i=0;i<7;i++) 
	{fgets(temp,80,fp); /*跳过一些行*/ 
	/*printf("%s\n",temp); */} 
	fgets(temp,80,fp); 
	i=0;j=0; 
	while (temp[i++]!=':') 
		; 
	while (temp[i]!='\n') 
		ip[j++]=temp[i++]; 
	ip[j]=0; 
	printf("IP=%s\n",ip); 
	fclose(fp); 
	system("del d:\\myip.txt"); 

	getch(); 
}