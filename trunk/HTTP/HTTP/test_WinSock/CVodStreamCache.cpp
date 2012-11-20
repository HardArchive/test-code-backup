#include "stdafx.h"
#include "CVodStreamCache.h"

// constructor
CVodStreamCache::CVodStreamCache()
{
	err = FALSE;
	pHost = NULL;
	pGet = NULL;
	requestHeader = NULL;
	responseHeader = NULL;
}

// destructor
CVodStreamCache::~CVodStreamCache()
{
	;
}

HANDLE CVodStreamCache::Create(char token[], char path[])
{
	// ������
	if (err)
	{
		return NULL;
	}

	InitSocketLib();	// ��ʼ��Socket������
	ParseURL(path);	// ����URL��ַ
	Socket();	// ����SOCKET����
	Connect(pHost);	// ��������

	FormatRequestHeader(pHost, pGet);	// ��ʽ������ͷ
	SendRequest(requestHeader);	// ��������ͷ
	GetResponseHeader();	// ��ȡ�����ķ���ͷ
	SetTimeOut(10000);	// ���ý��ջ��͵��ʱ��
	InitCache();	// ��ʼ��Cache����ȡ�ļ�����

	// ����ָ����С���ļ�
	cacheFile.open("E:\\zz.txt", ios::binary|ios::out);
	for (int i=0; i<cache.getTotalLen(); ++i)
	{
		cacheFile.put('\0');
	}
	cacheFile.close();

	if (err)
	{
		cout << "����ʧ�ܣ�" << endl;
	}
	else
	{
		cout << "�����ɹ���" << endl;
	}

	return NULL;
}

int CVodStreamCache::ReadBlock(long offset, int len, char *buff)
{
	cacheFile.open("E:\\zz.txt", ios::binary|ios::in);
	if (!cacheFile)
	{
		cout << "cannot open file!" << endl;
	}
	int rcv_bytes = 0;
	cacheFile.seekg(offset, ios::beg);
	cacheFile.read(buff, len);
	rcv_bytes = cacheFile.gcount();
	cacheFile.close();
	return rcv_bytes;
}

int CVodStreamCache::WriteBlock(long offset, int len, char *buff)
{
	FormatRequestHeader(pHost, pGet, offset, offset+len-1);	// ��ʽ������ͷ
	SendRequest(requestHeader);	// ��������ͷ
	//GetResponseHeader();	// ��ȡ�����ķ���ͷ
	//SetTimeOut(5000);	// ���ý��ջ��͵��ʱ��

	cacheFile.open("E:\\zz.txt", ios::binary|ios::in|ios::out);
	if (!cacheFile)
	{
		cout << "cannot open file" << endl;
	}
	int rcv_bytes = 0;
	rcv_bytes = recv(sock, buff, len, 0);
	
	if (rcv_bytes > 0)
	{
		cacheFile.seekp(offset, ios::beg);
		cacheFile.write(buff, rcv_bytes);
		cache.setRecvLen(cache.getRecvLen()+len);
	}
	cacheFile.close();
	return rcv_bytes;
}

void CVodStreamCache::Destroy()
{
	if (!err)
	{
		//delete[] pHost;

		closesocket(sock);
	}
	WSACleanup();
}

void CVodStreamCache::CacheInfo()
{
	;
}

// ��ʼ��Socket������
void CVodStreamCache::InitSocketLib()
{
	if (err) return;

	WORD wVersion;
	WSADATA wsaData;

	wVersion = MAKEWORD(2, 0);
	if (WSAStartup(wVersion, &wsaData) != 0)
	{
		cout << "�޷�װ��Socket�⣡" << endl;
		err = FALSE;
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 2)
	{
		cout << "�޷��ҵ����ʵ�Socket�⣡" << endl;
		err = FALSE;
		WSACleanup();
		return;
	}
}

// ����URL��ַ
void CVodStreamCache::ParseURL(char* url)
{
	if (err) return;

	pHost = new char[strlen(url)];
	strcpy(pHost, url);
	for (int i=0; i<strlen(pHost); ++i)
	{
		if (pHost[i]=='/' || pHost[i]=='\0')
		{
			pHost[i] = '\0';
			break;
		}
		++url;
	}

	pGet = new char[strlen(url)+1];
	if (*url == '\0')
	{
		strcpy(pGet, "/");
	}
	else
	{
		strcpy(pGet, url);
	}
}

