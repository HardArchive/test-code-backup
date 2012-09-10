//file RegistryDllUtil.h
/***************************************************************************************************
* 1�� Class      �� CRegistryDllUtil
* 2�� Version    �� 1.0
* 3�� Description�� COM���ע���뷴ע��
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-9-9 13:27:03
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
#ifndef __REGISTRY_DLL_UNTIL_H__
#define __REGISTRY_DLL_UNTIL_H__
#include <MessageboxUtil.h>
#include <Shlwapi.h>   
#pragma comment(lib, "Shlwapi.lib")   

namespace QNA
{
	//RegistryDll����ע��DLL��������DLL��ȫ·��������ֵ����ע��ɹ�����ʧ�ܣ�trueΪ�ɹ�����   
	bool RegistryDll(TCHAR* ptszDllPath)   
	{
		if (!ptszDllPath)
		{
			MessageboxUtil(NULL, _T("����·������!!!!!"), _T("DDLLע����ʾ"), MB_OK);
			return false;
		}
		if (!(PathFileExists(ptszDllPath) && (!PathIsDirectory(ptszDllPath))))   
		{   
			MessageboxUtil(NULL, MB_OK, "DLLע����ʾ", _T("ע��%s �ļ���ʱ�򣬷������󣺸��ļ������ڣ�/r/n"), ptszDllPath)
			return FALSE;   
		}   
		LRESULT (CALLBACK* lpDllEntryPoint)();    
		HINSTANCE hLib = LoadLibrary(ptszDllPath);   
		if (hLib < (HINSTANCE)HINSTANCE_ERROR)    
			return FALSE;    
		(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, "DllRegisterServer");    
		BOOL bRet = FALSE;   
		if (lpDllEntryPoint != NULL)   
		{   
			HRESULT hr = (*lpDllEntryPoint)();   
			bRet = SUCCEEDED(hr);   
			if (FAILED(hr))   
			{   
				CString szErrMsg;   
				if (GetErrMessage(hr, szErrMsg))   
				{   
					wprintf(L"ע��%s �ļ���ʱ�򣬷�������%s/r/n", szDllPath, szErrMsg);   
				}   
				else  
				{   
					wprintf(L"ע��%s �ļ���ʱ�򣬷������󣬵����޷���ȡ������Ϣ���������Ϊ��x%08x/r/n", szDllPath, hr);   
				}   
			}   
		}   
		FreeLibrary(hLib);   
		return bRet;   
	}   




}
#endif

//RegistryDll����ע��DLL��������DLL��ȫ·��������ֵ����ע��ɹ�����ʧ�ܣ�TRUEΪ�ɹ�����   
BOOL RegistryDll(CString& szDllPath)   
{   
	if (!(PathFileExists(szDllPath) && (!PathIsDirectory(szDllPath))))   
	{   
		wprintf(L"ע��%s �ļ���ʱ�򣬷������󣺸��ļ������ڣ�/r/n", szDllPath);   
		//����Ĵ�����Ϣ�б�Ҫ�Ļ������Ի���OutputDebugString�ķ�ʽ��ӡ����   
		return FALSE;   
	}   
	LRESULT (CALLBACK* lpDllEntryPoint)();    
	HINSTANCE hLib = LoadLibrary(szDllPath);   
	if (hLib < (HINSTANCE)HINSTANCE_ERROR)    
		return FALSE;    
	(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, "DllRegisterServer");    
	BOOL bRet = FALSE;   
	if (lpDllEntryPoint != NULL)   
	{   
		HRESULT hr = (*lpDllEntryPoint)();   
		bRet = SUCCEEDED(hr);   
		if (FAILED(hr))   
		{   
			CString szErrMsg;   
			if (GetErrMessage(hr, szErrMsg))   
			{   
				wprintf(L"ע��%s �ļ���ʱ�򣬷�������%s/r/n", szDllPath, szErrMsg);   
			}   
			else  
			{   
				wprintf(L"ע��%s �ļ���ʱ�򣬷������󣬵����޷���ȡ������Ϣ���������Ϊ��x%08x/r/n", szDllPath, hr);   
			}   
		}   
	}   
	FreeLibrary(hLib);   
	return bRet;   
}   

//UnRegistryDll������ע��DLL��������DLL��ȫ·��������ֵ����ע��ɹ�����ʧ�ܣ�TRUEΪ�ɹ�����   
BOOL UnRegistryDll(CString& szDllPath)   
{   
	if (!(PathFileExists(szDllPath) && (!PathIsDirectory(szDllPath))))   
	{   
		wprintf(L"��ע��%s �ļ���ʱ�򣬷������󣺸��ļ������ڣ�/r/n", szDllPath);   
		return FALSE;   
	}   
	LRESULT (CALLBACK* lpDllEntryPoint)();    
	HINSTANCE hLib = LoadLibrary(szDllPath);   
	if (hLib < (HINSTANCE)HINSTANCE_ERROR)    
		return FALSE;    
	(FARPROC&)lpDllEntryPoint = GetProcAddress(hLib, "DllUnregisterServer");    
	BOOL bRet = FALSE;   
	if (lpDllEntryPoint != NULL)   
	{   
		HRESULT hr = (*lpDllEntryPoint)();   
		bRet = SUCCEEDED(hr);   
		if (FAILED(hr))   
		{   
			CString szErrMsg;   
			if (GetErrMessage(hr, szErrMsg))   
			{   
				wprintf(L"��ע��%s �ļ���ʱ�򣬷�������%s/r/n", szDllPath, szErrMsg);   
			}   
			else  
			{   
				wprintf(L"��ע��%s �ļ���ʱ�򣬷������󣬵����޷���ȡ������Ϣ���������Ϊ��x%08x/r/n", szDllPath, hr);   
			}   
		}   
	}   
	FreeLibrary(hLib);   
	return bRet;   
}   