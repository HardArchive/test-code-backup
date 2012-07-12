#include "stdafx.h"

typedef int	(WINAPI *OpenCH366_DLL)(void);
typedef int (WINAPI* GetNet_DLL)(void);
typedef int (WINAPI* ChangeNet_DLL)(BOOL, int, BOOL); 
typedef void (WINAPI* CloseCH366_DLL)(void); 
typedef void (WINAPI* MyTRACE_DLL)(WCHAR*, ...);

bool g_bOpenFlag = false;
HMODULE g_hCh366Dll = NULL;
OpenCH366_DLL OpenCH366 = NULL;
GetNet_DLL GetNet = NULL;
ChangeNet_DLL ChangeNet = NULL;
CloseCH366_DLL CloseCH366 = NULL;
MyTRACE_DLL MyTRACE = NULL;

//extern "C" __declspec(dllexport) int OpenCH366(void);
//extern "C" __declspec(dllexport) int GetNet(void);
//extern "C" __declspec(dllexport) int ChangeNet(BOOL bNetBit, int iPowerOff, BOOL bInterFace);
////�ر�366�豸
//extern "C" __declspec(dllexport) void CloseCH366(void);
////�Զ���TRACE����
//extern "C" __declspec(dllexport) void MyTRACE(WCHAR *fmt, ...);

bool InitCH366DLL(void)
{
	hCh366Dll = LoadLibrary(L"CH366.dll");
	if (!hCh366Dll)
	{
		return false;
	}

	OpenCH366 = (OpenCH366_DLL)GetProcAddress(hCh366Dll, "OpenCH366");
	GetNet = (GetNet_DLL)GetProcAddress(hCh366Dll, "GetNet");
	ChangeNet = (ChangeNet_DLL)GetProcAddress(hCh366Dll, "ChangeNet");
	CloseCH366 = (CloseCH366_DLL)GetProcAddress(hCh366Dll, "CloseCH366");
	MyTRACE = (MyTRACE_DLL)GetProcAddress(hCh366Dll, "MyTRACE");

	if (!(OpenCH366&&GetNet&&ChangeNet&&CloseCH366&&MyTRACE))
	{
		return false;
	}

	g_bOpenFlag true;
	return true;
}

void ReleaseCh366(void)
{
	if (hCh366Dll)
	{
		FreeLibrary(hCh366Dll);
	}
	hCh366Dll = NULL;
	OpenCH366 = NULL;
	GetNet = NULL;
	ChangeNet = NULL;
	CloseCH366 = NULL;
	MyTRACE = NULL;
	g_bOpenFlag = false;

}