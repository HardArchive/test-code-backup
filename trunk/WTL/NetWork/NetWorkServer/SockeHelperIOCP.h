#pragma once

/***************************************************************************************************
* 1�� class      �� SockeHelperIOCP
* 2�� Version    �� *.*
* 3�� Description�� ��ɶ˿ڴ�����
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-11-30 17:06:01
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/

#define IO_BUFFER_LEN 512
//ÿ��IO����������
typedef struct PER_IO_OPERATION_DATA
{
	OVERLAPPED lapped;
	WSABUF dataBuf;
	CHAR buffer[IO_BUFFER_LEN];
	DWORD BytesSend;
	DWORD BytesRecv;
	DWORD dwFlags;
	DWORD dwNeed;
	DWORD dwRecvBytes;
	DWORD dwCommand;
	char szClientIP[32];
	PER_IO_OPERATION_DATA *pNext;
	//void InitBuf()
	//{
	//	this->dataBuf.len = sizeof(MsgHead);
	//	this->dataBuf.buf = this->buffer;
	//	::memset(this->dataBuf.buf, 0, BUFFERLEN);
	//	this->dwNeed = sizeof(MsgHead);
	//	this->dwRecvBytes = 0;
	//	this->dwCommand = 0;//����ֻ���հ�ͷ
	//}
	//void SetBuf(ULONG RecvLen, ULONG transLen, DWORD dwCommand)
	//{
	//	this->dataBuf.len = RecvLen;
	//	this->dataBuf.buf += transLen;
	//	::memset(this->dataBuf.buf, 0, BUFFERLEN-(this->dataBuf.buf-this->buffer));
	//	this->dwNeed = RecvLen;
	//	this->dwCommand = dwCommand;
	//}

}PERIODATA, *LPPERIODATA;

//�ⲿ�����߳̽ṹ
typedef struct THREAD_AFFERENT_DATA
{
	HANDLE hIoPort;                   //��ɶ˿�
	SOCKET sockListen;                //�����׽���
	CRITICAL_SECTION  csMem;          //�����ڴ��ٽ�������
	PVOID pAllMemory;                 //ָ������Ĵ���ڴ��׵�ַ
	DWORD dwDataCount;                //�ܵ���ʹ�õ����ݵ�����
	LPPERIODATA pstuIoDataUnUsed;     //�ѱ��δʹ�õ�����	
}THREADAFFERENT;

class CSockeHelperIOCP
{
public:
	CSockeHelperIOCP(void);
	~CSockeHelperIOCP(void);

public:
	

	//�̺߳���
private:
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);  //�����߳� - ���������߳�
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);  //�����߳� - ���մ��������߳�


public:
	bool Init(int nPort = 4567);     //��ʼ������
	void UnInit();   //�˳�������

private:
	bool MemoryApply(int iCount);       //�����ڴ�
	bool InitIocp(int nPort);           //����socket �󶨶˿� ����socket
	bool CreateWorkThread();
	//static void CloseClient(SOCKET clientSocket, LPPERIODATA lpOperationData); //�رտͻ��˵����Ӳ������ڴ�
	
private:
	//SOCKET m_sockListen;   //�����׽���
	THREADAFFERENT m_stuThreadAfferent;  //�����߳̽ṹ

	HANDLE m_hListenThreadHandle;   //���������߳̾��	
	HANDLE* m_pszThreadHandle;      //�����߳̾������	
	int m_iThreadCount;             //�����߳̾������ CPU*2
	
public:

};


