#include "stdafx.h"
//法1头
#include <iostream>
using namespace std;
#include "GetMACAddress.h"
#include <WinSock2.h>
#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")

// 函数声明
void output(PIP_ADAPTER_INFO pIpAdapterInfo);
//法2头
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#pragma comment(lib, "NETAPI32.LIB")

//法三头
#include <conio.h>

//#define bzero(x, sz) memset(x, 0, sz) 

///函数作用,输出网卡信息  方法一调用 
void output(PIP_ADAPTER_INFO pIpAdapterInfo)
{
	//可能有多网卡,因此通过循环去判断
	while (pIpAdapterInfo)
	{
		cout<<"网卡名称："<<pIpAdapterInfo->AdapterName<<endl;
		cout<<"网卡描述："<<pIpAdapterInfo->Description<<endl;
		cout<<"网卡MAC地址："/*<<pIpAdapterInfo->Address*/;
		for (UINT i = 0; i < pIpAdapterInfo->AddressLength; i++)
			if (i==pIpAdapterInfo->AddressLength-1)
			{
				printf("%02x\n", pIpAdapterInfo->Address[i]);
			}
			else
			{
				printf("%02x-", pIpAdapterInfo->Address[i]);
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

//方法一，获取MAC地址
void GetMACAddress_1()
{
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
	//得到结构体大小,用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
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


//typedef struct AdapterStrct
//{
//	int nIndex;			//网卡编号
//	string strMac;		//Mac地址（字符串形式）
//}ADPTSTRCT;

bool GetAdapterInfo(int adapter_num, string &mac_addr)
{
	//定义两个存放返回网卡信息的变量
	struct ASTAT
	{
		ADAPTER_STATUS adapt;
		NAME_BUFFER NameBuff[30];
	} Adapter;

	// 重置网卡，以便我们可以查询
	NCB Ncb = {0};
	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBRESET;
	Ncb.ncb_lana_num = adapter_num;  // 指定网卡号

	if (Netbios(&Ncb) != NRC_GOODRET)
	{
		mac_addr = "bad (NCBRESET): ";
		mac_addr += string((char*)Ncb.ncb_retcode);
		return false;
	}

	// 准备取得接口卡的状态块
	memset(&Ncb, 0, sizeof(Ncb));
	Ncb.ncb_command = NCBASTAT;
	Ncb.ncb_lana_num = adapter_num;
	strcpy((char *) Ncb.ncb_callname, "*");

	memset(&Adapter, 0, sizeof(Adapter));
	//指定返回的信息存放的变量 
	Ncb.ncb_buffer = (unsigned char *)&Adapter; 
	Ncb.ncb_length = sizeof(Adapter);

	// 发送NCBASTAT命令以获取网卡的信息 
	// 取得网卡的信息，并且如果网卡正常工作的话，返回标准的冒号分隔格式。
	if (Netbios(&Ncb) == 0)
	{
		char acMAC[18] = {0};
		// 把网卡MAC地址格式化成常用的16进制形式，如0010-A4E4-5802 
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
//第一种方法使用Microsoft的Netbios API。 
//这是一套通过Winsock提供底层网络支持的命令。
//使用Netbios的最大缺点是您必须在系统中安装了Netbios服务(如果您在windows网络中启用了文件共享的话，这就不是问题了)。
//除此此外，这种方法又快又准确。
void GetMACAddress_2()
{
	UCHAR uRetCode; 
	int i = 0; 
	string strMac_addr = "";

	// 取得网卡列表
	LANA_ENUM stuAdapterList /*= {0}*/;
	NCB Ncb;
	memset(&Ncb, 0, sizeof(NCB));
	Ncb.ncb_command = NCBENUM;
	Ncb.ncb_buffer = (unsigned char *)&stuAdapterList;
	Ncb.ncb_length = sizeof(stuAdapterList);

	//向网卡发送NCBENUM命令，以获取当前机器的网卡信息，如有多少个网卡、每张网卡的编号等 
	uRetCode = Netbios(&Ncb); 
	//获得所有网卡信息
	for(i=0; i < stuAdapterList.length ;i++)
	{
		GetAdapterInfo(stuAdapterList.lana[i], strMac_addr);
		printf("Adapter Num %d ; MAC:%s\r\n", stuAdapterList.lana[i], strMac_addr);
	}


}

//这种方法使用COM API创建一个GUID(全局唯一标识符)并从那里继承MAC地址。
//GUID通常用来标识COM组件以及系统中的其他对象。它们是由MAC地址(结合其他东西)计算得来的，表面上MAC地址就包含在其中。
//我说表面上是因为事实上并没有包含。
//我提供这种方法更多的是为了作为反面教材。您也许用这种方法能够得到MAC地址，但有时候您只会得到随机的十六进制数值。
//下面的例子十分简单，无需多讲。我们使用CoCreateGuid创建GUID，并将最后六个字节放入字符串中。它们可能是MAC地址，但并不是必然的。
void GetMACAddress_3()
{
	cout << "MAC address is: ";
	// 向COM要求一个UUID。如果机器中有以太网卡，
	// UUID最后的六个字节(Data4的2－7字节)应该是本地以太网卡的MAC地址。
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



