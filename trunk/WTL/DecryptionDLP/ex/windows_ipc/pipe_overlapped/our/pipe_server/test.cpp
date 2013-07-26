// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096

/* ȫ�ֱ��� */
HANDLE hPipe; 
HANDLE g_hCompletePort = NULL;
//I/O����
typedef struct IO_DATA_TAG			
{
	OVERLAPPED ol;			// �ص��ṹ
	char buf[BUFSIZE];	    // ���ݻ�����
	int  iOperation;	    // ��������
#define OP_READ   1
#define OP_WRITE  2
#define OP_ACCEPT 3
} IODATA, *PIODATA;


void HandleIo()
{

}

DWORD WINAPI ThreadProc(LPVOID lpvPara)
{
	PIODATA pstuIOData = NULL;
	OVERLAPPED* pOverlapped = NULL;
	DWORD dwBytes = 0, dwIOKey = 0;
	DWORD dwErrorCode = NO_ERROR;
	while (TRUE)
	{
		//========> ��ѯ��ɶ˿�״̬
		bool bRet = ::GetQueuedCompletionStatus(
			g_hCompletePort,
			&dwBytes,                                           //һ����ɺ��I/O�������������ݵ��ֽ���
			&dwIOKey,                                           //���ļ�I/O������ɺ����ڴ����֮�������ļ����
			&pOverlapped,                                       //Ϊ����IOCP���������õ�OVERLAPPED�ṹ
			INFINITE
			);

			//ֻ�е� ���ճ��� ����socket ���ջ�������Ϊ��ʱ���˳�
			if((0 == dwBytes) && (NULL == dwIOKey) && (NULL == pOverlapped))
				return 0;

		//����ȡ���ڴ����κνṹ����׵�ַ��
		//Ҫ�ṩ�Ĳ����ǣ��ṹ����ĳ����Ա��field���ĵ�ַaddress���ṹ�������type���ṩ��ַ�Ǹ���Ա������field�� 
		pOverlapped = CONTAINING_RECORD(pOverlapped, IODATA, ol);

		//GetQueuedCompletionStatusʧ���򷵻���ֵ
		if (!bRet)
		{
			DWORD dwFlag	= 0;
			dwErrorCode = ::GetLastError();
			//�ж�socket״̬  ������������������Ͷ��һ������

			bRet = ::GetOverlappedResult(hPipe, &pstuIOData->ol, &dwBytes, FALSE, &dwFlag);  //����ָ���׽ӿ���һ���ص������Ľ��

			if (!bRet)
			{
				dwErrorCode = ::GetLastError();
			}
		}
		else   
			dwErrorCode	= NO_ERROR;   //������ʧ��
		HandleIo();
	}
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
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


	//============> �����µ���ɶ˿�
	g_hCompletePort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	//============> ������ɶ˿ڣ����õ��߳�
	::CreateIoCompletionPort(hPipe,	g_hCompletePort, 0, 2);

	CreateThread(NULL,	NULL, ThreadProc, NULL, 0, NULL);




	return 0;
}

