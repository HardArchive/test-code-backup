#include "stdafx.h"
#include "KFunction.h"
#include "framework\kanregisterkey.h"
#include <TlHelp32.h>
#include "shellapi.h"
#include <ShlObj.h>

#pragma comment(lib, "shell32.lib")

CString KFunction::GetModulePath(HMODULE hModule)
{
	DWORD dwRet = 0;
	CString strTemp;
	TCHAR szFileName[MAX_PATH * 4 + 1] = {0};

	dwRet = GetModuleFileName(hModule, szFileName, MAX_PATH * 4);
	if (!dwRet) return _T("");

	strTemp = szFileName;
	int nPos = -1;
	nPos = strTemp.ReverseFind(_T('\\'));
	if (-1 == nPos)
	{
		nPos = strTemp.ReverseFind(_T('/'));
	}

	if (-1 == nPos) return _T("");

	return strTemp.Left(nPos + 1);
}

CString KFunction::GetPlatformLang(void)
{
	CKANRegisterKey reg;
	int nRetCode = 0;
	const int cnBufLen = 12;
	TCHAR szTempBuf[cnBufLen] = {0};
	CString strLang;

	LPCTSTR szKeyLang = _T("Lang");
	LPCTSTR szRegKisCommon = _T("SOFTWARE\\Kingsoft\\KISCommon");

	nRetCode = reg.Open(HKEY_LOCAL_MACHINE, szRegKisCommon);
	if (!nRetCode) goto Exit0;

	nRetCode = reg.Read(szKeyLang, szTempBuf, cnBufLen);
	if (!nRetCode) goto Exit0;

	strLang = szTempBuf;
	strLang.MakeLower();

Exit0:
	if (strLang.IsEmpty()) strLang = _T("chs");

	return strLang;
}

CString KFunction::GetPlatformRootPath(void)
{
	CKANRegisterKey reg;
	int nRetCode = 0;
	static CString s_strRootPath;
	TCHAR szTempBuf[MAX_PATH] = {0};
	LPCTSTR szRegKxeEngine = _T("SOFTWARE\\Kingsoft\\KISCommon\\KXEngine");
	LPCTSTR szKeyPath = _T("path");

	if (!s_strRootPath.IsEmpty()) goto Exit0;

	nRetCode = reg.Open(HKEY_LOCAL_MACHINE, szRegKxeEngine);
	if (!nRetCode) goto Exit0;

	nRetCode = reg.Read(szKeyPath, szTempBuf, MAX_PATH);
	if (!nRetCode) goto Exit0;

	s_strRootPath = szTempBuf;
	TCHAR ch = s_strRootPath.GetAt(s_strRootPath.GetLength() - 1);
	if (ch != _T('\\') && ch != _T('/'))
	{
		s_strRootPath += _T("\\");
	}
	s_strRootPath.MakeLower();

Exit0:
	return s_strRootPath;
}

CString KFunction::GetProductRootPath(void)
{
	CKANRegisterKey reg;
	int nRetCode = 0;
	static CString s_strRootPath;
	TCHAR szTempBuf[MAX_PATH] = {0};

	LPCTSTR szRegAntivirus = _T("SOFTWARE\\Kingsoft\\Antivirus");
	LPCTSTR szKeyPath = _T("ProgramPath");

	if (!s_strRootPath.IsEmpty()) goto Exit0;

	nRetCode = reg.Open(HKEY_LOCAL_MACHINE, szRegAntivirus);
	if (!nRetCode) goto Exit0;

	nRetCode = reg.Read(szKeyPath, szTempBuf, MAX_PATH);
	if (!nRetCode) goto Exit0;

	s_strRootPath = szTempBuf;
	TCHAR ch = s_strRootPath.GetAt(s_strRootPath.GetLength() - 1);
	if (ch != _T('\\') && ch != _T('/'))
	{
		s_strRootPath += _T("\\");
	}
	s_strRootPath.MakeLower();

Exit0:
	return s_strRootPath;
}

CString KFunction::GetPlatformResPath(void)
{
	CString strResPath;

	strResPath = KFunction::GetPlatformRootPath();
	strResPath += _T("ressrc\\");
	strResPath += GetPlatformLang();
	strResPath += _T("\\");
	strResPath.MakeLower();

	return strResPath;
}

BOOL KFunction::IsFileInUse(LPCTSTR szPath)
{
	BOOL bResult = FALSE;

	HANDLE hFile = CreateFile(szPath, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile ==  INVALID_HANDLE_VALUE)
	{
		DWORD dwErr = GetLastError();
		if (dwErr == ERROR_SHARING_VIOLATION || dwErr == ERROR_ACCESS_DENIED)
			bResult = TRUE;
	}
	else
		CloseHandle(hFile);

	return bResult;
}

