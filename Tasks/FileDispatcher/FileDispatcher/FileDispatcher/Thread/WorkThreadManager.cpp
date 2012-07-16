/*******************************************************************************
 * Copyright (C), 2011-2020, ��  ��
 * FileName: WorkThread.cpp
 * Author: ��  ��    Version: 1.0    Date: 2011-02-22
 * Description: �����̹߳�����
 * Version: 1.0     // �汾��Ϣ
 * Function List:   // ��Ҫ�������书��
 * 1. 
 * History:         // ��ʷ�޸ļ�¼
 * <author>   <time>   <version>   <desc>
 *  ��  ��  2011-02-22    1.0    build this moudle  
 ******************************************************************************/
#include "stdafx.h"
#include ".\WorkThreadManager.h"
#include "AppConfig.h"
#include "ChildProcess.h"
#include "DirWatch.h"
extern bool g_bRunInDebugMode;


const char MEM_SHARE_NAME[] = "LZWinService_MMFSharedData";
const DWORD MEM_SHARE_SIZE = 4 * 1024;
// ����߳���
static const u_int MAX_THREAD_COUNT = 100;

// ���캯��
CWorkThreadManager::CWorkThreadManager(): CNTService(ServiceName, ServiceName)
, m_hFileMap(NULL)
, m_RunMode(RUN_MODE_ERROR)
, m_nChildProcesses(0)
{
	m_dwServiceType = SERVICE_WIN32_OWN_PROCESS|SERVICE_INTERACTIVE_PROCESS;
	m_pszDependencies = NULL; // _T("MSSQLSERVER\0");
	m_dwControlsAccepted = SERVICE_ACCEPT_STOP;
	m_dwControlsAccepted |= SERVICE_ACCEPT_SHUTDOWN;
//	m_hServiceMutex = NULL;

	CWorkThread::CWorkThread();

}

// ���캯��
CWorkThreadManager::~CWorkThreadManager()
{

}

// �̹߳�������
int CWorkThreadManager::DoWork()
{
	TracePrint(LOG_INFO, "LZWinService CManagerThread::StartRun()\r\n");

	//CoInitialize(NULL);

	// ��ʼ��������Դ
	CAppConfig::Init();

	// ��������
	RegisterService(__argc, __argv);

	// �ͷ�������Դ
	CAppConfig::Free();

	//CoUninitialize();
	return 0;
}

void CWorkThreadManager::DoStop()
{
	ReportStatus(SERVICE_STOP_PENDING);
	TracePrint(LOG_INFO, _T("LZWinService Stop Command,so exit!\r\n"));

	if (!m_bQuit) m_bQuit = true;
}

void CWorkThreadManager::Run( DWORD argc, LPTSTR * argv )
{
	// �����޸� 2010-08-05 �������ģʽ״̬
	g_bRunInDebugMode = this->m_bDebug;

	//TracePrint(LOG_INFO, _T("LZWinService Begin Run !\r\n"));
	ReportStatus(SERVICE_START_PENDING);

	TracePrint(LOG_INFO, "LZWinService Begin Run ! %s\r\n", COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S"));
	ReportStatus(SERVICE_RUNNING);


	CAppConfig *app = *CAppConfig::GetInstance();
	if (app->m_RunMode < 2)
	{
		// ������ģʽ
		m_RunMode = RUN_MODE_DEFAULT;
		DoSingle();
	}
	else
	{
		if (m_cChildSeqNo == 0)
		{
			// ������ģʽ
			m_RunMode = RUN_MODE_PARENT;
			m_nChildProcesses = app->m_RunMode < 11 ? app->m_RunMode : 10;
			DoParent();
		}
		else if(m_cChildSeqNo >= '0' && m_cChildSeqNo <= '9')
		{
			// �ӽ���ģʽ
			m_RunMode = RUN_MODE_CHILD;
			DoChild();
		}
		else
		{
			// ��������
			m_RunMode = RUN_MODE_ERROR;
			TracePrint(LOG_INFO, "LZWinService run parameter [ %c ] error!", m_cChildSeqNo);
			this->Stop();
		}
	}
}

void CWorkThreadManager::Stop()
{
	CWorkThread::Stop();
}

int CWorkThreadManager::CreateMemShare()
{
	int nRet = 0;

	// Create a paging file-backed MMF to contain the edit control text.
	// The MMF is 4 KB at most and is named MMFSharedData.
	m_hFileMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, 
		PAGE_READWRITE, 0, MEM_SHARE_SIZE, MEM_SHARE_NAME);

	if (m_hFileMap != NULL) 
	{
		if (GetLastError() == ERROR_ALREADY_EXISTS) 
		{
			TracePrint(LOG_ERROR, "LZWinService Mapping already exists - not created.");
			CloseHandle(m_hFileMap);
			nRet = -3;
		} 
		else 
		{
			// File mapping created successfully.
			// Map a view of the file into the address space.
			m_MemStart = static_cast<char*>(MapViewOfFile(m_hFileMap,
				FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0));
			if (m_MemStart == NULL)
			{
				TracePrint(LOG_ERROR, "LZWinService Can't map view of file.\r\n");
				nRet = -2;
			}
			else
			{
				memset(m_MemStart, 0, MEM_SHARE_SIZE);
			}
		}

	}
	else 
	{
		TracePrint(LOG_ERROR, "LZWinService Can't create file mapping.\r\n");
		nRet = -1;
	}

	return nRet;
}

