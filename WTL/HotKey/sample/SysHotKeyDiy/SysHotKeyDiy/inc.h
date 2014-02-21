#pragma once


#define HOT_KEY_ID 10000
#define DIY_ERROR_CODE 99999
#define DIY_PROP_CODE 9999
#define MY_TRAYICON_MSG (WM_USER+200)

enum {
	FIND_FLAG_ID,
	FIND_FLAG_HOTKEY
};
#pragma warning(disable:4311)


typedef struct __HKNODE {
	DWORD dwHkID;
	CString strPath;
	CString strHotKey;
	DWORD dwSys;
	DWORD dwAsc;
}HKNODE;

typedef struct __PROGHWND {
	HWND hWnd;
	UINT iProcessID;
	CString strTitle;
}PROGHWND;

HWND FindAppWnd();
BOOL ProcessExsit(CString strName, UINT *ProcessID);
BOOL CALLBACK EnumWndProc( HWND hwnd, LPARAM lParam );
void AddTrayIcon(HWND in_hWnd, BOOL bHideWnd = TRUE);
void DelTrayIcon(HWND in_hWnd, BOOL bShowWnd = TRUE);