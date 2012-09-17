#ifndef NTHEADER_INCLUDED
#define NTHEADER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

typedef struct
{
    DWORD  UniqueProcessId;
    DWORD  UniqueThreadId;

} CLIENT_ID, *PCLIENT_ID;

typedef struct 
{
    WORD	Length;
    WORD	MaximumLength;
    PWCHAR  Buffer;

} UNICODE_STRING, *PUNICODE_STRING;

typedef struct
{
	LARGE_INTEGER	KernelTime;
	LARGE_INTEGER	UserTime;
	LARGE_INTEGER	CreateTime;
	DWORD			WaitTime;
	PVOID			StartAddress;
	CLIENT_ID		ClientId;
	DWORD			Priority;
	DWORD			BasePriority;
	ULONG			ContextSwitchCount;
	DWORD			State;
	DWORD			WaitReason;

} SYSTEM_THREADS;

//
//	Correct for Win2K/XP - don't know about NT though
//
typedef struct
{
	DWORD			NextEntryDelta;
	DWORD			ThreadCount;
	DWORD			Reserved1[6];
	LARGE_INTEGER	CreateTime;
	LARGE_INTEGER	UserTime;
	LARGE_INTEGER	KernelTime;
	UNICODE_STRING	ProcessName;
	DWORD			BasePriority;
	DWORD			ProcessId;
	DWORD			InheritiedFromProcessId;
	DWORD			HandleCount;
	DWORD			Reserved2[2];
	DWORD			PeakVirtualSize;
	DWORD			VirtualSize;
	DWORD			PageFaultCount;
	DWORD			PeakWorkingSetSize;
	DWORD			WorkingSetSize;
	DWORD			QuotaPeakPagedPoolUsage;
	DWORD			QuotaPagedPoolUsage;
	DWORD			QuotaPeakNonPagedPoolUsage;
	DWORD			QuotaNonPagedPoolUsage;
	DWORD			PagefileUsage;
	DWORD			PeakPagefileUsage;
	DWORD			PrivateBytes;
	DWORD			Unknown6[12];

	SYSTEM_THREADS	Threads[1];

} SYSTEM_PROCESSES;
#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif