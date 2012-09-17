//
//	REPORTVIEW.C
//
//	System-wide hook to force all CommonDialog ListViews to 
//  display in Report Mode
//
//	Build the DLL. Then register by typing (at the commandprompt):
//
//		regsvr32 reportview.dll
//
//	www.catch22.net
//
#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#pragma comment(linker, "/opt:nowin98")
#pragma comment(linker, "/merge:.text=.data")
#pragma comment(linker, "/merge:.rdata=.data")

#define REGLOC _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Windows")

HHOOK	g_hHook;
TCHAR	g_szPath[MAX_PATH];
TCHAR	g_szCurrent[0x1000];
HMODULE g_hInstance;

HKEY GetRegLoc()
{
	HKEY hKey = 0;
	RegCreateKeyEx(HKEY_LOCAL_MACHINE, REGLOC, 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);
	return hKey;
}

#pragma comment(linker, "/export:DllRegisterServer=_DllRegisterServer@0,PRIVATE")
#pragma comment(linker, "/export:DllUnregisterServer=_DllUnregisterServer@0,PRIVATE")

char *stristr(const char *String, const char *Pattern)
{
	char *pptr, *sptr, *start;
	
	for (start = (char *)String; *start != 0; start++)
	{
		// find start of pattern in string
		for( ; ((*start!=0) && (toupper(*start) != toupper(*Pattern))); start++)
			;

		if(0 == *start)
			return NULL;
		
		pptr = (char *)Pattern;
		sptr = (char *)start;
		
		while(toupper(*sptr) == toupper(*pptr))
		{
			sptr++;
			pptr++;
			
			// if end of pattern then pattern was found
			if(0 == *pptr)
				return start;
		}
	}

	return NULL;
}

//
//	DllRegisterServer. 
//
STDAPI DllRegisterServer()
{
	HKEY hKey;
	DWORD type;
	DWORD len;
	DWORD ret = E_UNEXPECTED;

	if((hKey = GetRegLoc()) == 0)
		return E_UNEXPECTED;

	// Get current AppInit_Dlls string
	if(ERROR_SUCCESS == RegQueryValueEx(hKey, _T("AppInit_Dlls"), 0, &type, g_szCurrent, &len))
	{
		// Make sure aren't already registered
		char *ptr = stristr(g_szCurrent, g_szPath);
		g_szCurrent[len] = 0;

		if(g_szCurrent[0] != 0)
			lstrcat(g_szCurrent, _T(","));

		ret = S_OK;

		// append our DLL path to the AppInit_Dlls path
		if(ptr == 0)
		{
			lstrcat(g_szCurrent, g_szPath);
			len = lstrlen(g_szCurrent);
			RegSetValueEx(hKey, _T("AppInit_Dlls"), 0, REG_SZ, g_szCurrent, len);
		}
	}
	
	RegCloseKey(hKey);

	return ret;
}

STDAPI DllUnregisterServer()
{
	HKEY hKey;
	DWORD type;
	DWORD len;
	DWORD ret = E_UNEXPECTED;

	if((hKey = GetRegLoc()) == 0)
		return E_UNEXPECTED;

	// Get current AppInit_Dlls string
	if(ERROR_SUCCESS == RegQueryValueEx(hKey, _T("AppInit_Dlls"), 0, &type, g_szCurrent, &len))
	{
		// Find where our DLL path is stored
		char *ptr = stristr(g_szCurrent, g_szPath);

		ret = S_OK;

		if(ptr != 0)
		{
			len = lstrlen(g_szPath);
			
			if(ptr > 0 && ptr[-1] == ',')
			{
				ptr--;
				len++;
			}

			memmove(ptr, ptr + len, lstrlen(g_szCurrent) - len + 1);
			RegSetValueEx(hKey, _T("AppInit_Dlls"), 0, REG_SZ, g_szCurrent, len);
		}
	}

	RegCloseKey(hKey);

	return S_OK;
}

//
//	Computer-based training hook. Used to trap window creation
//  of a common dialog (Open/Save), so that the ListView contained
//  in these dialogs can be changed to report-view before it is displayed.
//
static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if(nCode == HCBT_CREATEWND)
	{
		HWND hwnd = (HWND)wParam;
		HWND hwndParent;

		CBT_CREATEWND *cw = (CBT_CREATEWND *)lParam;

		TCHAR szClass[32];
		GetClassName(hwnd, szClass, 32);

		// Is this a ListView being created?
		if(lstrcmpi(szClass, _T("SysListView32")) == 0)
		{
			HMODULE hModule = GetModuleHandle(_T("comdlg32.dll"));

			hwndParent = cw->lpcs->hwndParent;

			if(hModule != (HMODULE)GetWindowLong(hwndParent, GWL_HINSTANCE))
				hwndParent = GetParent(hwndParent);
		
			// Make sure the parent window (the dialog) was created by
			// the common-dialog library 
			if(hModule == (HMODULE)GetWindowLong(hwndParent, GWL_HINSTANCE))
			{
				PostMessage(cw->lpcs->hwndParent, WM_COMMAND, MAKEWPARAM(28716, 0), 0);
			}
			/*else 
			{
				GetClassName(cw->lpcs->hwndParent, szClass, 32);
				
				if(lstrcmpi(szClass, _T("SHELLDLL_DefView")) == 0)
				{
					PostMessage(cw->lpcs->hwndParent, WM_COMMAND, MAKEWPARAM(28716, 0), 0);
				}
			}*/
		}
	}

	return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

void InstallHook(DWORD dwThreadId)
{
	g_hHook = SetWindowsHookEx(WH_CBT, CBTProc, 0, dwThreadId);
}

void RemoveHook(DWORD dwThreadId)
{
	UnhookWindowsHookEx(g_hHook);
	g_hHook = 0;
}

BOOL WINAPI DllMain(HMODULE hInstance, DWORD dwReason, PVOID lpReserved)
{
	switch(dwReason)
	{
	case DLL_PROCESS_ATTACH:

		g_hInstance = hInstance;
		GetModuleFileName(hInstance, g_szPath, MAX_PATH);

		DisableThreadLibraryCalls(hInstance);

		InstallHook(GetCurrentThreadId());

		return TRUE;

	case DLL_PROCESS_DETACH:
		RemoveHook(GetCurrentThreadId());
		break;
	}

	return TRUE;
}

#ifdef _DEBUG
int main()
{
	return 0;
}
#endif

BOOL WINAPI _DllMainCRTStartup(HMODULE hInstance, DWORD dwReason, PVOID lpReserved)
{
	return DllMain(hInstance, dwReason, lpReserved);
}
