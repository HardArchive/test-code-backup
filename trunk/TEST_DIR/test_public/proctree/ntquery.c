#include <windows.h>

typedef DWORD (WINAPI * ZWQUERYINFO )(DWORD	SystemInformationClass,
									  PVOID	SystemInformation,
									  DWORD	SystemInformationLength,
									  PDWORD	ReturnLength
									);

PVOID GetNativeProcAddr(CHAR *szFunction)
{
	return (PVOID)GetProcAddress(GetModuleHandle("NTDLL.DLL"), szFunction);
}

//
//  Wrapper routine around ZwQuerySystemInformation
//
BOOL GetQuerySystemInfoBuffer(ULONG InfoClass, PVOID *SysInfo, PULONG MemSize)
{
	BYTE   *buf = 0;
	ULONG	len = 0;
	ULONG   status;

	ZWQUERYINFO ZwQuerySystemInformation;

	ZwQuerySystemInformation = (ZWQUERYINFO)GetNativeProcAddr("ZwQuerySystemInformation");

	if(ZwQuerySystemInformation == 0)
		return FALSE;

	do 
	{
		if(buf != 0)
			free(buf);

		len += 0x10000;

		if((buf = (BYTE *)malloc(len)) == 0)
			return FALSE;

		status = ZwQuerySystemInformation(InfoClass, buf, len, &len);
		
	} while(status == 0xC0000004L);	// STATUS_INFO_LENGTH_MISMATCH

	if(status == 0)
	{
		if(MemSize) *MemSize = len;
		*SysInfo = buf;
		return TRUE;
	}
	else
	{
		free(buf);
		return FALSE;
	}
}

VOID FreeQuerySystemInfoBuffer(PVOID buf)
{
	if(buf) free(buf);
}
