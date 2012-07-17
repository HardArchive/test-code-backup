#include "stdafx.h"
#include "FindFileCore.h"

KFindFileCore::KFindFileCore()
{
	m_callback = NULL;
	m_bStop = FALSE;
}

KFindFileCore::~KFindFileCore()
{
	this->Stop();
}

void KFindFileCore::Stop(void)
{
	m_bStop = TRUE;
}

void KFindFileCore::SetCallback(PFNFindCallBack callback, LPVOID param)
{
	m_callback	= callback;
	m_param		= param;
}

void KFindFileCore::Find(const TCHAR* szPath, const TCHAR* szExpression /* = _T("*") */, int nFile /*= enumFile*/, int nDepth /*= -1*/)
{
	CString strPath = szPath;
	CString strExpression = szExpression;

	if (strPath.IsEmpty() || strExpression.IsEmpty() || !m_callback)
	{
		return;
	}

	m_bStop = FALSE;
	this->MakePath(strPath);	
	this->__Find(strPath, strExpression, nFile, nDepth);	
}

BOOL KFindFileCore::__Find(CString strPath, CString& strExpression, int nFile, int nDepth)
{
	BOOL bReturn = TRUE;
	BOOL bFindNext = FALSE;
	BOOL bCallBackReturn = FALSE;	
	int  nTempDepth = -1;
	WIN32_FIND_DATA find_data;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	CString strFindFileName = strPath + strExpression;

	if (!m_callback)
	{
		bReturn = FALSE;
		goto Exit0;
	}
	
	hFind = ::FindFirstFile(strFindFileName, &find_data);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		bReturn = TRUE;
		goto Next0;
	}

	do{
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (nFile != enumFindFile &&
				lstrcmp(find_data.cFileName, _T(".")) != 0 &&
				lstrcmp(find_data.cFileName, _T("..")) != 0)
			{
				bCallBackReturn = m_callback(strPath + find_data.cFileName + _T("\\"), enumFindDir, m_param);
				if (bCallBackReturn == FALSE)
				{
					bReturn = FALSE;
					goto Exit0;
				}
			}
		}
		else if (nFile != enumFindDir)
		{
			bCallBackReturn = m_callback(strPath + find_data.cFileName, enumFindFile, m_param);
			if (bCallBackReturn == FALSE)
			{
				bReturn = FALSE;
				goto Exit0;
			}
		}

		bFindNext = ::FindNextFile(hFind, &find_data);

	} while(bFindNext && !m_bStop);

	FindClose(hFind);

Next0:
	strFindFileName = strPath + _T("*");
	hFind = ::FindFirstFile(strFindFileName, &find_data);
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
				if (nDepth)
				{
					nTempDepth = nDepth == -1 ? nDepth : nDepth - 1;
					bCallBackReturn = this->__Find(strPath + find_data.cFileName + _T("\\"), strExpression, nFile, nTempDepth);
					if (bCallBackReturn == FALSE)
					{
						bReturn = FALSE;
						goto Exit0;
					}
				}
			}
		}

		bFindNext = ::FindNextFile(hFind, &find_data);

	} while(bFindNext && !m_bStop);

Exit0:

	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
	}
	return bReturn;
}

void KFindFileCore::MakePath(CString& strPath)
{
	if (!strPath.IsEmpty())
	{
		TCHAR ch = strPath.GetAt(strPath.GetLength() - 1);
		if (ch != _T('\\') &&
			ch != _T('/'))
		{
			strPath += "\\";
		}	
	}
}