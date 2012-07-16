/*******************************************************************************
 * Copyright (C), 2011-2020, 李  照
 * FileName: Trace.cpp
 * Author: 李  照    Version: 1.0    Date: 2012-01-05
 * Description: 工作线程管理类
 * Version: 1.0     // 版本信息
 * Function List:   // 主要函数及其功能
 * 1. 
 * History:         // 历史修改记录
 * <author>   <time>   <version>   <desc>
 *  李  照  2012-01-05    1.0    build this moudle  
 ******************************************************************************/
#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include "ChildProcess.h"

typedef
// 获取进程的主窗口
struct ProcessWindow
{
	DWORD dwProcessId;
	HWND hwndWindow;
}ProcessWindow;

// 查找进程主窗口的回调函数   
BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)   
{   
	ProcessWindow *pProcessWindow = (ProcessWindow *)lParam;   

	DWORD dwProcessId;   
	GetWindowThreadProcessId(hWnd, &dwProcessId);   

	// 判断是否是指定进程的主窗口   
	if (pProcessWindow->dwProcessId == dwProcessId && IsWindowVisible(hWnd) && GetParent(hWnd) == NULL)   
	{   
		pProcessWindow->hwndWindow = hWnd;   

		return FALSE;   
	}   

	return TRUE;   
}  

DWORD CChildProcess::Run(const char* szCmdText, const char* szPath)
{   
	//LPCTSTR lpszFile, LPCTSTR lpszParam;

	DWORD               exitCode = 0; 

	if (m_CmdText[0] != 0)
	{
		TracePrint(LOG_DEBUG, "LZWinService process is running!\r\n");
		return exitCode;
	}

	//////在CreatePipe、CreateProcess等Create系列函数中，   
	//////通常都有一个SECURITY_ATTRIBUTES类型的参数   
	////SECURITY_ATTRIBUTES saAttr = {0};   
	////saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);   
	//////若把该参数的bInheritHandle项设为TRUE，   
	//////则它创建出来的句柄可被子进程继承。   
	//////例如，用CreatePipe创建的管道可用于CreateProcess创建的进程   
	////saAttr.bInheritHandle = TRUE;    
	////saAttr.lpSecurityDescriptor = NULL;   

	////////// hInputRead是子进程的输入句柄，hInputWrite是父进程用于写入子进程输入的句柄   
	////////HANDLE hInputRead, hInputWrite;   
	////CreatePipe(&hInputRead, &hInputWrite, &saAttr, 0);   
	//////设置子进程不能继承接收输入管道的另一端：hInputWrite   
	////SetHandleInformation(hInputWrite, HANDLE_FLAG_INHERIT, 0);   

	STARTUPINFO         sInfo = {0};   
	sInfo.cb              = sizeof(STARTUPINFO);
	sInfo.wShowWindow     = SW_SHOW;
	////sInfo.hStdInput       = hInputRead;

	LPCTSTR lpCurrentDirectory = NULL;
	if (szPath != NULL && szPath[0] != '\0')
	{
		lpCurrentDirectory = szPath;
		strcpy_s(m_Path, sizeof(m_Path), szPath);
	}
	if(CreateProcess(   
		NULL,      //LPCTSTR lpApplicationName, // pointer to name of executable module   
		(LPSTR)szCmdText,   //LPTSTR lpCommandLine,  // pointer to command line string   
		NULL,      //LPSECURITY_ATTRIBUTES lpProcessAttributes,  // process security attributes   
		NULL,      //LPSECURITY_ATTRIBUTES lpThreadAttributes,   // thread security attributes   
		TRUE,     //BOOL bInheritHandles,  // handle inheritance flag   
		CREATE_NO_WINDOW,         //DWORD dwCreationFlags, // creation flags   
		NULL,      //LPVOID lpEnvironment,  // pointer to new environment block   
		lpCurrentDirectory,      //LPCTSTR lpCurrentDirectory,   // pointer to current directory name   
		&sInfo,    //LPSTARTUPINFO lpStartupInfo,  // pointer to STARTUPINFO   
		&pInfo))    //LPPROCESS_INFORMATION lpProcessInformation  // pointer to PROCESS_INFORMATION   
	{   
		strcpy_s(m_CmdText, sizeof(m_CmdText), szCmdText);

		TracePrint(LOG_DEBUG, "LZWinService child process id: %lu\r\n", pInfo.dwProcessId);
		////// 等待新进程初始化完毕
		////WaitForInputIdle(pInfo.hProcess, 5000);
		////ProcessWindow pw;
		////pw.hwndWindow = NULL;
		////pw.dwProcessId = pInfo.dwProcessId;		
		////// 查找主窗口
		////EnumWindows(EnumWindowCallBack, (LPARAM)&pw);
		////m_hwndWindow = pw.hwndWindow;
	}    
	else
	{   
		exitCode = GetLastError();
		TracePrint(LOG_WARNING,  _T("LZWinService Create Child Process failed: %ld\n"), exitCode);   
	}   

	return exitCode;   
}

CChildProcess::CChildProcess()
{
	memset(m_CmdText, 0, sizeof(m_CmdText));
	memset(m_Path, 0, sizeof(m_Path));
	memset(&pInfo, 0, sizeof(pInfo));
}

CChildProcess::~CChildProcess()
{
	////memset(m_CmdText, 0, sizeof(m_CmdText));
	////memset(m_Path, 0, sizeof(m_Path));
	////memset(&pInfo, 0, sizeof(pInfo));
}

int CChildProcess::Stop()
{
	// 等待进程退出
	WaitForSingleObject(pInfo.hProcess, 10000);

	CloseHandle( pInfo.hProcess );
	CloseHandle( pInfo.hThread );

	return 0;
}