BOOL KFunction::CreateDeepDirectory(LPCTSTR szPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	CString strPath(szPath);
	CString strDirectory;

	int nIndex = 0;
	if (strPath.Find(_T(':')) != -1)
	{
		nIndex = 3;
	}

	for (; nIndex < strPath.GetLength(); nIndex++)
	{
		if (szPath[nIndex] == _T('\\') || szPath[nIndex] == _T('/'))
		{
			strDirectory = strPath.Left(nIndex + 1);
			bRetCode = ::CreateDirectory(strDirectory, NULL);
			if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
				goto Exit0;
		}
	}

	bRetCode = ::CreateDirectory(szPath, NULL);
	if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS)
		goto Exit0;

	bReturn = TRUE;
Exit0:
	return bReturn;
}

typedef DWORD (WINAPI *PFN_GetModuleFileNameExW)(HANDLE hProcess, HMODULE hModule, LPWSTR lpFilename, DWORD nSize);
typedef DWORD (WINAPI *PFN_GetProcessImageFileNameW)(HANDLE hProcess, LPWSTR lpImageFileName, DWORD nSize);

static PFN_GetModuleFileNameExW g_pfnGetModuleFileNameExW = NULL;
static PFN_GetProcessImageFileNameW g_pfnGetProcessImageFileNameW = NULL;

BOOL _InitPsapiFunc(void)
{
	if (g_pfnGetModuleFileNameExW || g_pfnGetProcessImageFileNameW)
		return TRUE;

	HMODULE hModule = ::LoadLibrary(L"Psapi.dll");
	if (!hModule) return FALSE;

	g_pfnGetModuleFileNameExW = (PFN_GetModuleFileNameExW)::GetProcAddress(hModule, "GetModuleFileNameExW");
	g_pfnGetProcessImageFileNameW = (PFN_GetProcessImageFileNameW)::GetProcAddress(hModule, "GetProcessImageFileNameW");

	log_a("g_pfnGetModuleFileNameExW:0x%x\n", g_pfnGetModuleFileNameExW);
	log_a("g_pfnGetProcessImageFileNameW:0x%x\n", g_pfnGetProcessImageFileNameW);

	if (g_pfnGetModuleFileNameExW || g_pfnGetProcessImageFileNameW)
		return TRUE;

	return FALSE;
}

int KFunction::KillProcessByPath(LPCTSTR szPath, int* pnKillCount /* = NULL */, BOOL bDirectory /* = FALSE */)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	DWORD dwRetLen = 0;
	HANDLE hProcess = NULL;
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
	TCHAR szProcessPath[MAX_PATH] = { 0 }; 
	PROCESSENTRY32 pe32 = { 0 };
	int nKillCount = 0;
	int nFailCount = 0;
	CString strPath;

	_InitPsapiFunc();

	if (KFunction::IsWow64())
	{
		CString strDisk;
		TCHAR szDevicePath[MAX_PATH] = {0};
		
		strDisk.Format(_T("%c:"), szPath[0]);
		::QueryDosDevice(strDisk, szDevicePath, MAX_PATH);
		strPath = szDevicePath;
		KFunction::PathAddBackslash(strPath);

		if (_tcslen(szPath) > 3)
			strPath += &szPath[3];
	}
	else
		strPath = szPath;

	log_w(L"KillProcessByPath path:%s\n", (LPCTSTR)strPath);
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE) goto Exit0;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if(!Process32First(hProcessSnap, &pe32)) goto Exit0;
	int nCurrentPID = ::GetCurrentProcessId();

	do {
		if (pe32.th32ProcessID == nCurrentPID || 
			pe32.th32ProcessID == 0 ||
			pe32.th32ProcessID == 4)
		{
			continue;
		}
		hProcess = OpenProcess( 
			PROCESS_ALL_ACCESS,
			FALSE,
			pe32.th32ProcessID);
		if( NULL != hProcess ) 
		{ 
			dwRetLen = 0;
			if (KFunction::IsWow64() && g_pfnGetProcessImageFileNameW)
			{
				//64位系统下，这里要转换一下路径，不然会匹配失败
				dwRetLen = g_pfnGetProcessImageFileNameW(hProcess, szProcessPath, MAX_PATH);
			}
			else if (g_pfnGetModuleFileNameExW)
			{
				dwRetLen = g_pfnGetModuleFileNameExW(hProcess, NULL, szProcessPath, MAX_PATH);
			}

			if (dwRetLen)
			{
				if (bDirectory)
				{
					if (dwRetLen > strPath.GetLength() && _tcsnicmp(strPath, szProcessPath, strPath.GetLength()) == 0)
					{
						bRetCode = ::TerminateProcess(hProcess, 0);
						log_w(L"Terminate Process return:%d, pid:%d, path:%s\n", bRetCode, pe32.th32ProcessID, szProcessPath);
						if (!bRetCode) nFailCount++;
						else nKillCount++;
					}
				}
				else
				{
					if (dwRetLen == strPath.GetLength() && _tcsicmp(strPath, szProcessPath) == 0)
					{
						bRetCode = ::TerminateProcess(hProcess, 0);
						log_w(L"Terminate Process return:%d, pid:%d, path:%s\n", bRetCode, pe32.th32ProcessID, szProcessPath);
						if (!bRetCode) nFailCount++;
						else nKillCount++;
					}
				}
			}
			CloseHandle(hProcess); 
			hProcess = NULL;
		}
	} while(Process32Next(hProcessSnap, &pe32));

	bReturn = TRUE;

