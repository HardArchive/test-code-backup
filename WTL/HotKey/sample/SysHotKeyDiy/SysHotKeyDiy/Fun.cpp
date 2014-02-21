
#include "stdafx.h"
#include "inc.h"
#include "SysHotKeyDiy.h"

HWND FindAppWnd()
{
	HWND hWnd = NULL;
	UINT iProcessID = 0;
	BOOL bRet = FALSE;
		PROGHWND mItem;
		mItem.hWnd = NULL;
		mItem.strTitle = _T("系统热键");
		bRet = EnumWindows(EnumWndProc, (LPARAM)&mItem);
		if ( !bRet && GetLastError() == DIY_ERROR_CODE && mItem.hWnd != NULL ){
			hWnd = mItem.hWnd;
		}
	return hWnd;
}

BOOL CALLBACK EnumWndProc( HWND hwnd, LPARAM lParam )
{
	TCHAR szTitle[_MAX_PATH] = {0};
	PROGHWND *pItem = (PROGHWND *)lParam;
	HANDLE hProp = GetProp(hwnd, _T("SysHotKeyDiy_Prop"));
	if ( hProp ) {
		LONG LProp = (LONG)hProp;
		GetWindowText(hwnd, szTitle, _MAX_PATH);
		CString str(szTitle);
		if( LProp == DIY_PROP_CODE && !(str.IsEmpty()) && str.Find(pItem->strTitle) != -1) {
			pItem->hWnd = hwnd;
			SetLastError(DIY_ERROR_CODE);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL ProcessExsit(CString strName, UINT *ProcessID)
{
	HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 info;
	info.dwSize = sizeof(PROCESSENTRY32);
	BOOL ret = Process32First(handle, &info);
	if( ret == TRUE ){
		do {
			if (info.szExeFile == strName){
				if (ProcessID != NULL){
					*ProcessID = info.th32ProcessID;
				}
				return TRUE;
			}
			ret = Process32Next(handle, &info);
		} while(ret);
		CloseHandle(handle);
	}
	return FALSE;
}

void AddTrayIcon(HWND in_hWnd, BOOL bHideWnd)
{
	if ( bHideWnd == TRUE ){
		ShowWindow(in_hWnd, SW_MINIMIZE);
		ShowWindow(in_hWnd, SW_HIDE);
	}
	NOTIFYICONDATA tnd;
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd = in_hWnd;
	tnd.uID = IDR_MAINFRAME;
	tnd.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	tnd.uCallbackMessage = MY_TRAYICON_MSG;
	tnd.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	_tcscpy_s(tnd.szTip, sizeof(tnd.szTip), _T("系统热键 DIY ..."));
	Shell_NotifyIcon(NIM_ADD, &tnd);
}

void DelTrayIcon(HWND in_hWnd, BOOL bShowWnd)
{
	if ( bShowWnd == TRUE ){
		::ShowWindow(in_hWnd, SW_RESTORE);
		SetForegroundWindow(in_hWnd);
	}
	NOTIFYICONDATA tnid;
	tnid.cbSize = sizeof(NOTIFYICONDATA);
	tnid.hWnd = in_hWnd;
	tnid.uID = IDR_MAINFRAME;
	Shell_NotifyIcon(NIM_DELETE, &tnid);
}