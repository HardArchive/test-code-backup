/*
Module : FileFind.h
Purpose: Implementation of FileFind class for WTL which performs local file searches

Copyright (c) 2003 by iLYA Solnyshkin. 
E-mail: isolnyshkin@yahoo.com 
All rights reserved.
*/


#include "stdafx.h"
#include "FileFind.h"

////////////////////////////////////////////////////////////////////////////
// CFileFind implementation

CFileFind::CFileFind()
{
	m_pFoundInfo = NULL;
	m_pNextInfo = NULL;
	m_hContext = NULL;
	m_chDirSeparator = '\\';
}

CFileFind::~CFileFind()
{
	Close();
}

void CFileFind::Close()
{
	if (m_pFoundInfo != NULL)
	{
		delete m_pFoundInfo;
		m_pFoundInfo = NULL;
	}

	if (m_pNextInfo != NULL)
	{
		delete m_pNextInfo;
		m_pNextInfo = NULL;
	}

	if (m_hContext != NULL && m_hContext != INVALID_HANDLE_VALUE)
	{
		CloseContext();
		m_hContext = NULL;
	}
}

void CFileFind::CloseContext()
{
	::FindClose(m_hContext);
	return;
}

BOOL CFileFind::FindFile(LPCTSTR pstrName /* = NULL */, DWORD dwUnused /* = 0 */)
{
	Close();
	m_pNextInfo = new WIN32_FIND_DATA;
	m_bGotLast = FALSE;

	if (pstrName == NULL)
		pstrName = _T("*.*");
	lstrcpy(((WIN32_FIND_DATA*) m_pNextInfo)->cFileName, pstrName);

	m_hContext = ::FindFirstFile(pstrName, (WIN32_FIND_DATA*) m_pNextInfo);

	if (m_hContext == INVALID_HANDLE_VALUE)
	{
		DWORD dwTemp = ::GetLastError();
		Close();
		::SetLastError(dwTemp);
		return FALSE;
	}

	char pstrRoot[ _MAX_PATH ];
	LPCSTR pstr = _tfullpath((LPSTR)pstrRoot, pstrName, _MAX_PATH);

	// passed name isn't a valid path but was found by the API
	ATLASSERT(pstr != NULL);
	if (pstr == NULL)
	{
		m_strRoot = "";
		Close();
		::SetLastError(ERROR_INVALID_NAME);
		return FALSE;
	}
	else
	{
		// find the last forward or backward whack
		LPTSTR pstrBack  = _tcsrchr(pstrRoot, '\\');
		LPTSTR pstrFront = _tcsrchr(pstrRoot, '/');

		if (pstrFront != NULL || pstrBack != NULL)
		{
			if (pstrFront == NULL)
				pstrFront = (char*)pstrRoot;
			if (pstrBack == NULL)
				pstrBack = (char*)pstrRoot;

			// from the start to the last whack is the root

			if (pstrFront >= pstrBack)
				*pstrFront = '\0';
			else
				*pstrBack = '\0';
		}
		m_strRoot = "";
	}

	return TRUE;
}

BOOL CFileFind::MatchesMask(DWORD dwMask) const
{
	if( m_hContext == NULL)
		return FALSE;

	if (m_pFoundInfo != NULL)
		return (!!(((LPWIN32_FIND_DATA) m_pFoundInfo)->dwFileAttributes & dwMask));
	else
		return FALSE;
}

BOOL CFileFind::GetLastAccessTime(FILETIME* pTimeStamp) const
{
	if( m_hContext == NULL)
		return FALSE;
	if( pTimeStamp == NULL)
		return FALSE;

	if (m_pFoundInfo != NULL && pTimeStamp != NULL)
	{
		*pTimeStamp = ((LPWIN32_FIND_DATA) m_pFoundInfo)->ftLastAccessTime;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CFileFind::GetLastWriteTime(FILETIME* pTimeStamp) const
{
	if( m_hContext == NULL)
		return FALSE;
	if( pTimeStamp == NULL)
		return FALSE;

	if (m_pFoundInfo != NULL && pTimeStamp != NULL)
	{
		*pTimeStamp = ((LPWIN32_FIND_DATA) m_pFoundInfo)->ftLastWriteTime;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CFileFind::GetCreationTime(FILETIME* pTimeStamp) const
{
	if( m_hContext == NULL)
		return FALSE;

	if (m_pFoundInfo != NULL && pTimeStamp != NULL)
	{
		*pTimeStamp = ((LPWIN32_FIND_DATA) m_pFoundInfo)->ftCreationTime;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL CFileFind::IsDots() const
{
	if( m_hContext == NULL)
		return FALSE;

	// return TRUE if the file name is "." or ".." and
	// the file is a directory

	BOOL bResult = FALSE;
	if (m_pFoundInfo != NULL && IsDirectory())
	{
		LPWIN32_FIND_DATA pFindData = (LPWIN32_FIND_DATA) m_pFoundInfo;
		if (pFindData->cFileName[0] == '.')
		{
			if (pFindData->cFileName[1] == '\0' ||
				(pFindData->cFileName[1] == '.' &&
				 pFindData->cFileName[2] == '\0'))
			{
				bResult = TRUE;
			}
		}
	}

	return bResult;
}

BOOL CFileFind::FindNextFile()
{
	if( m_hContext == NULL)
		return FALSE;

	if (m_pFoundInfo == NULL)
		m_pFoundInfo = new WIN32_FIND_DATA;

	void* pTemp = m_pFoundInfo;
	m_pFoundInfo = m_pNextInfo;
	m_pNextInfo = pTemp;

	return ::FindNextFile(m_hContext, (LPWIN32_FIND_DATA) m_pNextInfo);
}

std::string CFileFind::GetFileURL() const
{
	if( m_hContext == NULL)
		return FALSE;

	std::string strResult = "file://";
	strResult += GetFilePath();
	return strResult;
}

std::string CFileFind::GetRoot() const
{
	if( m_hContext == NULL)
		return FALSE;

	return m_strRoot;
}

std::string CFileFind::GetFilePath() const
{
	if( m_hContext == NULL)
		return FALSE;

	std::string strResult = m_strRoot;
	if (strResult[strResult.size()-1] != '\\' &&
		strResult[strResult.size()-1] != '/')
		strResult += m_chDirSeparator;
	strResult += GetFileName();
	return strResult;
}

std::string CFileFind::GetFileTitle() const
{
	if( m_hContext == NULL)
		return FALSE;

	std::string strFullName = GetFileName();
	char strResult[_MAX_PATH];

	_tsplitpath( strFullName.c_str(), NULL, NULL, strResult, NULL);
	return strResult;
}

std::string CFileFind::GetFileName() const
{
	if( m_hContext == NULL)
		return FALSE;

	std::string ret;

	if (m_pFoundInfo != NULL)
		ret = ((LPWIN32_FIND_DATA) m_pFoundInfo)->cFileName;
	return ret;
}

DWORD CFileFind::GetLength() const
{
	if( m_hContext == NULL)
		return FALSE;

	if (m_pFoundInfo != NULL)
		return ((LPWIN32_FIND_DATA) m_pFoundInfo)->nFileSizeLow;
	else
		return 0;
}
