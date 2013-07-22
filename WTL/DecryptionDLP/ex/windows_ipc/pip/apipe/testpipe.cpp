#include "stdafx.h"
#include "Apipe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//The one and only application object
//CWinApp theApp;

int _tmain(int /*argc*/, TCHAR* /*argv*/[], TCHAR* /*envp*/[])
{
	//initialize MFC and print and error on failure (Note we must put up with some /analyze warnings for AfxWinInit even in VC 2008)
	//if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	//	return 1;

	CAnonymousPipe pipe;
	pipe.Create();
	BOOL bOpen = pipe.IsOpen();
	bOpen;
	static char* szWriteBuffer = "Mary had a little lamb";
	DWORD dwBytesWritten = 0;
	BOOL bWrite = pipe.Write(szWriteBuffer, static_cast<DWORD>(strlen(szWriteBuffer)), dwBytesWritten);
	bWrite;
	HANDLE hWrite = pipe.GetWriteHandle();
	hWrite;
	HANDLE hRead = pipe.GetReadHandle();
	hRead;
	char szReadBuffer[1024];
	memset(szReadBuffer, 0, sizeof(szReadBuffer));
	DWORD dwBytesRead = 0;
	DWORD dwTotalBytesAvailable = 0;
	DWORD dwBytesLeftThisMessage = 0;
	BOOL bRead = pipe.Peek(szReadBuffer, sizeof(szReadBuffer), &dwBytesRead, &dwTotalBytesAvailable, &dwBytesLeftThisMessage);
	bRead = pipe.Read(szReadBuffer, sizeof(szReadBuffer), dwBytesRead);

	return 0;
}  