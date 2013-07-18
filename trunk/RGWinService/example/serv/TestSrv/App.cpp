#include "stdafx.h"
#include "ntserv_msg.h"
#include "ntservEventLog.h"
#include "app.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//The one and only one application
CWinApp theApp;

#ifdef _DEBUG
BOOL CALLBACK EnumServices(void* /*pData*/, ENUM_SERVICE_STATUS& Service)
#else
BOOL CALLBACK EnumServices(void* /*pData*/, ENUM_SERVICE_STATUS& /*Service*/)
#endif
{
#ifdef _DEBUG
  TRACE(_T("Service name is %s\n"), Service.lpServiceName);
  TRACE(_T("Friendly name is %s\n"), Service.lpDisplayName);
#endif

  return TRUE; //continue enumeration
}


int _tmain(int /*argc*/, TCHAR* /*argv*/[], TCHAR* /*envp*/[])
{
  //Don't forget to explicitly initialize MFC, since we are in a console app (Note we must put up with some /analyze warnings for AfxWinInit even in VC 2008)
  if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
    return 1;

  //Uncomment the following code to test the persistance functions
  /* 
  DWORD dwError;
  CMyService testService;
  UINT nValue = testService.GetProfileInt(_T("General"), _T("IntValue"), 33, &dwError);
  CString sValue = testService.GetProfileString(_T("General"), _T("StringValue"), _T("DefaultValue"), &dwError);

  BOOL bSuccess = testService.WriteProfileInt(_T("General"), _T("IntValue"), 37);
  bSuccess = testService.WriteProfileString(_T("General"), _T("StringValue"), _T("Test Value"));

  nValue = testService.GetProfileInt(_T("General"), _T("IntValue"), 33, &dwError);
  sValue = testService.GetProfileString(_T("General"), _T("StringValue"), NULL, &dwError);

  CStringArray array;
  array.SetAtGrow(0, _T("First String"));
  array.SetAtGrow(1, _T("Second and Last String"));
  bSuccess = testService.WriteProfileStringArray(_T("General"), _T("StringArrayValue"), array);
  bSuccess = testService.GetProfileStringArray(_T("General"), _T("StringArrayValue"), array);
  bSuccess = testService.GetProfileStringArray(_T("General"), _T("Does Not Exist"), array);

  BYTE* pSetData = new BYTE[10];
  for (int i=0; i<10; i++)
    pSetData[i] = (BYTE) i;
	bSuccess = testService.WriteProfileBinary(_T("General"), _T("BinaryValue"), pSetData, 10);
  delete [] pSetData;

  array.SetSize(0);
  bSuccess = testService.GetProfileStringArray(_T("General"), _T("StringArrayValue"), array); 

  BYTE* pGetData = NULL;
  ULONG nBytes;
	bSuccess = testService.GetProfileBinary(_T("General"), _T("BinaryValue"), &pGetData, &nBytes);
  if (bSuccess)
  {
    delete [] pGetData;
  }

  testService.WriteProfileString(_T("General"), _T("BinaryValue"), NULL);
  testService.WriteProfileString(_T("General"), NULL, NULL);
  */

  //Uncomment the following to test out the instance enumeration function
  /*
  {
  CMyService testService2;
  CStringArray instances;
  DWORD dwError2;
  testService2.EnumerateInstances(instances, dwError2);
  for (int i=0; i<instances.GetSize(); i++)
    TRACE(_T("Found instance, %s\n"), instances.GetAt(i).operator LPCTSTR());
  }
  */

  //Uncomment the following code to test the CNTServiceControlManager
  //and the CNTScmService classes
  /*
  CNTServiceControlManager manager;
  SC_HANDLE hScm = manager;
  BOOL bSuccess2 = manager.Open(NULL, SC_MANAGER_CONNECT | SC_MANAGER_QUERY_LOCK_STATUS | SC_MANAGER_LOCK); //We only need SC_MANAGER_CONNECT, SC_MANAGER_LOCK_STATUS and SC_MANAGER_LOCK access here
  hScm = manager;
  LPQUERY_SERVICE_LOCK_STATUS lpLockStatus = NULL;
  bSuccess2 = manager.QueryLockStatus(lpLockStatus);
  if (bSuccess2)
  {
    BOOL bIsLocked = (lpLockStatus->fIsLocked != 0);
    bIsLocked;
    LPCTSTR lpszOwner = lpLockStatus->lpLockOwner;
    lpszOwner;
    DWORD dwDuration = lpLockStatus->dwLockDuration;
    dwDuration;
  }
  if (lpLockStatus)
    delete [] reinterpret_cast<BYTE*>(lpLockStatus);
  bSuccess2 = manager.Lock();
  lpLockStatus = NULL;
  bSuccess2 = manager.QueryLockStatus(lpLockStatus);
  if (bSuccess2)
  {
    BOOL bIsLocked = (lpLockStatus->fIsLocked != 0);
    bIsLocked;
    LPCTSTR lpszOwner = lpLockStatus->lpLockOwner;
    lpszOwner;
    DWORD dwDuration = lpLockStatus->dwLockDuration;
    dwDuration;
  }
  if (lpLockStatus)
    delete [] reinterpret_cast<BYTE*>(lpLockStatus);
  bSuccess2 = manager.EnumServices(SERVICE_WIN32, SERVICE_STATE_ALL, 0, EnumServices);
  CNTScmService service;
  SC_HANDLE hService = service;
  bSuccess2 = manager.OpenService(_T("PJSERVICE"), SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_QUERY_CONFIG | SERVICE_ENUMERATE_DEPENDENTS, service); //Use only the required access rights we need
  if (bSuccess2)
  {
    hService = service;
    //bSuccess2 = service.Start(0, NULL);
    //Sleep(4000);
    //bSuccess2 = service.Control(128);
    //bSuccess2 = service.Pause();
    //Sleep(4000);
    //bSuccess2 = service.Continue();
    BOOL bStop;
    bSuccess2 = service.AcceptStop(bStop);
    BOOL bPauseContinue;
    bSuccess2 = service.AcceptPauseContinue(bPauseContinue);
    BOOL bShutdown;
    bSuccess2 = service.AcceptShutdown(bShutdown);
    bSuccess2 = service.Stop();
    LPQUERY_SERVICE_CONFIG lpServiceConfig = NULL;
    bSuccess2 = service.QueryConfig(lpServiceConfig);
    if (bSuccess2)
    {
      TRACE(_T("dwServiceType is %d\n"),    lpServiceConfig->dwServiceType);
      TRACE(_T("dwStartType is %d\n"),      lpServiceConfig->dwStartType);
      TRACE(_T("dwErrorControl is %d\n"),   lpServiceConfig->dwErrorControl);
      TRACE(_T("lpBinaryPathName is %s\n"), lpServiceConfig->lpBinaryPathName);
      TRACE(_T("lpLoadOrderGroup is %s\n"), lpServiceConfig->lpLoadOrderGroup);
      TRACE(_T("dwTagId is %d\n"),          lpServiceConfig->dwTagId);
      TRACE(_T("lpDependencies are %s\n"),  lpServiceConfig->lpDependencies);
      TRACE(_T("lpServiceStartName is %s\n"), lpServiceConfig->lpServiceStartName);
      TRACE(_T("lpDisplayName is %s\n"), lpServiceConfig->lpDisplayName);
    }
    if (lpServiceConfig)
      delete [] reinterpret_cast<BYTE*>(lpServiceConfig);
    bSuccess2 = service.EnumDependents(SERVICE_STATE_ALL, 0, EnumServices);

    PSECURITY_DESCRIPTOR lpSecurityDescriptor = NULL;
    bSuccess2 = service.QueryObjectSecurity(DACL_SECURITY_INFORMATION, lpSecurityDescriptor);
    if (lpSecurityDescriptor)
      delete [] reinterpret_cast<BYTE*>(lpSecurityDescriptor);

    CString sDescription;
    bSuccess2 = service.QueryDescription(sDescription);

    LPSERVICE_FAILURE_ACTIONS pFailureActions = NULL;
    bSuccess2 = service.QueryFailureActions(pFailureActions);
    if (pFailureActions)
      delete [] reinterpret_cast<BYTE*>(pFailureActions);

    bSuccess2 = service.ChangeDelayAutoStart(TRUE);
    BOOL bDelayedAutoStart;
    bSuccess2 = service.QueryDelayAutoStart(bDelayedAutoStart);

    bSuccess2 = service.ChangeFailureActionsFlag(TRUE);
    BOOL bFailureActionsOnNonCrashFailures;
    bSuccess2 = service.QueryFailureActionsFlag(bFailureActionsOnNonCrashFailures);

    bSuccess2 = service.ChangeSidInfo(3); //3 is SERVICE_SID_TYPE_RESTRICTED
    DWORD dwServiceSidType;
    bSuccess2 = service.QuerySidInfo(dwServiceSidType);

    CStringArray sPrivileges;
    sPrivileges.Add(_T("SeIncreaseQuotaPrivilege"));
    bSuccess2 = service.ChangeRequiredPrivileges(sPrivileges);
    bSuccess2 = service.QueryRequiredPrivileges(sPrivileges);

    bSuccess2 = service.ChangePreferredNode(2, FALSE);
    USHORT usPreferredNode;
    BOOL bDelete;
    bSuccess2 = service.QueryPreferredNode(usPreferredNode, bDelete);

    PSERVICE_TRIGGER_INFO pTriggerInfo = NULL;
    bSuccess2 = service.QueryTrigger(pTriggerInfo);
    if (pTriggerInfo)
      delete [] reinterpret_cast<BYTE*>(pTriggerInfo);

    SERVICE_CONTROL_STATUS_REASON_PARAMS scsrp;
    bSuccess2 = service.Control(SERVICE_STOP, SERVICE_CONTROL_STATUS_REASON_INFO, &scsrp);

    SERVICE_STATUS_PROCESS ssp;
    bSuccess2 = service.QueryStatus(ssp);
  }
  */

  //Uncomment the following code to test the CNTEventLog 
  //and the CEventLogRecord classes
  /*
  CNTEventLog el;
  HANDLE hEventLog = el;
  BOOL bSuccess3 = el.OpenSecurity(NULL);
  hEventLog = el;
  bSuccess3 = el.OpenSystem(NULL);
  bSuccess3 = el.OpenApplication(NULL);
  DWORD dwOldest;
  bSuccess3 = el.GetOldestRecord(dwOldest);
  bSuccess3 = el.Backup(_T("C:\\BACKUPLOG.TXT"));
  bSuccess3 = el.Close();
  bSuccess3 = el.OpenBackup(NULL, _T("C:\\BACKUPLOG.TXT"));
  DWORD dwRecords;
  bSuccess3 = el.OpenSystem(NULL);
  bSuccess3 = el.GetNumberOfRecords(dwRecords);
  //bSuccess = el.Clear(NULL);
  CEventLogRecord record;
  DWORD ii=0;
  while (ii<dwRecords)
  {
    bSuccess3 = el.ReadNext(record);
    ii++;
  }
  bSuccess3 = el.ReadPrev(record);
  DWORD dwFull;
  bSuccess3 = el.GetFullInformation(dwFull);
  */

  //Uncomment the following code to test the 
  //CNTEventLogSource classes
  /*
  CNTEventLogSource els(NULL, _T("PJ's Demo NT Service"));
  LPCTSTR pString = _T("PJ's Demo NT Service");
  LPCTSTR* ppString = &pString;
  BOOL bSuccess4 = els.Register(NULL, pString);
  bSuccess4 = els.Deregister();
  DWORD dwError2 = GetLastError();
  dwError2;
  bSuccess4 = els.Report(EVENTLOG_INFORMATION_TYPE, 0, CNTS_MSG_SERVICE_STARTED, NULL, 1, 0, ppString, NULL);
  */

  //All that is required to get the service up and running
	CNTServiceCommandLineInfo cmdInfo;
  CMyService Service;
  Service.SetAllowCommandLine(TRUE);
  Service.SetAllowNameChange(TRUE);       
  Service.SetAllowDescriptionChange(TRUE);
  Service.SetAllowDisplayNameChange(TRUE);
	Service.ParseCommandLine(cmdInfo);
	return Service.ProcessShellCommand(cmdInfo);
}



