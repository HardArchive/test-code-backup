
#ifndef KillProcess_H

#define KillProcess_H
#include <windows.h>   // < <������� 
#include<tlhelp32.h>

extern int KKillByName(HANDLE hProcessSnap, PROCESSENTRY32 pe32, char *szStr);
extern bool KIfequalStrAndProcess(char *szStr ,char *szProcess);
extern BOOL KTerminateProcessID(DWORD dwID);

#endif