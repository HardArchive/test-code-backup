// test.cpp : 定义控制台应用程序的入口点。
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
			//TracePrint(LOG_INFO, "EEEEEEEE清除Barcode_ID对应关系 ConnID::%d; barcode:%s!!!\r\n", it1->second, it1->first.c_str());
			
			//如果it1为m_mapBarcodeID.end() ,在这里就会出现意外，在这里会阻塞，阻的死死的，程序就会停在这里。			 
			//就是这样，加个判断就OK了！！
			if(it1 != m_mapBarcodeID.end()) 
				it1 = m_mapBarcodeID.erase(it1);
			//或者 这样也可以  但是 ++it1是不行的
			//m_mapBarcodeID.erase(it1++);
			//break;
		}
		else
			it1++;
	}

	printf("erase已经完成!!!!\r\n");

	return 0;
}

