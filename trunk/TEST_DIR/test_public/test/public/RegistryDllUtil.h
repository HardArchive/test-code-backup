//file RegistryDllUtil.h
/***************************************************************************************************
* 1、 Class      ： CRegistryDllUtil
* 2、 Version    ： 1.0
* 3、 Description： COM组件注册与反注册
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-9-9 13:27:03
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
#ifndef __REGISTRY_DLL_UNTIL_H__
#define __REGISTRY_DLL_UNTIL_H__
#include <MessageboxUtil.h>
#include <Shlwapi.h>   
#pragma comment(lib, "Shlwapi.lib")   

namespace QNA
{
	//RegistryDll用来注册DLL，参数是DLL的全路径，返回值代表注册成功或者失败（true为成功）。   
	bool RegistryDll(TCHAR* ptszDllPath)   
	{
		if (!ptszDllPath)
		{
			MessageboxUtil(NULL, _T("传入路径错误!!!!!"), _T("DDLL注册提示"), MB_OK);
			return false;
		}
		if (!(PathFileExists(ptszDllPath) && (!PathIsDirectory(ptszDllPath))))   
		{   
			MessageboxUtil(NULL, MB_OK, "DLL注册提示", _T("注册%s 文件的时候，发生错误：该文件不存在！/r/n"), ptszDllPath)
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
					wprintf(L"注册%s 文件的时候，发生错误：%s/r/n", szDllPath, szErrMsg);   
				}   
				else  
				{   
					wprintf(L"注册%s 文件的时候，发生错误，但是无法获取错误信息，错误代码为：x%08x/r/n", szDllPath, hr);   
				}   
			}   
		}   
		FreeLibrary(hLib);   
		return bRet;   
	}   




}
#endif

//RegistryDll用来注册DLL，参数是DLL的全路径，返回值代表注册成功或者失败（TRUE为成功）。   
BOOL RegistryDll(CString& szDllPath)   
{   
	if (!(PathFileExists(szDllPath) && (!PathIsDirectory(szDllPath))))   
	{   
		wprintf(L"注册%s 文件的时候，发生错误：该文件不存在！/r/n", szDllPath);   
		//这里的错误信息有必要的话，可以换成OutputDebugString的方式打印出来   
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
				wprintf(L"注册%s 文件的时候，发生错误：%s/r/n", szDllPath, szErrMsg);   
			}   
			else  
			{   
				wprintf(L"注册%s 文件的时候，发生错误，但是无法获取错误信息，错误代码为：x%08x/r/n", szDllPath, hr);   
			}   
		}   
	}   
	FreeLibrary(hLib);   
	return bRet;   
}   

//UnRegistryDll用来反注册DLL，参数是DLL的全路径，返回值代表注册成功或者失败（TRUE为成功）。   
BOOL UnRegistryDll(CString& szDllPath)   
{   
	if (!(PathFileExists(szDllPath) && (!PathIsDirectory(szDllPath))))   
	{   
		wprintf(L"反注册%s 文件的时候，发生错误：该文件不存在！/r/n", szDllPath);   
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
				wprintf(L"反注册%s 文件的时候，发生错误：%s/r/n", szDllPath, szErrMsg);   
			}   
			else  
			{   
				wprintf(L"反注册%s 文件的时候，发生错误，但是无法获取错误信息，错误代码为：x%08x/r/n", szDllPath, hr);   
			}   
		}   
	}   
	FreeLibrary(hLib);   
	return bRet;   
}   