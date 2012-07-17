#ifndef __KSimpleLog_h__
#define __KSimpleLog_h__

#include <list>
#include <string>
#include <Windows.h>

class KSimpleLog
{
public:
	KSimpleLog();
	~KSimpleLog();

	BOOL Init(void);
	void Uninit(void);

	int LogA(LPCSTR szFormat, ...);

protected:
	static DWORD WINAPI LogThreadFunc(LPVOID p);
	DWORD RealLogFunc(void);
	BOOL OpenLogFile(void);
	void WriteLogFile(LPCSTR szLog, size_t sSize);
	void CloseLogFile(void);
	DWORD GetFileSize(LPCSTR szPathName);
	BOOL MoveFile_Replace(LPCSTR szDstPathName, LPCSTR szSrcPathName);
	BOOL CheckLogFileSize(void);
	BOOL GetLogFilePath(void);

private:
	BOOL m_bExit;
	FILE* m_fpLogFile;
	HANDLE m_hEnableLogEvent;
	HANDLE m_hNotifyLogEvent;
	HANDLE m_hLogThread;
	std::string m_strLogFile;
	CRITICAL_SECTION m_csLogData;
	std::list<std::string> m_lstLogData;
};

extern KSimpleLog g_logInst;

#define INIT_LOG() g_logInst.Init()
#define UNINIT_LOG() g_logInst.Uninit()
#define LOGA(format, ...) g_logInst.LogA(format, __VA_ARGS__)

#endif