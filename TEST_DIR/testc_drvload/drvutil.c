//
//	DRVUTIL.C
//
//	Device-driver loading/unloading support - Freeware
//
//	Version 1.0 20/03/2005 J Brown 
//
//	www.catch22.net
//
#include <windows.h>
#include <tchar.h>
#include "drvutil.h"

static HANDLE CreateNewFile(LPCSTR szFileName)
{
	return CreateFile(szFileName, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_NEW, 0, 0);
}

//
//	Make full path for driver based on current directory
//
static BOOL GetDriverFileName(LPTSTR szFile, UINT nLen, LPCTSTR szDriver)
{
	TCHAR *ptr;

	// build up full name of driver 
	if(!GetModuleFileName(0, szFile, nLen))
		return FALSE;

	// search for last slash
	if((ptr = strrchr(szFile, '\\')) == 0)
		return FALSE;
	else
		ptr++;

	// replace exe name with driver name
	wsprintf(ptr, _T("%s.sys"), szDriver);

	return TRUE;
}

static BOOL GetDriverTempName(LPTSTR szFile, UINT nLen, LPCTSTR szDriver)
{
	TCHAR *ptr;

	// get temporary directory
	if(!GetTempPath(nLen - lstrlen(szDriver) - 2, szFile))
		return FALSE;

	ptr = szFile + strlen(szFile);
	wsprintf(ptr, _T("%s.sys"), szDriver);

	return TRUE;
}

static BOOL GetDriverTempName2(LPTSTR szFile, UINT nLen)
{
	TCHAR szTemp[MAX_PATH];
	
	if(!GetTempPath(MAX_PATH, szTemp))
		return FALSE;
	
	return (BOOL)GetTempFileName(szTemp, 0, 0, szFile);
}

//
//	CreateDriverFile
//
//	Create a file (based on driver name) ready for driver to be written
//
HANDLE CreateDriverFile(LPCTSTR szDriverName, LPTSTR szPathName, UINT nLength)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	TCHAR  szRoot[4];
	
	// try to drop driver in same directory as current exe
	if(!GetDriverFileName(szPathName, nLength, szDriverName))
		return INVALID_HANDLE_VALUE;

	lstrcpyn(szRoot, szPathName, 4);
	
	// if this is a harddisk, try to create file for driver
	if(GetDriveType(szRoot) == DRIVE_FIXED)
	{
		hFile = CreateNewFile(szPathName);
	}

	// otherwise we need a temporary location on a fixed-disk
	if(hFile == INVALID_HANDLE_VALUE)
	{
		// failed, so try temporary directory
		if(!GetDriverTempName(szPathName, nLength, szDriverName))
			return INVALID_HANDLE_VALUE;

		// try to create again
		hFile = CreateNewFile(szPathName);
	}

	// still can't create??? just use any temporary name
	if(hFile == INVALID_HANDLE_VALUE)
	{
		if(!GetDriverTempName2(szPathName, nLength))
			return INVALID_HANDLE_VALUE;

		// try to create one last time
		hFile = CreateNewFile(szPathName);
	}

	return hFile;
}


//
//	OpenDeviceDriver
//
//	Return handle to the specified device-driver, which can
//	subsequently be used for calls to DeviceIoControl
//
HANDLE OpenDeviceDriver(LPCTSTR szDriverName)
{
    HANDLE hDevice;
	TCHAR  szFullName[200];

	wsprintf(szFullName, "\\\\.\\%s", szDriverName);

	hDevice = CreateFile (szFullName,
                          GENERIC_READ | GENERIC_WRITE,
                          0,
                          NULL,
                          OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL,
                          NULL
                          );

	if(hDevice == INVALID_HANDLE_VALUE)
		hDevice = 0;
	
	return hDevice;
}

//
//	RegisterDriver
//
//	Registers the specified device-driver
//
BOOL RegisterDriver(LPCTSTR szDriverName, LPCTSTR szPathName)
{
	SC_HANDLE	hSCManager;
	SC_HANDLE	hService;

	if(!(hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
		return FALSE;

    hService = CreateService   (hSCManager,				// SCManager database
								szDriverName,			// name of service
								szDriverName,			// name to display
								SERVICE_ALL_ACCESS,		// desired access
								SERVICE_KERNEL_DRIVER,	// service type
								SERVICE_DEMAND_START,	// start type = MANUAL
								SERVICE_ERROR_NORMAL,	// error control type
								szPathName,				// service's binary
								NULL,					// no load ordering group
								NULL,					// no tag identifier
								NULL,					// no dependencies
								NULL,					// LocalSystem account
								NULL					// no password
								);

	if(hService)
	{
		CloseServiceHandle(hService);
		CloseServiceHandle(hSCManager);
		return TRUE;
	}
	else
	{
		BOOL fStatus = GetLastError();
		
		CloseServiceHandle(hSCManager);

		// if driver is already registered then this is OK
		return fStatus == ERROR_SERVICE_EXISTS;
	}
}

//
//	StartDriver
//
//	Starts (Loads) the specified driver
//
BOOL StartDriver(LPCTSTR szDriverName)
{
	SC_HANDLE	hSCManager;
	SC_HANDLE	hService;
	BOOL		fStatus;

	if(!(hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
		return 0;

    if(!(hService = OpenService(hSCManager, szDriverName, SERVICE_ALL_ACCESS)))
	{
        CloseServiceHandle(hSCManager);
        return FALSE;
    }

    // start the driver
	if(!(fStatus = StartService(hService, 0, NULL)))
	{
		// if already running then this is OK!!
		if(GetLastError() == ERROR_SERVICE_ALREADY_RUNNING)
			fStatus = TRUE;
    }

    CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

    return fStatus;
}

//
//	StopDriver
//
//	Stops (unloads) the specified driver
//
BOOL StopDriver(LPCTSTR szDriverName)
{
 	SC_HANDLE		hSCManager;
	SC_HANDLE		hService;
	BOOL			fStatus;
	SERVICE_STATUS  serviceStatus;

	if(!(hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
		return 0;

    if(!(hService = OpenService(hSCManager, szDriverName, SERVICE_ALL_ACCESS)))
	{
        CloseServiceHandle(hSCManager);
        return FALSE;
    }

    if(!(fStatus = ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus)))
	{
        if(GetLastError() == ERROR_SERVICE_NOT_ACTIVE)
			fStatus = TRUE;
	}

    CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

    return fStatus;
}

//
//	UnregisterDriver
//
//	Unregisters the specified driver
//
BOOL UnregisterDriver(LPCTSTR szDriverName)
{
    SC_HANDLE  hService;
	SC_HANDLE  hSCManager;
    BOOL       fStatus;

	if(!(hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)))
		return FALSE;

    if(!(hService = OpenService(hSCManager, szDriverName, SERVICE_ALL_ACCESS)))
	{
        CloseServiceHandle(hSCManager);
        return FALSE;
    }

    fStatus = DeleteService(hService);

    CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);

    return fStatus;
}
