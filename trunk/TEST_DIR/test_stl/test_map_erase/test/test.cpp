// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include <map>
#include <string>
using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	map<string, DWORD> m_mapBarcodeID;

	map<string, DWORD>::iterator it1=m_mapBarcodeID.begin();
	map<string, DWORD>::iterator it2=m_mapBarcodeID.end(); 
	DWORD dwConnID = 0;
	DWORD dwNum = m_mapBarcodeID.size();
	m_mapBarcodeID[string("sabasdf0")] = 0;
	m_mapBarcodeID[string("sabasdf1")] = 1;
	m_mapBarcodeID[string("sabasdf2")] = 2;
	m_mapBarcodeID[string("sabasdf3")] = 3;
	m_mapBarcodeID[string("sabasdf4")] = 4;
	m_mapBarcodeID[string("sabasdf5")] = 5;
	m_mapBarcodeID[string("sabasdf6")] = 6;
	m_mapBarcodeID[string("sabasdf7")] = 7;
	m_mapBarcodeID[string("sabasdf8")] = 8;
	m_mapBarcodeID[string("sabasdf9")] = 9;
	dwNum = m_mapBarcodeID.size();
	for (map<string, DWORD>::iterator it1=m_mapBarcodeID.begin(); it1!=m_mapBarcodeID.end(); /*it1++*/)
	{
		if (dwConnID == it1->second)
		{
			//TracePrint(LOG_INFO, "EEEEEEEE���Barcode_ID��Ӧ��ϵ ConnID::%d; barcode:%s!!!\r\n", it1->second, it1->first.c_str());
			
			//���it1Ϊm_mapBarcodeID.end() ,������ͻ�������⣬���������������������ģ�����ͻ�ͣ�����			 
			//�����������Ӹ��жϾ�OK�ˣ���
			if(it1 != m_mapBarcodeID.end()) 
				it1 = m_mapBarcodeID.erase(it1);
			//���� ����Ҳ����  ���� ++it1�ǲ��е�
			//m_mapBarcodeID.erase(it1++);
			//break;
		}
		else
			it1++;
	}

	printf("erase�Ѿ����!!!!\r\n");

	return 0;
}