Exit0:

	if(NULL != hProcess) CloseHandle(hProcess);
	if(INVALID_HANDLE_VALUE != hProcessSnap) CloseHandle(hProcessSnap);

	if (pnKillCount)
		*pnKillCount = nKillCount;

	return nFailCount;
}

BOOL KFunction::GetDebugPrivilege(void)
{
	BOOL bReturn = FALSE;
	HANDLE hToken = NULL;
	int nRetCode = ERROR_SUCCESS;
	LUID CurrentLUID = { 0 };
	TOKEN_PRIVILEGES TokenPrivileges = { 0 };

	__try
	{
		nRetCode = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
		if (!nRetCode) __leave;

		nRetCode = ::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &CurrentLUID);
		if (!nRetCode) __leave;

		TokenPrivileges.PrivilegeCount = 1;
		TokenPrivileges.Privileges[0].Luid = CurrentLUID;
		TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		nRetCode = ::AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, 0, NULL, NULL);
		if (!nRetCode) __leave;

		bReturn = TRUE;
	}
	__finally
	{
		if (hToken)
		{
			::CloseHandle(hToken);
			hToken = NULL;
		}
	}

Exit0:
	return bReturn;
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
BOOL KFunction::IsWow64()
{
	static int s_nIsWow64 = -1;

	if (s_nIsWow64 != -1)
		return s_nIsWow64;

	LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
		GetModuleHandle(L"kernel32"),"IsWow64Process");

	if (NULL != fnIsWow64Process)
	{
		if (fnIsWow64Process(GetCurrentProcess(),&s_nIsWow64))
		{
			return s_nIsWow64;
		}
	}

	s_nIsWow64 = FALSE;
	
	return s_nIsWow64;
}

BOOL KFunction::RunApp(LPCTSTR szPath, LPCTSTR szFile, LPCTSTR szCmd, DWORD dwWaitTime /* = 0 */)
{
	BOOL bReturn = FALSE;
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };

	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;

	CString strExePath(szPath);	
	KFunction::PathAddBackslash(strExePath);
	strExePath += szFile;
	strExePath += _T(" ");
	strExePath += szCmd;

	bReturn = CreateProcessW(NULL,			// No module name (use command line). 
		(LPWSTR)(LPCTSTR)strExePath,      // Command line. 
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		TRUE,             // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		szPath,             // Use parent's starting directory. 
		&si,              // Pointer to STARTUPINFO structure.
		&pi				// Pointer to PROCESS_INFORMATION structure.
		);
	if (bReturn)
	{
		::WaitForSingleObject(pi.hThread, dwWaitTime);
	}

Exit0:
	if (pi.hProcess != NULL)
	{
		CloseHandle(pi.hProcess);
		pi.hProcess = NULL;
	}
	if (pi.hThread != NULL)
	{
		CloseHandle(pi.hThread);
		pi.hThread = NULL;
	}

	log_w(L"CreateProcess return:%d, path:%s, file:%s, cmd:%s\n", bReturn, szPath, szFile, szCmd);
	return bReturn;
}

BOOL KFunction::ShellRunApp(LPCTSTR szPath, LPCTSTR szFile, LPCTSTR szCmd)
{
	CString strExePath(szPath);	
	
	KFunction::PathAddBackslash(strExePath);
	strExePath += szFile;

	int nRetCode = (int)::ShellExecute(NULL, _T("open"), strExePath, szCmd, szPath, SW_HIDE);

	return nRetCode > 32;
}

void KFunction::PathAddBackslash(CString& strPath)
{
	if (strPath.IsEmpty()) return;

	TCHAR ch = strPath.GetAt(strPath.GetLength() - 1);
	if (ch != _T('\\') && ch != _T('/'))
	{
		strPath += _T("\\");
	}
}

