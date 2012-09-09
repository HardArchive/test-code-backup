#if !defined(AFX_UTILS_H__DEF02A65_CB32_46B8_9C74_5E7432392A9F__INCLUDED_)
#define AFX_UTILS_H__DEF02A65_CB32_46B8_9C74_5E7432392A9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <Winsvc.h>
#include "WinIoCtl.h"

#define IOCTL_LISTDRV_BUFFERED_IO		\
        CTL_CODE(FILE_DEVICE_UNKNOWN,	\
        0x802,							\
        METHOD_BUFFERED,	            \
        FILE_READ_DATA | FILE_WRITE_DATA)

typedef struct _HANDLE_INFO
{	
	USHORT tcDeviceName[260];
	USHORT tcFileName[260];
    ULONG uType;
}HANDLE_INFO;

typedef struct _ADDRESS_INFO
{
	PVOID pAddress;
}ADDRESS_INFO;

typedef LONG NTSTATUS;

#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

typedef struct _SYSTEM_HANDLE
{
    DWORD       dwProcessId;
    BYTE		bObjectType;
    BYTE		bFlags;
    WORD		wValue;
	PVOID       pAddress;
	DWORD GrantedAccess;
}SYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
    DWORD         dwCount;
    SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION, **PPSYSTEM_HANDLE_INFORMATION;

typedef enum _SYSTEM_INFORMATION_CLASS {
    SystemHandleInformation = 0X10,
} SYSTEM_INFORMATION_CLASS;

typedef NTSTATUS ( WINAPI *PNtQuerySystemInformation)
				 ( IN	SYSTEM_INFORMATION_CLASS SystemInformationClass,
				   OUT	PVOID					 SystemInformation,
				   IN	ULONG					 SystemInformationLength,
				   OUT	PULONG					 ReturnLength OPTIONAL );


BOOL StopAndUninstallDrv( HANDLE hDrvHandle );

HANDLE ExtractAndInstallDrv();

BOOL GetDrive( LPCTSTR lpDeviceFileName, CString& fsFileName, bool bDriveLetterOnly );

BOOL EnableTokenPrivilege(LPCTSTR pszPrivilege);

#endif
