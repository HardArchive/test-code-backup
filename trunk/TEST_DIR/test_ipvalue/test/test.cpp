// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"

#include "stdafx.h"
#include <iostream>
#include <string>
//#include <windows.h>
using namespace std;

int IPToValue(const string& strIP)
{
	//IPת��Ϊ��ֵ
	//û�и�ʽ���
	//����ֵ���ǽ��

	int a[4];
	string IP = strIP;
	string strTemp;
	size_t pos;
	size_t i=3;

	do
	{
		pos = IP.find(".");

		if(pos != string::npos)
		{
			strTemp = IP.substr(0,pos);
			a[i] = atoi(strTemp.c_str());
			i--;
			IP.erase(0,pos+1);
		}
		else
		{
			strTemp = IP;
			a[i] = atoi(strTemp.c_str());
			break;
		}

	}while(1);

	int nResult = (a[3]<<24) + (a[2]<<16)+ (a[1]<<8) + a[0];
	return nResult;
}

string ValueToIP(const int& nValue)
{
	//��ֵת��ΪIP
	//û�и�ʽ���
	//����ֵ���ǽ��

	char strTemp[20];
	sprintf( strTemp,"%d.%d.%d.%d",
		(nValue&0xff000000)>>24,
		(nValue&0x00ff0000)>>16,
		(nValue&0x0000ff00)>>8,
		(nValue&0x000000ff) );

	return string(strTemp);
}

int main(void)
{
	//����218.92.189.40ת����-631456472
	//cout<<hex<<-631456472 <<endl;//���da5cbd28

	string strIP= "218.92.189.40";
	cout<<dec<<IPToValue(strIP)<<endl;
	//cout<<hex<<IPToValue(strIP)<<endl;
	cout<<ValueToIP(-631456472)<<endl;

	//IPΪ��218.92.176.82ת���� -631459758
	strIP= "218.92.176.82";
	cout<<dec<<IPToValue(strIP)<<endl;
	//cout<<hex<<IPToValue(strIP)<<endl;
	cout<<ValueToIP(-631459758)<<endl;

	return 0 ;
}
//
//#include "stdafx.h"
//#include <iostream>
//#include <string>
////#include <windows.h>
//using namespace std;
//
//int IPToValue(const string& strIP)
//{
//	//IPת��Ϊ��ֵ
//	//û�и�ʽ���
//	//����ֵ���ǽ��
//
//	int a[4];
//	string IP = strIP;
//	string strTemp;
//	size_t pos;
//	size_t i=3;
//
//	do
//	{
//		pos = IP.find(".");
//
//		if(pos != string::npos)
//		{
//			strTemp = IP.substr(0,pos);
//			a[i] = atoi(strTemp.c_str());
//			i--;
//			IP.erase(0,pos+1);
//		}
//		else
//		{
//			strTemp = IP;
//			a[i] = atoi(strTemp.c_str());
//			break;
//		}
//
//	}while(1);
//
//	int nResult = (a[3]<<24) + (a[2]<<16)+ (a[1]<<8) + a[0];
//	return nResult;
//}
//
//string ValueToIP(const int& nValue)
//{
//	//��ֵת��ΪIP
//	//û�и�ʽ���
//	//����ֵ���ǽ��
//
//	char strTemp[20];
//	sprintf( strTemp,"%d.%d.%d.%d",
//		(nValue&0xff000000)>>24,
//		(nValue&0x00ff0000)>>16,
//		(nValue&0x0000ff00)>>8,
//		(nValue&0x000000ff) );
//	return string(strTemp);
//}
//
//int main(void)
//{
//	//����218.92.189.40ת����-631456472
//	//cout<<hex<<-631456472 <<endl;//���da5cbd28
//
//	string strIP= "218.92.189.40";
//	cout<<dec<<IPToValue(strIP)<<endl;
//	//cout<<hex<<IPToValue(strIP)<<endl;
//	cout<<ValueToIP(-631456472)<<endl;
//
//	//IPΪ��218.92.176.82ת���� -631459758
//	strIP= "218.92.176.82";
//	cout<<dec<<IPToValue(strIP)<<endl;
//	//cout<<hex<<IPToValue(strIP)<<endl;
//	cout<<ValueToIP(-631459758)<<endl;
//
//	return 0 ;
//}
//
////Ҳ����ʹ���ֳɵ�win socket���������´���ʹ��vc6.0ͨ����
//
//#include "stdafx.h"
//#include <iostream>
//#include <winsock2.h>
//
//#pragma comment(lib,"ws2_32.lib")
//using namespace std;
//
//int main(void)
//{
//	cout<<(int)htonl(inet_addr("218.92.189.40"))<<endl;//���-631456472
//	struct in_addr addr;
//	addr.S_un.S_addr = ntohl(-631456472);
//	cout<<inet_ntoa( addr )<<endl;//���218.92.189.40
//	return 0 ;
//}
