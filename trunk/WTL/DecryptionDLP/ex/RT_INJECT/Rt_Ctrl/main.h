#ifndef _MAIN_H
#define _MAIN_H

//#define _UNICODE

#include <windows.h>

#define MAXINJECTSIZE 4096


typedef HMODULE (WINAPI * LPLOADLIBRARY)(LPCTSTR);
typedef FARPROC (WINAPI * LPGETPROCADDRESS)(HMODULE,LPCTSTR);
typedef BOOL    (WINAPI * LPFREELIBRARY)(HMODULE);
typedef LONG    (WINAPI * LPGETWINDOWLONG)(HWND, int);
typedef LONG	(WINAPI * LPSETWINDOWLONG)(HWND, int, LONG);
typedef HANDLE  (WINAPI * LPOPENMUTEX)(DWORD, BOOL, LPCTSTR);
typedef BOOL	(WINAPI * LPINIT_BTN)(DWORD, LPCTSTR, RECT, HWND);

//注入DLL信息
typedef struct {
	LPLOADLIBRARY		prcLoadLib;                //LoadLibrary函数指针
	LPGETPROCADDRESS	prcGetProcAddr;            //GetProcAddress函数指针
	LPFREELIBRARY		prcFreeLib;                //FreeLibrary函数指针
	TCHAR				szLibPath[MAX_PATH+1];     //传入的DLL路径
	HWND				hWnd;                      //需要注入窗口句柄
	DWORD				dwIDBtn;                   //按钮ID
	RECT				rtBtn;                     //按钮大小
	TCHAR				szCaptionBtn[64];          //按钮显示字样
}INJECT_DLL, * LPINJECT_DLL;

#endif