// Watcher.cpp: implementation of the CWatcher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FileWatcher.h"
#include "Watcher.h"
#include "FileWatcherDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWatcher::CWatcher()
{

}

CWatcher::~CWatcher()
{
	this->Destroy();
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ�����Ŀ¼��ء�
//��������csStrPath ���Ŀ¼��·��
//����������Ŀ¼������������������ɶ˿ڣ���������
//	��Ŀ¼�������ɶ˿ڹ������������������̣߳������𣬱�������
//	Ͷ�ݵ�һ��IO���󣬵���Watch��
BOOL CWatcher::Create(CString csStrPath)
{
	//===========> ��Ŀ¼��������
	if ((this->m_hPath = CreateFile(
		csStrPath,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL
        )) == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	//============> �����µ���ɶ˿�
	if ((this->m_hIOCPApp = ::CreateIoCompletionPort(
		INVALID_HANDLE_VALUE,
		NULL,
		0,
		0
		)) == NULL)
	{
		this->CloseAllHandle();

		return FALSE;
	}
	//============> ������ɶ˿ڣ����õ��߳�
	if ((this->m_hIOCPNew = ::CreateIoCompletionPort(
		this->m_hPath,
		this->m_hIOCPApp,
		0,
		2
		)) == NULL)
	{
		this->CloseAllHandle();
		return FALSE;
	}
	//=========> ��������̣߳����𣬱�����
	if ((this->m_hThread = ::CreateThread(
		NULL,
		0,
		CWatcher::WatchThreadProc,
		this,
		CREATE_SUSPENDED,
		NULL
		)) == NULL)
	{
		this->CloseAllHandle();
		return FALSE;
	}
	//=============> Ͷ��IO����
	if (this->Watch() == FALSE)
	{
		//=======> ǿ���˳��̣߳���Ϊ�߳�û�й����������ǰ�ȫ����
		::TerminateThread(this->m_hThread, 0);

		this->CloseAllHandle();

		return FALSE;
	}
	//============> �ָ��̣߳����̹߳���
	::ResumeThread(this->m_hThread);
	return TRUE;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ��ر����д򿪵ľ��
//����������
//���������ر�����ɶ˿ڡ�������ɶ˿ڡ�Ŀ¼���߳̾��
VOID CWatcher::CloseAllHandle()
{
	::CloseHandle(this->m_hIOCPApp);
	::CloseHandle(this->m_hIOCPNew);
	::CloseHandle(this->m_hPath);
	::CloseHandle(this->m_hThread);
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ����
//����������
//������������ReadDirectoryChangesW���൱��Ͷ�ݵ�һ��IO����
BOOL CWatcher::Watch()
{
	DWORD dwByteRet = 0;

	//==========> ���OVERLAPPED�ṹ
	::memset(&this->m_stOverlapped, 0, sizeof(OVERLAPPED));

	//=========> ���Ŀ¼
	if (::ReadDirectoryChangesW(
		this->m_hPath,
		this->m_szBuffer,
		WTH_BUF_LEN,
		TRUE,
		FILE_NOTIFY_CHANGE_FILE_NAME |
		FILE_NOTIFY_CHANGE_DIR_NAME |
		FILE_NOTIFY_CHANGE_ATTRIBUTES |
		FILE_NOTIFY_CHANGE_SIZE |
		FILE_NOTIFY_CHANGE_LAST_WRITE |
		FILE_NOTIFY_CHANGE_LAST_ACCESS |
		FILE_NOTIFY_CHANGE_CREATION |
		FILE_NOTIFY_CHANGE_SECURITY,
		&dwByteRet,
		&this->m_stOverlapped,
		NULL 
		) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ�����̹߳���
//��������lpPara �����߳�ʱ�򴫵ݵ�ָ��
//��������ѭ������ѯ��ɶ˿�״̬�������ѯʧ�ܡ�����OnError���жϡ�
//	���dwByteRetΪ-1����ô˵���յ����˳�֪ͨ���ж�
//	���û�����⣬���������Ժ󣬴���OnWatch
DWORD WINAPI CWatcher::WatchThreadProc(LPVOID lpvPara)
{
	CWatcher *lpcsWatcher = NULL;
	DWORD dwByteRet = 0, dwIOKey = 0;
	LPOVERLAPPED lpstOverlapped = NULL;
	PFILE_NOTIFY_INFORMATION lpstNotify = NULL;
	char *lpszBuffer = NULL;
	CString csStrName1, csStrName2;

	lpcsWatcher = (CWatcher *)lpvPara;

	while (TRUE)
	{
		//========> ��ѯ��ɶ˿�״̬
		if (::GetQueuedCompletionStatus(
			lpcsWatcher->m_hIOCPApp,
			&dwByteRet,
			&dwIOKey,
			&lpstOverlapped,
			INFINITE
			) == TRUE)
		{
			//=========> �ж��Ƿ�ʧ��
			if (dwByteRet == 0)
			{
				lpcsWatcher->CloseAllHandle();
				break;
			}
			else
			{
				//========> �����ڴ棬�����ڴ�
				lpszBuffer = new char[dwByteRet];
				memcpy(lpszBuffer, lpcsWatcher->m_szBuffer, dwByteRet);

				//======> ����������ת����FILE_NOTIFY_INFORMATION�ṹ
				lpstNotify = (PFILE_NOTIFY_INFORMATION)lpszBuffer;

				//========= ת����CString���õ�����1
				csStrName1 = lpcsWatcher->WideCharToCString(
					lpstNotify->FileName,
					lpstNotify->FileNameLength);
				//========> �ж��Ƿ����ƫ��
				if (lpstNotify->NextEntryOffset != 0)
				{
					//==========> ����ڶ����ַ�ƫ��
					//======> һ��Ҫ�ǵ���
					// (PFILE_NOTIFY_INFORMATION)(lpszBuffer + lpstNotify->NextEntryOffset)
					lpstNotify = 
						(PFILE_NOTIFY_INFORMATION)(lpszBuffer + 
						lpstNotify->NextEntryOffset);
					//========= ת����CString���õ�����2
					csStrName2 = lpcsWatcher->WideCharToCString(
						lpstNotify->FileName,
						lpstNotify->FileNameLength);
				}

				//=======> ����OnWatch��ʣ�µĽ����˹��̴���
				lpcsWatcher->OnWatch(
					lpstNotify->Action, 
					csStrName1, 
					csStrName2);

				// �ͷ��ڴ�
				delete[] lpszBuffer;

				//======> �ٴ�Ͷ��IO����
				if (lpcsWatcher->Watch() == FALSE)
				{
					lpcsWatcher->CloseAllHandle();
				}
			}
		}
		else
		{
				lpcsWatcher->CloseAllHandle();
		}
	}
	return 0;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ����ַ�ת���ɶ��ֽ�CString
//��������lpszWideChar ���ַ���������nLength ����������
//�����������ַ�ת���ɶ��ֽ��Ժ���ת����CString����Ȼ�󷵻�
CString CWatcher::WideCharToCString(LPCWSTR lpszWideChar, UINT nLength)
{
	char szString[MAX_PATH] = {0};

	//======> ת��
	WideCharToMultiByte(
		CP_ACP,
		0,
		lpszWideChar,
		nLength / 2,
		szString,
		MAX_PATH,
		NULL,
		NULL 
		);
	return szString;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ�����Ŀ¼���
//����������
//���������رվ�����������̼߳�⵽��ʧ�ܣ���ȫ�˳�
VOID CWatcher::Destroy()
{
	this->CloseAllHandle();
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//�����ơ���ع���
//��������dwAction �������͡�csStrName �������
//������������������Ҫ����Destroy
VOID CWatcher::OnWatch(DWORD dwAction, CString csStrName1, CString csStrName2)
{
	//========> ���и����ڵĹ��̴���
	((CFileWatcherDlg *)::AfxGetMainWnd())->OnWatch(
		dwAction,
		csStrName1,
		csStrName2);

}

