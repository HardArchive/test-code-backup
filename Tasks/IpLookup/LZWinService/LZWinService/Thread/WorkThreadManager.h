/*******************************************************************************
 * Copyright (C), 2011-2020, ��  ��
 * FileName: WorkThreadManager.h
 * Author: ��  ��    Version: 1.0    Date: 2011-02-22
 * Description: �����̹߳�����
 * Version: 1.0     // �汾��Ϣ
 * Function List:   // ��Ҫ�������书��
 * 1. 
 * History:         // ��ʷ�޸ļ�¼
 * <author>   <time>   <version>   <desc>
 *  ��  ��  2011-02-22    1.0    build this moudle  
 ******************************************************************************/
#pragma once
#include "WorkThread.h"
#include "NTService.h"

#ifndef WORK_THREAD_MANAGER_H_
#define WORK_THREAD_MANAGER_H_
const LPCTSTR ServiceName = _T("LZWinService");


// �����̻߳���
class CWorkThreadManager: public CWorkThread, public CNTService
{
public:
	// ���캯��
	CWorkThreadManager();
	// ��������
	~CWorkThreadManager();

	virtual void	Run(DWORD argc, LPTSTR * argv);
	virtual void	Stop();

protected:
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
	virtual int        DoWork();
	// ֹͣ�̣߳�һ�㲻��Ҫ����
	// �������ش��������£�
	// void DoStop()
	// {
	//		m_bQuit = true;
	// }
	virtual void       DoStop();
private:
	// �ļ�ӳ����
	HANDLE m_hFileMap;
	// �ڴ�ӳ����׵�ַ
	char* m_MemStart;
	// ����ģʽ��0=������ģʽ��1=������ģʽ��2=�ӽ���ģʽ��-1=���ò�������
	int m_RunMode;
	// �ӽ�����
	u_int m_nChildProcesses;

	// ���������ڴ�
	inline int CreateMemShare();
	// �򿪹����ڴ�
	inline int OpenMemShare();

	// ����������ģʽ
	int DoSingle();
	// ����������ģʽ
	int DoParent();
	// �ӽ�������ģʽ
	int DoChild();
};

#define RUN_MODE_DEFAULT	0	// Ĭ������ģʽ
#define RUN_MODE_PARENT		1	// ����������ģʽ
#define RUN_MODE_CHILD		2	// �ӽ�������ģʽ
#define RUN_MODE_ERROR		-1	// ��������ģʽ�����в�������

#endif