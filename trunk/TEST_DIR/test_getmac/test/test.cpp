// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

// ͷ�ļ�����
#include "stdafx.h"

#include <iostream>
using namespace std;

#include "GetMACAddress.h"
// �������
int _tmain(int argc, _TCHAR* argv[])
{
	cout<<"1****************************************************"<<endl;
	GetMACAddress_1();
	//getchar();
	cout<<"2****************************************************"<<endl;
	GetMACAddress_2();  //2003��Ӧ���ò���  ����ȥXP������
	cout<<"3****************************************************"<<endl;
	GetMACAddress_3();
	cout<<"4****************************************************"<<endl;
	GetMACAddress_4();
	getchar();
	return 0;
}
