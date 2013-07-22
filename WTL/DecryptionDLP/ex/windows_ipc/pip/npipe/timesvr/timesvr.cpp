#include "stdafx.h"
#include "..\npipe.h"


//The one and only CWinApp instance
CWinApp theApp;


void _tmain(int /*argc*/, TCHAR* /*argv*/[])
{
  //initialize MFC and error on failure (Note we must put up with some /analyze warnings for AfxWinInit even in VC 2008) 
	if (!AfxWinInit(GetModuleHandle(NULL), NULL, NULL, SW_SHOW))
    return;

  _tprintf(_T("Time Server demo program has started...\n"));

  //Create the server pipe
  CNamedPipe serverPipe;
  try
  {
    serverPipe.Create(_T("TEST_PJTIME"), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_WAIT, 1, 4096, 4096, 1, NULL);

    //Test the state accessor functions
    ///*
    BOOL bblocking = serverPipe.IsBlockingPipe();
    bblocking;
    BOOL bClient = serverPipe.IsClientPipe();
    bClient;
    BOOL bServer = serverPipe.IsServerPipe();
    bServer;
    BOOL bMessage = serverPipe.IsMessagePipe();
    DWORD dwInstances = serverPipe.GetCurrentInstances();
    dwInstances;
    //DWORD dwCollectionCount = serverPipe.GetMaxCollectionCount();
    //DWORD dwCollectionTimeout = serverPipe.GetCollectionTimeout();
    DWORD dwOutboundBufferSize = serverPipe.GetOutboundBufferSize();
    dwOutboundBufferSize;
    DWORD dwInboundBufferSize = serverPipe.GetInboundBufferSize();
    dwInboundBufferSize;
    DWORD dwMaxInstances = serverPipe.GetMaxInstances();
    dwMaxInstances;
    //*/

    //Test the state mutator functions
    //*
    serverPipe.SetMode(FALSE, TRUE);
    bMessage = serverPipe.IsMessagePipe();
    //*/

    while (1)
    {
      serverPipe.ConnectClient();
  
      //Get the current local time
      SYSTEMTIME st;
      memset(&st, 0, sizeof(SYSTEMTIME));
      GetLocalTime(&st);

      //Write the time down the socket
      DWORD dwBytesWritten = serverPipe.Write(&st, sizeof(st));
      dwBytesWritten;

      //Print some details on what we just did
      TCHAR sTime[256];
      sTime[0] = _T('\0');
      GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, sTime, 256);
      TCHAR sDate[256];
      sDate[0] = _T('\0');
      GetDateFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, sDate, 256);
      _tprintf(_T("  Sending time %s %s\n"), sDate, sTime);

      serverPipe.Flush(); //Flush the pipe before we disconnect the client

      serverPipe.DisconnectClient();
      _tprintf(_T("  Disconnecting\n"));
    }
  }
  catch(CNamedPipeException* pEx)
  {
    _tprintf(_T("Named Pipe Exception, Error:%d, Description:%s\n"), pEx->m_dwError, pEx->GetErrorMessage().operator LPCTSTR());
    pEx->Delete();
  }
}


