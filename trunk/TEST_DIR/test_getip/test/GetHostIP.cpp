#include "stdafx.h"
#include "GetHostIP.h"
#include <iostream>
using namespace std;
#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")


///��������,���������Ϣ
void output(PIP_ADAPTER_INFO pIpAdapterInfo)
{
	//�����ж�����,���ͨ��ѭ��ȥ�ж�
	while (pIpAdapterInfo)
	{
		cout<<"�������ƣ�"<<pIpAdapterInfo->AdapterName<<endl;
		cout<<"����������"<<pIpAdapterInfo->Description<<endl;
		cout<<"����MAC��ַ��";
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
		cout<<"����IP��ַ���£�"<<endl;
		//���������ж�IP,���ͨ��ѭ��ȥ�ж�
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
	//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	ZeroMemory(pIpAdapterInfo, sizeof(IP_ADAPTER_INFO));
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
	if (ERROR_BUFFER_OVERFLOW==nRel)
	{
		//����������ص���ERROR_BUFFER_OVERFLOW
		//��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
		//��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
		//�ͷ�ԭ�����ڴ�ռ�
		delete pIpAdapterInfo;
		//���������ڴ�ռ������洢����������Ϣ
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
		nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
	}
	if (ERROR_SUCCESS==nRel)
	{
		//���������Ϣ
		output(pIpAdapterInfo);
	}
	//�ͷ��ڴ�ռ�
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
	//��ȡ������������
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR) {
		printf("Error %d when getting local host name.n", WSAGetLastError());
		return 1;
	}
	printf("Host name is: %s\n", host_name);
	//�����������ݿ��еõ���Ӧ�ġ�������
	struct hostent *phe = gethostbyname(host_name);
	if (phe == 0) {
		printf("Yow! Bad host lookup.");
		return 1;
	}
	//ѭ���ó����ػ�������IP��ַ
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
	{fgets(temp,80,fp); /*����һЩ��*/ 
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