CMyService::CMyService() : CNTService(_T("PJSERVICE"), _T("PJ's Demo NT Service"), SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE, _T("Description text for PJ's Demo Service")),
	                         m_lWantStop(FALSE),    //Simple boolean which is set to request the service to stop
                           m_lPaused(FALSE),      //The boolean used to represent that the service is paused
                           m_dwBeepInternal(1000) //The default beep interval of 1 second
{
  m_sDependencies.Add(_T("AudioSrv")); //For demonstration purposes, make this service dependent on Windows Audio. Please note that because of Session Isolation on Windows Vista and
                                       //later, when you start the "PJSERVICE" service, you will not hear any message beeps because the service is running in a different session
  
  //Uncomment the following code (or put it in the member variable initialization above) to setup the service with different requirements
  /*
  m_dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  m_dwStartType = SERVICE_AUTO_START;
  m_dwErrorControl = SERVICE_ERROR_NORMAL;
  m_sUserName = _T(".\\auser");
  m_sPassword = _T("apassword");
  */
}

void CMyService::ServiceMain(DWORD /*dwArgc*/, LPTSTR* /*lpszArgv*/)
{
	//Pretend that starting up takes some time
  ReportStatus(SERVICE_START_PENDING, 1, 1100);
	Sleep(1000);
  ReportStatus(SERVICE_RUNNING);

  //Report to the event log that the service has started successfully
  m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, CNTS_MSG_SERVICE_STARTED, m_sDisplayName);

	//The tight loop which constitutes the service
  long lOldPause = m_lPaused;
	while (!m_lWantStop)
	{
		//As a demo, we just do a message beep
    if (!m_lPaused)
      MessageBeep(MB_OK);

    //Wait for the specified time
	  Sleep(m_dwBeepInternal);

    //SCM has requested a Pause / Continue
    if (m_lPaused != lOldPause)
    {
      if (m_lPaused)
      {
   	    ReportStatus(SERVICE_PAUSED);

        //Report to the event log that the service has paused successfully
        m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, CNTS_MSG_SERVICE_PAUSED, m_sDisplayName);
      }
      else
      {
        ReportStatus(SERVICE_RUNNING);

        //Report to the event log that the service has stopped continued
        m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, CNTS_MSG_SERVICE_CONTINUED, m_sDisplayName);
      }
    }

    lOldPause = m_lPaused;
	}

	//Pretend that closing down takes some time
  ReportStatus(SERVICE_STOP_PENDING, 1, 1100);
	Sleep(1000);

  //Report to the event log that the service has stopped successfully
  m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, CNTS_MSG_SERVICE_STOPPED, m_sDisplayName);
}

void CMyService::OnStop()
{
	//Signal the other thread to end via the boolean variable
	InterlockedExchange(&m_lWantStop, TRUE);
}

void CMyService::OnPause()
{
	//Signal the other thread via the boolean variable
  InterlockedExchange(&m_lPaused, TRUE);
}

void CMyService::OnContinue()
{
	//Signal the other thread to continue via the boolean variable
  InterlockedExchange(&m_lPaused, FALSE);
}

void CMyService::OnUserDefinedRequest(DWORD dwControl)
{
  //Any value greater than 200 increments the doubles the beep frequency
  //otherwise the frequency is halved
  if (dwControl > 200)
    m_dwBeepInternal /= 2;
  else
    m_dwBeepInternal *= 2;

  //Report to the event log that the beep interval has been changed
  CString sInterval;
  sInterval.Format(_T("%d"), m_dwBeepInternal);
  m_EventLogSource.Report(EVENTLOG_INFORMATION_TYPE, MSG_SERVICE_SET_FREQUENCY, sInterval);
}

void CMyService::ShowHelp()
{
	_tprintf(_T("A demo service which just beeps the speaker\nUsage: testsrv [ -install | -uninstall | -remove | -start | -pause | -continue | -stop | -help | -? ]\n"));
}
