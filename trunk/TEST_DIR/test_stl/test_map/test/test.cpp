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
	m_mapBarcodeID[string("sabasdf")] = 5;
	dwNum = m_mapBarcodeID.size();
	for (map<string, DWORD>::iterator it1=m_mapBarcodeID.begin(); it1!=m_mapBarcodeID.end(); it1++)
	{
		if (dwConnID == it1->second)
		{
			//TracePrint(LOG_INFO, "EEEEEEEE���Barcode_ID��Ӧ��ϵ ConnID::%d; barcode:%s!!!\r\n", it1->second, it1->first.c_str());
			m_mapBarcodeID.erase(it1);
			break;
		}
	}


	return 0;
}

