/*
 *	$Header: $
 *
 *	$History: $
 */
#pragma once

class COverlappedIO
{
public:
					COverlappedIO(void);
	virtual			~COverlappedIO(void);

	void			Attach(HANDLE hIO);
	OVERLAPPED		*operator&()		{ return &m_op; }

	virtual bool	Read(BYTE *pbData, DWORD dwBufferSize, LPDWORD pdwBytesRead, HANDLE hStopEvent = INVALID_HANDLE_VALUE);
	virtual bool	Write(BYTE *pbData, DWORD dwBufferSize, HANDLE hStopEvent = INVALID_HANDLE_VALUE);

	virtual bool	Wait(LPDWORD pdwBytesTransferred, HANDLE hStopEvent = INVALID_HANDLE_VALUE);

private:
	HANDLE			m_hIO;
	OVERLAPPED		m_op;
};
