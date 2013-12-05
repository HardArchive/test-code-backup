// RemotePolicy.h : PROJECT_NAME 应用程序的主头文件
//
#include "NTService/NTService.h"
#include "WorkThread.h"

#pragma once

#ifndef __AFXWIN_H__
	#error 在包含用于 PCH 的此文件之前包含“stdafx.h”
#endif

#include "resource.h"		// 主符号


// CRemotePolicyApp:
// 有关此类的实现，请参阅 RemotePolicy.cpp
//
class CWorkThread;
class CRemotePolicyApp : public CWinApp, public CNTService
{
public:
	CRemotePolicyApp();

public:
	HANDLE m_hServerMutex;			// 确保只有一个进程实例运行
	HANDLE m_hStop;					// 服务停止的事件内核对象句柄
	CWorkThread* m_pWorkThread;	// CWorkThread实例指针
	REG_PARAM m_RegParam;	// 数据库配置参数

	// 虚函数
public:
	// 重写CWinApp虚函数
	virtual BOOL InitInstance();
				// 进程初始化
	virtual int ExitInstance();
				// 进程清理

	// 重写CNTService虚函数
	virtual void Run(DWORD dwArgc, LPTSTR * ppszArgv);
				// 响应SCM启动消息
	virtual void Stop();
				// 响应SCM停止消息
	virtual void UserControlFunc();
				// 响应SCM用户自定义消息（消息号129）
	BOOL LoadRegistry();

	DECLARE_MESSAGE_MAP()

};

extern CRemotePolicyApp theApp;
