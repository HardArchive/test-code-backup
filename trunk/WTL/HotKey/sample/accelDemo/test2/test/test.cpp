// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Resource.h"
#include <windows.h>
//#include "rc/resource.h"

#define ID_CMD_A      0x00000230
#define ID_CMD_B      0x00000231
#define ID_CMD_C      0x00000232
#define ID_CMD_D      0x00000233
#define ID_CMD_E      0x00000234
#define ID_CMD_F      0x00000235
#define ID_CMD_G      0x00000236
#define ID_CMD_H      0x00000237

//�������߸����ټ�,������Ϣ�ص������д������߸�����ID��
static ACCEL accel[]={
	{FVIRTKEY|FCONTROL,  VK_F5,    ID_CMD_A},
	{FVIRTKEY|FCONTROL,  VK_F6,    ID_CMD_B},
	{FVIRTKEY|FCONTROL,  VK_HOME,  ID_CMD_C},
	{FVIRTKEY|FCONTROL,  VK_END,   ID_CMD_D},
	{FVIRTKEY|FCONTROL,  'G',      ID_CMD_E},
	{FVIRTKEY|FCONTROL,  VK_SPACE, ID_CMD_F},
	{FVIRTKEY|FCONTROL,  'K',      ID_CMD_G},
	{FVIRTKEY|FCONTROL,  'Q',      ID_CMD_H},
};   
#define MAX_LOADSTRING 100
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

//BOOL InitApplication(HINSTANCE hInstance,int nCmdShow);
// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_QREADER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ��ʼ��Ӧ�ó��򣬲�����������.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;           // ��ʼ��ʧ��
	}
	//ʹ�ú���CreateAcceleratorTable������accel�м��ؼ��ټ���
	hAccelTable = CreateAcceleratorTable(accel, sizeof(accel)/sizeof(ACCEL));
	// ȡ�ò��ַ���Ϣֱ�����յ� WM_QUIT ��Ϣ.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//�ڷַ���Ϣǰ���������ü��ټ�����з��룬�����һ�����ټ�����
		//TranslateAccelerator�������з��룬���ټ����������Ϣ��
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//ɾ�����ټ���
	DestroyAcceleratorTable(hAccelTable);
	return msg.wParam;  // Returns the value from PostQuitMessage
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QREADER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_QREADER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case ID_CMD_A:
		case ID_CMD_B:
		case ID_CMD_C:
		case ID_CMD_D:
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case ID_CMD_H:
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_CMD_E:
		case ID_CMD_F:
		case ID_CMD_G:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
