/* ͷ�ļ� */
#include "stdafx.h"

#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
/* ���� */
#define BUFSIZE 512
/* ************************************
* int main(VOID) 
* ����	pipe ͨ�ŷ����������
**************************************/
int main(int argc, TCHAR *argv[]) 
{ 
	HANDLE hPipe; 
	LPTSTR lpvMessage=TEXT("********�ͻ���Ĭ�Ϸ�����Ϣ����������"); 
	TCHAR chBuf[BUFSIZE]; 
	BOOL fSuccess; 
	DWORD cbRead, cbWritten, dwMode; 
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\samplenamedpipe"); 

	if( argc > 1 )	// ��������˲�������ʹ������Ĳ���
		lpvMessage = argv[1];
	while (1) 
	{ 
		// ��һ������pipe
		hPipe = CreateFile( 
			lpszPipename,   // pipe �� 
			GENERIC_READ |   GENERIC_WRITE,		//  �ɶ���д
			0,              // ������
			NULL,           // Ĭ�ϰ�ȫ����
			OPEN_EXISTING,  // �Ѿ����ڣ��ɷ���˴�����
			0,              // Ĭ������
			NULL);    
		if (hPipe != INVALID_HANDLE_VALUE) 
			break; 

		// ������� ERROR_PIPE_BUSY ����ֱ���˳�  
		if (GetLastError() != ERROR_PIPE_BUSY) 
		{
			printf("Could not open pipe"); 
			return 0;
		}

		// �������pipeʵ�������ڷ�æ״̬���ȴ�2�롣
		if (!WaitNamedPipe(lpszPipename, 2000)) 
		{ 
			printf("Could not open pipe"); 
			return 0;
		} 
	} 

	// pipe�Ѿ����ӣ�����Ϊ��Ϣ��״̬ 
	dwMode = PIPE_READMODE_MESSAGE; 
	fSuccess = SetNamedPipeHandleState( 
		hPipe,    // ���
		&dwMode,  // ��״̬
		NULL,     // ��������󻺴�
		NULL);    // �������ʱ��
	if (!fSuccess) 
	{
		printf("SetNamedPipeHandleState failed"); 
		return 0;
	}

	// д��pipe
	fSuccess = WriteFile( 
		hPipe,                  // ���
		lpvMessage,             // д�������
		(lstrlen(lpvMessage)+1)*sizeof(TCHAR), // д�����ݵĳ���
		&cbWritten,             // ʵ��д������
		NULL);                  // �� overlapped 
	if (!fSuccess) 
	{
		printf("WriteFile failed"); 
		return 0;
	}

	do 
	{ 
		// ���ظ� 
		fSuccess = ReadFile( 
			hPipe,    // ���
			chBuf,    // ��ȡ���ݵĻ���
			BUFSIZE*sizeof(TCHAR),  // �����С
			&cbRead,  // ʵ�ʶ����ֽ�
			NULL);    // �� overlapped 

		if (! fSuccess && GetLastError() != ERROR_MORE_DATA) 
			break; //ʧ�ܣ��˳�

		_tprintf( TEXT("%s\n"), chBuf ); // ��ӡ���Ľ��
	} while (!fSuccess);  //  ERROR_MORE_DATA ���߳ɹ���ѭ��

	getch();//������˳�
	// �رվ��
	CloseHandle(hPipe);  
	return 0; 
}