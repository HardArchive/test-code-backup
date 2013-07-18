/*
Module : NTSERVSCMSERVICE.H
Purpose: Defines the interface for the class CNTScmService.
Created: PJN / 14-07-1998

Copyright (c) 1997 - 2010 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////////////// Macros / Defines //////////////////////////////

#pragma once

#ifndef __NTSERVSCMSERVICE_H__
#define __NTSERVSCMSERVICE_H__

#ifndef CNTSERVICE_EXT_CLASS
#define CNTSERVICE_EXT_CLASS
#endif

//Various defines from WinSvc.h to allow code to compile without the need for the latest Windows SDK
__if_not_exists (PFN_SC_NOTIFY_CALLBACK)
{
  typedef VOID ( CALLBACK * PFN_SC_NOTIFY_CALLBACK ) (__in PVOID pParameter);
}

__if_not_exists (SERVICE_NOTIFY_1)
{
  typedef struct _SERVICE_NOTIFY_1 
  {
    DWORD                   dwVersion;
    PFN_SC_NOTIFY_CALLBACK  pfnNotifyCallback;
    PVOID                   pContext;
    DWORD                   dwNotificationStatus;
    SERVICE_STATUS_PROCESS  ServiceStatus;
  } SERVICE_NOTIFY_1, *PSERVICE_NOTIFY_1;
}

__if_not_exists (SERVICE_NOTIFY_2A)
{
  typedef struct _SERVICE_NOTIFY_2A 
  {
    DWORD                   dwVersion;
    PFN_SC_NOTIFY_CALLBACK  pfnNotifyCallback;
    PVOID                   pContext;
    DWORD                   dwNotificationStatus;
    SERVICE_STATUS_PROCESS  ServiceStatus;
    DWORD                   dwNotificationTriggered;
    LPSTR                   pszServiceNames;
  } SERVICE_NOTIFY_2A, *PSERVICE_NOTIFY_2A;
}

__if_not_exists (SERVICE_NOTIFY_2W)
{
  typedef struct _SERVICE_NOTIFY_2W 
  {
    DWORD                   dwVersion;
    PFN_SC_NOTIFY_CALLBACK  pfnNotifyCallback;
    PVOID                   pContext;
    DWORD                   dwNotificationStatus;
    SERVICE_STATUS_PROCESS  ServiceStatus;
    DWORD                   dwNotificationTriggered;
    LPWSTR                  pszServiceNames;
  } SERVICE_NOTIFY_2W, *PSERVICE_NOTIFY_2W;
}

#ifdef UNICODE
__if_not_exists (SERVICE_NOTIFY_2)
{
  typedef SERVICE_NOTIFY_2W SERVICE_NOTIFY_2;
}
__if_not_exists (PSERVICE_NOTIFY_2)
{
  typedef PSERVICE_NOTIFY_2W PSERVICE_NOTIFY_2;
}
#else
__if_not_exists (SERVICE_NOTIFY_2)
{
  typedef SERVICE_NOTIFY_2A SERVICE_NOTIFY_2;
}
__if_not_exists (PSERVICE_NOTIFY_2)
{
  typedef PSERVICE_NOTIFY_2A PSERVICE_NOTIFY_2;
}
#endif // UNICODE
__if_not_exists (SERVICE_NOTIFYA)
{
  typedef SERVICE_NOTIFY_2A SERVICE_NOTIFYA, *PSERVICE_NOTIFYA;
}
__if_not_exists (SERVICE_NOTIFYW)
{
  typedef SERVICE_NOTIFY_2W SERVICE_NOTIFYW, *PSERVICE_NOTIFYW;
}
#ifdef UNICODE
__if_not_exists (SERVICE_NOTIFY)
{
  typedef SERVICE_NOTIFYW SERVICE_NOTIFY;
}
__if_not_exists (PSERVICE_NOTIFY)
{
  typedef PSERVICE_NOTIFYW PSERVICE_NOTIFY;
}
#else
__if_not_exists (SERVICE_NOTIFY)
{
  typedef SERVICE_NOTIFYA SERVICE_NOTIFY;
}
__if_not_exists (PSERVICE_NOTIFY)
{
  typedef PSERVICE_NOTIFYA PSERVICE_NOTIFY;
}
#endif // UNICODE

__if_not_exists (SERVICE_TRIGGER_SPECIFIC_DATA_ITEM)
{
  typedef struct _SERVICE_TRIGGER_SPECIFIC_DATA_ITEM
  {
      DWORD   dwDataType; // Data type -- one of SERVICE_TRIGGER_DATA_TYPE_* constants
  #ifdef __midl
      [range(0, 1024)]
  #endif
      DWORD   cbData;     // Size of trigger specific data
  #ifdef __midl
      [size_is(cbData)]
  #endif
      PBYTE   pData;      // Trigger specific data
  } SERVICE_TRIGGER_SPECIFIC_DATA_ITEM, *PSERVICE_TRIGGER_SPECIFIC_DATA_ITEM;
}

__if_not_exists (SERVICE_TRIGGER)
{
  typedef struct _SERVICE_TRIGGER
  {
      DWORD                       dwTriggerType;              // One of SERVICE_TRIGGER_TYPE_* constants
      DWORD                       dwAction;                   // One of SERVICE_TRIGGER_ACTION_* constants
      GUID    *                   pTriggerSubtype;            // Provider GUID if the trigger type is SERVICE_TRIGGER_TYPE_CUSTOM
                                                              // Device class interface GUID if the trigger type is
                                                              // SERVICE_TRIGGER_TYPE_DEVICE_INTERFACE_ARRIVAL
  #ifdef __midl
      [range(0, 64)]
  #endif
      DWORD                       cDataItems;                 // Number of data items in pDataItems array
  #ifdef __midl
      [size_is(cDataItems)]
  #endif
      PSERVICE_TRIGGER_SPECIFIC_DATA_ITEM  pDataItems;       // Trigger specific data
  } SERVICE_TRIGGER, *PSERVICE_TRIGGER;
}

__if_not_exists (SERVICE_TRIGGER_INFO)
{
  typedef struct _SERVICE_TRIGGER_INFO {
  #ifdef __midl
      [range(0, 64)]
  #endif
      DWORD                   cTriggers;  // Number of triggers in the pTriggers array
  #ifdef __midl
      [size_is(cTriggers)]
  #endif
      PSERVICE_TRIGGER        pTriggers;  // Array of triggers
      PBYTE                   pReserved;  // Reserved, must be NULL
  } SERVICE_TRIGGER_INFO, *PSERVICE_TRIGGER_INFO;
}

__if_not_exists (SERVICE_FAILURE_ACTIONS_FLAG)
{
  typedef struct _SERVICE_FAILURE_ACTIONS_FLAG 
  {
      BOOL       fFailureActionsOnNonCrashFailures;       // Failure actions flag
  } SERVICE_FAILURE_ACTIONS_FLAG, *LPSERVICE_FAILURE_ACTIONS_FLAG;
}

__if_not_exists (SERVICE_DELAYED_AUTO_START_INFO)
{
  typedef struct _SERVICE_DELAYED_AUTO_START_INFO 
  {
      BOOL       fDelayedAutostart;      // Delayed autostart flag
  } SERVICE_DELAYED_AUTO_START_INFO, *LPSERVICE_DELAYED_AUTO_START_INFO;
}

/*
typedef struct _SC_ACTION {
    SC_ACTION_TYPE  Type;
    DWORD           Delay;
} SC_ACTION, *LPSC_ACTION;

typedef struct _SERVICE_FAILURE_ACTIONSA {
    DWORD       dwResetPeriod;
    LPSTR       lpRebootMsg;
    LPSTR       lpCommand;
#ifdef __midl
    [range(0, 1024)]
#endif
    DWORD       cActions;
#ifdef __midl
    [size_is(cActions)]
#endif
    SC_ACTION * lpsaActions;
} SERVICE_FAILURE_ACTIONSA, *LPSERVICE_FAILURE_ACTIONSA;
typedef struct _SERVICE_FAILURE_ACTIONSW {
    DWORD       dwResetPeriod;
    LPWSTR      lpRebootMsg;
    LPWSTR      lpCommand;
#ifdef __midl
    [range(0, 1024)]
#endif
    DWORD       cActions;
#ifdef __midl
    [size_is(cActions)]
#endif
    SC_ACTION * lpsaActions;
} SERVICE_FAILURE_ACTIONSW, *LPSERVICE_FAILURE_ACTIONSW;
#ifdef UNICODE
typedef SERVICE_FAILURE_ACTIONSW SERVICE_FAILURE_ACTIONS;
typedef LPSERVICE_FAILURE_ACTIONSW LPSERVICE_FAILURE_ACTIONS;
#else
typedef SERVICE_FAILURE_ACTIONSA SERVICE_FAILURE_ACTIONS;
typedef LPSERVICE_FAILURE_ACTIONSA LPSERVICE_FAILURE_ACTIONS;
#endif // UNICODE
*/


