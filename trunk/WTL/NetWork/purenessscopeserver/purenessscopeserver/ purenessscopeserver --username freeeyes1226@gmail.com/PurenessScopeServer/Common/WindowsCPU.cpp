#ifdef WIN32

#include "WindowsCPU.h"

typedef  LONG  (WINAPI  *PROCNTQSI)(UINT,PVOID,ULONG,PULONG); 
PROCNTQSI  NtQuerySystemInformation; 

int GetProcessCPU_Idel()
{
	SYSTEM_PERFORMANCE_INFORMATION  SysPerfInfo; 
	SYSTEM_TIME_INFORMATION         SysTimeInfo; 
	SYSTEM_BASIC_INFORMATION        SysBaseInfo; 
	double  dbIdleTime; 
	double  dbSystemTime; 
	LONG  status; 
	LARGE_INTEGER  liOldIdleTime    =  {0,0}; 
	LARGE_INTEGER  liOldSystemTime  =  {0,0}; 

	NtQuerySystemInformation = (PROCNTQSI)GetProcAddress(GetModuleHandle("ntdll"), "NtQuerySystemInformation"); 

	if(!NtQuerySystemInformation) 
	{
		return -1;
	}

	//获得进程执行时间
	status = NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL); 
	if(status != NO_ERROR)
	{
		return -1;
	}

	//获得系统时间
	status = NtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),NULL); 
	if(status != NO_ERROR)
	{
		return -1;
	}

	//获得CPU idel时间
	status  = NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL); 
	if  (status  !=  NO_ERROR)
	{
		return -1;
	}

	liOldIdleTime    =  SysPerfInfo.liIdleTime; 
	liOldSystemTime  =  SysTimeInfo.liKeSystemTime;

	::Sleep(1000);

	//获得系统时间
	status = NtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),NULL); 
	if(status != NO_ERROR)
	{
		return -1;
	}

	//获得CPU idel时间
	status  = NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL); 
	if  (status  !=  NO_ERROR)
	{
		return -1;
	}

	dbIdleTime   = Li2Double(SysPerfInfo.liIdleTime) - Li2Double(liOldIdleTime); 
	dbSystemTime = Li2Double(SysTimeInfo.liKeSystemTime) - Li2Double(liOldSystemTime); 

	dbIdleTime = dbIdleTime/dbSystemTime; 

	dbIdleTime  =  100.0  -  dbIdleTime  *  100.0/(double)SysBaseInfo.bKeNumberProcessors  +  0.5;
	return (int)(dbIdleTime* 100);
}

int GetProcessMemorySize()
{
	PROCESS_MEMORY_COUNTERS pmc;  
	if(::GetProcessMemoryInfo(::GetCurrentProcess(), &pmc, sizeof(pmc)))  
	{  
		int nWorkSize = (int)pmc.WorkingSetSize;  
		int nPageSize = (int)pmc.PagefileUsage;  
		return nWorkSize + nPageSize;  
	}  
	return -1;  
}

#endif
