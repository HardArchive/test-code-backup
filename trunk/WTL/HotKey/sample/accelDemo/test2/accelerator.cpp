#include "stdafx.h"

// accelerator.cpp

#include "accelerator.h"

//���ټ�����һ��ACCEL�ṹ���鹹�ɡ�ÿ���ṹ����һ�������ļ��ټ�����������Ϣ��
//typedef struct tagACCEL {
//#ifndef _MAC
//	BYTE   fVirt;               /* Also called the flags field */�������
//	WORD   key;
//	WORD   cmd;
//#else
//	WORD   fVirt;               /* Also called the flags field */
//	WORD   key;
//	DWORD  cmd;
//#endif
//} ACCEL, *LPACCEL;

BOOL OnCreate(HWND, LPCREATESTRUCT lpCreateStruct)
{
	ACCEL accel[] =
	{
		{ FSHIFT | FVIRTKEY, 'N', SC_MINIMIZE }, // ��С������Shift+ N
		{ FSHIFT | FVIRTKEY, 'X', SC_MAXIMIZE }, // ��󻯴��� Shift + X
		{ FSHIFT | FVIRTKEY, 'R', SC_RESTORE }, // ��ԭ���� Shift + R
		{ FALT | FCONTROL | FVIRTKEY, 'P', ID_PRIN }, // ������� Ctrl + Alt + P
		{ FCONTROL | FVIRTKEY, VK_F4, SC_CLOSE }, // �˳� Ctrl + F4
	};

	// �������ټ���
	*(HACCEL*)lpCreateStruct->lpCreateParams = CreateAcceleratorTable(accel, 5);

	return TRUE;
}

VOID OnCommand(HWND hwnd, INT id, HWND, UINT)
{
	HDC hDC;
	RECT rc;
	TCHAR szTime[32];
	SYSTEMTIME systime;

	switch (id)
	{
		// �������
	case ID_PRIN:
		GetClientRect(hwnd, &rc);
		GetLocalTime(&systime);
		wsprintf(szTime, _T("%04d.%02d.%02d %02d:%02d:%02d"),
			systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
		DrawText(hDC = GetDC(hwnd), szTime, -1, &rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		ReleaseDC(hwnd, hDC);
		break;
	}
}

VOID OnDestroy(HWND) { PostQuitMessage(0); }

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hwnd, WM_DESTROY, OnDestroy);

	default: return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

//VOID AcceleratorMain()
int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	WNDCLASS wnd = { 0 };
	MSG msg;
	HACCEL hAccel;

	wnd.lpfnWndProc        = WndProc;
	wnd.hInstance        = GetModuleHandle(NULL);
	wnd.hCursor            = LoadCursor(NULL, IDC_ARROW);
	wnd.hbrBackground    = (HBRUSH)COLOR_WINDOWFRAME;
	wnd.lpszClassName    = _T("Accelerator");

	RegisterClass(&wnd);

	if (!CreateWindow((TCHAR*)_T("Accelerator"), _T("Accelerator"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, wnd.hInstance, &hAccel))
		ExitProcess(0);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		// �任���ټ���Ϣ
		if (!TranslateAccelerator(msg.hwnd, hAccel, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//    DestroyAcceleratorTable(hAccel);

	ExitProcess(msg.wParam);
}


