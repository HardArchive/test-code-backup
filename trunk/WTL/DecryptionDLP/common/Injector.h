// Injector.h: ����ԭ�Ͷ���.
//
#ifndef _INJECTOR_H_INCLUDED
#define _INJECTOR_H_INCLUDED

BOOL WINAPI LoadLib(DWORD dwProcessId, LPTSTR lpszLibName);
BOOL WINAPI FreeLib(DWORD dwProcessId, LPTSTR lpszLibName);

#endif 
