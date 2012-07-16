#include "stdafx.h"
#include "ClientService.h"
#include <winsock2.h>
// 是否调试模式运行
extern BOOL g_bRunInDebugMode;
 
const LPCTSTR ServiceName = _T("400PhoneClientService");
const LPCTSTR PROGRAM_MUTEX_NAME=_T("400PhoneClientrService");


// CAuthenCenterServiceApp 构造

CClientService::CClientService():CNTService(ServiceName, ServiceName)
{

}

// CAuthenCenterServiceApp 初始化

BOOL CClientService::InitInstance()
{
	WSAData wsdata;
	if(WSAStartup(MAKEWORD(2,2),&wsdata))
	{
		printf("WSAStartup failure! \n");
		return FALSE;
	}

	// 启动服务
	RegisterService(__argc, __argv);
	return TRUE;
}


//程序退出
int CClientService::ExitInstance()
{
	WSACleanup();
	return TRUE;
}

//运行服务
void CClientService::Run(DWORD argc, LPTSTR * argv)
{
	g_bRunInDebugMode = this->m_bDebug;
	TracePrint(LOG_INFO, "400Phone client服务开始运行 ! \n");
	ReportStatus(SERVICE_RUNNING);

	//fileSearch.SetPath("F:\\GycProject\\c++Pro\\PhoneClient\\pwd\\");
	//fileSearch.SetPath(szPath/*"c:\\yunlong\\"*/);
	fileSearch.Start();

}

//停止服务
void CClientService::Stop()
{
	fileSearch.Stop();
	ReportStatus(SERVICE_STOP_PENDING);
	TracePrint(LOG_INFO, _T("400Phone client服务接收了Stop停止命令\r\n"));
	ZeroMemory(g_szMinitorFolderPah, sizeof(g_szMinitorFolderPah));
}