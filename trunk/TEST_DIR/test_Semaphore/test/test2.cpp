#include "stdafx.h"
#include<windows.h>
#include<process.h>
#include<iostream>
using namespace std;

CRITICAL_SECTION cs;

typedef struct
{
    HANDLE h1;
    HANDLE h2;
    HANDLE h3;
    HANDLE h4;
    int a;
} PARAMS,*PPARAMS;

void read1(PVOID pvoid)
{
    while(TRUE)
    {
        volatile PPARAMS pparams=(PPARAMS)pvoid;
        WaitForSingleObject(pparams->h2,INFINITE);
        EnterCriticalSection(&cs);
        cout<<"读线程1开始读取...\n";
        cout<<(pparams->a)<<endl;
        LeaveCriticalSection(&cs);
        Sleep(1000);
        ReleaseSemaphore(pparams->h1,1,NULL);
    }
}

void read2(PVOID pvoid)
{
    while(TRUE)
    {
        volatile PPARAMS pparams=(PPARAMS)pvoid;
        WaitForSingleObject(pparams->h3,INFINITE);
        EnterCriticalSection(&cs);
        cout<<"读线程2开始读取...\n";
        cout<<(pparams->a)<<endl;
        LeaveCriticalSection(&cs);
        Sleep(1000);
        ReleaseSemaphore(pparams->h1,1,NULL);
    }
}

void read3(PVOID pvoid)
{
    while(TRUE)
    {
        volatile PPARAMS pparams=(PPARAMS)pvoid;
        WaitForSingleObject(pparams->h4,INFINITE);
        EnterCriticalSection(&cs);
        cout<<"读线程3开始读取...\n";
        cout<<(pparams->a)<<endl;
        LeaveCriticalSection(&cs);
        Sleep(1000);
        ReleaseSemaphore(pparams->h1,1,NULL);
    }
}

void write(PVOID pvoid)
{
    while(TRUE)
    {
        volatile PPARAMS pparams=(PPARAMS)pvoid;
		//等待 获取信号量
        WaitForSingleObject(pparams->h1,INFINITE);
        WaitForSingleObject(pparams->h1,INFINITE);
        WaitForSingleObject(pparams->h1,INFINITE);
        cout<<"=================\n";
        cout<<"写线程开始写入...\n";
        pparams->a=rand()%256;
        cout<<"写入"<<(pparams->a)<<endl;
		//信号量增加可被获取的记数
        ReleaseSemaphore(pparams->h2,1,NULL);
        ReleaseSemaphore(pparams->h3,1,NULL);
        ReleaseSemaphore(pparams->h4,1,NULL);
    }
}
//write线程写了之后，read1,read2,read3才能读，且只有3个线程都读完，write函数才能向buffer中写。
int main()
{
    PARAMS params;
    params.h1=CreateSemaphore(NULL,3,3,NULL);
    params.h2=CreateSemaphore(NULL,0,1,NULL);
    params.h3=CreateSemaphore(NULL,0,1,NULL);
    params.h4=CreateSemaphore(NULL,0,1,NULL);
    InitializeCriticalSection(&cs);
    _beginthread(read1,0,&params);
    _beginthread(read2,0,&params);
    _beginthread(read3,0,&params);
    _beginthread(write,0,&params);
    // HANDLE hEvent;
    // hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
    // WaitForSingleObject(hEvent,INFINITE);
    int a;
    cin>>a;
    DeleteCriticalSection(&cs);
    return 0;
}