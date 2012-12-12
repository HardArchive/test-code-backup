
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <iostream>
using namespace std;
#pragma comment(lib, "Iphlpapi.lib")
// 函数声明
void output(PIP_ADAPTER_INFO pIpAdapterInfo);

// 程序入口
int _tmain(int argc, _TCHAR* argv[])
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
	getchar();
	return 0;
}

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


bool GetHostIP(TCHAR* ptszOutIP)
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