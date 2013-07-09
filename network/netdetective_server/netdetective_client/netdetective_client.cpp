// netdetective_server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
//#include "../global/IOCPServerHelper.h"
#include "../global/ClientHelper.h"

#include "../global/ShareMemory.h"

int GetXMLContecnt(PBYTE pOutBuf, int iMaxBufLen)
{
	int iXMLLen = 0;
	RG::CShareMemory clsShareMemory;
	if (!clsShareMemory.Create(_T("Global\\IPC_SHARE"), 0, "F:\\Projects\\test-code-backup\\trunk\\network\\netdetective_server\\bin\\ab.xml"))
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



int _tmain(int argc, _TCHAR* argv[])
{
	CClientHelper clsClientHelper;

	BYTE szbySendBuf[2048];
	PDATAHEAD pstuHead = (PDATAHEAD)szbySendBuf;
	InitHead(pstuHead);
	pstuHead->dwSerialNo = 5;
	pstuHead->dwPacketLen = GetXMLContecnt(szbySendBuf+sizeof(DATAHEAD), 2048-sizeof(DATAHEAD));

	pstuHead->dwPacketLen += sizeof(DATAHEAD);
	int iSendLen = pstuHead->dwPacketLen;
	//pstuHead->HtonlEx();
	
	clsClientHelper.Start("192.168.30.14", 6666);
	Sleep(1000);

	clsClientHelper.Send(szbySendBuf, iSendLen);



	Sleep(1000*30);
	clsClientHelper.Stop();
	return 0;
}

