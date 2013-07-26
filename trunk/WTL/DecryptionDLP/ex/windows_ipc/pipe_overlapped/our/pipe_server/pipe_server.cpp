/* ͷ�ļ� */
#include "stdafx.h"

#include <windows.h> 
#include <stdio.h>
#include <tchar.h>
/* ���� */
#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096
/* �ṹ���� */
typedef struct 
{ 
	OVERLAPPED oOverlap; 
	HANDLE hPipeInst; 
	TCHAR chRequest[BUFSIZE]; 
	DWORD cbRead;
	TCHAR chReply[BUFSIZE]; 
	DWORD cbToWrite; 
} PIPEINST, *LPPIPEINST; 
/* �������� */
VOID DisconnectAndClose(LPPIPEINST); 
BOOL CreateAndConnectInstance(LPOVERLAPPED); 
BOOL ConnectToNewClient(HANDLE, LPOVERLAPPED); 
VOID GetAnswerToRequest(LPPIPEINST); 
VOID WINAPI CompletedWriteRoutine(DWORD, DWORD, LPOVERLAPPED); 
VOID WINAPI CompletedReadRoutine(DWORD, DWORD, LPOVERLAPPED); 
/* ȫ�ֱ��� */
HANDLE hPipe; 
/* ************************************
* int main(VOID) 
* ����	pipe ͨ�ŷ����������
**************************************/
int main(VOID) 
{ 
	HANDLE hConnectEvent; 
	OVERLAPPED oConnect; 
	LPPIPEINST lpPipeInst; 
	DWORD dwWait, cbRet; 
	BOOL fSuccess, fPendingIO; 

	// �������Ӳ������¼����� 
	hConnectEvent = CreateEvent( 
		NULL,    // Ĭ������
		TRUE,    // �ֹ�reset
		TRUE,    // ��ʼ״̬ signaled 
		NULL);   // δ����

	if (hConnectEvent == NULL) 
	{
		printf("CreateEvent failed with %d.\n", GetLastError()); 
		return 0;
	}
	// OVERLAPPED �¼�
	oConnect.hEvent = hConnectEvent; 

	printf("��������ʵ�����ȴ�����\r\n");
	// ��������ʵ�����ȴ�����  
	fPendingIO = CreateAndConnectInstance(&oConnect); 

	while (1) 
	{
		printf("�ȴ��ͻ������ӻ��д�������\r\n");
		// �ȴ��ͻ������ӻ��д������� 
		dwWait = WaitForSingleObjectEx( 
			hConnectEvent,  // �ȴ����¼� 
			INFINITE,       // ���޵ȴ�
			TRUE);       

		
		switch (dwWait) 
		{ 
		case 0:		
			// pending 
			if (fPendingIO) 
			{ 
				printf("��ȡ Overlapped I/O �Ľ��\r\n");
				// ��ȡ Overlapped I/O �Ľ��
				fSuccess = GetOverlappedResult( 
					hPipe,     // pipe ��� 
					&oConnect, // OVERLAPPED �ṹ 
					&cbRet,    // �Ѿ����͵�������
					FALSE);    // ���ȴ�
				if (!fSuccess) 
				{
					printf("ConnectNamedPipe (%d)\n", GetLastError()); 
					return 0;
				}
			} 

			// �����ڴ� 
			lpPipeInst = (LPPIPEINST) HeapAlloc(GetProcessHeap(),0,sizeof(PIPEINST)); 
			if (lpPipeInst == NULL) 
			{
				printf("GlobalAlloc failed (%d)\n", GetLastError()); 
				return 0;
			}
			lpPipeInst->hPipeInst = hPipe; 

			printf("����д��ע��CompletedWriteRoutine��CompletedReadRoutine���໥����\r\n");
			// ����д��ע��CompletedWriteRoutine��CompletedReadRoutine���໥����
			lpPipeInst->cbToWrite = 0; 
			printf("main CompletedWriteRoutine д��pipe��������ɺ���\r\n"); 
			CompletedWriteRoutine(0, 0, (LPOVERLAPPED) lpPipeInst); 

			printf("�ٴ���һ������ʵ��������Ӧ��һ���ͻ��˵�����\r\n");
			// �ٴ���һ������ʵ��������Ӧ��һ���ͻ��˵�����
			fPendingIO = CreateAndConnectInstance( 
				&oConnect); 
			break; 

			// ��д��� 
		case WAIT_IO_COMPLETION: 
			break; 

		default: 
			{
				printf("WaitForSingleObjectEx (%d)\n", GetLastError()); 
				return 0;
			}
		} 
	} 
	return 0; 
} 

