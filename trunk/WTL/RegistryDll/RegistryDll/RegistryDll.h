// RegistryDll.h

#include <Shlwapi.h>   
#pragma comment(lib, "Shlwapi.lib")    



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