////////////////////////////// Forward declaration ////////////////////////////

class CNTServiceControlManager;


////////////////////////////// Classes ////////////////////////////////////////

//An encapsulation of a service as returned from querying the SCM (i.e. an SC_HANDLE)
class CNTSERVICE_EXT_CLASS CNTScmService
{
public:
//typedefs
  typedef BOOL (CALLBACK* ENUM_SERVICES_PROC)(void* pUserData, ENUM_SERVICE_STATUS& ss);

//Constructors / Destructors
	CNTScmService();
	~CNTScmService();

//Methods
  //Releases the underlying SC_HANDLE
  void Close();

	//Allows access to the underlying SC_HANDLE representing the service
	operator SC_HANDLE() const;

	//Attach / Detach support from an SDK SC_HANDLE
	BOOL Attach(SC_HANDLE hService);
	SC_HANDLE Detach();

	//Changes the configuration of this service
	BOOL ChangeConfig(DWORD dwServiceType,	      // type of service 
									  DWORD dwStartType,	        // when to start service 
 									  DWORD dwErrorControl,	      // severity if service fails to start 
 									  LPCTSTR lpBinaryPathName,	  // pointer to service binary file name 
									  LPCTSTR lpLoadOrderGroup,	  // pointer to load ordering group name 
									  LPDWORD lpdwTagId,	        // pointer to variable to get tag identifier 
 									  LPCTSTR lpDependencies,	    // pointer to array of dependency names 
 									  LPCTSTR lpServiceStartName, // pointer to account name of service 
 									  LPCTSTR lpPassword,	        // pointer to password for service account  
 									  LPCTSTR lpDisplayName 	    // pointer to display name 
                    ) const;

