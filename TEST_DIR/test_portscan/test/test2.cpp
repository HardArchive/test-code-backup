// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#include <afxext.h>
#include <time.h>
#include <winsock.h>
// 编译时需使用的库
#pragma comment(lib,"wsock32.lib")
// select()成员定义
#define ZERO (fd_set *)0
// 变量定义
int maxth, scanok, scannum;
int portip, hoststart, hoststop, startport, endport;  //定义了开始I和结束P地址，开始和结束端口
long searchnum, searched;
void usage(char *);   // 定义显示使用方法函数
void playx(int);   // 定义状态提示函数
void setip2(char *);  // 定义设置IP函数
void customport(char *, char *, char *); // 定义自定义扫描端口函数
void portscannow(int);  // 定义端口扫描扫描

// 主程序
int main(int argc, char *argv[])
{
    WSADATA wsadata;
    // 清屏
    system("cls.exe");
    // 显示版本信息
    printf("\r\n==================== HUC Command Line PortScanner V0.2 ====================");
    printf("\r\n=============== By Lion, Welcome to [url]http://www.cnhonker.net[/url] ===============\r\n\n");

    // 检查输入
    if ((argc < 3) || (argc > 4))
    {
        // 显示帮助提示
        usage(argv[0]);
        return -1;
    }

    // 检测是否为port扫描
    if(!(stricmp(strlwr(argv[1]), "-p") == 0))
    {
        usage(argv[0]);
        return -1;
    }

    // 程序初始化
    if (WSAStartup(MAKEWORD(1, 1), &wsadata) != 0) //如果初始化错误
    {
        printf("\r\nWsatartup error");        //出错信息
        return -1;
    }

    // 端口扫描参数转换
    // 如果参数为三个
    if (argc == 3)
    {
        // 直接设置IP
        setip2(argv[2]);
    }
    // 如果参数为四个
    else if (argc == 4)
    {
        // 进入定制端口扫描处理
        customport(argv[0], argv[2], argv[3]);
    }
    // 参数过多显示帮助
    else
    {
        usage(argv[0]);
        return -1;
    }

    // 扫描端口开始
    portscannow(argc);

    WSACleanup();
    return 0;
}

// 帮助提示函数
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

// 进度提示
void playx(int play = 0)
{
	int i = 0 ;
    // 进度条
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
        printf(" =%s=\r", plays[play]); //显示进度
        Sleep(10);
    }
}

// 设置IP
void setip2(char *cp)
{
    int host;
    struct hostent *testhost;
    char *startip = "", *endip = "";

    // 判断是否为 192.168.0.1-192.168.0.254 形式的输入
    if (strstr(cp, "-") && strlen(cp) > 15 && strlen(cp) < 32)
    {
        // 提取出结束IP
        endip = strchr(cp, '-') + 1;
        // 提取出开始IP
        strncpy(startip, cp, strlen(cp) - strlen(strchr(cp, 45)));

        // 给控制要扫描IP段的变量赋值
        hoststart = ntohl(inet_addr(startip));
        hoststop = ntohl(inet_addr(endip));
    }
    else
    {
        // 取得输入的主机地址
        testhost = gethostbyname(startip);

        // 如果地址不存在
        if(!testhost)
        {
            WSACleanup( );
            printf("\r\nCan&#39;t get ip of: %s", cp);
            exit(-1);
        }
        // 给控制要扫描IP段的变量赋值
        memcpy(&host, testhost->h_addr, 4);
        hoststop = hoststart = ntohl(host);
    }
}

// 测试线程是否已满
void TestThread(int thread = 200)
{
    for (;;)
    {
        playx();

        // 测试线程是否已满
        if (maxth > thread)
            Sleep(100);
        else break;
    }
    return;
}

// 等待线程结束函数
void WaitThreadEnd()
{
    // 延时
    Sleep(6000);

    // 显示等待提示
    printf("\r    \r\n");
    printf(" Wait ( %d )Thread end...\r\n", maxth);

    for(;;)
    {
        // 判断所有线程是否已经结束
        if (maxth > 0)
        {
            // 延时等待线程序结束
            Sleep(100);
            playx();
            continue;
        }
        else break;
    }
    printf("\r\n");
    return;
}

// 定制端口扫描参数定义
void customport(char *cp, char *cp2, char *cp3)
{
    int intport;
    char *checker;

    // 处理要扫描的端口
    // 扫描开始端口变量赋值
    startport = atoi(cp2);
    // 扫描结束端口变量赋值
    endport = atoi(cp2);

    // 判断是否 21-80 形式
    if (strstr(cp2, "-"))
    {
        intport = atoi(checker = strchr(cp2, '-') + 1);
        if (intport > 0 && intport < 65536)
            // 扫描结束端口变量赋值
            endport = intport;
    }

    // 端口大小判断
    if (startport < 0 || startport > 65536 || endport < 0 || endport > 65535)
    {
        usage(cp);
        exit(-1);
    }
    // 处理ip地址
    setip2(cp3);
}

