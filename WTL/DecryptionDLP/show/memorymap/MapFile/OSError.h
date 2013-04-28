#pragma once

typedef void (*PLogProc)(const TCHAR* msg);

class COSError
{
public:
	static bool m_show_out;
	static FILE* m_log;
	static PLogProc m_proc;
	static void LogMessage(const TCHAR* msg);
	static void LogMessage(const TCHAR* header, const TCHAR* msg);
	static void GetOSError(TCHAR* buf, size_t buflen, const TCHAR* pname, const TCHAR* pparam=NULL, unsigned errorno=0);
	static void LogOSError(const TCHAR* pname, const TCHAR* pparam=NULL, unsigned errorno=0);
	static void ThrowOSError(const TCHAR* pname, const TCHAR* pparam=NULL, unsigned errorno=0);
	static bool CheckError(unsigned errorno);
	static void ThrowError(unsigned errorno);
};