BOOL KFunction::_CopyFolder(LPCTSTR szSrcPath, LPCTSTR szDstPath, BOOL& bReboot)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	WIN32_FIND_DATA find_data;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	CString strSrcPath(szSrcPath);
	CString strDstPath(szDstPath);

	hFind = ::FindFirstFile(strSrcPath + _T("*"), &find_data);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		bReturn = TRUE;
		goto Exit0;
	}

	do{
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmp(find_data.cFileName, _T(".")) != 0 &&
				lstrcmp(find_data.cFileName, _T("..")) != 0)
			{
				bRetCode = ::CreateDirectory(strDstPath + find_data.cFileName, NULL);
				if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS) goto Exit0;

				bRetCode = KFunction::_CopyFolder(strSrcPath + find_data.cFileName + _T("\\"),
					strDstPath + find_data.cFileName + _T("\\"), bReboot);
				if (!bRetCode) goto Exit0;
			}
		}
		else
		{
			bRetCode = ::CopyFile(strSrcPath + find_data.cFileName, strDstPath + find_data.cFileName, FALSE);
			if (!bRetCode) 
			{
				bRetCode = ::CopyFile(strSrcPath + find_data.cFileName, strDstPath + find_data.cFileName + _T("_up"), FALSE);
				if (!bRetCode) goto Exit0;
				bRetCode = ::MoveFileEx(strDstPath + find_data.cFileName + _T("_up"), strDstPath + find_data.cFileName, MOVEFILE_DELAY_UNTIL_REBOOT | MOVEFILE_REPLACE_EXISTING);
				if (!bRetCode) goto Exit0;
				bReboot = TRUE;
			}
		}

		bRetCode = ::FindNextFile(hFind, &find_data);

	} while(bRetCode);

	bReturn = TRUE;
Exit0:
	if (hFind)
		FindClose(hFind);

	return bReturn;
}

BOOL KFunction::_CopyFolder_Delay(LPCTSTR szSrcPath, LPCTSTR szDstPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	WIN32_FIND_DATA find_data;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	CString strSrcPath(szSrcPath);
	CString strDstPath(szDstPath);

	hFind = ::FindFirstFile(strSrcPath + _T("*"), &find_data);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		bReturn = TRUE;
		goto Exit0;
	}

	do{
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmp(find_data.cFileName, _T(".")) != 0 &&
				lstrcmp(find_data.cFileName, _T("..")) != 0)
			{
				bRetCode = ::CreateDirectory(strDstPath + find_data.cFileName, NULL);
				if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS) goto Exit0;

				bRetCode = KFunction::_CopyFolder_Delay(strSrcPath + find_data.cFileName + _T("\\"),
					strDstPath + find_data.cFileName + _T("\\"));
				if (!bRetCode) goto Exit0;
			}
		}
		else
		{
			::CopyFile(strSrcPath + find_data.cFileName, strDstPath + find_data.cFileName + _T("_up"), FALSE);
			bRetCode = ::MoveFileEx(strDstPath + find_data.cFileName + _T("_up"), strDstPath + find_data.cFileName, MOVEFILE_DELAY_UNTIL_REBOOT | MOVEFILE_REPLACE_EXISTING);
			if (!bRetCode) goto Exit0;
		}

		bRetCode = ::FindNextFile(hFind, &find_data);

	} while(bRetCode);

	bReturn = TRUE;
Exit0:
	if (hFind)
		FindClose(hFind);

	return bReturn;
}

BOOL KFunction::CopyFolder(LPCTSTR szSrcPath, LPCTSTR szDstPath, BOOL& bReboot)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	CString strSrcPath(szSrcPath);
	CString strDstPath(szDstPath);

	KFunction::PathAddBackslash(strSrcPath);
	KFunction::PathAddBackslash(strDstPath);

	bRetCode = CreateDeepDirectory(strDstPath);
	if (!bRetCode) goto Exit0;

	bRetCode = _CopyFolder(strSrcPath, strDstPath, bReboot);
	if (!bRetCode) goto Exit0;

	bReturn = TRUE;

Exit0:
	log_w(L"KFunction::CopyFolder return:%d, bRetoot:%d, src:%s, dst:%s\n", bReturn, bReboot, szSrcPath, szDstPath);

	return bReturn;
}

BOOL KFunction::CopyFolder_Delay(LPCTSTR szSrcPath, LPCTSTR szDstPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	CString strSrcPath(szSrcPath);
	CString strDstPath(szDstPath);

	KFunction::PathAddBackslash(strSrcPath);
	KFunction::PathAddBackslash(strDstPath);

	bRetCode = CreateDeepDirectory(strDstPath);
	if (!bRetCode) goto Exit0;

	bRetCode = _CopyFolder_Delay(strSrcPath, strDstPath);
	if (!bRetCode) goto Exit0;

	bReturn = TRUE;

Exit0:
	log_w(L"KFunction::CopyFolder_Delay return:%d, src:%s, dst:%s\n", bReturn, szSrcPath, szDstPath);

	return bReturn;
}

