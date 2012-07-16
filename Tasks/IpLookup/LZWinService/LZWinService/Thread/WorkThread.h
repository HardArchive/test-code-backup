/*******************************************************************************
 * Copyright (C), 2011-2020, 李  照
 * FileName: WorkThread.h
 * Author: 李  照    Version: 1.0    Date: 2011-02-22
 * Description: 工作线程基类
 * Version: 1.0     // 版本信息
 * Function List:   // 主要函数及其功能
 * 1. Start()
 * 2. Stop()
 * 3. SetPriority(int nPriority)
 * 4. Terminate();
 * History:         // 历史修改记录
 * <author>   <time>   <version>   <desc>
 *  李  照  2011-02-22    1.0    build this moudle  
 ******************************************************************************/
#pragma once
//#include "..\Version.h"

#ifndef WORK_THREAD_H_
#define WORK_THREAD_H_

// 工作线程基类
class CWorkThread
{
public:
	// 构造函数
	CWorkThread();
	// 析构函数
	virtual ~CWorkThread();

	// Operations
public:
	// 开始一个线程
	int                Start(void* pParam=NULL);
	// 停止线程
	void               Stop(bool bSameWithStartThread=true);
	// 设置线程优先级
	void               SetPriority(int nPriority){ SetThreadPriority(m_hThread, nPriority); }
	// 终止线程
	void               Terminate();
	// 是否正在运行
	bool               IsRunning() { return m_bIsRunning; }
	// 获取线程句柄
	HANDLE             GetHandle(){ return m_hThread; }
	// 获取线程ID
	UINT               GetID(){ return m_nThreadID; }
protected:
	// 是否退出线程
	volatile bool m_bQuit;
	// 线程输入参数指针
	void* m_pParam;

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
	virtual int        DoWork(){return 0;}

	// 停止线程，一般不需要重载
	// 常见重载代码框架如下：
	// void DoStop()
	// {
	//		m_bQuit = true;
	// }
	virtual void       DoStop(){ if (!m_bQuit) m_bQuit = true; }

	// 初始化线程
	virtual bool       InitInstance(){return true;}

	// 退出线程
	virtual int        ExitInstance(){return 0;}

private:
	// 线程句柄
	HANDLE             m_hThread;
	// 线程ID
	UINT               m_nThreadID;
	// 是否正在运行
	bool               m_bIsRunning;

	// 主线程中运行
#ifdef C_RUNTIME_THREAD
	static int         MainThreadProc(CWorkThread *pThread);
#else
	static DWORD WINAPI MainThreadProc( LPVOID lpParam );
#endif
};
#endif