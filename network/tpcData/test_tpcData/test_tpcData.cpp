// test_tpcData.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include "TraceUtil.h"
using namespace RG;
#include "comm_protocol.h"
#include "tpcData.h"
#include "DelayLoadUtil.h"

bool g_bExitFlag = false;
typedef BOOL WINAPI QUEUEUSERWORKITEM(LPTHREAD_START_ROUTINE, PVOID Context, ULONG Flags);


DWORD WINAPI WorkerThreadProc(LPVOID lpParam)
{
	if (!lpParam) return 0;
	fn_GetUserStatusInfo GetUserStatusInfo = (fn_GetUserStatusInfo)lpParam;	

	while (g_bExitFlag)
	{
		USERSTATUSINFO stuUserStatusInfo;
		memset(&stuUserStatusInfo, 0, sizeof(USERSTATUSINFO));
		int iRet = GetUserStatusInfo(&stuUserStatusInfo);
		if (1 == iRet)
		{
			TRACE(_T("Code:%d;description:%s;cardtype:%s;cardid:%s;name:%s;room:%d;ip:%s;mac:%s;time:%s\r\n"),
				stuUserStatusInfo.emUserStatus,
				stuUserStatusInfo.tszDescription,
				stuUserStatusInfo.tszCardType,
				stuUserStatusInfo.tszCardID,
				stuUserStatusInfo.tszUserName,
				stuUserStatusInfo.iRoomID,
				stuUserStatusInfo.tszIP,
				stuUserStatusInfo.tszMAC,
				stuUserStatusInfo.tszTime);

			printf("Code:%d;description:%s;cardtype:%s;cardid:%s;name:%s;room:%d;ip:%s;mac:%s;time:%s\r\n",
				stuUserStatusInfo.emUserStatus,
				stuUserStatusInfo.tszDescription,
				stuUserStatusInfo.tszCardType,
				stuUserStatusInfo.tszCardID,
				stuUserStatusInfo.tszUserName,
				stuUserStatusInfo.iRoomID,
				stuUserStatusInfo.tszIP,
				stuUserStatusInfo.tszMAC,
				stuUserStatusInfo.tszTime);
			Sleep(1000);
		}
	}

	return 1;
}


int _tmain(int argc, _TCHAR* argv[])
{
	TDelayLoadUtil clsDelayLoadUtil(_T("F:\\Projects\\test-code-backup\\trunk\\network\\tpcData\\bin\\tpcData.dll"));
	fn_Init Init = (fn_Init)clsDelayLoadUtil.GetProcAddress(_T("Init"));

	fn_UnInit UnInit = (fn_UnInit)clsDelayLoadUtil.GetProcAddress(_T("UnInit"));

	fn_GetUserStatusInfo pfnGetUserStatusInfo = (fn_GetUserStatusInfo)clsDelayLoadUtil.GetProcAddress(_T("GetUserStatusInfo"));

	if (Init)
	{
		Init(_T("0.0.0.0"), 6666);
	}

	g_bExitFlag = true;
	QueueUserWorkItem((LPTHREAD_START_ROUTINE)WorkerThreadProc, (LPVOID)pfnGetUserStatusInfo, WT_EXECUTELONGFUNCTION);

	char chInput = 0;
	const char  ch = 'q';
	printf("输入字符q退出!!!!\r\n");
	scanf("%c", &chInput);
	system("cls");  //清屏
	while(ch != chInput)
	{
		printf("输入错误字符:%c\r\n", chInput);
		Sleep(1000);
		system("cls");  //清屏
		printf("输入字符q退出!!!!\r\n");
		printf("输入字符p打印收到的数据!!!!\r\n");
		scanf("%c", &chInput);		
	}

	if (UnInit)
	{
		g_bExitFlag = false;
		UnInit();
	}
	Sleep(3000);

	return 0;
}

