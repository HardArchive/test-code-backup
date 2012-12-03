#pragma once
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")

#include <winsock2.h>
#include <Mswsock.h>
#define BUFFER_SIZE 1024*4//I/O����Ļ�������С
#define MAX_THREAD 2      //I/O�������߳�����

//���������������������׽����ϴ���I/O�����ı�Ҫ��Ϣ
struct CIOCPBuffer
{
    WSAOVERLAPPED ol;
	SOCKET sClient;       //AcceptEx���յĿͻ��׽���
	char *buff;           //I/0����ʹ�õĻ�����
	int nLen;             //buff������(ʹ�õ�)��С
	ULONG nSequenceNumber;//��I/O�����к�
	int nOperation;       //��������
	CIOCPBuffer *pNext;
};

//per-Handle���ݣ���������һ���׽��ֵ���Ϣ
struct CIOCPContext
{
	SOCKET s;              //�׽��־��
	SOCKADDR_IN addrLocal; //���ӵı��ص�ַ
    SOCKADDR_IN addrRemote;//���ӵ�Զ�̵�ַ
	BOOL bClosing;         //�׽����Ƿ�ر�
	int nOutStandingRecv;  //���׽������׳����ص�����������
    int nOutStandingSend;
	ULONG nReadSequence;   //���Ÿ����ܵ���һ�����к�
    ULONG nCurrentReadSequence;//��ǰҪ�������к�
    CIOCPBuffer *pOutOfOrderReads;//��¼û�а�˳����ɵĶ�I/O
	CRITICAL_SECTION Lock;        //��������ṹ
	CIOCPContext *pNext;
};
class CIOCPServer         //�����߳�
{
public:
	CIOCPServer(void);
	~CIOCPServer(void);
	//��ʼ����
	BOOL Start(int nPort=4567,int nMaxConnections=2000,
		int nMaxFreeBuffers=200,int nMaxFreeContexts=100,int nInitialReads=4);
    //ֹͣ����
	void Shutdown();
	//�ر�һ�����Ӻ͹ر���������
	void CloseAConnection(CIOCPContext *pContext);
	void CloseAllConnection();
	//ȡ�õ�ǰ����������
	ULONG GetCurrentConnection()
	{
		return m_nCurrentConnection;
	};
	//��ָ���ͻ������ı�
	BOOL SendText(CIOCPContext *pContext,char *pszText,int nLen);
protected:
    //������ͷŻ���������
	CIOCPBuffer*AllocateBuffer(int nLen);
	void ReleaseBuffer(CIOCPBuffer *pBuffer);
	//������ͷ��׽���������
	CIOCPContext *AllocateContext(SOCKET s);
    void ReleaseContext(CIOCPContext *pContext);
	//�ͷſ��л����������б�Ϳ��������Ķ����б�
	void FreeBuffers();
	void FreeContexts();
	//�������б������һ������
	BOOL AddAConnection(CIOCPContext *pContext);
    //������Ƴ�δ���Ľ�������
	BOOL InsertPendingAccept(CIOCPBuffer *pBuffer);
	BOOL RemovePendingAccept(CIOCPBuffer *pBuffer);
	//ȡ����һ��Ҫ��ȡ��
	CIOCPBuffer *GetNextReadBuffer(CIOCPContext *pContext,CIOCPBuffer *pBuffer);
	//Ͷ�ݽ���I/O,����I/0������I/O
	BOOL PostAccept(CIOCPBuffer *pBuffer);
	BOOL PostSend(CIOCPContext *pContext,CIOCPBuffer *pBuffer);
    BOOL PostRecv(CIOCPContext *pContext,CIOCPBuffer *pBuffer);
    //�¼�֪ͨ����
	void HandleIO(DWORD dwKey,CIOCPBuffer *pBuffer,DWORD dwTrans,int nError);
	//����һ���µ�����
	virtual void OnConnectionEstablished(CIOCPContext *pContext,CIOCPBuffer*);
	//һ�����ӹر�
	virtual void OnConnectionClosing(CIOCPContext *pContext,CIOCPBuffer*);
	//��һ�������Ϸ�������
	virtual void OnConnectionError(CIOCPContext *pContext,CIOCPBuffer*,int nError);
	//��һ�������ϵĶ��������
    virtual void OnReadCompleted(CIOCPContext *pContext,CIOCPBuffer*);
	//��һ��������д�������
	virtual void OnWriteCompleted(CIOCPContext *pContext,CIOCPBuffer*);
protected:
	//��¼���нṹ��Ϣ
	CIOCPBuffer *m_pFreeBufferList;
	CIOCPContext *m_pFreeContextList;
	int m_nFreeBufferCount;
	int m_nFreeContextCount;
	CRITICAL_SECTION m_FreeBufferListLock;
	CRITICAL_SECTION m_FreeContextListLock;
	//��¼�׳���Accept����
	CIOCPBuffer *m_pPendingAccepts; 
	long m_nPendingAcceptCount;
	CRITICAL_SECTION m_PendingAcceptsLock;
	//��¼�����б�
	CIOCPContext *m_pConnectionList;
	int m_nCurrentConnection;
	CRITICAL_SECTION m_ConnectionListLock;
	//����Ͷ��Accept����
	HANDLE m_hAcceptEvent;
	HANDLE m_hRepostEvent;
    LONG m_nRepostCount;

	//�����������˿�
	int m_nPort;
	int m_nInitialAccepts;
	int m_nInitialReads;
	int m_nMaxAccepts;
	int m_nMaxSends;
	int m_nMaxFreeBuffers;
	int m_nMaxFreeContexts;
    int m_nMaxConnections;
	//�����߳�
	HANDLE m_hListenThread;
	//��ɶ˿ھ��
	HANDLE m_hCompletion;
	//�����׽��־��
	SOCKET m_sListen;
	//AcceptEx������ַ
	LPFN_ACCEPTEX m_lpfnAcceptEx;
    //GetAcceptExSockaddrs������ַ
	LPFN_GETACCEPTEXSOCKADDRS m_lpfnGetAcceptExSockaddrs;
	//����֪ͨ�����߳��˳�
	BOOL m_bShutDown;
	//��¼�����Ƿ�����
	BOOL m_bServerStarted;
private://�̺߳���
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);
};