/* ************************************
* CompletedWriteRoutine 
* 	д��pipe��������ɺ���
*	�ӿڲμ�FileIOCompletionRoutine�ص���������
*
*	��д�������ʱ�����ã���ʼ������һ���ͻ��˵�����
**************************************/
VOID WINAPI CompletedWriteRoutine(
								  DWORD dwErr, 
								  DWORD cbWritten, 
								  LPOVERLAPPED lpOverLap) 
{ 
	LPPIPEINST lpPipeInst; 
	BOOL fRead = FALSE; 
	// ����overlapʵ��
	lpPipeInst = (LPPIPEINST) lpOverLap; 

	// ���û�д���
	if ((dwErr == 0) && (cbWritten == lpPipeInst->cbToWrite)) 
	{		
		printf("CompletedWriteRoutine  ReadFileEx ��ȡ client ��Ϣ\r\n"); 
		fRead = ReadFileEx( 
			lpPipeInst->hPipeInst, 
			lpPipeInst->chRequest, 
			BUFSIZE*sizeof(TCHAR), 
			(LPOVERLAPPED) lpPipeInst, 
			// д��������ɺ󣬵���CompletedReadRoutine
			(LPOVERLAPPED_COMPLETION_ROUTINE) CompletedReadRoutine); 
	}	
	if (! fRead) 
		// �����Ͽ�����
		DisconnectAndClose(lpPipeInst); 
} 

/* ************************************
* CompletedReadRoutine 
* 	��ȡpipe��������ɺ���
*	�ӿڲμ�FileIOCompletionRoutine�ص���������
*
*	�����������ʱ�����ã�д��ظ�
**************************************/
VOID WINAPI CompletedReadRoutine(
								 DWORD dwErr, 
								 DWORD cbBytesRead, 
								 LPOVERLAPPED lpOverLap) 
{ 
	printf("CompletedReadRoutine ��ȡpipe��������ɺ���\r\n"); 
	LPPIPEINST lpPipeInst; 
	BOOL fWrite = FALSE; 

	// ����overlapʵ��
	lpPipeInst = (LPPIPEINST) lpOverLap; 

	// ���û�д���
	if ((dwErr == 0) && (cbBytesRead != 0)) 
	{ 
		// ���ݿͻ��˵��������ɻظ�
		GetAnswerToRequest(lpPipeInst); 
		// ���ظ�д�뵽pipe
		fWrite = WriteFileEx( 
			lpPipeInst->hPipeInst, 
			lpPipeInst->chReply,	//����Ӧд��pipe
			lpPipeInst->cbToWrite, 
			(LPOVERLAPPED) lpPipeInst, 
			// д����ɺ󣬵���CompletedWriteRoutine
			(LPOVERLAPPED_COMPLETION_ROUTINE) CompletedWriteRoutine); 
	} 

	if (! fWrite) 
		// �����Ͽ�����
		DisconnectAndClose(lpPipeInst); 
} 

/* ************************************
* VOID DisconnectAndClose(LPPIPEINST lpPipeInst) 
* ����	�Ͽ�һ�����ӵ�ʵ��
* ����	lpPipeInst���Ͽ����رյ�ʵ�����
**************************************/
VOID DisconnectAndClose(LPPIPEINST lpPipeInst) 
{ 
	// �ر�����ʵ��
	printf("�ر�����ʵ��\r\n"); 
	//DisconnectNamedPipe�Ͽ�һ���ͻ���һ�������ܵ������� 
	if (! DisconnectNamedPipe(lpPipeInst->hPipeInst) ) 
	{
		printf("DisconnectNamedPipe failed with %d.\n", GetLastError());
	}
	// �ر� pipe ʵ���ľ�� 
	CloseHandle(lpPipeInst->hPipeInst); 
	// �ͷ�
	if (lpPipeInst != NULL) 
		HeapFree(GetProcessHeap(),0, lpPipeInst); 
} 

