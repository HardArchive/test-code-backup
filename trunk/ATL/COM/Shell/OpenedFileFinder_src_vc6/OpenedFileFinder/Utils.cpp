// Utils.cpp: implementation of the HelperFunctions class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Utils.h"
#include <Winsvc.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const LPCTSTR DRV_DOS_NAME = _T("\\\\.\\ListFileDrv");
const LPCTSTR DRV_NAME = _T("ListOpenedFileDrv");
const LPCTSTR DRV_FILE_NAME = _T("ListOpenedFileDrv.sys");

// Function resolves the fosedevice name to drive name
BOOL GetDrive( LPCTSTR pszDosName, CString& csDrive, bool bDriveLetterOnly )
{
	TCHAR tcDeviceName[50];
	TCHAR tcDrive[3] = _T("A:");

	// Iterating through the drive letters
	for ( TCHAR actDrive = _T('A'); actDrive <= _T('Z'); actDrive++ )
	{
		tcDrive[0] = actDrive;
		// Query the device for the drive letter
		if ( QueryDosDevice( tcDrive, tcDeviceName, 50 ) != 0 )
		{
            // Is this the drive letter we are looking for?
			if ( _tcsnicmp( tcDeviceName, pszDosName, _tcslen( tcDeviceName ) ) == 0 )
			{
                if( bDriveLetterOnly )
                {
                    csDrive = tcDrive;
                }
                else
                {
                    csDrive = pszDosName;
                    csDrive.Replace( tcDeviceName, tcDrive );
                }
				return TRUE;
			}
		}
	}
	return FALSE;
}

// This Function extracts the Driver in its resource to a file and then install,start it
HANDLE ExtractAndInstallDrv()
{
	SC_HANDLE hSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	SC_HANDLE hService = OpenService( hSCManager , DRV_NAME, SERVICE_ALL_ACCESS);
	CString csPath;
	if( 0 == hService )
	{
		//Service not installed. So install the service.

		OSVERSIONINFO info = { 0 }; 
		info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&info); 
		int nDriverId = IDR_VISTA_DRIVER;
		if( info.dwMajorVersion == 5 )
		{
			nDriverId = IDR_XPDRIVER;
		}
		// First extract the resource
		HINSTANCE hModule= AfxGetInstanceHandle();
		HRSRC hRsrc = FindResource(hModule, MAKEINTRESOURCE(nDriverId),_T("BINARY"));
		HGLOBAL hDrvRsrc = LoadResource(hModule, hRsrc);
		DWORD dwDriverSize = SizeofResource(hModule, hRsrc);
		LPVOID lpvDriver = LockResource(hDrvRsrc);
		CFile File;
        if( !File.Open( DRV_FILE_NAME, CFile::modeCreate|CFile::modeWrite ))
        {
            return 0;
        }
        File.Write( lpvDriver, dwDriverSize );
		csPath = File.GetFilePath();
		File.Close();
		
		hService = CreateService( hSCManager, DRV_NAME, DRV_NAME, SERVICE_ALL_ACCESS,
								  SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
								  csPath, NULL, NULL, NULL, NULL, NULL );
		
		if( 0 == hService )
		{
			CloseServiceHandle(hSCManager);
			return 0;
		}
	}

	if( !StartService( hService, 0, NULL ))
	{
		if( GetLastError() != ERROR_SERVICE_ALREADY_RUNNING )
		{
			DeleteService(hService);
			CloseServiceHandle(hService);
			CloseServiceHandle(hSCManager);			
			DeleteFile( csPath );
			return 0;
		}
		
	}
	// Delete the temp file...
	DeleteFile( csPath );
	HANDLE hFile = CreateFile( DRV_DOS_NAME, GENERIC_READ|GENERIC_WRITE, 
								FILE_SHARE_READ | FILE_SHARE_WRITE, 0,
								OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0 );
	if( INVALID_HANDLE_VALUE == hFile )
	{
		hFile = 0;
		SERVICE_STATUS  stSrvStatus = {0};
		ControlService( hService, SERVICE_CONTROL_STOP, &stSrvStatus );
	}	
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return hFile;
}

// Function stops the service and then deletes it.
BOOL StopAndUninstallDrv( HANDLE hDrvHandle )
{
	CloseHandle( hDrvHandle );
	SC_HANDLE hSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS );
	SC_HANDLE hService = OpenService( hSCManager , DRV_NAME, SERVICE_ALL_ACCESS );
	SERVICE_STATUS  stSrvStatus = {0};
	ControlService( hService, SERVICE_CONTROL_STOP, &stSrvStatus );
	BOOL bDeleted = DeleteService(hService);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);	
	return bDeleted;
}


BOOL EnableTokenPrivilege(LPCTSTR pszPrivilege)
{
    HANDLE hToken        = 0;
    TOKEN_PRIVILEGES tkp = {0}; 

    // Get a token for this process.
    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES |
                          TOKEN_QUERY, &hToken))
    {
        return FALSE;
    }

    // Get the LUID for the privilege. 
    if(LookupPrivilegeValue(NULL, pszPrivilege,
                            &tkp.Privileges[0].Luid)) 
    {
        tkp.PrivilegeCount = 1;  // one privilege to set    
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        // Set the privilege for this process. 
        AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
                              (PTOKEN_PRIVILEGES)NULL, 0); 

        if (GetLastError() != ERROR_SUCCESS)
           return FALSE;
        
        return TRUE;
    }

    return FALSE;
}