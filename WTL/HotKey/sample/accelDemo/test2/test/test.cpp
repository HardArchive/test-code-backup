// test.cpp : 定义控制台应用程序的入口点。
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

//定义了七个加速键,请在消息回调函数中处理这七个命令ID。
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
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

//BOOL InitApplication(HINSTANCE hInstance,int nCmdShow);
// 此代码模块中包含的函数的前向声明:
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
	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_QREADER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 初始化应用程序，并生成主窗口.
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;           // 初始化失败
	}
	//使用函数CreateAcceleratorTable从数组accel中加载加速键表
	hAccelTable = CreateAcceleratorTable(accel, sizeof(accel)/sizeof(ACCEL));
	// 取得并分发消息直到接收到 WM_QUIT 消息.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		//在分发消息前首先试着用加速键表进行翻译，如果是一个加速键则由
		//TranslateAccelerator函数进行翻译，不再继续处理该消息。
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//删除加速键表
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		// 分析菜单选择:
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
		// TODO: 在此添加任意绘图代码...
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

// “关于”框的消息处理程序。
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
