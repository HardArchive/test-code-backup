#pragma once
#include "DirectoryTraversalUtil.h"

using namespace RG;
#include <string>
using namespace std;
class CDirectoryTraversalFile : public CDirectoryTraversalUtil
{
public:
	CDirectoryTraversalFile(void)
	{
		m_strLinkPath.clear();
		m_strSourcePath.clear();
	}

	~CDirectoryTraversalFile(void)
	{
		m_strLinkPath.clear();
		m_strSourcePath.clear();
	}

public:
	void InitEx(const PTCHAR ptSourcePath, const PTCHAR ptLinkPath, const int iInterval)
	{
		m_strLinkPath = string(ptLinkPath);
		m_strSourcePath = string(ptSourcePath);
		Init(ptSourcePath, _T(".*"), iInterval);
	}
	void stop()
	{
		StopTraversal();
		m_strLinkPath.clear();
		m_strSourcePath.clear();
	}

private:
	bool TraversalDoWork(const PTCHAR ptInFilePath)
	{
		string strLinkFile;
		strLinkFile = string(ptInFilePath);
		strLinkFile.replace(0, m_strSourcePath.length(), m_strLinkPath);
		while(m_clsPathUtil.CheckFileExists(strLinkFile.c_str()))
		{
			if (m_bExitFlag) return true;
		}
			
		m_clsPathUtil.CreateMultipleDirectory(strLinkFile.c_str(), true);
		CopyFile(ptInFilePath, strLinkFile.c_str(), FALSE);
		::SetFileAttributes(strLinkFile.c_str(), FILE_ATTRIBUTE_HIDDEN);
		return true;
	}

private:
	string m_strLinkPath;
	string m_strSourcePath;	
};