// 端口扫描函数
UINT portscan(LPVOID port)
{
    int addr = portip; // 取得要扫描的地址
    int sock;
    struct fd_set mask;
    struct timeval timeout;
    struct sockaddr_in server;
    unsigned long flag = 1;

    // 创建一个sock
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // 创建sock失败处理
    if (sock == INVALID_SOCKET)
    {
        printf("\r\nSock Error:%s", WSAGetLastError());
        maxth --;
        return -1;
    }

    // 给sock成员赋值
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(addr); // 要扫描的地址
    server.sin_port = htons(short(port)); // 要扫描的端口

    // 显示进度
    playx();

    // 调用ioctlsocket()设置套接字为非阻塞模式
    if (ioctlsocket(sock, FIONBIO, &flag) != 0)
    {
        // 设置失败处理
        printf("\r\nSock Error:%s", WSAGetLastError());
        closesocket(sock);
        maxth --;
        return -1;
    }

    // 调用connect()连接远程主机端口
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    timeout.tv_sec = 18; // 超时限制为18秒
    timeout.tv_usec = 0;
    FD_ZERO(&mask);  // 清空集合mask
    FD_SET(sock, &mask); // 将sock放入集合mask中

    // 用select() 处理扫描结果
    switch(select(sock + 1, ZERO, &mask, ZERO, &timeout))
    {
    case -1:
    {
        printf("\r\nSelect() error");
        maxth --;
        return -1;
    }

    // sock超时处理
    case 0:
    {
        maxth --;
        closesocket(sock);
        return -1;
    }

    default:
        if(FD_ISSET(sock, &mask))
        {
            // 禁止sock发送和接受数据
            shutdown(sock, 0);

            // 设置输出结果格式
            printf(" [Found:] %s Port: %d open.\r\n", inet_ntoa(server.sin_addr), ntohs(server.sin_port));

            // 关闭sock
            closesocket(sock);
            scanok ++;
            maxth --;
            return 1;
        }
    }
    return 0;
}

// 扫描开始主函数
void portscannow(int xp)
{
    int sport;
    char *timenow, timebuf[32];

    // 定义默认扫描的端口
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
        "12345"  //这里你也可以自己定义要扫描的端口
    };

    // 显示扫描开始的时间
    timenow = _strtime(timebuf);
    printf("\r\nPortScan Start Time: %s\r\n\n", timenow);

    // 计数器初始化.
    maxth = 0;
    scanok = 0;
    scannum = 0;
    searched = 0;

    // 计算要扫描的端口数量
    searchnum = hoststop - hoststart + 1;
    if(xp == 3)
        searchnum = searchnum * 32;
    if(xp == 4)
        searchnum = searchnum * (endport - startport + 1);

    // 端口扫描开始
    for (portip = hoststart; portip <= hoststop; portip ++, scannum ++)
    {
        // *.*.*.0和*.*.*.255 地址处理
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
        // 默认端口扫描
        // scan 192.168.0.1
        // scan 192.168.0.1-192.168.0.254
        if (xp == 3)
        {
            for (sport = 0; sport < 32; sport ++, maxth ++, searched ++)
            {
                // 测试当前线程是否大于180
                TestThread(180);
                // 产生新的线程处理端口扫描
                CWinThread *pthread = AfxBeginThread(portscan, LPVOID(atoi((char *)ports[sport])));
                //延时
                Sleep(120);
            }
        }

        // 自定义端口扫描
        // scan -p 21 192.168.0.1
        // scan -p 21-80 192.168.0.1-192.168.0.254
        if (xp == 4)
        {
            // 计算要扫描的端口
            sport = endport - startport;
            if(sport > 500 )
            {
                // 扫描自定义的端口
                for(sport = startport; sport <= endport; sport ++, maxth ++, searched ++)
                {
                    TestThread(2000);
                    // 产生新的线程处理端口扫描
                    CWinThread *pthread = AfxBeginThread(portscan, LPVOID(sport));
                    // 延时
                    Sleep(10);
                }
            }
            else
            {
                // 扫描自定义的端口
                for(sport = startport; sport <= endport; sport ++, maxth ++, searched ++)
                {
                    // 测试当前线程是否大于250
                    TestThread(250);
                    // 产生新的线程处理端口扫描
                    CWinThread *pthread = AfxBeginThread(portscan, LPVOID(sport));
                    // 延时
                    Sleep(100);
                    playx();
                }
            }
        }
    }

    // 等待所有的线程结束
    WaitThreadEnd();

    // 显示端口扫描结束时间
    timenow = _strtime(timebuf);
    printf("\r\nPortScan End Time: %s", timenow);
    printf("\r\nScan %d Hosts completed. Open %d Ports!\r\n", scannum, scanok);
}