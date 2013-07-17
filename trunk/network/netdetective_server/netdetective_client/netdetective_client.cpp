// netdetective_server.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//#include "../global/IOCPServerHelper.h"
#include "../global/ClientHelper.h"

#include "../global/ShareMemory.h"
#include "../third_party/socket_module/FuncHelper.h"
#include "../third_party/pugixml/pugixml.hpp"
#include "../third_party/pugixml/pugiconfig.hpp"
typedef TCHAR* PTCHAR;

//��ȡXML����
int GetXMLContecnt(const TCHAR* ptXMLPath, PBYTE pOutBuf, int iMaxBufLen)
{
	int iXMLLen = 0;
	RG::CShareMemory clsShareMemory;
	if (!clsShareMemory.Create(_T("Global\\IPC_SHARE"), 0, ptXMLPath))
	{
		TRACE("��XML�ļ�ʧ�ܣ�����\r\n");
		return iXMLLen;
	}
	iXMLLen = clsShareMemory.GetFileMapLen();
	if (iXMLLen>iMaxBufLen)
	{
		return -1;
	}
	PBYTE pbyTem = (PBYTE)clsShareMemory.GetBasePoint();
	if (!pbyTem) return -2;

	memcpy(pOutBuf, pbyTem, iMaxBufLen);


	clsShareMemory.Close();
	pbyTem = NULL;
	return iXMLLen;
}


void InitHead(PDATAHEAD pstuInHead)
{
	pstuInHead->Reset();
	memcpy(pstuInHead->szHeadFlag, PACKAGE_MARK, 4);
}

const TCHAR g_tszXMLPath[5][MAX_PATH] ={_T("ab1.xml"),	_T("ab2.xml"),	_T("ab3.xml"),	_T("ab4.xml"), _T("config.xml")};


//��ȡXML·�� ptInPath��ǰ·�� iTypeΪXML����
PTCHAR GetXmlPath(PTCHAR ptInPath, int iType)
{
	if (GetExePath(ptInPath))
	{
		strcat(ptInPath, "\\xml\\");
		strcat(ptInPath, g_tszXMLPath[iType]);
	}
	return ptInPath;
}

string GetXMLConfig(char* pszInConfigType)
{
	string strConfig;
	TCHAR tszTem[MAX_PATH] = {0};
	pugi::xml_document doc;
	if (!doc.load_file(GetXmlPath(tszTem, 4))) return strConfig;
	pugi::xml_node xml_Node_Data = doc.child("server");
	
	strConfig = xml_Node_Data.child(pszInConfigType).first_child().value();
	return strConfig;
}

//������Ϣ ����Ϊ��Ϣ����
void SendUserStatusInfo(int iSendType)
{
	int iSendLen = 0;
	TCHAR tszTem[MAX_PATH] = {0};
	BYTE szbySendBuf[2048] = {0};
	static DWORD dwSerialNo = 0;
	CClientHelper clsClientHelper;
	clsClientHelper.Start(GetXMLConfig("ip").c_str(), atoi(GetXMLConfig("port").c_str()));
	PDATAHEAD pstuHead = (PDATAHEAD)szbySendBuf;
	InitHead(pstuHead);
	pstuHead->dwSerialNo = dwSerialNo++;
	pstuHead->dwPacketLen = GetXMLContecnt(GetXmlPath(tszTem, iSendType-1), szbySendBuf+sizeof(DATAHEAD), 2048-sizeof(DATAHEAD));
	pstuHead->dwPacketLen += sizeof(DATAHEAD);
	iSendLen = pstuHead->dwPacketLen;
	clsClientHelper.Send(szbySendBuf, iSendLen);
	printf("�������ݳɹ�������:%d!!!!!\r\n\n", iSendType);
	printf("���̽������ݡ���\r\n");
	int iTimeOut = 5;
	while(true)
	{
		if (!iTimeOut)
		{
			printf("\r\n�������ݳ�ʱ!!!!!\r\n");
			Sleep(1000);
			break;
		}
		if (clsClientHelper.m_bRevcFlag)
		{
			printf("�������!!!!!\r\n");
			break;
		}
		printf("\r����timeout:%ds", iTimeOut--);
		Sleep(1000);
	}

	clsClientHelper.Stop();
}


int _tmain(int argc, _TCHAR* argv[])
{


	bool bExitFlag = true;
	int iInput = '0';
	int iSendLen = 0;


	while(bExitFlag)
	{
		printf("�˳�������0:\r\n");
		printf("��ס��Ϣ������1:\r\n");
		printf("�˷���Ϣ������2:\r\n");
		printf("������Ϣ������3:\r\n");
		printf("������Ϣ������4:\r\n");	
		printf("������0-4֮�������:");
		scanf("%d", &iInput);
		//iInput = getchar()-48;
		if (0==iInput)
		{
			bExitFlag = false;
			printf("ִ���˳�����\r\n");
			continue;
		}
		if (iInput<0 || iInput>4)
		{
			printf("������0-4֮�������:");
			Sleep(150);
			system("cls");  //����
			continue;
		}
		SendUserStatusInfo(iInput);
		system("cls");  //����
	}	

	return 0;
}

