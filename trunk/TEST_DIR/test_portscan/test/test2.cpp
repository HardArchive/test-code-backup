// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//#include <afxext.h>
#include <time.h>
#include <winsock.h>
// ����ʱ��ʹ�õĿ�
#pragma comment(lib,"wsock32.lib")
// select()��Ա����
#define ZERO (fd_set *)0
// ��������
int maxth, scanok, scannum;
int portip, hoststart, hoststop, startport, endport;  //�����˿�ʼI�ͽ���P��ַ����ʼ�ͽ����˿�
long searchnum, searched;
void usage(char *);   // ������ʾʹ�÷�������
void playx(int);   // ����״̬��ʾ����
void setip2(char *);  // ��������IP����
void customport(char *, char *, char *); // �����Զ���ɨ��˿ں���
void portscannow(int);  // ����˿�ɨ��ɨ��

// ������
int main(int argc, char *argv[])
{
    WSADATA wsadata;
    // ����
    system("cls.exe");
    // ��ʾ�汾��Ϣ
    printf("\r\n==================== HUC Command Line PortScanner V0.2 ====================");
    printf("\r\n=============== By Lion, Welcome to [url]http://www.cnhonker.net[/url] ===============\r\n\n");

    // �������
    if ((argc < 3) || (argc > 4))
    {
        // ��ʾ������ʾ
        usage(argv[0]);
        return -1;
    }

    // ����Ƿ�Ϊportɨ��
    if(!(stricmp(strlwr(argv[1]), "-p") == 0))
    {
        usage(argv[0]);
        return -1;
    }

    // �����ʼ��
    if (WSAStartup(MAKEWORD(1, 1), &wsadata) != 0) //�����ʼ������
    {
        printf("\r\nWsatartup error");        //������Ϣ
        return -1;
    }

    // �˿�ɨ�����ת��
    // �������Ϊ����
    if (argc == 3)
    {
        // ֱ������IP
        setip2(argv[2]);
    }
    // �������Ϊ�ĸ�
    else if (argc == 4)
    {
        // ���붨�ƶ˿�ɨ�账��
        customport(argv[0], argv[2], argv[3]);
    }
    // ����������ʾ����
    else
    {
        usage(argv[0]);
        return -1;
    }

    // ɨ��˿ڿ�ʼ
    portscannow(argc);

    WSACleanup();
    return 0;
}

// ������ʾ����
void usage(char *prog)
{
    printf("Usage: %s <Option>", prog);
    printf("\r\n\n <Option>:");
    printf("\r\n -p [ Port|StartPort-EndPort ] < HostName|IP|StartIP-EndIP >");
    printf("\r\n\n Example: ");
    printf("\r\n %s -p 192.168.0.1", prog);
    printf("\r\n %s -p 192.168.0.1-192.168.0.254", prog);
    printf("\r\n %s -p 21-80 192.168.0.1", prog);
    printf("\r\n %s -p 21-80 192.168.0.1-192.168.0.254\r\n", prog);
    return;
}

// ������ʾ
void playx(int play = 0)
{
	int i = 0 ;
    // ������
    char *plays[12] =
    {
        " | ",
        " / ",
        " - ",
        " \\ ",
        " | ",
        " / ",
        " - ",
        " \\ ",
        " | ",
        " / ",
        " - ",
        " \\ ",
    };

    if (searchnum != 0)
    {
        for (i = 0 ; i <= 3; i ++)
        {
            printf(" =%s= %d%s Completed.  \r", plays[i] , searched * 100 / (searchnum + 1), "%");
            Sleep(5);
        }
    }
    else
    {
        printf(" =%s=\r", plays[play]); //��ʾ����
        Sleep(10);
    }
}

