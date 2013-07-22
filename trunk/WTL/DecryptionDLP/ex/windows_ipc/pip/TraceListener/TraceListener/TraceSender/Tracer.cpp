//---------------------------------------------------------------------------
//
//	"Tracer.cpp"
//
//  Puts trace data in a named pipe
//
//	by Holger Kloos
//
//---------------------------------------------------------------------------

#include "stdafx.h"
#include "Tracer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const char* sPipeName = "\\\\.\\pipe\\HolgersTracer";



CTracer::CTracer()
{
	m_hPipe = INVALID_HANDLE_VALUE;
}

CTracer::~CTracer()
{
	if (m_hPipe != INVALID_HANDLE_VALUE)
		CloseHandle(m_hPipe);

}

bool CTracer::Connect()
{
	m_hPipe = CreateFile(sPipeName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	return m_hPipe != INVALID_HANDLE_VALUE;
}

bool CTracer::MakeTrace(const char* pStr, ...) const
{
	if (m_hPipe == INVALID_HANDLE_VALUE)
		return false;

	char buffer[1024];

	va_list argList;
	va_start(argList, pStr);
	
	DWORD nCount = _vsnprintf(buffer, sizeof(buffer), pStr, argList);

	va_end(argList);

	if (nCount <= 0)
		return false;

	DWORD nWritten = 0;
	return WriteFile(m_hPipe, (BYTE*)buffer, nCount, &nWritten, NULL) != FALSE && nWritten == nCount;
}

