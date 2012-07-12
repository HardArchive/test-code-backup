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

//��ʼ��CH364PGMDLL
bool Init364PGMDLL(void)
{
	g_hCh364PGMDll = LoadLibrary(L"CH364PGMDLL.dll");
	if(NULL == g_hCh364PGMDll)
	{// ����DLLʧ��,����δ��װ��ϵͳ��
#ifdef _DEBUG
		OutputDebugString(L"�޷�����CH364���������CH364PGMDLL\r\n");
#endif	
		MessageBox(NULL, L"�޷�����CH364���������CH364PGMDLL", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
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
	OutputDebugString(L"CH364PGM��ʼ���ɹ�������������\r\n");

	g_bOpenFlag = true;
	return true;
}
//�ͷ�CH364PGMDLL
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
	OutputDebugString(L"CH364PGM�ͷųɹ�������������\r\n");
}
















//HANDLE	WINAPI	InitCH364IODrv(    //��ʹ��CH364����֮ǰ���ô˺���������ʼ������,��ʼ���ɹ��󷵻�һ����Ч�ľ��.
//							   HWND            WindowsHandle);//��������������
//BOOL	WINAPI	FindCH366(         //����CH364оƬ,��ȡ�����Ϣ
//						  HANDLE          DrvHandle,     //��ʼ������ʱ���ص���Ч���
//						  PULONG          CH364_ID,      //CH364оƬ��λ�ú�,��������ָ����ʼоƬλ�ú�
//						  PULONG          IS_CH366,           
//						  PVOID           CfgRegBaseAddr);//CH364���üĴ�����ַ
//BOOL	WINAPI	CH364mReadIoByte(   // ��I/O�˿ڶ�ȡһ���ֽ�
//								 HANDLE          DrvHandle,
//								 PVOID			iAddr,          // ָ��I/O�˿ڵĵ�ַ
//								 PUCHAR			oByte );        // ָ��һ���ֽڵ�Ԫ,���ڱ����ȡ���ֽ�����
//BOOL	WINAPI	CH364mWriteIoByte(  // ��I/O�˿�д��һ���ֽ�
//								  HANDLE          DrvHandle,
//								  PVOID			iAddr,          // ָ��I/O�˿ڵĵ�ַ
//								  UCHAR			iByte );        // ��д����ֽ�����
//BOOL	WINAPI	CloseCH364IODrv(
//								HANDLE          DrvHandle);     //�ڽ�������ǰ�������ʹ�øú��������رղ���