// ����IP
void setip2(char *cp)
{
    int host;
    struct hostent *testhost;
    char *startip = "", *endip = "";

    // �ж��Ƿ�Ϊ 192.168.0.1-192.168.0.254 ��ʽ������
    if (strstr(cp, "-") && strlen(cp) > 15 && strlen(cp) < 32)
    {
        // ��ȡ������IP
        endip = strchr(cp, '-') + 1;
        // ��ȡ����ʼIP
        strncpy(startip, cp, strlen(cp) - strlen(strchr(cp, 45)));

        // ������Ҫɨ��IP�εı�����ֵ
        hoststart = ntohl(inet_addr(startip));
        hoststop = ntohl(inet_addr(endip));
    }
    else
    {
        // ȡ�������������ַ
        testhost = gethostbyname(startip);

        // �����ַ������
        if(!testhost)
        {
            WSACleanup( );
            printf("\r\nCan&#39;t get ip of: %s", cp);
            exit(-1);
        }
        // ������Ҫɨ��IP�εı�����ֵ
        memcpy(&host, testhost->h_addr, 4);
        hoststop = hoststart = ntohl(host);
    }
}

// �����߳��Ƿ�����
void TestThread(int thread = 200)
{
    for (;;)
    {
        playx();

        // �����߳��Ƿ�����
        if (maxth > thread)
            Sleep(100);
        else break;
    }
    return;
}

// �ȴ��߳̽�������
void WaitThreadEnd()
{
    // ��ʱ
    Sleep(6000);

    // ��ʾ�ȴ���ʾ
    printf("\r    \r\n");
    printf(" Wait ( %d )Thread end...\r\n", maxth);

    for(;;)
    {
        // �ж������߳��Ƿ��Ѿ�����
        if (maxth > 0)
        {
            // ��ʱ�ȴ��߳������
            Sleep(100);
            playx();
            continue;
        }
        else break;
    }
    printf("\r\n");
    return;
}

// ���ƶ˿�ɨ���������
void customport(char *cp, char *cp2, char *cp3)
{
    int intport;
    char *checker;

    // ����Ҫɨ��Ķ˿�
    // ɨ�迪ʼ�˿ڱ�����ֵ
    startport = atoi(cp2);
    // ɨ������˿ڱ�����ֵ
    endport = atoi(cp2);

    // �ж��Ƿ� 21-80 ��ʽ
    if (strstr(cp2, "-"))
    {
        intport = atoi(checker = strchr(cp2, '-') + 1);
        if (intport > 0 && intport < 65536)
            // ɨ������˿ڱ�����ֵ
            endport = intport;
    }

    // �˿ڴ�С�ж�
    if (startport < 0 || startport > 65536 || endport < 0 || endport > 65535)
    {
        usage(cp);
        exit(-1);
    }
    // ����ip��ַ
    setip2(cp3);
}