	//Send a command to the service
	BOOL Control(DWORD dwControl);
	DWORD Control(DWORD dwControl, DWORD dwInfoLevel, PVOID pControlParams);
	

	//These functions call Control() with the 
	//standard predefined control codes
	BOOL Stop() const;			 //Ask the service to stop
	BOOL Pause() const;			 //Ask the service to pause
	BOOL Continue() const;	 //Ask the service to continue
	BOOL Interrogate() const; //Ask the service to update its status to the SCM

  //Waits for a service to stop with a configurable timeout
  BOOL WaitForStop(DWORD dwTimeout);

  //Waits for a service to have a certain status (with a configurable timeout)
  BOOL WaitForServiceStatus(DWORD dwTimeout, DWORD dwWaitForStatus, DWORD dwPollingInterval = 250);

	//Start the execution of the service
	BOOL Start(DWORD dwNumServiceArgs,	      // number of arguments 
			       LPCTSTR* lpServiceArgVectors 	// address of array of argument string pointers  
		         ) const;	

	//Determines what Control codes this service supports
	BOOL AcceptStop(BOOL& bStop);                   //Ask the service can it stop
	BOOL AcceptPauseContinue(BOOL& bPauseContinue);	//Ask the service can it pause continue
	BOOL AcceptShutdown(BOOL& bShutdown);           //Ask the service if it is notified of shutdowns

	//Get the most return status of the service reported to the SCM by this service
	BOOL QueryStatus(SERVICE_STATUS& ServiceStatus) const;

	//Get the configuration parameters of this service from the SCM
	BOOL QueryConfig(LPQUERY_SERVICE_CONFIG& lpServiceConfig) const;

	//Add a new service to the SCM database
	BOOL Create(CNTServiceControlManager& Manager, // handle to service control manager database  
 						  LPCTSTR lpServiceName,	         // pointer to name of service to start 
						  LPCTSTR lpDisplayName,	         // pointer to display name 
						  DWORD dwDesiredAccess,	         // type of access to service 
						  DWORD dwServiceType,	           // type of service 
						  DWORD dwStartType,	             // when to start service 
						  DWORD dwErrorControl,	           // severity if service fails to start 
						  LPCTSTR lpBinaryPathName,	       // pointer to name of binary file 
						  LPCTSTR lpLoadOrderGroup,	       // pointer to name of load ordering group 
						  LPDWORD lpdwTagId,	             // pointer to variable to get tag identifier 
						  LPCTSTR lpDependencies,	         // pointer to array of dependency names 
						  LPCTSTR lpServiceStartName,	     // pointer to account name of service 
						  LPCTSTR lpPassword 	             // pointer to password for service account 
              );

	//Mark this service as to be deleted from the SCM.
	BOOL Delete() const;

	//Enumerate the services that this service depends upon
	BOOL EnumDependents(DWORD dwServiceState,	                // state of services to enumerate 
                      void* pUserData,                      // User defined data
 										  ENUM_SERVICES_PROC lpEnumServicesFunc // The callback function to use
                      ) const;

