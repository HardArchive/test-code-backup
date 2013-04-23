#pragma once
#include "DirectoryTraversalFile.h"

class CHandleFile 
{
public:
	CHandleFile(void);
	~CHandleFile(void);

public:
	bool Start(const PTCHAR ptSourcePath, const PTCHAR ptLinkPath, const int iInterval);
	bool Stop();
private:
	bool HandleFile();
	static UINT WINAPI WorkerThreadProc( LPVOID lpThreadParameter);   //IOCP工作线程


private:
	bool m_bExit;              //退出标志  true为开始  false 停止
	CDirectoryTraversalFile m_clsDirectoryTraversalFile;
	
};
