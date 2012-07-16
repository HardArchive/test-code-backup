/*******************************************************************************
 * Copyright (C), 2011-2020, ��  ��
 * FileName: Trace.h
 * Author: ��  ��    Version: 1.0    Date: 2012-01-05
 * Description: �����̹߳�����
 * Version: 1.0     // �汾��Ϣ
 * Function List:   // ��Ҫ�������书��
 * 1. 
 * History:         // ��ʷ�޸ļ�¼
 * <author>   <time>   <version>   <desc>
 *  ��  ��  2012-01-05    1.0    build this moudle  
 ******************************************************************************/
#pragma once

#ifndef CHILD_PROCESS_H_
#define CHILD_PROCESS_H_

class CChildProcess
{
public:
	DWORD Run(const char* szCmdText, const char* szPath);

	// ֹͣ����
	int Stop();

	// ���캯��
	CChildProcess();
	// ��������
	~CChildProcess();
public:
	// ���н���ס��
	PROCESS_INFORMATION pInfo;
private:
	// ������
	char m_CmdText[1024];
	// ����Ŀ¼
	char m_Path[MAX_PATH];
};


#endif