#include "stdafx.h"
#include "PhoneService.h"

// 是否调试模式运行
extern BOOL g_bRunInDebugMode;
 
const LPCTSTR ServiceName = _T("400PhoneServerService");
const LPCTSTR PROGRAM_MUTEX_NAME=_T("400PhoneServerService");


// CAuthenCenterServiceApp 构造

CPhoneService::CPhoneService():CNTService(ServiceName, ServiceName)
{
}

// CAuthenCenterServiceApp 初始化

BOOL CPhoneService::InitInstance()
{
	if(!recvData.Init())
	{
		return FALSE;
	}
	// 启动服务
	RegisterService(__argc, __argv);
	return TRUE;
}


//程序退出
int CPhoneService::ExitInstance()
{
	recvData.UnInit();
	return TRUE;
}

//运行服务
void CPhoneService::Run(DWORD argc, LPTSTR * argv)
{
	g_bRunInDebugMode = this->m_bDebug;
	TracePrint(LOG_INFO, "400Phone服务开始运行 ! \n");
	ReportStatus(SERVICE_RUNNING);
	recvData.Start();
}

//停止服务
void CPhoneService::Stop()
{
	this->recvData.Stop();
	ReportStatus(SERVICE_STOP_PENDING);
	TracePrint(LOG_INFO, _T("400Phone服务接收了Stop停止命令\r\n"));
}