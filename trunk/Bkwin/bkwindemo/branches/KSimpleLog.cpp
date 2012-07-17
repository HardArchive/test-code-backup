#include "stdafx.h"
#include "KSimpleLog.h"

#define defLogFileName	"kavsetup.log"
#define defEnableLog	1
#define defLogEventName	_T("Global\\BFD88F2D-0990-4de4-AD0F-764F5894389A-%d")

KSimpleLog g_logInst;
KSimpleLog::KSimpleLog()
{
	m_bExit = FALSE;
	m_hEnableLogEvent = NULL;
	m_hNotifyLogEvent = NULL;
	m_hLogThread = NULL;
	m_fpLogFile = NULL;
}

KSimpleLog::~KSimpleLog()
{
	this->Uninit();
}

BOOL KSimpleLog::Init()
{
	BOOL bReturn = FALSE;
	SECURITY_DESCRIPTOR sd = { 0 };
	SECURITY_ATTRIBUTES sa = { 0 };
	TCHAR szEnableEventName[100] = {0};

	GetLogFilePath();
	CheckLogFileSize();

	::InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	::SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = &sd;

	::InitializeCriticalSection(&m_csLogData);
	_stprintf(szEnableEventName, defLogEventName, ::GetCurrentProcessId());
	m_hEnableLogEvent = ::CreateEvent(&sa, TRUE, defEnableLog, szEnableEventName);
	m_hNotifyLogEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	if (!m_hEnableLogEvent || !m_hNotifyLogEvent)
		return FALSE;

	m_hLogThread = ::CreateThread(NULL, 0, LogThreadFunc, this, 0, NULL);
	if (!m_hLogThread) return FALSE;

	bReturn = TRUE;

Exit0:
	return bReturn;
}

void KSimpleLog::Uninit()
{
	m_bExit = TRUE;

	if (m_hLogThread)
	{
		if (m_hNotifyLogEvent)
			::SetEvent(m_hNotifyLogEvent);

		WaitForSingleObject(m_hLogThread, INFINITE);
	}

	if (m_hNotifyLogEvent)
	{
		CloseHandle(m_hNotifyLogEvent);
		m_hNotifyLogEvent = NULL;
	}

	if (m_hLogThread)
	{
		CloseHandle(m_hLogThread);
		m_hLogThread = NULL;
	}
}

#define defMaxBufferLen 500
int KSimpleLog::LogA(LPCSTR szFormat, ...)
{
	int n_return = 0;

	if (WAIT_OBJECT_0 == WaitForSingleObject(m_hEnableLogEvent, 0))
	{
		va_list arglist;
		char time_buffer[50] = {0};
		char log_buffer[defMaxBufferLen] = {0};
		
		va_start(arglist, szFormat);
		n_return = _vsnprintf(log_buffer, defMaxBufferLen - 1, szFormat, arglist);
		va_end(arglist);

		SYSTEMTIME sys_time;
		::GetLocalTime(&sys_time);
		
		sprintf(time_buffer, "%d-%d-%d %02d:%02d:%02d %03d  ", 
			sys_time.wYear, sys_time.wMonth, sys_time.wDay, sys_time.wHour, sys_time.wMinute, sys_time.wSecond, sys_time.wMilliseconds);

		std::string strLog;
		strLog = time_buffer;
		strLog += log_buffer;

		::EnterCriticalSection(&m_csLogData);
		m_lstLogData.push_back(strLog);
		::LeaveCriticalSection(&m_csLogData);

		::SetEvent(m_hNotifyLogEvent);
	}

	return n_return;
}

DWORD WINAPI KSimpleLog::LogThreadFunc(LPVOID p)
{
	KSimpleLog* pThis = (KSimpleLog*)p;

	return pThis->RealLogFunc();
}

DWORD KSimpleLog::RealLogFunc(void)
{
	DWORD dwWaitRet = 0;
	std::list<std::string> lstLog;
	do 
	{
		dwWaitRet = WaitForSingleObject(m_hNotifyLogEvent, INFINITE);
		if (dwWaitRet != WAIT_OBJECT_0) break;	//error

		do 
		{
			::EnterCriticalSection(&m_csLogData);
			lstLog.swap(m_lstLogData);
			::ResetEvent(m_hNotifyLogEvent);
			::LeaveCriticalSection(&m_csLogData);

			if (lstLog.empty()) break;

			if (OpenLogFile())
			{
				std::string strLog;
				while (!lstLog.empty())
				{
					strLog = lstLog.front();
					lstLog.pop_front();

					WriteLogFile(strLog.c_str(), strLog.size());
				}
				CloseLogFile();
			}

		} while (TRUE);
		
		if (m_bExit) break;	//write all log to file, then exit
		
	} while (TRUE);

	return 0;
}

BOOL KSimpleLog::OpenLogFile(void)
{
	if (!m_fpLogFile)
	{
		m_fpLogFile = fopen(m_strLogFile.c_str(), "ab");
	}

	return m_fpLogFile != NULL;
}

void KSimpleLog::WriteLogFile(LPCSTR szLog, size_t sSize)
{
	if (m_fpLogFile)
	{
		printf(szLog);
		printf("\n");
		fwrite(szLog, 1, sSize, m_fpLogFile);
		fwrite("\r\n", 1, 2, m_fpLogFile);
	}
}

void KSimpleLog::CloseLogFile(void)
{
	if (m_fpLogFile)
	{
		fclose(m_fpLogFile);
		m_fpLogFile = NULL;
	}
}

DWORD KSimpleLog::GetFileSize(LPCSTR szPathName)
{
	DWORD dwFileSize = 0;
	HANDLE hFile = ::CreateFileA(
		szPathName, 
		GENERIC_READ, 
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	dwFileSize = ::GetFileSize(hFile, NULL);
	if (dwFileSize == INVALID_FILE_SIZE)
	{
		dwFileSize = 0;
	}

	CloseHandle(hFile);

	return dwFileSize;
}

BOOL KSimpleLog::MoveFile_Replace(LPCSTR szDstPathName, LPCSTR szSrcPathName)
{
	::SetFileAttributesA(szDstPathName, 0);
	::DeleteFileA(szDstPathName);
	::SetFileAttributesA(szSrcPathName, 0);

	return ::MoveFileA(szSrcPathName, szDstPathName);
}

BOOL KSimpleLog::CheckLogFileSize(void)
{
	const DWORD dwMaxLogFileSize = 512 * 1024;

	DWORD dwFileSize = GetFileSize(m_strLogFile.c_str());
	if (dwFileSize >= dwMaxLogFileSize)
	{
		std::string strFileName(m_strLogFile);
		strFileName += "0";
		MoveFile_Replace(strFileName.c_str(), m_strLogFile.c_str());
	}

	return TRUE;
}

BOOL KSimpleLog::GetLogFilePath(void)
{
	char szPath[MAX_PATH] = {0};

	::GetModuleFileNameA((HMODULE)&__ImageBase, szPath, MAX_PATH);

	::PathRemoveFileSpecA(szPath);
	::PathAddBackslashA(szPath);

	m_strLogFile = szPath;
	//m_strLogFile += "log\\";
	m_strLogFile += defLogFileName;

	return TRUE;
}