#ifndef _gstruct_h
#define _gstruct_h

HWND WINAPI GetParentFromHwnd(HWND hWnd);
HWND WINAPI GetWindowFromPoint(POINT pt);
void WINAPI DrawBorder(HWND hWnd);
void WINAPI GetPassText(HWND hWndPass, HWND hWndShowPass);
BOOL WINAPI InitHook(HWND hWnd);
BOOL WINAPI ReleaseHook();

BOOL WINAPI RT_CTRL_BTN(LPCSTR lpszLibFile, HWND hWnd, DWORD dwID, LPRECT pRtBtn, LPCTSTR szCaptionBtn);

#endif
