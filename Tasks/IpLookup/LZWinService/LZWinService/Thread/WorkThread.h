/*******************************************************************************
 * Copyright (C), 2011-2020, ��  ��
 * FileName: WorkThread.h
 * Author: ��  ��    Version: 1.0    Date: 2011-02-22
 * Description: �����̻߳���
 * Version: 1.0     // �汾��Ϣ
 * Function List:   // ��Ҫ�������书��
 * 1. Start()
 * 2. Stop()
 * 3. SetPriority(int nPriority)
 * 4. Terminate();
 * History:         // ��ʷ�޸ļ�¼
 * <author>   <time>   <version>   <desc>
 *  ��  ��  2011-02-22    1.0    build this moudle  
 ******************************************************************************/
#pragma once
//#include "..\Version.h"

#ifndef WORK_THREAD_H_
#define WORK_THREAD_H_

// �����̻߳���
class CWorkThread
{
public:
	// ���캯��
	CWorkThread();
	// ��������
	virtual ~CWorkThread();

	// Operations
public:
	// ��ʼһ���߳�
	int                Start(void* pParam=NULL);
	// ֹͣ�߳�
	void               Stop(bool bSameWithStartThread=true);
	// �����߳����ȼ�
	void               SetPriority(int nPriority){ SetThreadPriority(m_hThread, nPriority); }
	// ��ֹ�߳�
	void               Terminate();
	// �Ƿ���������
	bool               IsRunning() { return m_bIsRunning; }
	// ��ȡ�߳̾��
	HANDLE             GetHandle(){ return m_hThread; }
	// ��ȡ�߳�ID
	UINT               GetID(){ return m_nThreadID; }
protected:
	// �Ƿ��˳��߳�
	volatile bool m_bQuit;
	// �߳��������ָ��
	void* m_pParam;

	// �̹߳������̣�һ����Ҫ����
	// �������ش��������£�
	// int DoWork()
	// {
	//		while(!m_bQuit)
	//		{
	//			//����
	//			Sleep(10);
	//		}
	//		return 0;
	// }
	virtual int        DoWork(){return 0;}

	// ֹͣ�̣߳�һ�㲻��Ҫ����
	// �������ش��������£�
	// void DoStop()
	// {
	//		m_bQuit = true;
	// }
	virtual void       DoStop(){ if (!m_bQuit) m_bQuit = true; }

	// ��ʼ���߳�
	virtual bool       InitInstance(){return true;}

	// �˳��߳�
	virtual int        ExitInstance(){return 0;}

private:
	// �߳̾��
	HANDLE             m_hThread;
	// �߳�ID
	UINT               m_nThreadID;
	// �Ƿ���������
	bool               m_bIsRunning;

	// ���߳�������
#ifdef C_RUNTIME_THREAD
	static int         MainThreadProc(CWorkThread *pThread);
#else
	static DWORD WINAPI MainThreadProc( LPVOID lpParam );
#endif
};
#endif