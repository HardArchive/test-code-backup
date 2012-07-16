/*******************************************************************************
 * Copyright (C), 2011-2020, ��  ��
 * FileName: Trace.cpp
 * Author: ��  ��    Version: 1.0    Date: 2012-01-05
 * Description: �����̹߳�����
 * Version: 1.0     // �汾��Ϣ
 * Function List:   // ��Ҫ�������书��
 * 1. 
 * History:         // ��ʷ�޸ļ�¼
 * <author>   <time>   <version>   <desc>
 *  ��  ��  2012-01-05    1.0    build this moudle  
 ******************************************************************************/
#include "stdafx.h"
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include "ChildProcess.h"

typedef
// ��ȡ���̵�������
struct ProcessWindow
{
	DWORD dwProcessId;
	HWND hwndWindow;
}ProcessWindow;

// ���ҽ��������ڵĻص�����   
BOOL CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)   
{   
	ProcessWindow *pProcessWindow = (ProcessWindow *)lParam;   

	DWORD dwProcessId;   
	GetWindowThreadProcessId(hWnd, &dwProcessId);   

	// �ж��Ƿ���ָ�����̵�������   
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

	//////��CreatePipe��CreateProcess��Createϵ�к����У�   
	//////ͨ������һ��SECURITY_ATTRIBUTES���͵Ĳ���   
	////SECURITY_ATTRIBUTES saAttr = {0};   
	////saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);   
	//////���Ѹò�����bInheritHandle����ΪTRUE��   
	//////�������������ľ���ɱ��ӽ��̼̳С�   
	//////���磬��CreatePipe�����Ĺܵ�������CreateProcess�����Ľ���   
	////saAttr.bInheritHandle = TRUE;    
	////saAttr.lpSecurityDescriptor = NULL;   

	////////// hInputRead���ӽ��̵���������hInputWrite�Ǹ���������д���ӽ�������ľ��   
	////////HANDLE hInputRead, hInputWrite;   
	////CreatePipe(&hInputRead, &hInputWrite, &saAttr, 0);   
	//////�����ӽ��̲��ܼ̳н�������ܵ�����һ�ˣ�hInputWrite   
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
		////// �ȴ��½��̳�ʼ�����
		////WaitForInputIdle(pInfo.hProcess, 5000);
		////ProcessWindow pw;
		////pw.hwndWindow = NULL;
		////pw.dwProcessId = pInfo.dwProcessId;		
		////// ����������
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
	// �ȴ������˳�
	WaitForSingleObject(pInfo.hProcess, 10000);

	CloseHandle( pInfo.hProcess );
	CloseHandle( pInfo.hThread );

	return 0;
}
