// Ping.cpp: implementation of the CPing class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Ping.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPing::CPing()
{
	//��ʼ��ICMP��
	icmp_data = NULL;       
	seq_no = 0;
	recvbuf = NULL;
	m_bRecordRout = FALSE;
	lpdest = NULL;
	datasize = DEF_PACKET_SIZE;
	
	WSADATA wsaData;        //��ʼ��socket
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("%s\n","Sorry, you cannot load socket dll!");
		return ;
	}
	m_hSocket = INVALID_SOCKET;       
	//m_hSocket�Ƿ���ICMP����socket, ��ֵ��32λ����FFFFFFFF,Ҳ����-1
	
}

CPing::~CPing()
{
	if(lpdest)        //char *lpdest; �ǳ�Ա����
		delete []lpdest;
}



void CPing::Ping(int timeout)
{   
	m_hSocket = WSASocket (AF_INET, SOCK_RAW, IPPROTO_ICMP, NULL, 0,
		WSA_FLAG_OVERLAPPED);
	//���Socket����ΪSOCK_RAW��Э��ΪIPPROTO_ICMP
	if (m_hSocket == INVALID_SOCKET)
	{
		printf("%s\n","socket ����ʧ��!");
		return ;
    }
	
    if (m_bRecordRout)        //BOOL m_bRecordRout;
    {
        // Ϊÿһ��ICMP������IPͷѡ��
        ZeroMemory(&m_ipopt, sizeof(m_ipopt));        //IpOptionHeader   m_ipopt;
        m_ipopt.code = IP_RECORD_ROUTE; // ��¼·��ѡ��
        m_ipopt.ptr  = 4;               // ָ���һ����ַ��ƫ��
        m_ipopt.len  = 39;              // ��ѡͷ�ĳ���
		// ��IPͷ������ѡ��
        int ret = setsockopt(m_hSocket, IPPROTO_IP, IP_OPTIONS,
            (char *)&m_ipopt, sizeof(m_ipopt));
		
		
        if (ret == SOCKET_ERROR)
        {
            printf("%s\n","����socketЭ��ѡ�����!");
        }
    }
	
    //���÷��ͽ��ճ�ʱֵ, timeout�Ǳ��������β� //����ʱ��
    int bread = setsockopt(m_hSocket, SOL_SOCKET, SO_RCVTIMEO,
		(char*)&timeout, sizeof(timeout));   
    if(bread == SOCKET_ERROR)
    {
        printf("%s\n","����socket���ճ�ʱѡ�����!");
        return ;
    }
    memset(&m_addrDest, 0, sizeof(m_addrDest));  //m_addrDestĿ�ĵ�ַ
    //
    //�����Ҫ�Ļ���Ҫ����������ΪIP
    m_addrDest.sin_family = AF_INET;
    if ((m_addrDest.sin_addr.s_addr = inet_addr(lpdest)) == INADDR_NONE)
    {   
		struct hostent *hp = NULL;
		
        if ((hp = gethostbyname(lpdest)) != NULL)
        {
            memcpy(&(m_addrDest.sin_addr), hp->h_addr, hp->h_length);
            m_addrDest.sin_family = hp->h_addrtype;
            printf("m_addrDest.sin_addr = %s\n", inet_ntoa(m_addrDest.sin_addr));
        }
        else
        {
			printf("%s\n","���������������!");
            return ;
        }
    }        
	
    //
    // ����ICMP��      
    datasize += sizeof(IcmpHeader);  
	
    icmp_data =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		MAX_PACKET);
    recvbuf =(char*) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
		MAX_PACKET);
    if (!icmp_data)
    {
        printf("%s\n","�ѷ������!");
        return ;
    }
    memset(icmp_data,0,MAX_PACKET);
    FillICMPData(icmp_data,datasize);
    //
    // ��ʼ���ͽ���ICMP��
	int nCount=0;
    while(1)
    {
        int        bwrote;
		
        if (nCount++ == 4)
            break;
		
        ((IcmpHeader*)icmp_data)->i_cksum = 0;
        ((IcmpHeader*)icmp_data)->timestamp = GetTickCount();
        ((IcmpHeader*)icmp_data)->i_seq = seq_no++;
        ((IcmpHeader*)icmp_data)->i_cksum = checksum((USHORT*)icmp_data, datasize);
		
        bwrote = sendto(m_hSocket, icmp_data, datasize, 0,
			(struct sockaddr*)&m_addrDest, sizeof(m_addrDest));
        if (bwrote == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
            {
				printf("%s\n","Timed out !");
                continue;
            }
			printf("%s\n","�������ݺ������ô���!");
            return ;
        }
        if (bwrote < datasize)
        {
			printf("Wrote %d bytes \r\n",bwrote);
        }
		
		int fromlen = sizeof(m_addrFrom);
        bread = recvfrom(m_hSocket, recvbuf, MAX_PACKET, 0,
			(struct sockaddr*)&m_addrFrom, &fromlen);
        if (bread == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
            {
				printf("%s\n","Timed out !");
                continue;
            }
			printf("%s\n","�������ݺ������ô���!");
            return ;
        }
        DecodeICMPHeader(recvbuf, bread, &m_addrFrom);  
    }
}