int CWorkThreadManager::OpenMemShare()
{
	int nRet = 0;
	// See if a memory-mapped file named MMFSharedData already exists.
	m_hFileMap = OpenFileMapping(FILE_MAP_READ,	FALSE, MEM_SHARE_NAME);

	if (m_hFileMap != NULL)
	{
		// The MMF does exist, map it into the process's address space.
		m_MemStart = static_cast<char*>(MapViewOfFile(m_hFileMap, 
			FILE_MAP_READ, 0, 0, 0));

		if (m_MemStart != NULL) 
		{
			TracePrint(LOG_DEBUG, "LZWinService map view ok.\r\n");
		} 
		else 
		{
			TracePrint(LOG_ERROR, "LZWinService Can't map view.\r\n");
			nRet = -2;
		}
	} 
	else 
	{
		TracePrint(LOG_ERROR, "LZWinService Can't open mapping.\r\n");
		nRet = -1;
	}

	return nRet;
}

int CWorkThreadManager::DoParent()
{
	// ���������ڴ�
	CreateMemShare();

	CAppConfig *app = *CAppConfig::GetInstance();
	CChildProcess childList[10];
	char szCmdText[MAX_PATH] = {0};
	szCmdText[0] = '\"';
	DWORD dwCmdTextLen = GetModuleFileName(NULL, &szCmdText[1], MAX_PATH);
	strcat_s(szCmdText, MAX_PATH, "\" -d");
	dwCmdTextLen += 5;
	szCmdText[dwCmdTextLen+1] = 0;
	for (int i=0; i<m_nChildProcesses; i++)
	{
		szCmdText[dwCmdTextLen] = i+48;
		childList[i].Run(szCmdText, app->AppDir);
	}

	// �߳̿��ܣ���ֹ�߳���ֹ
	while(!m_bQuit)
	{

		//����
		Sleep(100);
	}

	// ���˳���־
	if (m_MemStart != NULL)
	{
		memset(m_MemStart, 1, MEM_SHARE_SIZE);
		TracePrint(LOG_ERROR, "LZWinService write exit flags.\r\n");
	}
	// �ر��ӽ���
	for (int i=0; i<m_nChildProcesses; i++)
	{
		childList[i].Stop();
	}

	// �ͷ���ؾ��
	if (m_MemStart != NULL)
	{
		UnmapViewOfFile(m_MemStart);
		m_MemStart = NULL;
	}
	if (m_hFileMap != NULL)
	{
		CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
	}

	return 0;
}

int CWorkThreadManager::DoChild()
{
	// �򿪹����ڴ�
	int nIndex = m_cChildSeqNo - '0';
	int nRet = OpenMemShare();

	// �߳̿��ܣ���ֹ�߳���ֹ
	while(!m_bQuit)
	{
		if (m_MemStart[nIndex] != 0)
		{
			this->Stop();
		}

		//����
		Sleep(100);
	}

	if (m_MemStart != NULL)
	{
		UnmapViewOfFile(m_MemStart);
		m_MemStart = NULL;
	}
	if (m_hFileMap != NULL)
	{
		CloseHandle(m_hFileMap);
		m_hFileMap = NULL;
	}

	return 0;
}

int CWorkThreadManager::DoSingle()
{
	// �߳̿��ܣ���ֹ�߳���ֹ
	int iCount = 0;
	CAppConfig *app = *CAppConfig::GetInstance();
	TracePrint(LOG_INFO, "WatchDirPath��%s��DestDirPath�� %s��DestFilePath��%s;����\r\n", app->m_szWatchDirPath, app->m_szDestDirPath, app->m_szDestFilePath);

	CDirWatch objDirWatch(app->m_szWatchDirPath, app->m_szDestDirPath, app->m_szDestFilePath);
	while(!m_bQuit)
	{

		objDirWatch.TraverseDirectory(app->m_szWatchDirPath, true);
		//����
		Sleep(100);
		iCount++;
		//������150��֮��ɾ�������е��ļ�ɾ��
		if (!(iCount%150))
		{   
			iCount = 0;
			objDirWatch.DeleteAll(m_bQuit);
		}
	}

	return 0;
}