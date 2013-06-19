//响应创建按钮的消息
#include <windows.h>
#include <commctrl.h>

DWORD g_dwID = 0;
WNDPROC g_wndProc = NULL;
HWND  g_hWnd = NULL;
TCHAR g_szCaption[255] = {'\0'};

BOOL WINAPI DllMain(HANDLE hInstance, ULONG Command, LPVOID Reserved)
{
	return TRUE;
}


LRESULT APIENTRY RC_WndProcA(		//此函数暂时没用
    HWND hwndDlg, 
    UINT uMsg, 
    WPARAM wParam, 
    LPARAM lParam) 
{
	switch (uMsg){
	case WM_COMMAND:
		switch (HIWORD(wParam)){
		case BN_CLICKED:
			if (LOWORD(wParam) == g_dwID){
				MessageBox(NULL, "take careful, you click my button", "haha", MB_OK);
				return 0;
			}
			break;
		}
		break;
	}
	return CallWindowProc(g_wndProc, hwndDlg, uMsg, wParam, lParam); 

}

BOOL WINAPI RC_InitBtnA(DWORD dwID, LPCTSTR szCaption, RECT r, HWND hWnd)
{
	
	HWND hwndButton = CreateWindow(
		"BUTTON",   // predefined class 
		szCaption,       // button text 
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		r.left,
		r.top,
		r.right - r.left,
		r.bottom - r.top,
		hWnd,
		NULL,
		(HINSTANCE) GetWindowLong(hWnd, GWL_HINSTANCE), 
		NULL);
	if (!hwndButton)return FALSE;

	MSG msg;

	::SetWindowPos(hwndButton, HWND_TOP, 		r.left,
						r.top,
						r.right - r.left,
						r.bottom - r.top, SWP_NOACTIVATE);

	InvalidateRect(hwndButton, NULL, TRUE);
	UpdateWindow(hwndButton);

	BOOL bRet;

	while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
	{ 
		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			if (msg.message == WM_LBUTTONUP){
				POINT pt;
				pt.x = LOWORD(msg.lParam);
				pt.y = HIWORD(msg.lParam); 

				MessageBox(NULL, "take careful, you click my button", "haha", MB_OK);
				::SendMessage(hwndButton, WM_KILLFOCUS, NULL, 0);
				continue;
			}
			TranslateMessage(&msg); 
			DispatchMessage(&msg); 
		}
	}

	return hwndButton != NULL;
}

