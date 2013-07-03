#pragma once

/**********************************
����: ����Windows��Ϣ
����: 
			bDispatchQuitMsg	: �Ƿ�ת�� WM_QUIT ��Ϣ
									TRUE : ת����Ĭ�ϣ�
									FALSE: ��ת���������� FALSE

����ֵ:		TRUE  : ������Ϣ
			FALSE : bDispatchQuitMsg ����Ϊ FALSE ���յ� WM_QUIT ��Ϣ		
**********************************/
BOOL	PeekMessageLoop(BOOL bDispatchQuitMsg = TRUE);

/**********************************
����: �ȴ�ָ��ʱ��, ͬʱ����Windows��Ϣ
����: (�ο�: MsgWaitForMultipleObjectsEx() )
		dwHandles		: ����Ԫ�ظ���
		szHandles		: ����������
		dwMilliseconds	: �ȴ�ʱ�� (����)
		dwWakeMask		: ��Ϣ���˱�ʶ
		dwFlags			: �ȴ�����

����ֵ: (0 ~ dwHandles - 1): �ȴ��ɹ�
		WAIT_TIMEOUT		: ��ʱ
		WAIT_FAILED			: ִ��ʧ��
**********************************/
DWORD WaitForMultipleObjectsWithMessageLoop(DWORD dwHandles, HANDLE szHandles[], DWORD dwMilliseconds = INFINITE, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);

/**********************************
����: �ȴ�ָ��ʱ��, ͬʱ����Windows��Ϣ
����: (�ο�: MsgWaitForMultipleObjectsEx() )
		hHandle			: ������
		dwMilliseconds	: �ȴ�ʱ�� (����)
		dwWakeMask		: ��Ϣ���˱�ʶ
		dwFlags			: �ȴ�����

����ֵ: TRUE: �ȴ��ɹ���FALSE: ��ʱ		
**********************************/
BOOL MsgWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds = INFINITE, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);

/**********************************
����: �ȴ�ָ��ʱ��, ͬʱ����Windows��Ϣ
����: (�ο�: MsgWaitForMultipleObjectsEx() )
		dwMilliseconds	: �ȴ�ʱ�� (����)
		dwWakeMask		: ��Ϣ���˱�ʶ
		dwFlags			: �ȴ�����

����ֵ: MsgWaitForMultipleObjectsEx() �����Ĳ������		
**********************************/
void WaitWithMessageLoop(DWORD dwMilliseconds, DWORD dwWakeMask = QS_ALLINPUT, DWORD dwFlags = MWMO_INPUTAVAILABLE);

/**********************************
����: �ȴ���Sleep()�����ȴ�ĳ������С��ָ��ֵ
����: 
		plWorkingItemCount		: ���ӱ���
		lMaxWorkingItemCount	: ָ��ֵ
		dwCheckInterval			: ����� (����)

����ֵ: 		
**********************************/
void WaitForWorkingQueue(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval);
/**********************************
����: �ȴ���Sleep()�����ȴ�ĳ��������С�� 0
����: 
		plWorkingItemCount		: ���ӱ���
		dwCheckInterval			: ����� (����)

����ֵ: 		
**********************************/
void WaitForComplete	(long* plWorkingItemCount, DWORD dwCheckInterval);

/**********************************
����: �ȴ���WaitWithMessageLoop()�����ȴ�ĳ������С��ָ��ֵ
����: 
		plWorkingItemCount		: ���ӱ���
		lMaxWorkingItemCount	: ָ��ֵ
		dwCheckInterval			: ����� (����)

����ֵ: 		
**********************************/
void MsgWaitForWorkingQueue	(long* plWorkingItemCount, long lMaxWorkingItemCount, DWORD dwCheckInterval = 10);
/**********************************
����: �ȴ���WaitWithMessageLoop()�����ȴ�ĳ��������С�� 0
����: 
		plWorkingItemCount		: ���ӱ���
		dwCheckInterval			: ����� (����)

����ֵ: 		
**********************************/
void MsgWaitForComplete		(long* plWorkingItemCount, DWORD dwCheckInterval = 10);
