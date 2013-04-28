// ipc_server.cpp : �������̨Ӧ�ó������ڵ㡣
//
/*
*  author: mikezhang
*
*  ��ʾ�����������������󣬷ֱ�����ipc_client.exe��ipc_server.exe��Ȼ����ipc_client.exe�Ŀ���̨����������
*         ĳЩ�ַ�������ipc_server.exe�Ŀ���̨�ϾͿ�����ʾ������ע��������������ݽ�����ͬ�����ơ�
*
*  ������ ʱ�е�������www.coolshang.com  ��ӭ��ļ��룡
*
*  ���⽻����������������ģ�����ϵmike19840321@tom.com
*  ����ͨ�ţ�����ͬ���������ڴ�
*  ��������ʾ������֪ʶ�� 1�������ڴ��ʹ�� 2 ͬ�����ƣ�ʹ���Զ��¼�
*
*   ע��������ֻ�Ǿ�����ʾ�����ã��������ƵĴ��룬�����н�׳�У����������ܵ��Ż���
*      �����Ժ����ķ���ֵ���������˼��쳣�ȵȡ�
*      ����Ȥ�����ѿ���һ��̽�֡�
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

    //�Զ��¼�
    HANDLE h_event_a = CreateEvent(NULL, FALSE, FALSE, (LPCWSTR)"Global\\IPC_event_a");
    HANDLE h_event_b = CreateEvent(NULL, FALSE, FALSE, (LPCWSTR)"Global\\IPC_event_b");

    memset(szMsg, 0, sizeof(szMsg));

    //�����ڴ�
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
        *  ������ ʱ�е�������www.coolshang.com  ��ӭ��ļ��룡
        *
        *  ����ͨ�ţ�����ͬ���������ڴ�
        */


        cout << endl << "waiting for connectting..........." << endl;

        //ע�⣬��wait��setEvent����client�˵ĳ����෴
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

