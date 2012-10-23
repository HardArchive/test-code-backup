// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//
//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

#include <winsock2.h>
#include <windef.h>
#include <iostream>
#include <vector>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

void CopyRight();
typedef unsigned int uint;
vector<uint> v;
int main()
{
	WORD wsVersion = MAKEWORD(2,0); //socket的版本
	WSADATA wsaData;    //这个结构被用来存储被WSAStartup函数调用后返回的Windows Sockets数据。它包含Winsock.dll 执行的数据。
	struct sockaddr_in sin;   //sockaddr_in结构,表示socket address and internet style
	uint BeginPort;     //开始端口
	uint EndPort;     //结束端口
	uint CurrentPort;    //正在扫描的端口
	uint i=0;
	char HostName[256];    //要扫描的主机
	SOCKET s;      //保存创建socket时的返回值
	int OpenPort;     //开放端口个数
	OpenPort = 0;

	CopyRight();

	cout<<"Please input the HostName:";
	cin>>HostName;
	cout<<"Input the Begin Port:";
	cin>>BeginPort;
	cout<<"Input the End port:";
	cin>>EndPort;

	//对用户输入的端口进行判断
	while((BeginPort > EndPort || BeginPort < 0 || BeginPort >65535 || EndPort <0 || EndPort >65535))
	{
		cout<<"You have input some wrong paramters.Try again."<<endl;
		cout<<"Please input the HostName:";
		cin>>HostName;
		cout<<"Input the Begin Port:";
		cin>>BeginPort;
		cout<<"Input the End port:";
		cin>>EndPort;
	}

	if(WSAStartup(wsVersion,&wsaData))
	{
		cout<<"Initial Failed!"<<endl;  return -1;
	}
	cout<<endl<<"Now Scan "<<HostName<<"'s Specified Ports Status."<<endl<<endl;

	//循环判断范围内的端口，以判断端口是否开放 ，并记录结果
	for(CurrentPort=BeginPort; CurrentPort<=EndPort;++CurrentPort)
	{
		s = socket(AF_INET,SOCK_STREAM,0);
		if(s == INVALID_SOCKET)
		{
			cout<<"Establish Socket Failed."<<endl;  WSACleanup();  
		}

		//给结构成员赋值
		sin.sin_family = AF_INET;
		sin.sin_port = htons(CurrentPort);
		sin.sin_addr.S_un.S_addr = inet_addr(HostName);
		//建立连结
		if(connect(s,(struct sockaddr*)&sin,sizeof(sin)) == SOCKET_ERROR)
		{
			cout<<"Port: "<<CurrentPort<<", Status: Not open."<<endl;
			closesocket(s);
		}
		else
		{
			cout<<"Port: "<<CurrentPort<<", Status: Opened."<<endl;
			::MessageBox(NULL, _T("OK"), _T("OK"), MB_OK);
			OpenPort ++;  v.push_back(CurrentPort); closesocket(s);
		}  
	}

	cout<<endl<<"Resaults: Host "<<HostName<<" has opened "<<OpenPort<<" port.Opened port as follow:"<<endl;
	cout<<"===================================================================="<<endl;
	for(i=0;i<v.size();++i)
	{
		cout<<v.at(i)<<" ";
		if((i+1)%10==0)cout<<endl;
	}
	cout<<endl;
	cout<<"===================================================================="<<endl;
	closesocket(s);
	WSACleanup();

	return 0;
}

//以下为版权函数内容
void CopyRight()
{
	cout<<"PortScan V1.0 by:Code Panda."<<endl;
	cout<<"======================================================"<<endl;
	cout<<"Mail: ";
	cout<<"Q  Q: 512711380"<<endl;
	cout<<"Motto:Stormrise,self-confidence unchanged."<<endl;
	cout<<"======================================================"<<endl<<endl;
}

