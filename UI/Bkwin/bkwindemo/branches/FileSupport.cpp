/********************************************************************
created:	2008/09/24
filename: 	FileSupport.cpp
file base:	FileSupport
file ext:	cpp
author:		huchao

purpose:	Windows 文件函数的封装，使处理文件、路径更方便
*********************************************************************/
#include "StdAfx.h"
#include "FileSupport.h"
#include "3rdparty/CRC32.h"
#include <shellapi.h>

CFileSupport::CFileSupport(void)
{
}

CFileSupport::~CFileSupport(void)
{
}

//////////////////////////////////////////////////////////////////////////
BOOL CFileSupport::DeleteFileEx(LPCWSTR lpszFullPath)
{
	return _DeleteFileEx(lpszFullPath);
}

BOOL CFileSupport::DeleteFileUntilReboot(LPCWSTR lpszFullPath)
{
	BOOL bFunRet = FALSE;

	if (!::PathFileExists(lpszFullPath))
		goto Exit1;

	if (!::MoveFileEx(lpszFullPath, NULL, MOVEFILE_DELAY_UNTIL_REBOOT))
		goto Exit0;

Exit1:
	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSupport::MoveFileUntilReboot(LPCWSTR lpszExistingFileName, LPCWSTR lpNewFileName)
{
	BOOL bFunRet = FALSE;

	if (wcscmp(lpszExistingFileName, lpNewFileName) == 0)
		goto Exit0;

	if (!::PathFileExists(lpNewFileName))
		goto Exit0;

	if (!::MoveFileEx(lpszExistingFileName, lpNewFileName, MOVEFILE_DELAY_UNTIL_REBOOT))
		goto Exit0;

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

//////////////////////////////////////////////////////////////////////////
DWORD CFileSupport::GetFileSize(LPCWSTR lpszFullPath, LPDWORD lpFileSizeHigh /* = NULL */)
{
	DWORD dwFunRet = 0;
	DWORD dwFileSizeHigh = 0;
	DWORD* pdwFileSizeHigh = NULL;
	DWORD dwSizeTemp = 0;
	HANDLE hFile = NULL;

	hFile = ::CreateFile(lpszFullPath, 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		FALSE, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		goto Exit0;

	dwSizeTemp = ::GetFileSize(hFile, &dwFileSizeHigh);
	pdwFileSizeHigh = &dwFileSizeHigh;
	if (lpFileSizeHigh && pdwFileSizeHigh)
		*lpFileSizeHigh = *pdwFileSizeHigh;

	if (dwSizeTemp == INVALID_FILE_SIZE)
		goto Exit0;

	dwFunRet = dwSizeTemp;
Exit0:
	if (!hFile)
	{
		::CloseHandle(hFile);
		hFile = NULL;
	}

	return dwFunRet;
}

BOOL CFileSupport::GetFileVersionString(LPCWSTR lpszFullPath, LPWSTR lpszVersion)
{
	BOOL bFunRet = FALSE;
	WORD wVer1 = 0;
	WORD wVer2 = 0;
	WORD wVer3 = 0;
	WORD wVer4 = 0;

	if (!_GetFileVersion(lpszFullPath, &wVer1, &wVer2, &wVer3, &wVer4))
		goto Exit0;

	wsprintf(lpszVersion, TEXT("%d.%d.%d.%d"), wVer1, wVer2, wVer3, wVer4);

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

CString CFileSupport::GetFileVersionString(LPCWSTR lpszFullPath)
{
	CString strFunRet;
	WORD wVer1 = 0;
	WORD wVer2 = 0;
	WORD wVer3 = 0;
	WORD wVer4 = 0;

	if (!_GetFileVersion(lpszFullPath, &wVer1, &wVer2, &wVer3, &wVer4))
		goto Exit0;

	strFunRet.Format(TEXT("%d.%d.%d.%d"), wVer1, wVer2, wVer3, wVer4);

Exit0:
	return strFunRet;
}

BOOL CFileSupport::GetFileVersion(LPCWSTR lpszFullPath, WORD* pwVer1, WORD* pwVer2, WORD* pwVer3, WORD* pwVer4)
{
	return _GetFileVersion(lpszFullPath, pwVer1, pwVer2, pwVer3, pwVer4);
}

BOOL CFileSupport::GetFileCreationTime(LPCWSTR lpszFullPath, LPSYSTEMTIME lpTime)
{
	BOOL bFunRet = FALSE;
	FILETIME ft = {0};

	if (!_GetFileTime(lpszFullPath, &ft, NULL, NULL))
		goto Exit0;

	if (!::FileTimeToSystemTime(&ft, lpTime))
		goto Exit0;

	if (!::SystemTimeToTzSpecificLocalTime(NULL, lpTime, lpTime))
		goto Exit0;

	bFunRet = TRUE;
Exit0:

	return bFunRet;
}

BOOL CFileSupport::GetFileLastAccessTime(LPCWSTR lpszFullPath, LPSYSTEMTIME lpTime)
{
	BOOL bFunRet = FALSE;
	FILETIME ft = {0};

	if (!_GetFileTime(lpszFullPath, NULL, &ft, NULL))
		goto Exit0;

	if (!::FileTimeToSystemTime(&ft, lpTime))
		goto Exit0;

	if (!::SystemTimeToTzSpecificLocalTime(NULL, lpTime, lpTime))
		goto Exit0;

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSupport::GetFileLastWrite(LPCWSTR lpszFullPath, LPSYSTEMTIME lpTime)
{
	BOOL bFunRet = FALSE;
	FILETIME ft = {0};

	if (!_GetFileTime(lpszFullPath, NULL, NULL, &ft))
		goto Exit0;

	if (!::FileTimeToSystemTime(&ft, lpTime))
		goto Exit0;

	if (!::SystemTimeToTzSpecificLocalTime(NULL, lpTime, lpTime))
		goto Exit0;

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

//////////////////////////////////////////////////////////////////////////
UINT CFileSupport::GetFileCRC(LPCWSTR lpszFullPath)
{
	UINT	uCRCRet = 0;
	BOOL	bRetCode = 0;
	DWORD	dwReaded = 0;
	PUCHAR	pBuffer = NULL;
	HANDLE	hFile = NULL;
	const size_t cnsize = 1024 * 1024;	// 如果文件大小大于 1MB，则每次取 1MB 进行计算

	__try
	{
		hFile = ::CreateFile(lpszFullPath, 
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			goto Exit0;
		}

		pBuffer = (PUCHAR)malloc(cnsize);
		while (pBuffer)
		{
			bRetCode = ::ReadFile(hFile, pBuffer, cnsize, &dwReaded, NULL);
			if (bRetCode && dwReaded)
			{
				uCRCRet = CRC32(uCRCRet, pBuffer, dwReaded);
			}
			else
			{
				break;
			}
		}
	}
	__finally
	{
		if (hFile && hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			hFile = NULL;
		}
		if (pBuffer)
		{
			free(pBuffer);
			pBuffer = NULL;
		}
	}

Exit0:
	return uCRCRet;
}

//////////////////////////////////////////////////////////////////////////
BOOL CFileSupport::CreateDirectoryEx(LPCWSTR lpszFullPath, BOOL bDelSameNameFile /* = FALSE */)
{
	BOOL bFunRet = FALSE;
	int nTempLen = 0;
	CString strTempPath;
	CString strTemp;

	strTempPath = _ConfirmPathString(lpszFullPath);

	do 
	{
		nTempLen = strTempPath.Find('\\', nTempLen + 1);

		strTemp = strTempPath.Left(nTempLen + 1);

		if (::PathIsRoot(strTemp))
			continue;

		_CreateDirectory(strTemp, bDelSameNameFile);

	} while(nTempLen != strTempPath.GetLength() - 1);

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSupport::GetCurFilePath(LPWSTR lpszFullPath)
{
	BOOL bFunRet = FALSE;
	
	if (!_GetCurFilePath(lpszFullPath))
		goto Exit0;

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

CString CFileSupport::GetCurFilePath()
{
	CString strFunRet;
	WCHAR szCurrentPath[MAX_PATH] = {0};

	if (!_GetCurFilePath(szCurrentPath))
		goto Exit0;

	strFunRet = szCurrentPath;

Exit0:
	return strFunRet;
}

BOOL CFileSupport::GetCurFileName(LPWSTR lpszFullPath)
{
	BOOL bFunRet = FALSE;

	if (!_GetCurFileName(lpszFullPath))
		goto Exit0;

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

CString CFileSupport::GetCurFileName(void)
{
	CString strFunRet;
	WCHAR szCurrentFileName[MAX_PATH] = {0};

	if (!_GetCurFileName(szCurrentFileName))
		goto Exit0;

	strFunRet = szCurrentFileName;

Exit0:
	return strFunRet;
}

BOOL CFileSupport::PathAddBackslash(LPWSTR lpszFullPath)
{
	BOOL bFunRet = FALSE;

	if (wcslen(lpszFullPath) == 0)
		goto Exit0;

	::PathAddBackslash(lpszFullPath);

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

CString CFileSupport::PathAddBackslashStr(const CString& strFullPath)
{
	CString strFunRet;
	int nStrLen = 0;

	nStrLen = strFullPath.GetLength();
	if (nStrLen == 0)
		goto Exit0;

	strFunRet = strFullPath;

	if (strFunRet.Right(1) != '\\')
		strFunRet += TEXT("\\");

Exit0:
	return strFunRet;	
}

BOOL CFileSupport::PathRemoveBackslash(LPWSTR lpszFullPath)
{
	BOOL bFunRet = FALSE;

	::PathRemoveBackslash(lpszFullPath);

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

CString CFileSupport::PathRemoveBackslashStr(const CString& strFullPath)
{
	CString strFunRet;
	int nStrLen = 0;

	nStrLen = strFullPath.GetLength();
	if (nStrLen == 0)
		goto Exit0;

	strFunRet = strFullPath;

	if (strFunRet.Right(1) == '\\')
		strFunRet.Delete(nStrLen - 1);

Exit0:
	return strFunRet;
}

BOOL CFileSupport::PathAddSlash(LPWSTR lpszFullPath)
{
	BOOL bFunRet = FALSE;
	WCHAR* pwChar = NULL;
	int nStrLen = 0;

	nStrLen = (int)wcslen(lpszFullPath);
	if (nStrLen == 0)
		goto Exit0;

	pwChar = lpszFullPath + nStrLen - 1;
	if (*pwChar != '/')
		wcscat(lpszFullPath, TEXT("/"));

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

CString CFileSupport::PathAddSlashStr(const CString& strFullPath)
{
	CString strFunRet;
	int nStrLen = 0;

	nStrLen = strFullPath.GetLength();
	if (nStrLen == 0)
		goto Exit0;

	strFunRet = strFullPath;

	if (strFunRet.Right(1) != '/')
		strFunRet += TEXT("/");

Exit0:
	return strFunRet;	
}

BOOL CFileSupport::PathRemoveSlash(LPWSTR lpszFullPath)
{
	BOOL bFunRet = FALSE;

	WCHAR* pwChar = NULL;
	int nStrLen = 0;

	nStrLen = (int)wcslen(lpszFullPath);
	if (nStrLen == 0)
		goto Exit0;

	pwChar = lpszFullPath + nStrLen - 1;
	if (*pwChar != '/')
		*pwChar = '\0';

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

CString CFileSupport::PathRemoveSlashStr(const CString& strFullPath)
{
	CString strFunRet;
	int nStrLen = 0;

	nStrLen = strFullPath.GetLength();
	if (nStrLen == 0)
		goto Exit0;

	strFunRet = strFullPath;

	if (strFunRet.Right(1) == '/')
		strFunRet.Delete(nStrLen - 1);

Exit0:
	return strFunRet;
}

BOOL CFileSupport::RelpacePathChar(LPWSTR lpszFullPath, const WCHAR cDest, const WCHAR cOrg)
{
	BOOL bFunRet = FALSE;
	CString strTemp;

	strTemp = lpszFullPath;
	strTemp.Replace(cOrg, cDest);
	wcscpy(lpszFullPath, strTemp);

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

CString CFileSupport::RelpacePathCharStr(const CString& strFullPtah, 
										 const WCHAR cDest, 
										 const WCHAR cOrg)
{
	CString strFunRet;

	strFunRet = strFullPtah;
	strFunRet.Replace(cOrg, cDest);

Exit0:
	return strFunRet;
}

BOOL CFileSupport::GetTempFileName(LPWSTR lpszTempFile)
{
	BOOL bFunRet = FALSE;
	DWORD dwRet = 0;
	
	dwRet = ::GetTempPath(MAX_PATH, lpszTempFile);
	if (dwRet == 0)
		goto Exit0;

	dwRet = ::GetTempFileName(lpszTempFile, TEXT("~"), 0, lpszTempFile);
	if (dwRet == 0)
		goto Exit0;

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

CString CFileSupport::GetTempFileName()
{
	CString strFunRet;
	WCHAR szTempPath[MAX_PATH] = {0};
	DWORD dwRet = 0;

	dwRet = ::GetTempPath(MAX_PATH, szTempPath);
	if (dwRet == 0)
		goto Exit0;

	dwRet = ::GetTempFileName(szTempPath, TEXT("~"), 0, szTempPath);
	if (dwRet == 0)
		goto Exit0;

	strFunRet = szTempPath;
Exit0:
	return strFunRet;
}

//////////////////////////////////////////////////////////////////////////
BOOL CFileSupport::_GetFileVersion(LPCWSTR lpszFullPath, WORD* pwVer1, WORD* pwVer2, WORD* pwVer3, WORD* pwVer4)
{
	BOOL bFunRet = FALSE;
	DWORD dwFileInfoSize = 0;
	BYTE* pFileInfo = NULL;
	VS_FIXEDFILEINFO* pFixedFileInfo = NULL;
	UINT cbQuery = 0;
	DWORD dwHandle = 0;

	dwFileInfoSize = ::GetFileVersionInfoSize(lpszFullPath, &dwHandle);
	pFileInfo = new BYTE[dwFileInfoSize];
	if (!dwFileInfoSize || !pFileInfo)
		goto Exit0;
	
	if (!::GetFileVersionInfo(lpszFullPath, dwHandle, dwFileInfoSize, pFileInfo))
		goto Exit0;

	if (!::VerQueryValue(pFileInfo, _T("\\"), (void**)&pFixedFileInfo, &cbQuery))
		goto Exit0;

	*pwVer1 = HIWORD(pFixedFileInfo->dwFileVersionMS);
	*pwVer2 = LOWORD(pFixedFileInfo->dwFileVersionMS);
	*pwVer3 = HIWORD(pFixedFileInfo->dwFileVersionLS);
	*pwVer4 = LOWORD(pFixedFileInfo->dwFileVersionLS);

	bFunRet = TRUE;
Exit0:
	if (pFileInfo)
	{
		delete[] pFileInfo;
		pFileInfo = NULL;
	}

	return bFunRet;
}

BOOL CFileSupport::_GetFileTime(LPCWSTR lpszFullPath,
								LPFILETIME lpCreationTime, 
								LPFILETIME lpLastAccessTime,
								LPFILETIME lpLastWriteTime)
{
	BOOL bFunRet = FALSE;
	BOOL bTemp = FALSE;
	HANDLE hFile = NULL;

	hFile = ::CreateFile(lpszFullPath, 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		FALSE, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		goto Exit0;

	bTemp = ::GetFileTime(hFile, 
		lpCreationTime, 
		lpLastAccessTime, 
		lpLastWriteTime);
	if (!bTemp)
		goto Exit0;

	bFunRet = TRUE;
Exit0:
	if (!hFile)
	{
		::CloseHandle(hFile);
		hFile = NULL;
	}

	return bFunRet;
}

BOOL CFileSupport::_CreateDirectory(LPCWSTR lpszDir, BOOL bDelSameNameFile)
{
	BOOL bFunRet = FALSE;
	BOOL bTemp = FALSE;
	WCHAR szTempDir[MAX_PATH] = {0};

	wcscpy(szTempDir, lpszDir);

	if (wcslen(szTempDir) == 0)
		goto Exit0;

	if (::PathIsDirectory(szTempDir))
		goto Exit1;

	if (bDelSameNameFile)
	{
		if (FALSE == ::PathIsDirectory(szTempDir))
		{
			// 存在了与文件夹同名的文件，先删除同名的文件
			::PathRemoveBackslash(szTempDir);

			if (!_DeleteFileEx(szTempDir))
				goto Exit0;
		}
	}

	bTemp = ::CreateDirectory(szTempDir, FALSE);
	if (!bTemp)
		goto Exit0;

Exit1:
	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

CString CFileSupport::_ConfirmPathString(const CString& strPath)
{
	CString strFunRet;

	if (strPath.IsEmpty())
		goto Exit0;

	if (!::PathIsRoot(strPath.Left(3)))
		goto Exit0;

	strFunRet = strPath;
	strFunRet.Replace('/', '\\');

	if (strFunRet.Right(1) != '\\')
		strFunRet += TEXT("\\");

Exit0:
	return strFunRet;
}

BOOL CFileSupport::_DeleteFileEx(LPCWSTR lpszFullPath)
{
	BOOL bFunRet = FALSE;
	int nTemp = 0;
	WCHAR szPathTemp[MAX_PATH + 2] = {0};
	CString strTemp;

	if (wcslen(lpszFullPath) > MAX_PATH)
		goto Exit0;

	strTemp = lpszFullPath;
	strTemp.Replace('/', '\\');
	wcsncpy(szPathTemp, strTemp, strTemp.GetLength());

	if (::PathFileExists(szPathTemp))
	{
		::SetFileAttributes(szPathTemp, FILE_ATTRIBUTE_NORMAL);
		nTemp = ::DeleteFile(szPathTemp);
		if (nTemp == 0)
		{	// 删除失败，则用 SHFileOperation 删除
			SHFILEOPSTRUCT sf = {0};
			sf.hwnd = NULL;
			sf.wFunc = FO_DELETE;
			sf.pFrom = szPathTemp;
			sf.pTo = NULL;
			sf.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_SILENT;
			sf.fAnyOperationsAborted = FALSE;

			nTemp = ::SHFileOperation(&sf);
			if (nTemp != 0)
			{
				bFunRet = FALSE;
				goto Exit0;
			}
		}
	}

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSupport::_GetCurFilePath(LPWSTR lpszFullPath)
{
	BOOL bFunRet = FALSE;
	WCHAR szFilePath[MAX_PATH] = {0};
	WCHAR szFileNameTemp[MAX_PATH] = {0};
	DWORD dwRet = 0;

	dwRet = ::GetModuleFileName(NULL, 
		szFilePath, 
		MAX_PATH);
	if (dwRet == 0)
		goto Exit0;

	wcscpy(szFileNameTemp, ::PathFindFileName(szFilePath));
	wcsncpy(lpszFullPath, szFilePath, wcslen(szFilePath) - wcslen(szFileNameTemp));

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSupport::_GetCurFileName(LPWSTR lpszFullPath)
{
	BOOL bFunRet = FALSE;
	WCHAR szFilePath[MAX_PATH] = {0};
	DWORD dwRet = 0;

	dwRet = ::GetModuleFileName(NULL, 
		szFilePath, 
		MAX_PATH);
	if (dwRet == 0)
		goto Exit0;

	wcscpy(lpszFullPath, ::PathFindFileName(szFilePath));

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}