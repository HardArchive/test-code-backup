/*
 *	$Header: $
 *
 *	$History: $
 */
#include "StdAfx.h"
#include "overlappedio.h"

COverlappedIO::COverlappedIO()
{
	m_hIO = INVALID_HANDLE_VALUE;

	//	Initialise our overlapped structure and create the event
	memset(&m_op, 0, sizeof(m_op));
	m_op.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
}

COverlappedIO::~COverlappedIO(void)
{
	CloseHandle(m_op.hEvent);
}

void COverlappedIO::Attach(HANDLE hIO)
{
	assert(hIO != INVALID_HANDLE_VALUE);
	m_hIO = hIO;
}

bool COverlappedIO::Read(BYTE *pbData, DWORD dwBufferSize, LPDWORD pdwBytesRead, HANDLE hStopEvent)
{
	assert(pbData);
	assert(dwBufferSize);
	assert(pdwBytesRead);

	if (m_hIO != INVALID_HANDLE_VALUE)
	{
		::ReadFile(m_hIO, pbData, dwBufferSize, pdwBytesRead, &m_op);

		if (hStopEvent != INVALID_HANDLE_VALUE)
			return Wait(pdwBytesRead, hStopEvent);
		else
			return true;
	}

	//	Something went wrong
	return false;
}

bool COverlappedIO::Write(BYTE *pbData, DWORD dwBufferSize, HANDLE hStopEvent)
{
	assert(pbData);
	assert(dwBufferSize);

	DWORD  dwBytesWritten;

	if (m_hIO != INVALID_HANDLE_VALUE && ::WriteFile(m_hIO, pbData, dwBufferSize, &dwBytesWritten, &m_op))
	{
		if (hStopEvent != INVALID_HANDLE_VALUE)
			return Wait(&dwBytesWritten, hStopEvent);
		else
			return true;
	}

	//	Something went wrong
	return false;
}

bool COverlappedIO::Wait(LPDWORD pdwBytesTransferred, HANDLE hStopEvent)
{
	HANDLE haOverlapped[2] = { m_op.hEvent, hStopEvent };

	if (m_hIO != INVALID_HANDLE_VALUE)
	{
		switch (WaitForMultipleObjects(hStopEvent == INVALID_HANDLE_VALUE ? 1 : 2, haOverlapped, FALSE, INFINITE))
		{
		case WAIT_OBJECT_0:
			//	Signalled on the overlapped event handle, check the result
			if (GetOverlappedResult(m_hIO, &m_op, pdwBytesTransferred, FALSE))
				return true;

		case WAIT_OBJECT_0 + 1:
			//	Signalled to stop, just stop...
			return false;
		}
	}

	return false;
}