// ����URL���HTTP����ͷ
void CVodStreamCache::FormatRequestHeader(char* pHost, char* pGet, long lFrom/* =0 */, long lTo/* =0 */)
{
	if (err) return;

	if (requestHeader != NULL)
	{
		delete[] requestHeader;
		requestHeader = NULL;
	}

	requestHeader = new char[1024];
	memset(requestHeader, 0, sizeof(requestHeader));

	// �����������·�����汾
	strcat(requestHeader, "GET ");
	strcat(requestHeader, pGet);
	strcat(requestHeader, " HTTP/1.1");
	strcat(requestHeader, "\r\n");

	// ����
	strcat(requestHeader, "Host: ");
	strcat(requestHeader, pHost);
	strcat(requestHeader, "\r\n");

	/*if(pReferer != NULL)
	{
		strcat(requestHeader, "Referer:");
		strcat(requestHeader, pReferer);
		strcat(requestHeader, "\r\n");		
	}*/

	// ���յ���������
	strcat(requestHeader, "Accept:*/*");
	strcat(requestHeader, "\r\n");

	// ���������
	strcat(requestHeader, "User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)");
	strcat(requestHeader, "\r\n");

	// �������ã�����
	strcat(requestHeader, "Connection:Keep-Alive");
	strcat(requestHeader, "\r\n");

	// Cookie
	/*if(pCookie != NULL)
	{
		strcat(requestHeader, "Set Cookie:0");
		strcat(requestHeader, pCookie);
		strcat(requestHeader, "\r\n");
	}*/

	// �����������ʼ�ֽ�λ��(�ϵ������Ĺؼ�)
	if (lFrom > 0)
	{
		char temp[10];

		strcat(requestHeader, "Range: bytes=");
		_ltoa(lFrom, temp, sizeof(temp));
		strcat(requestHeader, temp);
		strcat(requestHeader, "-");

		if (lTo > lFrom)
		{
			_ltoa(lTo, temp, sizeof(temp));
			strcat(requestHeader, temp);
		}
		strcat(requestHeader, "\r\n");
	}

	// ���һ�У�����
	strcat(requestHeader, "\r\n");

	cout << endl << "���͵�����ͷΪ��" << endl << requestHeader;
}

// ����SOCKET����
void CVodStreamCache::Socket()
{
	if (err) return;

	struct protoent *ppe; 
	ppe=getprotobyname("tcp"); 

	sock = socket(PF_INET, SOCK_STREAM, ppe->p_proto);
	if (sock == INVALID_SOCKET)
	{
		cout << "����Socket����ʧ�ܣ�" << endl;
		err = TRUE;
		return;
	}
}

// ��������
void CVodStreamCache::Connect(char* pHost, int port/* =80 */)
{
	if (err) return;

	// ������������ȡIP��ַ
	hostent* pHostEnt = gethostbyname(pHost);
	if (pHostEnt == NULL)
	{
		cout << "��ȡIP��ַʧ�ܣ�" << endl;
		err = TRUE;
		return;
	}

	// ����
	struct in_addr ip_addr;
	memcpy(&ip_addr, pHostEnt->h_addr_list[0], 4);	// h_addr_list[0]��4���ֽ�,ÿ���ֽ�8λ

	struct sockaddr_in destaddr;
	memset((void*)&destaddr, 0, sizeof(destaddr));
	destaddr.sin_family = AF_INET;
	destaddr.sin_port = htons(port);
	destaddr.sin_addr = ip_addr;

	if (0 != connect(sock, (struct sockaddr*)&destaddr, sizeof(destaddr)))
	{
		cout << "����ʧ�ܣ�" << endl;
		err = TRUE;
		return;
	}
}

// ��������ͷ
void CVodStreamCache::SendRequest(const char* requestHeader)
{
	if (err) return;

	if (SOCKET_ERROR == send(sock, requestHeader, strlen(requestHeader), 0))
	{
		cout << "����ͷ����ʧ�ܣ�" << endl;
		err = TRUE;
		return;
	}
}

// ��ȡ�����ķ���ͷ
void CVodStreamCache::GetResponseHeader()
{
	if (err) return;

	if (responseHeader != NULL)
	{
		delete[] responseHeader;
		responseHeader = NULL;
	}

	responseHeader = new char[1024];
	memset(responseHeader, 0, sizeof(responseHeader));
	char c;
	int i = 0;
	while (1)
	{
		recv(sock, &c, 1, 0);
		responseHeader[i++] = c;
		if (i >= 4)
		{
			if (responseHeader[i-4]=='\r' && responseHeader[i-3]=='\n' &&
				responseHeader[i-2]=='\r' && responseHeader[i-1]=='\n')
			{
				break;
			}
		}
	}
	responseHeader[i] = '\0';

	cout << "���յĻ�ӦͷΪ��" << endl << responseHeader << endl;
}

// ���ý��ջ��͵��ʱ��
void CVodStreamCache::SetTimeOut(int nTime, int type/* =0 */)
{
	if (err) return;

	if (type == 0)
	{
		type = SO_RCVTIMEO;
	}
	else
	{
		type = SO_SNDTIMEO;
	}

	if (setsockopt(sock, SOL_SOCKET, type, (char*)&nTime, sizeof(nTime)) != 0)
	{
		cout << "����ʧ�ܣ�" << endl;
		err = TRUE;
		return;
	}
}

// ��ʼ��Cache����ȡ�ļ�����
void CVodStreamCache::InitCache()
{
	if (err) return;

	char *pos;
	char strResponse[] = "Content-Length";
	pos = strstr(responseHeader, strResponse);
	pos += (strlen(strResponse) + 2);

	long len = 0;
	while (*pos != '\r')
	{
		len = len * 10 + *pos - 48;
		++pos;
	}

	cache.setTotalLen(len);
	cache.setRecvLen(0);
}