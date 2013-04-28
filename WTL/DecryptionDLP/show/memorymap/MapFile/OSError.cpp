#include "StdAfx.h"
#include "OSError.h"
#include "TimeWrap.h"
#include "Synchronize.h"

bool COSError::m_show_out=true;
FILE* COSError::m_log=NULL;
PLogProc COSError::m_proc=NULL;

void COSError::LogMessage(const TCHAR* msg)
{
	static CLock lock;
	TCHAR txt[512];
	size_t len = CTimeWrap::GetNow(txt, _countof(txt));
	_stprintf_s(txt + len, _countof(txt) - len, _T("  %s%s"), msg, _T("\r\n"));
	lock.Lock();
	try
	{
		if(m_proc!=NULL)
			m_proc(txt);
		 else if(m_show_out)
			 _tprintf_s(_T("%s"), txt);

		if(m_log != NULL)
			_fputts(txt, m_log);
	}
	catch(...)
	{
	}
	lock.UnLock();
}

void COSError::LogMessage(const TCHAR* header, const TCHAR* msg)
{

	TCHAR txt[256];
	_stprintf_s(txt, _countof(txt), _T("%s:%s"), header, msg);
	LogMessage(txt);
}

void COSError::GetOSError(TCHAR* buf, size_t buflen, const TCHAR* pname, const TCHAR* pparam, unsigned errorno)
{
	if(errorno==0)
		errorno = GetLastError();

	TCHAR MsgBuf[512];

	int len = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorno, 
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		MsgBuf, 
		_countof(MsgBuf),
		NULL);

	if(len > 0)
	{
		_stprintf_s(buf, buflen, _T("%s %s failed with error %d: %s"), pname!=NULL? pname:_T(""), pparam!=NULL? pparam:_T(""), errorno, MsgBuf);
		return;
	}
	_stprintf_s(buf, buflen, _T("%s %s failed with error %d: unknown"), pname!=NULL? pname:_T(""), pparam!=NULL? pparam:_T(""), errorno);
}

void COSError::LogOSError(const TCHAR* pname, const TCHAR* pparam, unsigned errorno)
{
	TCHAR buf[512];
	GetOSError(buf, _countof(buf), pname, pparam, errorno);
	LogMessage(buf);
}

void COSError::ThrowOSError(const TCHAR* pname, const TCHAR* pparam, unsigned errorno)
{
	TCHAR buf[512];
	GetOSError(buf, _countof(buf), pname, pparam, errorno);
	LogMessage(buf);
	throw buf; 	
}

bool COSError::CheckError(unsigned errorno)
{
	if(errorno == 0)
	{
		return true;
	}
	TCHAR buf[512];
	GetOSError(buf, _countof(buf), NULL, NULL, errorno);
	LogMessage(buf);
	return false;
}

void COSError::ThrowError(unsigned errorno)
{
	if(errorno == 0)
	{
		return;
	}
	TCHAR buf[512];
	GetOSError(buf, _countof(buf), NULL, NULL, errorno);
	LogMessage(buf);
	throw buf; 
}