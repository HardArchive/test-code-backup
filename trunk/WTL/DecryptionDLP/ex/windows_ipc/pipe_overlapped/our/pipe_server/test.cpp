// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#define PIPE_TIMEOUT 5000
#define BUFSIZE 4096

/* 全局变量 */
HANDLE hPipe; 
HANDLE g_hCompletePort = NULL;
//I/O数据
typedef struct IO_DATA_TAG			
{
	OVERLAPPED ol;			// 重叠结构
	char buf[BUFSIZE];	    // 数据缓冲区
	int  iOperation;	    // 操作类型
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
		//========> 查询完成端口状态
		bool bRet = ::GetQueuedCompletionStatus(
			g_hCompletePort,
			&dwBytes,                                           //一次完成后的I/O操作所传送数据的字节数
			&dwIOKey,                                           //当文件I/O操作完成后，用于存放与之关联的文件句柄
			&pOverlapped,                                       //为调用IOCP机制所引用的OVERLAPPED结构
			INFINITE
			);

			//只有当 接收长度 接收socket 接收缓冲区都为空时才退出
			if((0 == dwBytes) && (NULL == dwIOKey) && (NULL == pOverlapped))
				return 0;

		//用于取得内存中任何结构体的首地址，
		//要提供的参数是：结构体中某个成员（field）的地址address、结构体的类型type、提供地址那个成员的名字field。 
		pOverlapped = CONTAINING_RECORD(pOverlapped, IODATA, ol);

		//GetQueuedCompletionStatus失败则返回零值
		if (!bRet)
		{
			DWORD dwFlag	= 0;
			dwErrorCode = ::GetLastError();
			//判断socket状态  如果还处于启用则继续投递一个接收

			bRet = ::GetOverlappedResult(hPipe, &pstuIOData->ol, &dwBytes, FALSE, &dwFlag);  //返回指定套接口上一个重叠操作的结果

			if (!bRet)
			{
				dwErrorCode = ::GetLastError();
			}
		}
		else   
			dwErrorCode	= NO_ERROR;   //否则标记失败
		HandleIo();
	}
	return 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
	LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\samplenamedpipe"); 
	// 创建named pipe	 
	hPipe = CreateNamedPipe( 
		lpszPipename,             // pipe 名 
		PIPE_ACCESS_DUPLEX |      // 可读可写
		FILE_FLAG_OVERLAPPED,     // overlapped 模式 
		// pipe模式
		PIPE_TYPE_MESSAGE |       // 消息类型 pipe 
		PIPE_READMODE_MESSAGE |   // 消息读模式 
		PIPE_WAIT,                // 阻塞模式
		PIPE_UNLIMITED_INSTANCES, // 无限制实例
		BUFSIZE*sizeof(TCHAR),    // 输出缓存大小
		BUFSIZE*sizeof(TCHAR),    // 输入缓存大小
		PIPE_TIMEOUT,             // 客户端超时
		NULL);                    // 默认安全属性
	if (hPipe == INVALID_HANDLE_VALUE) 
	{
		printf("CreateNamedPipe failed with %d.\n", GetLastError()); 
		return 0;
	}


	//============> 创建新的完成端口
	g_hCompletePort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	//============> 关联完成端口，启用单线程
	::CreateIoCompletionPort(hPipe,	g_hCompletePort, 0, 2);

	CreateThread(NULL,	NULL, ThreadProc, NULL, 0, NULL);




	return 0;
}

