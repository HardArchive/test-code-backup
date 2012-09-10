// RegistryDll.h

#include <Shlwapi.h>   
#pragma comment(lib, "Shlwapi.lib")    



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