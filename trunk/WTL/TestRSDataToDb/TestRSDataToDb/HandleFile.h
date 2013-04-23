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
	static UINT WINAPI WorkerThreadProc( LPVOID lpThreadParameter);   //IOCP�����߳�


private:
	bool m_bExit;              //�˳���־  trueΪ��ʼ  false ֹͣ
	CDirectoryTraversalFile m_clsDirectoryTraversalFile;
	
};