BOOL KFunction::CopyFolder_Force(LPCTSTR szSrcPath, LPCTSTR szDstPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	CString strSrcPath(szSrcPath);
	CString strDstPath(szDstPath);

	KFunction::PathAddBackslash(strSrcPath);
	KFunction::PathAddBackslash(strDstPath);

	bRetCode = CreateDeepDirectory(strDstPath);
	if (!bRetCode) goto Exit0;

	bRetCode = _CopyFolder_Force(strSrcPath, strDstPath);
	if (!bRetCode) goto Exit0;

	bReturn = TRUE;

Exit0:
	log_w(L"KFunction::CopyFolder_Force return:%d, src:%s, dst:%s\n", bReturn, szSrcPath, szDstPath);

	return bReturn;
}

BOOL KFunction::_CopyFolder_Force(LPCTSTR szSrcPath, LPCTSTR szDstPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	WIN32_FIND_DATA find_data;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	CString strSrcPath(szSrcPath);
	CString strDstPath(szDstPath);

	hFind = ::FindFirstFile(strSrcPath + _T("*"), &find_data);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		bReturn = TRUE;
		goto Exit0;
	}

	do{
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmp(find_data.cFileName, _T(".")) != 0 &&
				lstrcmp(find_data.cFileName, _T("..")) != 0)
			{
				bRetCode = ::CreateDirectory(strDstPath + find_data.cFileName, NULL);
				if (!bRetCode && ::GetLastError() != ERROR_ALREADY_EXISTS) goto Exit0;

				bRetCode = KFunction::_CopyFolder_Force(strSrcPath + find_data.cFileName + _T("\\"),
					strDstPath + find_data.cFileName + _T("\\"));
				if (!bRetCode) goto Exit0;
			}
		}
		else
		{			
			bRetCode = ::CopyFile(strSrcPath + find_data.cFileName, strDstPath + find_data.cFileName, FALSE);
			if (!bRetCode)
			{
				bRetCode = ::MoveFile(strDstPath + find_data.cFileName, strDstPath + find_data.cFileName + _T("_del"));
				if (bRetCode)
				{
					::MoveFileEx(strDstPath + find_data.cFileName + _T("_del"), NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
					bRetCode = ::CopyFile(strSrcPath + find_data.cFileName, strDstPath + find_data.cFileName, FALSE);
					log_w(L"CopyFolder_Force CopyFile fail, try rename return:%d, filename:%s\n", bRetCode, strDstPath + find_data.cFileName);
					if (!bRetCode)
					{
						bRetCode = ::CopyFile(strSrcPath + find_data.cFileName, strDstPath + find_data.cFileName + _T("_up"), FALSE);
						if (bRetCode)
							::MoveFileEx(strDstPath + find_data.cFileName + _T("_up"), strDstPath + find_data.cFileName, MOVEFILE_DELAY_UNTIL_REBOOT | MOVEFILE_REPLACE_EXISTING);		
					}
				}
				else
				{
					log_w(L"CopyFolder_Force rename fail, try MoveFileEx filename:%s\n", strDstPath + find_data.cFileName);
					bRetCode = ::CopyFile(strSrcPath + find_data.cFileName, strDstPath + find_data.cFileName + _T("_up"), FALSE);
					if (bRetCode)
						::MoveFileEx(strDstPath + find_data.cFileName + _T("_up"), strDstPath + find_data.cFileName, MOVEFILE_DELAY_UNTIL_REBOOT | MOVEFILE_REPLACE_EXISTING);		
				}
			}

			if (KFunction::IsDriverFile(find_data.cFileName))
			{
				if (_tcsicmp(find_data.cFileName, _T("kisknl64.sys")) == 0)
				{
					if (KFunction::IsWow64())
					{
						KFunction::EnableWow64FsRedirection(FALSE);
						bRetCode = ::CopyFile(strSrcPath + find_data.cFileName, KFunction::GetDriversPath() + _T("kisknl.sys"), FALSE);
						log_w(L"Copy driver file return:%d, src:%ws, dst:%ws\n", bRetCode, strSrcPath + find_data.cFileName, KFunction::GetDriversPath() + _T("kisknl.sys"));
						KFunction::EnableWow64FsRedirection(TRUE);
					}
				}
				else if (_tcsicmp(find_data.cFileName, _T("kisknl.sys")) == 0)
				{
					if (!KFunction::IsWow64())
					{
						bRetCode = ::CopyFile(strSrcPath + find_data.cFileName, KFunction::GetDriversPath() + _T("kisknl.sys"), FALSE);
						log_w(L"Copy driver file return:%d, src:%ws, dst:%ws\n", bRetCode, strSrcPath + find_data.cFileName, KFunction::GetDriversPath() + _T("kisknl.sys"));
					}
				}
				else
				{
					bRetCode = ::CopyFile(strSrcPath + find_data.cFileName, KFunction::GetDriversPath() + find_data.cFileName, FALSE);
					log_w(L"Copy driver file return:%d, src:%ws, dst:%ws\n", bRetCode, strSrcPath + find_data.cFileName, KFunction::GetDriversPath() + find_data.cFileName);
				}
			}
		}

		bRetCode = ::FindNextFile(hFind, &find_data);

	} while(bRetCode);

	bReturn = TRUE;