	//Get the security information associated with this service
	BOOL QueryObjectSecurity(SECURITY_INFORMATION dwSecurityInformation,	// type of security information requested  
			                     PSECURITY_DESCRIPTOR& lpSecurityDescriptor	  // address of security descriptor 
						   					   ) const;

	//Set the security descriptor associated with this service
	BOOL SetObjectSecurity(SECURITY_INFORMATION dwSecurityInformation,	// type of security information requested  
			                   PSECURITY_DESCRIPTOR lpSecurityDescriptor 	// address of security descriptor 
		                     ) const;

  //Windows 2000+ specific functions
  BOOL QueryDescription(CString& sDescription) const;
  BOOL ChangeDescription(const CString& sDescription);
  BOOL QueryFailureActions(LPSERVICE_FAILURE_ACTIONS& pActions) const;
  BOOL ChangeFailureActions(LPSERVICE_FAILURE_ACTIONS pActions);
  BOOL QueryStatus(SERVICE_STATUS_PROCESS& ssp) const;

  //Windows Vista / 2008+ specific functions
  DWORD NotifyStatusChange(DWORD dwNotifyMask, PSERVICE_NOTIFY pNotifyBuffer) const;
  BOOL  QueryDelayAutoStart(BOOL& bDelayedAutoStart) const;
  BOOL  ChangeDelayAutoStart(BOOL bDelayedAutoStart);
  BOOL  QueryFailureActionsFlag(BOOL& bFailureActionsOnNonCrashFailures) const;
  BOOL  ChangeFailureActionsFlag(BOOL bFailureActionsOnNonCrashFailures);
  BOOL  QuerySidInfo(DWORD& dwServiceSidType) const;
  BOOL  ChangeSidInfo(DWORD dwServiceSidType);
  BOOL  QueryRequiredPrivileges(CStringArray& privileges) const;
  BOOL  ChangeRequiredPrivileges(const CStringArray& priviledges);
  BOOL  QueryPreShutdown(DWORD& dwPreshutdownTimeout) const;
  BOOL  ChangePreShutdown(DWORD dwPreshutdownTimeout);

  //Windows 7 / 2008 RC+ specific functions
  BOOL ChangeTrigger(PSERVICE_TRIGGER_INFO pTriggerInfo);
  BOOL QueryTrigger(PSERVICE_TRIGGER_INFO& pTriggerInfo) const;
  BOOL ChangePreferredNode(USHORT usPreferredNode, BOOL bDelete);
  BOOL QueryPreferredNode(USHORT& usPreferredNode, BOOL& bDelete) const;

protected:
//typedefs
  typedef BOOL (WINAPI QUERYSERVICECONFIG2)(SC_HANDLE, DWORD, LPBYTE, DWORD, LPDWORD);
  typedef QUERYSERVICECONFIG2* LPQUERYSERVICECONFIG2;
  typedef BOOL (WINAPI CHANGESERVICECONFIG2)(SC_HANDLE, DWORD, LPVOID);
  typedef CHANGESERVICECONFIG2* LPCHANGESERVICECONFIG2;
  typedef BOOL (WINAPI QUERYSERVICESTATUSEX)(SC_HANDLE, SC_STATUS_TYPE, LPBYTE, DWORD, LPDWORD);
  typedef QUERYSERVICESTATUSEX* LPQUERYSERVICESTATUSEX;
  typedef BOOL (WINAPI NOTIFYSERVICESTATUSCHANGE)(SC_HANDLE, DWORD, PSERVICE_NOTIFY);
  typedef NOTIFYSERVICESTATUSCHANGE* LPNOTIFYSERVICESTATUSCHANGE;
  typedef BOOL (WINAPI CONTROLSERVICEEX)(SC_HANDLE, DWORD, DWORD, PVOID);
  typedef CONTROLSERVICEEX* LPCONTROLSERVICEEX;

//Member variables
  SC_HANDLE                   m_hService;
  HINSTANCE                   m_hAdvapi32;                     //Instance handle of the "ADVAPI32.DLL" which houses all of the NT Service functions
  LPQUERYSERVICECONFIG2       m_lpfnQueryServiceConfig2;       //QueryServiceConfig2 function pointer
  LPCHANGESERVICECONFIG2      m_lpfnChangeServiceConfig2;      //ChangeServiceConfig2 function pointer
  LPQUERYSERVICESTATUSEX      m_lpfnQueryServiceStatusEx;      //QueryServiceStatusEx function pointer
  LPNOTIFYSERVICESTATUSCHANGE m_lpfnNotifyServiceStatusChange; //NotifyServiceStatusChange function pointer
  LPCONTROLSERVICEEX          m_lpfnControlServiceEx;          //ControlServiceEx function pointer
};

#endif //__NTSERVSCMSERVICE_H__
