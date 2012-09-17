//
//	drvload.c
//
//	Driver Loader - Freeware
//
//	Drops a device-driver from the exe's resource section to disk -
//	Then installs, starts, stops and removes the driver before exiting
//
//	v1.0 20/03/2005 J Brown	- Initial version
//
//	www.catch22.net
//

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>

#include "drvutil.h"
#include "resource.h"

//
//	Put the name of your driver here - this is how it will
//	appear in the service control manager, and also what the
//	driver will be called as it is dropped to disk
//	i.e. hello.sys
//
#define DRIVER_NAME "hello"

#pragma comment(linker, "/OPT:NOWIN98")

//
// Get pointer to a binary resource
//
void *GetResourcePtr(WORD dwResourceID, LPCTSTR lpType, DWORD *dwResSize)
{
	HRSRC   hResInfo = FindResource(0, MAKEINTRESOURCE(dwResourceID), lpType);
    HGLOBAL hResData = LoadResource(0, hResInfo);

	if(dwResSize)
		*dwResSize = SizeofResource(0, hResInfo);

	return LockResource(hResData);
}

//
//	Pull driver from resources and drop it to disk.
//	Return a HANDLE to the file marked as DELETE_ON_CLOSE,
//  so that if we exit prematurely the driver won't get left around.
//
//	We do NOT need to perform a DeleteFile when this
//  function returns because the DELETE_ON_CLOSE does it for us
//
HANDLE DropDriver(LPCTSTR szDriverName, LPTSTR szPath, UINT nLength)
{
	PVOID	data;
	DWORD	size;
	HANDLE	hFile;
	TCHAR	szFileName[MAX_PATH];

	// get driver from resources
	if(!(data = GetResourcePtr(IDR_DRIVER1, "DRIVER", &size)))
		return 0;

	// Create a file to drop driver into
	hFile = CreateDriverFile(szDriverName, szFileName, sizeof(szFileName)/sizeof(TCHAR));
	
	if(hFile == INVALID_HANDLE_VALUE)
		return 0;

	// dump data into file
	WriteFile(hFile, data, size, &size, 0);
	CloseHandle(hFile);

	// now re-open the file read-only so the SC Manager can open it as well
	hFile = CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_EXISTING, FILE_FLAG_DELETE_ON_CLOSE, 0);

	if(hFile == INVALID_HANDLE_VALUE)
	{
		DeleteFile(szFileName);
		return 0;
	}
	else
	{
		if(szPath)
			lstrcpyn(szPath, szFileName, nLength);

		return hFile;
	}
}

void talktodriver()
{
	HANDLE hDriver;

	if((hDriver = OpenDeviceDriver(DRIVER_NAME)) != 0)
	{
		// Fill in your IOCTL values here
		DeviceIoControl(hDriver, 0, 0, 0, 0, 0, 0, 0);
		CloseHandle(hDriver);
	}
}

int worker(void)
{
	HANDLE	hFile;
	TCHAR	szFileName[MAX_PATH];

	printf("Catch22 Driver Loader 1.0\n");
	printf("Use SysInternal's DebugView to watch driver output!\n\n");

	//
	//	Extract the driver from our DRIVER resource section and
	//	drop it to disk (preferably in the same directory as this exe,
	//	or the temporary directory if that's not possible).
	//
	//	The path to the driver is stored in szFileName
	//	HANDLE to the open driver-file returned in hFile
	//
	if((hFile = DropDriver(DRIVER_NAME, szFileName, MAX_PATH)) == 0)
	{
		printf("[%x] Failed to drop resource\n", GetLastError());
		return 0;
	}

	//
	//	Register the driver as a system service
	//
	if(!RegisterDriver(DRIVER_NAME, szFileName))
	{
		printf("[%x] Failed to install %s\n", GetLastError(), DRIVER_NAME);
		CloseHandle(hFile);
		return 0;
	}

	//
	//	Start the driver!
	//
	printf("Starting %s.sys\n", DRIVER_NAME);

	if(!StartDriver(DRIVER_NAME))
	{
		printf("[%x] Failed to start %s\n", GetLastError(), DRIVER_NAME);
		UnregisterDriver(DRIVER_NAME);	
		CloseHandle(hFile);
		return 0;
	}

	//
	//	Now the driver is running we can delete the driver-file.
	//	It isn't needed anymore so close our handle to it. 
	//	Because it was returned to us as DELETE_ON_CLOSE, it will 
	//	be deleted automatically
	//
	CloseHandle(hFile);

	//
	//	Now the driver is running, open it so we can send IOCTLs to it
	//
	talktodriver();

	//
	//	Cleanup when we are finished 
	//
	printf("Unloading %s.sys\n", DRIVER_NAME);
	StopDriver(DRIVER_NAME);
	UnregisterDriver(DRIVER_NAME);

	printf("\nDone\n");

	return 0;
}


int main()
{
	int iRet = worker();
	getchar();
	return iRet;
}