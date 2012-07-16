/*******************************************************************************
 * Copyright (C), 2011-2020, 李  照
 * FileName: Trace.h
 * Author: 李  照    Version: 1.0    Date: 2012-01-05
 * Description: 工作线程管理类
 * Version: 1.0     // 版本信息
 * Function List:   // 主要函数及其功能
 * 1. 
 * History:         // 历史修改记录
 * <author>   <time>   <version>   <desc>
 *  李  照  2012-01-05    1.0    build this moudle  
 ******************************************************************************/
#pragma once

#ifndef CHILD_PROCESS_H_
#define CHILD_PROCESS_H_

class CChildProcess
{
public:
	DWORD Run(const char* szCmdText, const char* szPath);

	// 停止工作
	int Stop();

	// 构造函数
	CChildProcess();
	// 析构函数
	~CChildProcess();
public:
	// 运行进程住处
	PROCESS_INFORMATION pInfo;
private:
	// 命令行
	char m_CmdText[1024];
	// 运行目录
	char m_Path[MAX_PATH];
};


#endif