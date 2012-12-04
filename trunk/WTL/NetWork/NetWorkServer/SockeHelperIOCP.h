#pragma once

/***************************************************************************************************
* 1、 class      ： SockeHelperIOCP
* 2、 Version    ： *.*
* 3、 Description： 完成端口处理类
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-11-30 17:06:01
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
extern bool g_bExit;   //全局退出函数

#define IO_BUFFER_LEN 512
//每次IO操作的数据
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
	//	this->dwCommand = 0;//代表只接收包头
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


class CSockeHelperIOCP
{
public:
	CSockeHelperIOCP(void);
	~CSockeHelperIOCP(void);

	//线程函数
private:
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);  //监听线程 - 接受连接线程
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);  //工作线程 - 接收处理数据线程


public:
	bool Init(int nPort = 4567);     //初始化函数
	void UnInit();   //退出清理函数

private:
	bool MemoryApply(int iCount);       //申请内存
	bool InitIocp(int nPort);           //创建socket 绑定端口 监听socket
	bool CreateWorkThread();
	//static void CloseClient(SOCKET clientSocket, LPPERIODATA lpOperationData); //关闭客户端的连接并回收内存
	
private:
	PVOID m_pAllMemory;                 //指向申请的大块内存首地址
	DWORD m_dwDataCount;                //总的已使用的数据的数量
	LPPERIODATA m_pstuIoDataUnUsed;     //已标记未使用的数据

	
	HANDLE m_hIoPort;      //完成端口	
	SOCKET m_sockListen;   //监听套接字

	HANDLE m_hListenThreadHandle;   //接收连接线程句柄	
	HANDLE* m_pszThreadHandle;      //接收线程句柄数组	
	int m_iThreadCount;             //接收线程句柄数量 CPU*2
	
public:

};


