// test.cpp : �������̨Ӧ�ó������ڵ㡣
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
	WORD wsVersion = MAKEWORD(2,0); //socket�İ汾
	WSADATA wsaData;    //����ṹ�������洢��WSAStartup�������ú󷵻ص�Windows Sockets���ݡ�������Winsock.dll ִ�е����ݡ�
	struct sockaddr_in sin;   //sockaddr_in�ṹ,��ʾsocket address and internet style
	uint BeginPort;     //��ʼ�˿�
	uint EndPort;     //�����˿�
	uint CurrentPort;    //����ɨ��Ķ˿�
	uint i=0;
	char HostName[256];    //Ҫɨ�������
	SOCKET s;      //���洴��socketʱ�ķ���ֵ
	int OpenPort;     //���Ŷ˿ڸ���
	OpenPort = 0;

	CopyRight();

	cout<<"Please input the HostName:";
	cin>>HostName;
	cout<<"Input the Begin Port:";
	cin>>BeginPort;
	cout<<"Input the End port:";
	cin>>EndPort;

	//���û�����Ķ˿ڽ����ж�
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

	//ѭ���жϷ�Χ�ڵĶ˿ڣ����ж϶˿��Ƿ񿪷� ������¼���
	for(CurrentPort=BeginPort; CurrentPort<=EndPort;++CurrentPort)
	{
		s = socket(AF_INET,SOCK_STREAM,0);
		if(s == INVALID_SOCKET)
		{
			cout<<"Establish Socket Failed."<<endl;  WSACleanup();  
		}

		//���ṹ��Ա��ֵ
		sin.sin_family = AF_INET;
		sin.sin_port = htons(CurrentPort);
		sin.sin_addr.S_un.S_addr = inet_addr(HostName);
		//��������
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

//����Ϊ��Ȩ��������
void CopyRight()
{
	cout<<"PortScan V1.0 by:Code Panda."<<endl;
	cout<<"======================================================"<<endl;
	cout<<"Mail: ";
	cout<<"Q  Q: 512711380"<<endl;
	cout<<"Motto:Stormrise,self-confidence unchanged."<<endl;
	cout<<"======================================================"<<endl<<endl;
}

