// netdetective_server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "../global/IOCPServerHelper.h"
#include "../global/ClientHelper.h"

#include "../global/ShareMemory.h"
#include "../third_party/socket_module/FuncHelper.h"
#include "../third_party/pugixml/pugixml.hpp"
#include "../third_party/pugixml/pugiconfig.hpp"
typedef TCHAR* PTCHAR;

//获取XML内容
int GetXMLContecnt(const TCHAR* ptXMLPath, PBYTE pOutBuf, int iMaxBufLen)
{
	int iXMLLen = 0;
	RG::CShareMemory clsShareMemory;
	if (!clsShareMemory.Create(_T("Global\\IPC_SHARE"), 0, ptXMLPath))
	{
		TRACE("打开XML文件失败！！！\r\n");
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


//获取XML路径 ptInPath当前路径 iType为XML类型
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

//发送消息 参数为消息类型
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
	printf("发送数据成功，类型:%d!!!!!\r\n\n", iSendType);
	printf("等侍接收数据……\r\n");
	int iTimeOut = 5;
	while(true)
	{
		if (!iTimeOut)
		{
			printf("\r\n接收数据超时!!!!!\r\n");
			Sleep(1000);
			break;
		}
		if (clsClientHelper.m_bRevcFlag)
		{
			printf("接收完成!!!!!\r\n");
			break;
		}
		printf("\r……timeout:%ds", iTimeOut--);
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
		printf("退出请输入0:\r\n");
		printf("入住消息请输入1:\r\n");
		printf("退房消息请输入2:\r\n");
		printf("上线消息请输入3:\r\n");
		printf("下线消息请输入4:\r\n");	
		printf("请输入0-4之间的数字:");
		scanf("%d", &iInput);
		//iInput = getchar()-48;
		if (0==iInput)
		{
			bExitFlag = false;
			printf("执行退出命令\r\n");
			continue;
		}
		if (iInput<0 || iInput>4)
		{
			printf("请输入0-4之间的数字:");
			Sleep(150);
			system("cls");  //清屏
			continue;
		}
		SendUserStatusInfo(iInput);
		system("cls");  //清屏
	}	

	return 0;
}