void CPing::Cleanup()
{
	if (m_hSocket != INVALID_SOCKET)
        closesocket(m_hSocket);
    HeapFree(GetProcessHeap(), 0, recvbuf);
    HeapFree(GetProcessHeap(), 0, icmp_data);
	
    return ;
}

void CPing::FillICMPData(char *icmp_data, int datasize)
{
	IcmpHeader *icmp_hdr = NULL;
    char       *datapart = NULL;
	
    icmp_hdr = (IcmpHeader*)icmp_data;
    icmp_hdr->i_type = ICMP_ECHO;        // Request an ICMP echo
    icmp_hdr->i_code = 0;
    icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_seq = 0;
	
    datapart = icmp_data + sizeof(IcmpHeader);
}

void CPing::DecodeIPOptions(char *buf, int bytes)
{
    IpOptionHeader *ipopt = NULL;
    IN_ADDR         inaddr;
    int             i;
    HOSTENT        *host = NULL;
	
    ipopt = (IpOptionHeader *)(buf + 20);
	
	printf("%s\n","Ping ���: ");
    for(i = 0; i < (ipopt->ptr / 4) - 1; i++)
    {
        inaddr.S_un.S_addr = ipopt->addr;
        if (i != 0)
		{
			printf("%s","  ");
		}
        host = gethostbyaddr((char *)&inaddr.S_un.S_addr,
			sizeof(inaddr.S_un.S_addr), AF_INET);
		
        if (host)
		{
			printf("(%-15s) %s \r\n", inet_ntoa(inaddr), host->h_name);
		}
        else
		{
			printf("(%-15s)\r\n", inet_ntoa(inaddr));
		}
    }
    return;
}

USHORT CPing::checksum(USHORT *buffer, int size)
{
	unsigned long cksum=0;
	
    while (size > 1)
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size)
    {
        cksum += *(UCHAR*)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >>16);
    return (USHORT)(~cksum);
}

void CPing::SetConfigure(char * host,BOOL recordrout,int size)
{
    if(lpdest)
    {
		delete[] lpdest;
		lpdest=NULL;
    }
    m_bRecordRout = recordrout;
    datasize = size;
    lpdest = new char [strlen(host)+1];
    strcpy(lpdest,host);
}

void CPing::DecodeICMPHeader(char *buf, int bytes, SOCKADDR_IN *from)
{
	IpHeader       *iphdr = NULL;
    IcmpHeader     *icmphdr = NULL;
    unsigned short  iphdrlen;
    DWORD           tick;
    static   int    icmpcount = 0;
	
    iphdr = (IpHeader *)buf;
    iphdrlen = iphdr->h_len * 4;
    tick = GetTickCount();
	
    if ((iphdrlen == MAX_IP_HDR_SIZE) && (!icmpcount))
        DecodeIPOptions(buf, bytes);
	
    if (bytes  < iphdrlen + ICMP_MIN)
    {
		printf("Too few bytes from %s \r\n",inet_ntoa(from->sin_addr));
    }
    icmphdr = (IcmpHeader*)(buf + iphdrlen);
	
    if (icmphdr->i_type != ICMP_ECHOREPLY)
    {
		printf("nonecho type %d recvd \r\n", icmphdr->i_type);
        return;
    }
    if (icmphdr->i_id != (USHORT)GetCurrentProcessId())
    {
		printf("someone else's packet! \r\n");
        return ;
    }
	
	printf("%d bytes from %s: \r\n", bytes, inet_ntoa(from->sin_addr));
	printf(" icmp_seq = %d. \r\n", icmphdr->i_seq);
	printf(" time: %d ms \r\n", tick - icmphdr->timestamp);
    icmpcount++;
    return;
}
