完成端口过程
一、CreateIoCompletionPort
//1、创建一个未关联任何文件的I/O完成端口。
WINBASEAPI
__out_opt
HANDLE
WINAPI
CreateIoCompletionPort(
    __in     HANDLE FileHandle,                    //是关联的文件句柄。
    __in_opt HANDLE ExistingCompletionPort,        //是已经存在的完成端口。如果为NULL，则为新建一个IOCP。
    __in     ULONG_PTR CompletionKey,              //是传送给处理函数的参数
    __in     DWORD NumberOfConcurrentThreads       //是有多少个线程在访问这个消息队列。当参数ExistingCompletionPort不为0的时候，系统忽略该参数，当该参数为0表示允许同时相等数目于处理器个数的线程访问该消息队列。
    );
hCompletePort	= ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);  //创建一个未关联任何文件的I/O完成端口。

//2、关联一个已打开的文件实例和新建的或已存在的I/0完成端口
::CreateIoCompletionPort((HANDLE)m_soListen, m_hCompletePort, m_soListen, 0)

二、CreateIoCompletionPort

WINBASEAPI
BOOL
WINAPI
GetQueuedCompletionStatus(
    __in  HANDLE CompletionPort,
    __out LPDWORD lpNumberOfBytesTransferred,
    __out PULONG_PTR lpCompletionKey,
    __out LPOVERLAPPED *lpOverlapped,
    __in  DWORD dwMilliseconds
    );


socket IOCP流程
1、创建监听socket 绑定监听端口
2、创建一个新的未关联任何文件(socket)的完成端口对象hCompletePort(CreateIoCompletionPort)
3、创建工作线程称为IO服务线程(线程调用GetQueuedCompletionStatus取得socket状态)
4、向第二步创建的完成端口对象hCompletePort关联第一步创建的socket(CreateIoCompletionPort)
5、

猜想  文件IOCP流程
1、打开文件取得文件句柄CreateFile
2、

///////////////////////////////重叠IO////////////////////////////////////////
WSAGetOverlappedResult(
    __in SOCKET s,
    __in LPWSAOVERLAPPED lpOverlapped,
    __out LPDWORD lpcbTransfer,
    __in BOOL fWait,
    __out LPDWORD lpdwFlags
    );
GetOverlappedResult(
    __in  HANDLE hFile,
    __in  LPOVERLAPPED lpOverlapped,
    __out LPDWORD lpNumberOfBytesTransferred,
    __in  BOOL bWait
    );