// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

class map_value_finder
{
public:
	map_value_finder(const DWORD& dwCmp):m_dwCmp(dwCmp){}
	bool operator ()(const std::map<string, DWORD>::value_type &pair)
	{
		return pair.second == m_dwCmp;
	}
private:
	const DWORD &m_dwCmp;                    
};

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


	//�ڶ��ֱ�����ʽ
	map<string, DWORD>::iterator it01 = m_mapBarcodeID.find(string("sabasdf0"));
	map<string, DWORD>::iterator it02 = m_mapBarcodeID.find(string("sabasdf1"));
	map<string, DWORD>::iterator it03;

	map<string, DWORD>::iterator it11 = std::find_if(m_mapBarcodeID.begin(), m_mapBarcodeID.end(), map_value_finder(1));
	map<string, DWORD>::iterator it22 = m_mapBarcodeID.find(string("sabasdf1"));
	

	m_mapBarcodeID.erase(std::find_if(m_mapBarcodeID.begin(), m_mapBarcodeID.end(), map_value_finder(1)));
	//map<string, DWORD>::iterator it33 = std::find_if(m_mapBarcodeID.begin(), m_mapBarcodeID.end(), map_value_finder(1));
	//it33::val
	//if (1 == it33->second)
	//{
	//	m_mapBarcodeID.erase(it33);
	//}	

	printf("erase�Ѿ����!!!!\r\n");

	//map<int,int> A;
	//A.insert(make_pair(1,2));
	//A.erase(A.find(1)); 

	return 0;
}