Exit0:
	if (hFind)
		FindClose(hFind);

	return bReturn;
}

BOOL KFunction::_IsEnableCopy(LPCTSTR szSrcPath, LPCTSTR szDstPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	WIN32_FIND_DATA find_data;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	CString strSrcPath(szSrcPath);
	CString strDstPath(szDstPath);

	hFind = ::FindFirstFile(strSrcPath + _T("*"), &find_data);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		bReturn = TRUE;
		goto Exit0;
	}

	do{
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmp(find_data.cFileName, _T(".")) != 0 &&
				lstrcmp(find_data.cFileName, _T("..")) != 0)
			{
				bRetCode = KFunction::_IsEnableCopy(strSrcPath + find_data.cFileName + _T("\\"),
					strDstPath + find_data.cFileName + _T("\\"));
				if (!bRetCode) goto Exit0;
			}
		}
		else
		{
			if (KFunction::IsFileInUse(strDstPath + find_data.cFileName))
			{
				log_w(L"KFunction::_IsEnableCopy IsFileInUse return:1, path:%s\n", (LPCTSTR)(strDstPath + find_data.cFileName));
				goto Exit0;
			}
		}

		bRetCode = ::FindNextFile(hFind, &find_data);

	} while(bRetCode);

	bReturn = TRUE;
Exit0:
	if (hFind)
		FindClose(hFind);

	return bReturn;
}

BOOL KFunction::IsEnableCopy(LPCTSTR szSrcPath, LPCTSTR szDstPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	CString strSrcPath(szSrcPath);
	CString strDstPath(szDstPath);

	KFunction::PathAddBackslash(strSrcPath);
	KFunction::PathAddBackslash(strDstPath);

	bRetCode = _IsEnableCopy(strSrcPath, strDstPath);
	if (!bRetCode) goto Exit0;

	bReturn = TRUE;

Exit0:
	log_w(L"KFunction::IsEnableCopy return:%d, src:%s, dst:%s\n", bReturn, szSrcPath, szDstPath);
	return bReturn;
}

BOOL KFunction::IsDirectoryExist(LPCTSTR szPath)
{
	DWORD dwAttr = ::GetFileAttributes(szPath);

	if (dwAttr == -1 || (dwAttr & FILE_ATTRIBUTE_DIRECTORY) == 0)
		return FALSE;

	return TRUE;
}

BOOL KFunction::IsScomRegistered(LPCTSTR szGUID, LPCTSTR szPath)
{
	int nRetCode = 0;
	BOOL bReturn = FALSE;	
	CString strValue;
	CString strSubKey;
	CKANRegisterKey key;

	strSubKey.Format(_T("SOFTWARE\\Kingsoft\\KISCommon\\KXEngine\\CLASSES_ROOT\\CLSID\\%s\\InprocServer32"), szGUID);
	nRetCode = key.Open(HKEY_LOCAL_MACHINE, strSubKey);
	if (!nRetCode) goto Exit0;

	DWORD dwLen = MAX_PATH;
	PTCHAR pBuf = strValue.GetBuffer(MAX_PATH + 1);
	nRetCode = key.Read(NULL, pBuf, dwLen);
	pBuf[MAX_PATH] = 0;
	strValue.ReleaseBuffer();
	if (strValue.IsEmpty()) goto Exit0;

	strValue.Replace(_T('/'), _T('\\'));
	if (strValue.CompareNoCase(szPath) == 0)
	{
		bReturn = TRUE;
	}

Exit0:
	log_w(L"IsScomRegistered return:%d, guid:%s, path:%s\n", bReturn, szGUID, szPath);
	return bReturn;
}

