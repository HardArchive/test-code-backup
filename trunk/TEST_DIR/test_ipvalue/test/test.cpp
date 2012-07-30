// test.cpp : 定义控制台应用程序的入口点。
//

//#include "stdafx.h"

#include "stdafx.h"
#include <iostream>
#include <string>
//#include <windows.h>
using namespace std;

int IPToValue(const string& strIP)
{
	//IP转化为数值
	//没有格式检查
	//返回值就是结果

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
	//数值转化为IP
	//没有格式检查
	//返回值就是结果

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
	//对于218.92.189.40转化后-631456472
	//cout<<hex<<-631456472 <<endl;//输出da5cbd28

	string strIP= "218.92.189.40";
	cout<<dec<<IPToValue(strIP)<<endl;
	//cout<<hex<<IPToValue(strIP)<<endl;
	cout<<ValueToIP(-631456472)<<endl;

	//IP为：218.92.176.82转化后 -631459758
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
//	//IP转化为数值
//	//没有格式检查
//	//返回值就是结果
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
//	//数值转化为IP
//	//没有格式检查
//	//返回值就是结果
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
//	//对于218.92.189.40转化后-631456472
//	//cout<<hex<<-631456472 <<endl;//输出da5cbd28
//
//	string strIP= "218.92.189.40";
//	cout<<dec<<IPToValue(strIP)<<endl;
//	//cout<<hex<<IPToValue(strIP)<<endl;
//	cout<<ValueToIP(-631456472)<<endl;
//
//	//IP为：218.92.176.82转化后 -631459758
//	strIP= "218.92.176.82";
//	cout<<dec<<IPToValue(strIP)<<endl;
//	//cout<<hex<<IPToValue(strIP)<<endl;
//	cout<<ValueToIP(-631459758)<<endl;
//
//	return 0 ;
//}
//
////也可以使用现成的win socket函数，以下代码使用vc6.0通过：
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
//	cout<<(int)htonl(inet_addr("218.92.189.40"))<<endl;//输出-631456472
//	struct in_addr addr;
//	addr.S_un.S_addr = ntohl(-631456472);
//	cout<<inet_ntoa( addr )<<endl;//输出218.92.189.40
//	return 0 ;
//}
