/*******************************************************************************
 * Copyright (C), 2011-2020, 李  照
 * FileName: WorkThreadManager.h
 * Author: 李  照    Version: 1.0    Date: 2011-02-22
 * Description: 工作线程管理类
 * Version: 1.0     // 版本信息
 * Function List:   // 主要函数及其功能
 * 1. 
 * History:         // 历史修改记录
 * <author>   <time>   <version>   <desc>
 *  李  照  2011-02-22    1.0    build this moudle  
 ******************************************************************************/
#pragma once
#include "WorkThread.h"
#include "NTService.h"

#ifndef WORK_THREAD_MANAGER_H_
#define WORK_THREAD_MANAGER_H_
const LPCTSTR ServiceName = _T("LZWinService");


// 工作线程基类
class CWorkThreadManager: public CWorkThread, public CNTService
{
public:
	// 构造函数
	CWorkThreadManager();
	// 析构函数
	~CWorkThreadManager();

	virtual void	Run(DWORD argc, LPTSTR * argv);
	virtual void	Stop();

protected:
	// 线程工作过程，一般需要重载
	// 常见重载代码框架如下：
	// int DoWork()
	// {
	//		while(!m_bQuit)
	//		{
	//			//……
	//			Sleep(10);
	//		}
	//		return 0;
	// }
	virtual int        DoWork();
	// 停止线程，一般不需要重载
	// 常见重载代码框架如下：
	// void DoStop()
	// {
	//		m_bQuit = true;
	// }
	virtual void       DoStop();
private:
	// 文件映射句柄
	HANDLE m_hFileMap;
	// 内存映射的首地址
	char* m_MemStart;
	// 运行模式。0=单进程模式；1=父进程模式；2=子进程模式；-1=调用参数错误。
	int m_RunMode;
	// 子进程数
	u_int m_nChildProcesses;

	// 创建共享内存
	inline int CreateMemShare();
	// 打开共享内存
	inline int OpenMemShare();

	// 单进程运行模式
	int DoSingle();
	// 父进程运行模式
	int DoParent();
	// 子进程运行模式
	int DoChild();
};

#define RUN_MODE_DEFAULT	0	// 默认运行模式
#define RUN_MODE_PARENT		1	// 父进程运行模式
#define RUN_MODE_CHILD		2	// 子进程运行模式
#define RUN_MODE_ERROR		-1	// 错误运行模式，运行参数错误

#endif