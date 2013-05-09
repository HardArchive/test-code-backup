// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "ShareMemory.h"
#include <string>
#include <map>
using namespace std;
//��ȡ��ǰ��������Ŀ¼ �ɹ�����true��ʧ�ܷ���false
bool GetExePath(TCHAR* ptInPath)
{
	TCHAR* ptTem = NULL;
	TCHAR tszTemp[MAX_PATH] = {0};
	//��ȡ��ǰĿ¼  //�����ǻ�ȡ��ǰ�����ļ�������·�� 
	if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptInPath)
		return false; 

	ptTem = _tcsrchr(tszTemp, _T('\\'));
	memcpy(ptInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
	return true;
}

void GetDomain(char* PInDomianList, map<string, int>& mapDomainID)
{
	int iCount = 0, iNum = 0;
	char* pTem = PInDomianList;
	char szDomian[MAX_PATH] = {0};
	int iLen = strlen(PInDomianList);
	while(*pTem)
	{		
		if ((0x0D == *pTem)&&(0x0A == *(pTem+1)))
		{
			pTem += 2;
			mapDomainID[string(szDomian)] = iCount++;
			memset(szDomian, 0, MAX_PATH);
			iNum = 0;
		}
		szDomian[iNum++] = *pTem;
		pTem++;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	DWORD dwTem = 0;
	dwTem += 0xFFFFFFFF;
	dwTem +=1;
	dwTem +=1;
	int iTem = 0;
	iTem = 0xFFFFFFFF; //-1
	iTem = 0;          //0
	iTem = 0x7FFFFFFF; //�������2147483647   2^31-1
	iTem = 0x80000000; //��С����-2147483648  2^31
	//�����ڶ�����λ���� �����������������һλΪ0 ��������һλΪ1
	//�����Ĳ����ԭ����ͬ
	//�����Ĳ��룺�������ľ���ֵ�Ķ�������ʽ��λȡ���ټ� 1��

	unsigned int uiTem = 0;
	uiTem = 0xFFFFFFFF;  //����޷�������4294967295 2^32-1
	RG::CShareMemory clsShareMemory;
	TCHAR tszPath[MAX_PATH] = {0};

	if (!GetExePath(tszPath)) return 0;
	_tcscat_s(tszPath, MAX_PATH-_tcslen(tszPath), _T("\\DomianBlacklist.ini"));
	clsShareMemory.Open(_T("Global\\IPC_SHARE"), 0, tszPath);
	char* pTem = (char*)clsShareMemory.GetBasePoint();
	map<string, int> mapDomainID;
	GetDomain(pTem, mapDomainID);
	clsShareMemory.Close();
	pTem = NULL;


	RG::TRACE(_T("�ļ��Ѿ���ȡ���"));	
	
	return 0;
}

