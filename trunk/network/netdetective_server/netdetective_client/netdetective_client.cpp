// netdetective_server.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
//#include "../global/IOCPServerHelper.h"
#include "../global/ClientHelper.h"

#include "../global/ShareMemory.h"
#include "../third_party/socket_module/FuncHelper.h"

typedef TCHAR* PTCHAR;

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

const TCHAR g_tszXMLPath[4][MAX_PATH] ={_T("ab1.xml"),	_T("ab2.xml"),	_T("ab3.xml"),	_T("ab4.xml")};

////��ȡ��ǰ��������Ŀ¼ �ɹ�����true��ʧ�ܷ���false
//bool GetExePath(PTCHAR ptInPath)
//{
//	PTCHAR ptTem = NULL;
//	TCHAR tszTemp[MAX_PATH] = {0};
//	//��ȡ��ǰĿ¼  //�����ǻ�ȡ��ǰ�����ļ�������·�� 
//	if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptInPath)
//		return false; 
//
//	ptTem = _tcsrchr(tszTemp, _T('\\'));
//	memcpy(ptInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
//	return true;
//}

PTCHAR GetXmlPath(PTCHAR ptInPath, int iType)
{
	if (GetExePath(ptInPath))
	{
		strcat(ptInPath, "\\");
		strcat(ptInPath, g_tszXMLPath[iType]);
	}
	return ptInPath;
}

void SendUserStatusInfo(int iSendType)
{
	int iSendLen = 0;
	TCHAR tszTem[MAX_PATH] = {0};
	BYTE szbySendBuf[2048] = {0};
	static DWORD dwSerialNo = 0;
	CClientHelper clsClientHelper;
	clsClientHelper.Start("116.228.54.106", 6666);
	PDATAHEAD pstuHead = (PDATAHEAD)szbySendBuf;
	InitHead(pstuHead);
	pstuHead->dwSerialNo = dwSerialNo++;
	pstuHead->dwPacketLen = GetXMLContecnt(GetXmlPath(tszTem, iSendType-1), szbySendBuf+sizeof(DATAHEAD), 2048-sizeof(DATAHEAD));
	pstuHead->dwPacketLen += sizeof(DATAHEAD);
	iSendLen = pstuHead->dwPacketLen;
	clsClientHelper.Send(szbySendBuf, iSendLen);
	printf("�������ݳɹ�������:%d!!!!!\r\n", iSendType);
	int iTimeOut = 5;
	while(true)
	{
		if (!iTimeOut)
		{
			printf("�������ݳ�ʱ����\r\n");
			Sleep(1000);
			break;
		}
		if (clsClientHelper.m_bRevcFlag)
		{
			printf("�������!!!!!\r\n");
			break;
		}
		printf("���̽������ݡ���timeout:%d\r\n", iTimeOut--);
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