BOOL KFunction::RebootSystem()
{
	HANDLE hToken = NULL;
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;

	bRetCode = OpenProcessToken(
		GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, 
		&hToken
		);
	if (!bRetCode)
	{
		goto Exit0;
	}

	TOKEN_PRIVILEGES tkp; 
	bRetCode = LookupPrivilegeValue(
		NULL, 
		SE_SHUTDOWN_NAME, 
		&tkp.Privileges[0].Luid
		); 
	if (!bRetCode)
		goto Exit0;

	tkp.PrivilegeCount = 1;  
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 

	AdjustTokenPrivileges(
		hToken, 
		FALSE, 
		&tkp, 
		0, 
		(PTOKEN_PRIVILEGES)NULL,
		0
		); 
	if (GetLastError() != ERROR_SUCCESS) 
	{
		goto Exit0;
	}

	DWORD dwOption = 0;

	dwOption = EWX_REBOOT | EWX_FORCE;
	bRetCode = ExitWindowsEx(
		dwOption, 
		SHTDN_REASON_MAJOR_APPLICATION
		);
	goto Exit0;

	bReturn = TRUE;

Exit0:
	if (hToken) CloseHandle(hToken);

	log_a("KFunction::RebootSystem return:%d\n", bReturn);

	return bReturn;
}

BOOL KFunction::UninstallService(LPCTSTR szServiceName)
{
	BOOL bReturn = FALSE;
	SC_HANDLE hScm = NULL;
	SC_HANDLE hService = NULL;

	hScm = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (!hScm) goto Exit0;

	hService = ::OpenService(
		hScm,
		szServiceName,
		DELETE
		);
	if (!hService) goto Exit0;

	bReturn = ::DeleteService(hService);

Exit0:
	if (hScm)
		CloseServiceHandle(hScm);

	if (hService)
		CloseServiceHandle(hService);
	
	if (!bReturn)
	{
		CString strSubKey;
		strSubKey.Format(_T("SYSTEM\\CurrentControlSet\\Services\\%s"), szServiceName);
		LONG lRetCode = DeleteSubKeyTree(HKEY_LOCAL_MACHINE, strSubKey);
		log_a("UninstallService DeleteSubKeyTree:%ws return:%d\n", szServiceName, lRetCode);
	}
	else
	{
		log_a("UninstallService:%ws succeed\n", szServiceName);
	}

	return bReturn;
}

CString KFunction::GetDriversPath(void)
{
	static CString strDriversPath;

	if (!strDriversPath.IsEmpty())
		return strDriversPath;

	TCHAR szSysPath[MAX_PATH];
	DWORD dwRetLen = 0;

	dwRetLen = ::GetSystemDirectory(szSysPath, MAX_PATH);
	if (!dwRetLen) goto Exit0;

	strDriversPath = szSysPath;
	KFunction::PathAddBackslash(strDriversPath);
	strDriversPath += _T("drivers\\");	

Exit0:
	return strDriversPath;
}

CString KFunction::GetKisAllUserPath(void)
{
	CString strPath;
	TCHAR szAppDataPath[MAX_PATH] = {0};

	BOOL bRetCode = SHGetSpecialFolderPath(NULL, szAppDataPath, CSIDL_COMMON_APPDATA, TRUE);
	if (!bRetCode) goto Exit0;

	strPath = szAppDataPath;
	if (strPath.IsEmpty()) goto Exit0;

	KFunction::PathAddBackslash(strPath);
	strPath += _T("kingsoft\\kis\\");

	KFunction::CreateDeepDirectory(strPath);

	if (!KFunction::IsDirectoryExist(strPath))
	{
		strPath.Empty();
	}

Exit0:
	return strPath;
}

BOOL KFunction::DeleteFolder(LPCTSTR szPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	CString strPath(szPath);

	KFunction::PathAddBackslash(strPath);

	bRetCode = _DeleteFolder(strPath);
	if (!bRetCode) goto Exit0;

	bReturn = TRUE;

Exit0:
	log_w(L"KFunction::DeleteFolder return:%d, path:%s\n", bReturn, strPath);

	return bReturn;
}

BOOL KFunction::_DeleteFolder(LPCTSTR szPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	WIN32_FIND_DATA find_data;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	CString strPath(szPath);

	hFind = ::FindFirstFile(strPath + _T("*"), &find_data);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		bReturn = TRUE;
		goto Exit0;
	}

	do{
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (lstrcmp(find_data.cFileName, _T(".")) != 0 &&
				lstrcmp(find_data.cFileName, _T("..")) != 0)
			{
				bRetCode = KFunction::_DeleteFolder(strPath + find_data.cFileName + _T("\\"));
				if (!bRetCode) goto Exit0;
			}
		}
		else
		{
			::SetFileAttributes(strPath + find_data.cFileName, 0);
			bRetCode = ::DeleteFile(strPath + find_data.cFileName);
			if (!bRetCode)
			{
				::MoveFileEx(strPath + find_data.cFileName, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
			}
		}

		bRetCode = ::FindNextFile(hFind, &find_data);

	} while(bRetCode);

	bReturn = TRUE;
Exit0:
	if (hFind)
		FindClose(hFind);

	RemoveDirectory(strPath);

	return bReturn;
}

