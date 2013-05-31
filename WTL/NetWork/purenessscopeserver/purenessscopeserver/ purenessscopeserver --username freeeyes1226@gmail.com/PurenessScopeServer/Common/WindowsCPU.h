#ifndef _WINDOWSCPU_H
#define _WINDOWSCPU_H

#ifdef WIN32

#include  <windows.h>
#include  <Psapi.h>
#include  <conio.h> 
#include  <stdio.h> 

#define  SystemBasicInformation  0 
#define  SystemPerformanceInformation  2 
#define  SystemTimeInformation  3 

#define  Li2Double(x)  ((double)((x).HighPart)  *  4.294967296E9  +  (double)((x).LowPart)) 

typedef  struct 
{ 
	DWORD  dwUnknown1; 
	ULONG  uKeMaximumIncrement; 
	ULONG  uPageSize; 
	ULONG  uMmNumberOfPhysicalPages; 
	ULONG  uMmLowestPhysicalPage; 
	ULONG  uMmHighestPhysicalPage; 
	ULONG  uAllocationGranularity; 
	PVOID  pLowestUserAddress; 
	PVOID  pMmHighestUserAddress; 
	ULONG  uKeActiveProcessors; 
	BYTE  bKeNumberProcessors; 
	BYTE  bUnknown2; 
	WORD  wUnknown3; 
}  SYSTEM_BASIC_INFORMATION; 

typedef  struct 
{ 
	LARGE_INTEGER  liIdleTime; 
	DWORD  dwSpare[76]; 
}  SYSTEM_PERFORMANCE_INFORMATION; 

typedef  struct 
{ 
	LARGE_INTEGER  liKeBootTime; 
	LARGE_INTEGER  liKeSystemTime; 
	LARGE_INTEGER  liExpTimeZoneBias; 
	ULONG  uCurrentTimeZoneId; 
	DWORD  dwReserved; 
}  SYSTEM_TIME_INFORMATION; 



int GetProcessCPU_Idel();
int GetProcessMemorySize();

#endif

#endif
