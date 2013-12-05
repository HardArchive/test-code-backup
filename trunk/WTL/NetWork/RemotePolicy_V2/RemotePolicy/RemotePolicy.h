// RemotePolicy.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//
#include "NTService/NTService.h"
#include "WorkThread.h"

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CRemotePolicyApp:
// �йش����ʵ�֣������ RemotePolicy.cpp
//
class CWorkThread;
class CRemotePolicyApp : public CWinApp, public CNTService
{
public:
	CRemotePolicyApp();

public:
	HANDLE m_hServerMutex;			// ȷ��ֻ��һ������ʵ������
	HANDLE m_hStop;					// ����ֹͣ���¼��ں˶�����
	CWorkThread* m_pWorkThread;	// CWorkThreadʵ��ָ��
	REG_PARAM m_RegParam;	// ���ݿ����ò���

	// �麯��
public:
	// ��дCWinApp�麯��
	virtual BOOL InitInstance();
				// ���̳�ʼ��
	virtual int ExitInstance();
				// ��������

	// ��дCNTService�麯��
	virtual void Run(DWORD dwArgc, LPTSTR * ppszArgv);
				// ��ӦSCM������Ϣ
	virtual void Stop();
				// ��ӦSCMֹͣ��Ϣ
	virtual void UserControlFunc();
				// ��ӦSCM�û��Զ�����Ϣ����Ϣ��129��
	BOOL LoadRegistry();

	DECLARE_MESSAGE_MAP()

};

extern CRemotePolicyApp theApp;
