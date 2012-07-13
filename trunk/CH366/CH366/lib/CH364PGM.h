#pragma once
#include "stdafx.h"

typedef HANDLE (WINAPI *InitCH364IODrv_DLL)(HWND);
typedef BOOL (WINAPI* FindCH366_DLL)(HANDLE, PULONG, PULONG, PVOID); 
typedef BOOL (WINAPI* CH364mReadIoByte_DLL)(HANDLE, PVOID, PUCHAR);
typedef BOOL (WINAPI* CH364mWriteIoByte_DLL)(HANDLE, PVOID, UCHAR); 
typedef BOOL (WINAPI* CloseCH364IODrv_DLL)(HANDLE);


bool g_bOpenFlag = false;
HMODULE g_hCh364PGMDll = NULL;
InitCH364IODrv_DLL    InitCH364IODrv  = NULL;
FindCH366_DLL        FindCH366        = NULL;
CH364mReadIoByte_DLL  CH364mReadIoByte  = NULL;
CH364mWriteIoByte_DLL CH364mWriteIoByte = NULL;
CloseCH364IODrv_DLL   CloseCH364IODrv  = NULL;

//初始化CH364PGMDLL
bool Init364PGMDLL(void)
{
	g_hCh364PGMDll = LoadLibrary(L"CH364PGMDLL.dll");
	if(NULL == g_hCh364PGMDll)
	{// 加载DLL失败,可能未安装到系统中
#ifdef _DEBUG
		OutputDebugString(L"无法加载CH364驱动程序的CH364PGMDLL\r\n");
#endif	
		MessageBox(NULL, L"无法加载CH364驱动程序的CH364PGMDLL", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		return false;
	}

	InitCH364IODrv = (InitCH364IODrv_DLL)GetProcAddress(g_hCh364PGMDll, "InitCH364IODrv");
	FindCH366 = (FindCH366_DLL)GetProcAddress(g_hCh364PGMDll, "FindCH366");
	CH364mReadIoByte = (CH364mReadIoByte_DLL)GetProcAddress(g_hCh364PGMDll, "CH364mReadIoByte");
	CH364mWriteIoByte = (CH364mWriteIoByte_DLL)GetProcAddress(g_hCh364PGMDll, "CH364mWriteIoByte");
	CloseCH364IODrv = (CloseCH364IODrv_DLL)GetProcAddress(g_hCh364PGMDll, "CloseCH364IODrv");

	if (!(InitCH364IODrv&&FindCH366&&CH364mReadIoByte&&CH364mWriteIoByte&&CloseCH364IODrv))
	{
		return false;
	}
	OutputDebugString(L"CH364PGM初始化成功………………\r\n");

	g_bOpenFlag = true;
	return true;
}
//释放CH364PGMDLL
void ReleaseCh364PGM(void)
{
	if (g_hCh364PGMDll)
	{
		FreeLibrary(g_hCh364PGMDll);
	}
	g_hCh364PGMDll = NULL;
	InitCH364IODrv  = NULL;
	FindCH366        = NULL;
	CH364mReadIoByte  = NULL;
	CH364mWriteIoByte = NULL;
	CloseCH364IODrv  = NULL;

	g_bOpenFlag = false;
	OutputDebugString(L"CH364PGM释放成功………………\r\n");
}
















//HANDLE	WINAPI	InitCH364IODrv(    //在使用CH364函数之前先用此函数先做初始化操作,初始化成功后返回一个有效的句柄.
//							   HWND            WindowsHandle);//程序主窗体名柄
//BOOL	WINAPI	FindCH366(         //搜索CH364芯片,获取相关信息
//						  HANDLE          DrvHandle,     //初始化函数时返回的有效句柄
//						  PULONG          CH364_ID,      //CH364芯片的位置号,可以事先指定起始芯片位置号
//						  PULONG          IS_CH366,           
//						  PVOID           CfgRegBaseAddr);//CH364配置寄存器基址
//BOOL	WINAPI	CH364mReadIoByte(   // 从I/O端口读取一个字节
//								 HANDLE          DrvHandle,
//								 PVOID			iAddr,          // 指定I/O端口的地址
//								 PUCHAR			oByte );        // 指向一个字节单元,用于保存读取的字节数据
//BOOL	WINAPI	CH364mWriteIoByte(  // 向I/O端口写入一个字节
//								  HANDLE          DrvHandle,
//								  PVOID			iAddr,          // 指定I/O端口的地址
//								  UCHAR			iByte );        // 待写入的字节数据
//BOOL	WINAPI	CloseCH364IODrv(
//								HANDLE          DrvHandle);     //在结束程序前务必请先使用该函数来做关闭操作