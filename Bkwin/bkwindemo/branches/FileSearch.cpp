/********************************************************************
created:	2008/10/17
filename: 	FileSearch.cpp
file base:	FileSearch
file ext:	cpp
author:		huchao

purpose:	Windows 文件搜索函数的封装
*********************************************************************/
#include "StdAfx.h"
#include "FileSearch.h"
#include "FileSupport.h"

CFileSearch::CFileSearch(void)
{
	m_CurSearchStatus = enumStatus_Stoped;
	m_bIsSearchChildDiretory = FALSE;
}

CFileSearch::~CFileSearch(void)
{
	if (m_CurSearchStatus != enumStatus_Stoped)
		assert(FALSE);
}

BOOL CFileSearch::InitSearch()
{
	BOOL bFunRet = FALSE;

	if (m_CurSearchStatus != enumStatus_Stoped)
	{
		assert(FALSE);
		goto Exit0;
	}
	
	m_CurSearchStatus = enumStatus_Inited;

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSearch::UnInitSearch()
{
	BOOL bFunRet = FALSE;

	m_CurSearchStatus = enumStatus_Stoped;

	if (!FreeItemMemory())
		goto Exit0;

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSearch::SetSearchChildDirectory(BOOL bIsSearchChild)
{
	BOOL bFunRet = FALSE;

	if (m_CurSearchStatus != enumStatus_Inited)
	{
		assert(FALSE);
		goto Exit0;
	}

	m_bIsSearchChildDiretory = bIsSearchChild;

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSearch::AddFileToSearchList(const CString& strFileFullPath)
{
	BOOL bFunRet = FALSE;
	pSearchFileInfo pSFI = NULL;

	if (m_CurSearchStatus != enumStatus_Inited)
	{
		assert(FALSE);
		goto Exit0;
	}

	if (!::PathFileExists(strFileFullPath))
		goto Exit0;

	if (::PathIsDirectory(strFileFullPath))
		goto Exit0;

	pSFI = new SearchFileInfo;
	pSFI->strFullPath = strFileFullPath;
	m_vecFileInfo.push_back(pSFI);

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSearch::AddSearchDiretory(const CString& strFullPath)
{
	BOOL bFunRet = FALSE;
	CString strSearchPath;

	if (m_CurSearchStatus != enumStatus_Inited)
	{
		assert(FALSE);
		goto Exit0;
	}

	strSearchPath = CFileSupport::PathAddBackslashStr(strFullPath);

	if (!::PathFileExists(strSearchPath))
		goto Exit0;

	if (!::PathIsDirectory(strSearchPath))
		goto Exit0;

	m_vecSearchDir.push_back(strSearchPath);

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSearch::DoSearch()
{
	BOOL bFunRet = FALSE;

	if (m_CurSearchStatus != enumStatus_Inited)
	{
		assert(FALSE);
		goto Exit0;
	}

	m_CurSearchStatus = enumStatus_Search;

	_FindFiles();

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSearch::GetSearchResultList(vecFileInfo& vecFiles)
{
	BOOL bFunRet = FALSE;

	vecFiles = m_vecFileInfo;

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSearch::FreeItemMemory()
{
	BOOL bFunRet = FALSE;
	std::vector<pSearchFileInfo>::iterator iter;
	pSearchFileInfo pSFI = NULL;

	for (iter = m_vecFileInfo.begin(); iter != m_vecFileInfo.end(); iter++)
	{
		pSFI = *iter;
		if (pSFI)
		{
			delete pSFI;
			pSFI = NULL;
		}
	}

	bFunRet = TRUE;
Exit0:
	return bFunRet;
}

BOOL CFileSearch::_FindFiles()
{
	std::vector<CString>::iterator iter;

	for (iter = m_vecSearchDir.begin(); iter != m_vecSearchDir.end(); iter++)
	{
		_AddFindFilesToList(*iter);
	}

	return TRUE;
}

BOOL CFileSearch::_AddFindFilesToList(const CString& strDirectory)
{
	BOOL bFunRet = FALSE;
	WIN32_FIND_DATA finddata;
	HANDLE hFind = NULL;
	CString strTempSearchPath;
	CString strTemp;
	pSearchFileInfo pSFI = NULL;

	strTempSearchPath = CFileSupport::PathAddBackslashStr(strDirectory);
	strTempSearchPath += TEXT("*.*");

	hFind = ::FindFirstFile(strTempSearchPath, &finddata);
	if (hFind == INVALID_HANDLE_VALUE) 
		goto Exit0;

	while (::FindNextFile(hFind, &finddata))
	{
		if (wcscmp(finddata.cFileName, TEXT(".")) == 0 || 
			wcscmp(finddata.cFileName, TEXT("..")) == 0)
			continue;

		strTemp = CFileSupport::PathAddBackslashStr(strDirectory);
		strTemp += finddata.cFileName;

		if (::PathIsDirectory(strTemp))
		{
			if (m_bIsSearchChildDiretory)
				_AddFindFilesToList(strTemp);

			continue;
		}

		pSFI = new SearchFileInfo;
		pSFI->strFullPath = strTemp;
		m_vecFileInfo.push_back(pSFI);	// 添加到列表
	}

	bFunRet = TRUE;
Exit0:
	if (hFind)
	{
		::FindClose(hFind);
		hFind = NULL;
	}
	return bFunRet;
}