
#include "StdAfx.h"
#include "CSySkin.h"

CSySkin::CSySkin(void)
{
	m_hModule = NULL;
	m_pFunc = NULL;
	m_strPath = GetSrandPath();
	m_strDLLPath = m_strPath + _T("Skin.dll");
	m_strSkinPath = m_strPath + _T("Skin.skn");
}

CSySkin::~CSySkin(void)
{
}

CString CSySkin::GetModulePathName()
{
	CString strPath;
	TCHAR szPath[_MAX_PATH] = {0};
	DWORD dwRet = GetModuleFileName(NULL, szPath, _MAX_PATH);
	if ( dwRet > 0 ) {
		strPath.Format(_T("%s"), szPath);
		int index = strPath.ReverseFind('\\');
		strPath = strPath.Left(index+1);
	}
	return strPath;
}

CString CSySkin::GetSysPath()
{
	CString strPath;
	TCHAR szPath[_MAX_PATH] = {0};
	UINT iRet = GetSystemDirectory(szPath, _MAX_PATH);
	if ( iRet > 0 ) {
		strPath.Format(_T("%s"), szPath);
		strPath += _T("\\");
	}
	return strPath;
}

CString CSySkin::GetSrandPath()
{
	CString strResult, strTmp;
	CTime mTime = CTime::GetCurrentTime();
	strResult = mTime.Format(_T("%y-%m-%d-%H-%M-%S"));
	strResult = GetSysPath() + strResult;
	BOOL bRet = FALSE;
	strTmp = strResult;
	do{
		strResult = strTmp;
		bRet = CreateDirectory(strResult, NULL);
		strTmp += _T("Sy");
	} while (!bRet);
	strResult += _T("\\");
	HideFolder(strResult);
	return strResult;
}

BOOL CSySkin::HideFolder(CString in_Path)
{
	return SetFileAttributes(in_Path, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM);
}

CStringA CSySkin::W2CStringA(CString in_Str)
{
	CStringA strResult;
#ifdef _UNICODE
	CW2A w2a(in_Str);
	PCHAR pszResult = (LPSTR)w2a;
	strResult.Format("%s", pszResult);
#else
	strResult = in_Str;
#endif
	return strResult;
}

CStringW CSySkin::A2CStringW(CString in_Str)
{
	CStringW strResult;
#ifndef _UNICODE
	CA2W a2w(in_Str);
	PWCHAR pszResult = (LPWSTR)a2w;
	strResult.Format( L"%s", pszResult);
#else
	strResult = in_Str;
#endif
	return strResult;
}


BOOL CSySkin::ReleaseRes(UINT nRes_ID, CString strRes_Type, CString in_Path)
{
	if ( nRes_ID > 0 && strRes_Type.GetLength() > 0 && in_Path.GetLength() > 0 ) {
		HRSRC hRsrc = FindResource(NULL, MAKEINTRESOURCE(nRes_ID), strRes_Type);
		if ( hRsrc != NULL ) {
			DWORD dwSize = SizeofResource(NULL, hRsrc);
			if ( dwSize > 0 ) {
				HGLOBAL hGlobal = LoadResource(NULL, hRsrc); 
				if ( hGlobal != NULL ) {
					LPVOID pBuffer = LockResource(hGlobal); 
					if ( pBuffer != NULL) {
						CFile mFile;
						BOOL bRet = mFile.Open(in_Path, CFile::modeCreate | CFile::modeReadWrite);
						if ( bRet ) {
							mFile.Write(pBuffer, dwSize);
							mFile.Close();
							return TRUE;
						}
					}
				}
			}
		}
	}
	return FALSE;
}

BOOL CSySkin::ReleaseRes(CString in_strBase64, CString in_Path)
{
	if ( in_strBase64.GetLength() > 0 && in_Path.GetLength() > 0 ) {
		int iSrcLen = in_strBase64.GetLength();
		int iRstLen = Base64DecodeGetRequiredLength( iSrcLen );
		iRstLen++;
		LPBYTE szByteData = new BYTE[iRstLen];
		ZeroMemory(szByteData, iRstLen);
		BOOL bRet = Base64Decode(W2CStringA(in_Path), iSrcLen, szByteData, &iRstLen);
		if ( bRet ) {
			CFile mFile;
			bRet = mFile.Open(in_Path, CFile::modeCreate | CFile::modeReadWrite);
			if ( bRet ) {
				mFile.Write(szByteData, iRstLen);
				mFile.Close();
				delete [] szByteData;
				return TRUE;
			}
		}
		delete [] szByteData;
	}
	return FALSE;
}

BOOL CSySkin::LoadDLLFromRes(UINT nRes_ID, CString strRes_Type)
{
	BOOL bRet = FALSE;
	bRet = ReleaseRes(nRes_ID, strRes_Type, m_strDLLPath);
	if ( bRet ) {
		return LoadDLLFromPath(m_strDLLPath);
	}
	return FALSE;
}

BOOL CSySkin::LoadDLLFromPath(CString in_Path)
{
	m_strDLLPath = in_Path;
	m_hModule  = LoadLibrary(m_strDLLPath);
	if ( m_hModule ) {
		PCHAR pFuncName = "LoadSkin";
		m_pFunc = (PFUNC)GetProcAddress(m_hModule, pFuncName);
		if ( m_pFunc ) {
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CSySkin::LoadSkinFromRes(UINT nRes_ID, CString strRes_Type)
{
	return ReleaseRes(nRes_ID, strRes_Type, m_strSkinPath);
}

BOOL CSySkin::LoadSkinFromPath(CString in_Path)
{
	m_strSkinPath = in_Path;
	return TRUE;
}

BOOL CSySkin::InstallSkin()
{
	if ( m_pFunc && m_strSkinPath.GetLength() > 0 && PathFileExists(m_strSkinPath) ) {
		return m_pFunc(W2CStringA(m_strSkinPath).GetBuffer());
	}
	return FALSE;
}

BOOL CSySkin::UnInstallSkin(BOOL bDelete)
{
	BOOL bRet = FreeLibrary(m_hModule);
	if ( bDelete == TRUE ) {
		DeleteFile(m_strDLLPath);
		DeleteFile(m_strSkinPath);
	}
	if ( m_strPath.GetLength() > 0 && bDelete == TRUE ) {
		RemoveDirectory(m_strPath);
	}
	return bRet;
}