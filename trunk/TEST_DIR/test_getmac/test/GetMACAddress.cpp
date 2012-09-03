#include "stdafx.h"
//��1ͷ
#include <iostream>
using namespace std;
#include "GetMACAddress.h"
#include <WinSock2.h>
#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

// ��������
void output(PIP_ADAPTER_INFO pIpAdapterInfo);
//��2ͷ
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "NETAPI32.LIB")

//����ͷ
#include <conio.h>

//#define bzero(x, sz) memset(x, 0, sz) 

///��������,���������Ϣ  ����һ���� 
void output(PIP_ADAPTER_INFO pIpAdapterInfo)
{
	//�����ж�����,���ͨ��ѭ��ȥ�ж�
	while (pIpAdapterInfo)
	{
		cout<<"�������ƣ�"<<pIpAdapterInfo->AdapterName<<endl;
		cout<<"����������"<<pIpAdapterInfo->Description<<endl;
		cout<<"����MAC��ַ��"/*<<pIpAdapterInfo->Address*/;
		for (UINT i = 0; i < pIpAdapterInfo->AddressLength; i++)
			if (i==pIpAdapterInfo->AddressLength-1)
			{
				printf("%02x\n", pIpAdapterInfo->Address[i]);
			}
			else
			{
				printf("%02x-", pIpAdapterInfo->Address[i]);
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

//����һ����ȡMAC��ַ
void GetMACAddress_1()
{
	//PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//�õ��ṹ���С,����GetAdaptersInfo����
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
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


//typedef struct AdapterStrct
//{
//	int nIndex;			//�������
//	string strMac;		//Mac��ַ���ַ�����ʽ��
//}ADPTSTRCT;

bool GetAdapterInfo(int adapter_num, string &mac_addr)
{
	//����������ŷ���������Ϣ�ı���
	struct ASTAT
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER NameBuff[30];
	} Adapter;

	// �����������Ա����ǿ��Բ�ѯ
	NCB Ncb = {0};
	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBRESET;
	Ncb.ncb_lana_num = adapter_num;  // ָ��������

	if (Netbios(&Ncb) != NRC_GOODRET)
	{
		mac_addr = "bad (NCBRESET): ";
		mac_addr += string((char*)Ncb.ncb_retcode);
		return false;
	}

	// ׼��ȡ�ýӿڿ���״̬��
	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBASTAT;
	Ncb.ncb_lana_num = adapter_num;
	strcpy((char *) Ncb.ncb_callname, "*");

	memset(&Adapter, 0, sizeof(Adapter));
	//ָ�����ص���Ϣ��ŵı��� 
	Ncb.ncb_buffer = (unsigned char *)&Adapter; 
	Ncb.ncb_length = sizeof(Adapter);

	// ����NCBASTAT�����Ի�ȡ��������Ϣ 
	// ȡ����������Ϣ����������������������Ļ������ر�׼��ð�ŷָ���ʽ��
	if (Netbios(&Ncb) == 0)
	{
		char acMAC[18] = {0};
		// ������MAC��ַ��ʽ���ɳ��õ�16������ʽ����0010-A4E4-5802 
		sprintf(acMAC, "%02X:%02X:%02X:%02X:%02X:%02X",
			int (Adapter.adapt.adapter_address[0]),
			int (Adapter.adapt.adapter_address[1]),
			int (Adapter.adapt.adapter_address[2]),
			int (Adapter.adapt.adapter_address[3]),
			int (Adapter.adapt.adapter_address[4]),
			int (Adapter.adapt.adapter_address[5]));
		mac_addr = acMAC;
		return true;
	}
	else
	{
		mac_addr = "bad (NCBASTAT): ";
		mac_addr += string((char*)Ncb.ncb_retcode);
		return false;
	}
} 

//
//��һ�ַ���ʹ��Microsoft��Netbios API�� 
//����һ��ͨ��Winsock�ṩ�ײ�����֧�ֵ����
//ʹ��Netbios�����ȱ������������ϵͳ�а�װ��Netbios����(�������windows�������������ļ�����Ļ�����Ͳ���������)��
//���˴��⣬���ַ����ֿ���׼ȷ��
void GetMACAddress_2()
{
	UCHAR uRetCode; 
	int i = 0; 
	string strMac_addr = "";

	// ȡ�������б�
	LANA_ENUM stuAdapterList /*= {0}*/;
	NCB Ncb;
	memset(&Ncb, 0, sizeof(NCB));
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (unsigned char *)&stuAdapterList;
	Ncb.ncb_length = sizeof(stuAdapterList);

	//����������NCBENUM����Ի�ȡ��ǰ������������Ϣ�����ж��ٸ�������ÿ�������ı�ŵ� 
	uRetCode = Netbios(&Ncb); 
	//�������������Ϣ
	for(i=0; i < stuAdapterList.length ;i++)
	{
		GetAdapterInfo(stuAdapterList.lana[i], strMac_addr);
		printf("Adapter Num %d ; MAC:%s\r\n", stuAdapterList.lana[i], strMac_addr);
	}


}

//���ַ���ʹ��COM API����һ��GUID(ȫ��Ψһ��ʶ��)��������̳�MAC��ַ��
//GUIDͨ��������ʶCOM����Լ�ϵͳ�е�����������������MAC��ַ(�����������)��������ģ�������MAC��ַ�Ͱ��������С�
//��˵����������Ϊ��ʵ�ϲ�û�а�����
//���ṩ���ַ����������Ϊ����Ϊ����̲ġ���Ҳ�������ַ����ܹ��õ�MAC��ַ������ʱ����ֻ��õ������ʮ��������ֵ��
//���������ʮ�ּ򵥣�����ི������ʹ��CoCreateGuid����GUID��������������ֽڷ����ַ����С����ǿ�����MAC��ַ���������Ǳ�Ȼ�ġ�
void GetMACAddress_3()
{
	cout << "MAC address is: ";
	// ��COMҪ��һ��UUID���������������̫������
	// UUID���������ֽ�(Data4��2��7�ֽ�)Ӧ���Ǳ�����̫������MAC��ַ��
	GUID uuid;
	CoCreateGuid(&uuid);
	// Spit the address out
	char mac_addr[18];
	sprintf(mac_addr,"%02X:%02X:%02X:%02X:%02X:%02X",
		uuid.Data4[2],
		uuid.Data4[3],
		uuid.Data4[4],
		uuid.Data4[5],
		uuid.Data4[6],
		uuid.Data4[7]);
	cout << mac_addr << endl;
	//getch();
}



