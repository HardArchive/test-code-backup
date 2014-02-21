//////////////////////////////////////////////////////////////////////////
//
//   查杀进程
//
//////////////////////////////////////////////////////////////////////////
#include "KillProcess.h"

#define ERR_UNABLE_TO_KILL	1005  // 自定义
#define ERR_UNKNOWN_PROCESS 1006  // 自定义

bool KIfequalStrAndProcess(char *szStr ,char *szProcess)
{
	int i;
	if (szStr==NULL && szProcess==NULL)
		return FALSE;
	for (i=0; ;i++)
	{
		if (szStr[i]=='\0' && (szProcess[i]=='.' || szProcess[i]=='\0'))
			return TRUE;
		if (szStr[i]==szProcess[i])
			continue;
		if (((szStr[i]>=65 && szStr[i]<=90) || (szStr[i]>=97 && szStr[i]<=122)) &&
			((szProcess[i]>=65 && szProcess[i]<=90) || (szProcess[i]>=97 && szProcess[i]<=122)))
		{
			if (szStr[i]-32==szProcess[i] || szStr[i]==szProcess[i]-32) 
				continue;
			else
				return FALSE;
		}
		else
			return FALSE;
	}
}

BOOL KTerminateProcessID(DWORD dwID)
{
	BOOL bRet = FALSE;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwID);
	if(hProcess!=NULL)
	{
		bRet = TerminateProcess(hProcess, 0);
	}
	CloseHandle(hProcess);
	return bRet;
}

int KKillByName(HANDLE hProcessSnap, PROCESSENTRY32 pe32, char *szStr)
{
	BOOL bMore = Process32First(hProcessSnap,&pe32);
	while (bMore)
	{
		if (KIfequalStrAndProcess(szStr, pe32.szExeFile)==TRUE)
		{
			if(KTerminateProcessID(pe32.th32ProcessID)==FALSE)
				return ERR_UNABLE_TO_KILL;// 自定义的 #define ERR_UNABLE_TO_KILL	1005
			else 
				return TRUE;
		} 
		bMore = Process32Next(hProcessSnap, &pe32);
	}	
	return ERR_UNKNOWN_PROCESS; // 自定义的 #define ERR_UNKNOWN_PROCESS 1006
}