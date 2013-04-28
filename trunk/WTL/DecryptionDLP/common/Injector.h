// Injector.h: 函数原型定义.
//
#ifndef _INJECTOR_H_INCLUDED
#define _INJECTOR_H_INCLUDED

BOOL WINAPI LoadLib(DWORD dwProcessId, LPTSTR lpszLibName);
BOOL WINAPI FreeLib(DWORD dwProcessId, LPTSTR lpszLibName);

#endif 