// �˿�ɨ�躯��
UINT portscan(LPVOID port)
{
    int addr = portip; // ȡ��Ҫɨ��ĵ�ַ
    int sock;
    struct fd_set mask;
    struct timeval timeout;
    struct sockaddr_in server;
    unsigned long flag = 1;

    // ����һ��sock
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // ����sockʧ�ܴ���
    if (sock == INVALID_SOCKET)
    {
        printf("\r\nSock Error:%s", WSAGetLastError());
        maxth --;
        return -1;
    }

    // ��sock��Ա��ֵ
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(addr); // Ҫɨ��ĵ�ַ
    server.sin_port = htons(short(port)); // Ҫɨ��Ķ˿�

    // ��ʾ����
    playx();

    // ����ioctlsocket()�����׽���Ϊ������ģʽ
    if (ioctlsocket(sock, FIONBIO, &flag) != 0)
    {
        // ����ʧ�ܴ���
        printf("\r\nSock Error:%s", WSAGetLastError());
        closesocket(sock);
        maxth --;
        return -1;
    }

    // ����connect()����Զ�������˿�
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    timeout.tv_sec = 18; // ��ʱ����Ϊ18��
    timeout.tv_usec = 0;
    FD_ZERO(&mask);  // ��ռ���mask
    FD_SET(sock, &mask); // ��sock���뼯��mask��

    // ��select() ����ɨ����
    switch(select(sock + 1, ZERO, &mask, ZERO, &timeout))
    {
    case -1:
    {
        printf("\r\nSelect() error");
        maxth --;
        return -1;
    }

    // sock��ʱ����
    case 0:
    {
        maxth --;
        closesocket(sock);
        return -1;
    }

    default:
        if(FD_ISSET(sock, &mask))
        {
            // ��ֹsock���ͺͽ�������
            shutdown(sock, 0);

            // ������������ʽ
            printf(" [Found:] %s Port: %d open.\r\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

            // �ر�sock
            closesocket(sock);
            scanok ++;
            maxth --;
            return 1;
        }
    }
    return 0;
}

// ɨ�迪ʼ������
void portscannow(int xp)
{
    int sport;
    char *timenow, timebuf[32];

    // ����Ĭ��ɨ��Ķ˿�
    char *ports[32] =
    {
        "21",
        "22",
        "23",
        "25",
        "53",
        "79",
        "80",
        "110",
        "111",
        "113",
        "123",
        "135",
        "139",
        "143",
        "443",
        "512",
        "513",
        "514",
        "515",
        "540",
        "1080",
        "1433",
        "1521",
        "1524",
        "3306",
        "3389",
        "5631",
        "6000",
        "6112",
        "8000",
        "8080",
        "12345"  //������Ҳ�����Լ�����Ҫɨ��Ķ˿�
    };

    // ��ʾɨ�迪ʼ��ʱ��
    timenow = _strtime(timebuf);
    printf("\r\nPortScan Start Time: %s\r\n\n", timenow);

    // ��������ʼ��.
    maxth = 0;
    scanok = 0;
    scannum = 0;
    searched = 0;

    // ����Ҫɨ��Ķ˿�����
    searchnum = hoststop - hoststart + 1;
    if(xp == 3)
        searchnum = searchnum * 32;
    if(xp == 4)
        searchnum = searchnum * (endport - startport + 1);

    // �˿�ɨ�迪ʼ
    for (portip = hoststart; portip <= hoststop; portip ++, scannum ++)
    {
        // *.*.*.0��*.*.*.255 ��ַ����
        if ((portip % 256) == 0 || (portip % 256) == 255)
        {
            if(xp == 3)
                searchnum = searchnum - 32;
            if(xp == 4)
                searchnum = searchnum - (endport - startport + 1);
            scannum --;
            playx();
            continue;
        }

        //if(i > 11) i = 0;
        // Ĭ�϶˿�ɨ��
        // scan 192.168.0.1
        // scan 192.168.0.1-192.168.0.254
        if (xp == 3)
        {
            for (sport = 0; sport < 32; sport ++, maxth ++, searched ++)
            {
                // ���Ե�ǰ�߳��Ƿ����180
                TestThread(180);
                // �����µ��̴߳���˿�ɨ��
                CWinThread *pthread = AfxBeginThread(portscan, LPVOID(atoi((char *)ports[sport])));
                //��ʱ
                Sleep(120);
            }
        }

        // �Զ���˿�ɨ��
        // scan -p 21 192.168.0.1
        // scan -p 21-80 192.168.0.1-192.168.0.254
        if (xp == 4)
        {
            // ����Ҫɨ��Ķ˿�
            sport = endport - startport;
            if(sport > 500 )
            {
                // ɨ���Զ���Ķ˿�
                for(sport = startport; sport <= endport; sport ++, maxth ++, searched ++)
                {
                    TestThread(2000);
                    // �����µ��̴߳���˿�ɨ��
                    CWinThread *pthread = AfxBeginThread(portscan, LPVOID(sport));
                    // ��ʱ
                    Sleep(10);
                }
            }
            else
            {
                // ɨ���Զ���Ķ˿�
                for(sport = startport; sport <= endport; sport ++, maxth ++, searched ++)
                {
                    // ���Ե�ǰ�߳��Ƿ����250
                    TestThread(250);
                    // �����µ��̴߳���˿�ɨ��
                    CWinThread *pthread = AfxBeginThread(portscan, LPVOID(sport));
                    // ��ʱ
                    Sleep(100);
                    playx();
                }
            }
        }
    }

    // �ȴ����е��߳̽���
    WaitThreadEnd();

    // ��ʾ�˿�ɨ�����ʱ��
    timenow = _strtime(timebuf);
    printf("\r\nPortScan End Time: %s", timenow);
    printf("\r\nScan %d Hosts completed. Open %d Ports!\r\n", scannum, scanok);
}