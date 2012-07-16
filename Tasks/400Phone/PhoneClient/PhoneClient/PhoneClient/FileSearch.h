#ifndef _RAINSOFT_FILESEARCH_H_
#define _RAINSOFT_FILESEARCH_H_

#include "Tool.h"
#include "SendQueue.h"

class CFileSearch
{
private:
	BOOL m_bIsContinue;
	//char *m_pszFolderPath;
	static CSendQueue m_Queue;

public:
	CFileSearch()
	{
		this->m_bIsContinue=TRUE;
		//this->m_pszFolderPath=NULL;
		m_Queue.Start();
	}
	//void SetPath(char *pFolderPath)
	//{
	//	this->m_pszFolderPath=pFolderPath;
	//}
	static void DoFoundFile(const char *szFolderPath);
	static BOOL ParseFileInfo(const char *pszFilePath,FileInfo &fiRecord);
	//static BOOL CreateMyFile(const char *pszFolderPath,const char *pszFileName,int iFileNum,const char *pFileContent);

	

	void Start();
	void Stop();
};
#endif