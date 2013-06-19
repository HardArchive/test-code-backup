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

//ע��DLL��Ϣ
typedef struct {
	LPLOADLIBRARY		prcLoadLib;                //LoadLibrary����ָ��
	LPGETPROCADDRESS	prcGetProcAddr;            //GetProcAddress����ָ��
	LPFREELIBRARY		prcFreeLib;                //FreeLibrary����ָ��
	TCHAR				szLibPath[MAX_PATH+1];     //�����DLL·��
	HWND				hWnd;                      //��Ҫע�봰�ھ��
	DWORD				dwIDBtn;                   //��ťID
	RECT				rtBtn;                     //��ť��С
	TCHAR				szCaptionBtn[64];          //��ť��ʾ����
}INJECT_DLL, * LPINJECT_DLL;

#endif