/* ************************************
* BOOL CreateAndConnectInstance(LPOVERLAPPED lpoOverlap)
* ����	��������ʵ��
* ����	lpoOverlap������overlapped IO�Ľṹ
* ����ֵ	�Ƿ�ɹ�
**************************************/
BOOL CreateAndConnectInstance(LPOVERLAPPED lpoOverlap) 
{ 
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\samplenamedpipe"); 
	// ����named pipe	 
	hPipe = CreateNamedPipe( 
		lpszPipename,             // pipe �� 
		PIPE_ACCESS_DUPLEX |      // �ɶ���д
		FILE_FLAG_OVERLAPPED,     // overlapped ģʽ 
		// pipeģʽ
		PIPE_TYPE_MESSAGE |       // ��Ϣ���� pipe 
		PIPE_READMODE_MESSAGE |   // ��Ϣ��ģʽ 
		PIPE_WAIT,                // ����ģʽ
		PIPE_UNLIMITED_INSTANCES, // ������ʵ��
		BUFSIZE*sizeof(TCHAR),    // ��������С
		BUFSIZE*sizeof(TCHAR),    // ���뻺���С
		PIPE_TIMEOUT,             // �ͻ��˳�ʱ
		NULL);                    // Ĭ�ϰ�ȫ����
	if (hPipe == INVALID_HANDLE_VALUE) 
	{
		printf("CreateNamedPipe failed with %d.\n", GetLastError()); 
		return 0;
	}

	// ���ӵ��µĿͻ���
	return ConnectToNewClient(hPipe, lpoOverlap); 
}

/* ************************************
* BOOL ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo)
* ����	��������ʵ��  �����൱��socket�е�Ͷ��һ���ȴ�����
* ����	lpoOverlap������overlapped IO�Ľṹ
* ����ֵ	�Ƿ�ɹ�
**************************************/
BOOL ConnectToNewClient(HANDLE hPipe, LPOVERLAPPED lpo) 
{ 
	BOOL fConnected, fPendingIO = FALSE; 

	// ��ʼһ�� overlapped ���� 
	printf("��ʼһ�� overlapped ���� �൱��socket�е�Ͷ��һ���ȴ�����\r\n"); 
	fConnected = ConnectNamedPipe(hPipe, lpo); 

	if (fConnected) 
	{
		printf("ConnectNamedPipe failed with %d.\n", GetLastError()); 
		return 0;
	}
	switch (GetLastError()) 
	{ 
		// overlapped���ӽ�����. 		
	case ERROR_IO_PENDING: 
		printf("overlapped���ӽ�����\r\n"); 
		fPendingIO = TRUE; 
		break; 
		// �Ѿ����ӣ����Eventδ��λ 
	case ERROR_PIPE_CONNECTED: 
		printf("�Ѿ����ӣ����Eventδ��λ\r\n"); 
		if (SetEvent(lpo->hEvent)) 
			break; 
		// error
	default: 
		{
			printf("ConnectNamedPipe failed with %d.\n", GetLastError());
			return 0;
		}
	} 
	return fPendingIO; 
}

// TODO���ݿͻ��˵����󣬸�����Ӧ
VOID GetAnswerToRequest(LPPIPEINST pipe)
{
	_tprintf( TEXT("[%d] %s\n"), pipe->hPipeInst, pipe->chRequest);
	lstrcpyn( pipe->chReply,  TEXT("******������Ĭ�ϻظ� Default answer from server") ,BUFSIZE);
	pipe->cbToWrite = (lstrlen(pipe->chReply)+1)*sizeof(TCHAR);
}