LONG KFunction::DeleteSubKeyTree(HKEY hKey, LPCTSTR lpSubKey)
{
	LONG lResult;
	HKEY hSubKey;
	DWORD dwIndex, cbName;
	TCHAR szSubKey[512];
	FILETIME ft;

	lResult = RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &hSubKey);
	if (lResult != ERROR_SUCCESS)
	{
		RegCloseKey(hSubKey);
		return lResult;
	}

	dwIndex = 0;
	cbName = sizeof(szSubKey)/sizeof(szSubKey[0]);
	while (ERROR_SUCCESS == (lResult = RegEnumKeyEx(hSubKey, dwIndex, szSubKey, &cbName, NULL, NULL, NULL, &ft)))
	{
		DeleteSubKeyTree(hSubKey, szSubKey);
	}

	RegCloseKey(hSubKey);
	lResult = RegDeleteKey(hKey, lpSubKey);

	return lResult;
}

BOOL KFunction::IsProcessRunning(LPCTSTR szPath)
{
	BOOL bReturn = FALSE;
	BOOL bRetCode = FALSE;
	DWORD dwRetLen = 0;
	HANDLE hProcess = NULL;
	HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
	TCHAR szProcessPath[MAX_PATH] = { 0 }; 
	PROCESSENTRY32 pe32 = { 0 };
	int nKillCount = 0;
	int nFailCount = 0;
	CString strPath;

	_InitPsapiFunc();

	if (KFunction::IsWow64())
	{
		CString strDisk;
		TCHAR szDevicePath[MAX_PATH] = {0};

		strDisk.Format(_T("%c:"), szPath[0]);
		::QueryDosDevice(strDisk, szDevicePath, MAX_PATH);
		strPath = szDevicePath;
		KFunction::PathAddBackslash(strPath);

		if (_tcslen(szPath) > 3)
			strPath += &szPath[3];
	}
	else
		strPath = szPath;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE) goto Exit0;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	if(!Process32First(hProcessSnap, &pe32)) goto Exit0;
	int nCurrentPID = ::GetCurrentProcessId();

	do {
		if (pe32.th32ProcessID == nCurrentPID || 
			pe32.th32ProcessID == 0 ||
			pe32.th32ProcessID == 4)
		{
			continue;
		}

		hProcess = OpenProcess( 
			PROCESS_ALL_ACCESS,
			FALSE,
			pe32.th32ProcessID);
		if( NULL != hProcess ) 
		{ 
			dwRetLen = 0;
			if (KFunction::IsWow64() && g_pfnGetProcessImageFileNameW)
			{
				//64位系统下，这里要转换一下路径，不然会匹配失败
				dwRetLen = g_pfnGetProcessImageFileNameW(hProcess, szProcessPath, MAX_PATH);
			}
			else if (g_pfnGetModuleFileNameExW)
			{
				dwRetLen = g_pfnGetModuleFileNameExW(hProcess, NULL, szProcessPath, MAX_PATH);
			}

			if (dwRetLen)
			{
				if (dwRetLen == strPath.GetLength() && _tcsicmp(strPath, szProcessPath) == 0)
				{
					bReturn = TRUE;
					goto Exit0;
				}
			}
			CloseHandle(hProcess); 
			hProcess = NULL;
		}
	} while(Process32Next(hProcessSnap, &pe32));


Exit0:

	if(NULL != hProcess) CloseHandle(hProcess);
	if(INVALID_HANDLE_VALUE != hProcessSnap) CloseHandle(hProcessSnap);

	return bReturn;
}

BOOL KFunction::IsDriverFile(LPCTSTR szPath)
{
	int nLength = _tcslen(szPath);

	if (nLength < 5)
		return FALSE;

	if (_tcsicmp(&szPath[nLength - 4], _T(".sys")) == 0)
	{
		return TRUE;
	}
	
	return FALSE;
}

typedef UINT (WINAPI *PFN_Wow64EnableWow64FsRedirection)(__in BOOLEAN bWow64FsEnableRedirection);

BOOL KFunction::EnableWow64FsRedirection(BOOL bEnable)
{
	static PFN_Wow64EnableWow64FsRedirection pfnWow64EnableWow64FsRedirection = NULL;

	if (!pfnWow64EnableWow64FsRedirection)
	{
		HMODULE hModule = GetModuleHandle( L"Kernel32.dll" );
		if( hModule != NULL )
		{
			pfnWow64EnableWow64FsRedirection = (PFN_Wow64EnableWow64FsRedirection)GetProcAddress( hModule, "Wow64EnableWow64FsRedirection" );
		}
	}

	if (pfnWow64EnableWow64FsRedirection)
	{
		pfnWow64EnableWow64FsRedirection(bEnable);
	}

	return TRUE;
}
