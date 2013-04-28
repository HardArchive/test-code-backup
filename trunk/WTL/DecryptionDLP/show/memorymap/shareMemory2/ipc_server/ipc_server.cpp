// ipc_server.cpp : 定义控制台应用程序的入口点。
//
/*
*  author: mikezhang
*
*  演示方法：两个程序编译后，分别启动ipc_client.exe和ipc_server.exe。然后在ipc_client.exe的控制台中随意输入
*         某些字符串，在ipc_server.exe的控制台上就可以显示出来。注意两个程序的数据交换和同步机制。
*
*  酷尚网 时尚的社区，www.coolshang.com  欢迎你的加入！
*
*  有意交流技术或互联网创意的，请联系mike19840321@tom.com
*  进程通信，进程同步，共享内存
*  本程序演示了两个知识点 1，共享内存的使用 2 同步机制，使用自动事件
*
*   注：本程序只是具有演示的作用，不是完善的代码，不具有健壮行，不具有性能的优化，
*      ，不对函数的返回值做处理，不顾及异常等等。
*      感兴趣的朋友可以一起探讨。
*/
#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
using namespace std;

#define BUF_SIZE 256
char szName[] = "Global\\MyFileMappingObject";
char szMsg[100];

int main()
{
    HANDLE hMapFile;
    LPCTSTR pBuf;

    //自动事件
    HANDLE h_event_a = CreateEvent(NULL, FALSE, FALSE, (LPCWSTR)"Global\\IPC_event_a");
    HANDLE h_event_b = CreateEvent(NULL, FALSE, FALSE, (LPCWSTR)"Global\\IPC_event_b");

    memset(szMsg, 0, sizeof(szMsg));

    //共享内存
    hMapFile = CreateFileMapping(
                   INVALID_HANDLE_VALUE,    // use paging file
                   NULL,                    // default security
                   PAGE_READWRITE,          // read/write access
                   0,                       // max. object size
                   BUF_SIZE,                // buffer size
                   (LPCWSTR)szName);        // name of mapping object

    if (hMapFile == NULL)
    {
        printf("Could not create file mapping object (%d).\n",
               GetLastError());
        return 1;
    }
    pBuf = (LPTSTR) MapViewOfFile(hMapFile,   // handle to map object
                                  FILE_MAP_ALL_ACCESS, // read/write permission
                                  0,
                                  0,
                                  BUF_SIZE);

    if (pBuf == NULL)
    {
        printf("Could not map view of file (%d).\n",
               GetLastError());
        return 2;
    }


    CopyMemory((PVOID)pBuf, szMsg, sizeof(szMsg));


    cout << "server is starting........." << endl;
    while(1)
    {

        /*
        *  author: mikezhang
        *
        *  酷尚网 时尚的社区，www.coolshang.com  欢迎你的加入！
        *
        *  进程通信，进程同步，共享内存
        */


        cout << endl << "waiting for connectting..........." << endl;

        //注意，先wait再setEvent，与client端的程序相反
        WaitForSingleObject(h_event_a, INFINITE);

        CopyMemory((PVOID)szMsg, pBuf, sizeof(szMsg));
        cout << "received message from client: ";
        for(int i = 0; i < sizeof(szMsg); i++)
            cout << szMsg[i];
        SetEvent(h_event_b);


    }


    UnmapViewOfFile(pBuf);

    CloseHandle(hMapFile);
    CloseHandle(h_event_a);
    CloseHandle(h_event_b);

    return 0;
}

