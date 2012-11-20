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
	// 出错了
	if (err)
	{
		return NULL;
	}

	InitSocketLib();	// 初始化Socket函数库
	ParseURL(path);	// 解析URL地址
	Socket();	// 创建SOCKET对象
	Connect(pHost);	// 建立连接

	FormatRequestHeader(pHost, pGet);	// 格式化请求头
	SendRequest(requestHeader);	// 发送请求头
	GetResponseHeader();	// 获取完整的返回头
	SetTimeOut(10000);	// 设置接收或发送的最长时间
	InitCache();	// 初始化Cache，获取文件长度

	// 创建指定大小的文件
	cacheFile.open("E:\\zz.txt", ios::binary|ios::out);
	for (int i=0; i<cache.getTotalLen(); ++i)
	{
		cacheFile.put('\0');
	}
	cacheFile.close();

	if (err)
	{
		cout << "创建失败！" << endl;
	}
	else
	{
		cout << "创建成功！" << endl;
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
	FormatRequestHeader(pHost, pGet, offset, offset+len-1);	// 格式化请求头
	SendRequest(requestHeader);	// 发送请求头
	//GetResponseHeader();	// 获取完整的返回头
	//SetTimeOut(5000);	// 设置接收或发送的最长时间

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

// 初始化Socket函数库
void CVodStreamCache::InitSocketLib()
{
	if (err) return;

	WORD wVersion;
	WSADATA wsaData;

	wVersion = MAKEWORD(2, 0);
	if (WSAStartup(wVersion, &wsaData) != 0)
	{
		cout << "无法装载Socket库！" << endl;
		err = FALSE;
		return;
	}
	if (LOBYTE(wsaData.wVersion) != 2)
	{
		cout << "无法找到合适的Socket库！" << endl;
		err = FALSE;
		WSACleanup();
		return;
	}
}

// 解析URL地址
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

// 根据URL输出HTTP请求头
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

	// 方法，请求的路径，版本
	strcat(requestHeader, "GET ");
	strcat(requestHeader, pGet);
	strcat(requestHeader, " HTTP/1.1");
	strcat(requestHeader, "\r\n");

	// 主机
	strcat(requestHeader, "Host: ");
	strcat(requestHeader, pHost);
	strcat(requestHeader, "\r\n");

	/*if(pReferer != NULL)
	{
		strcat(requestHeader, "Referer:");
		strcat(requestHeader, pReferer);
		strcat(requestHeader, "\r\n");		
	}*/

	// 接收的数据类型
	strcat(requestHeader, "Accept:*/*");
	strcat(requestHeader, "\r\n");

	// 浏览器类型
	strcat(requestHeader, "User-Agent:Mozilla/4.0 (compatible; MSIE 5.00; Windows 98)");
	strcat(requestHeader, "\r\n");

	// 连接设置，保持
	strcat(requestHeader, "Connection:Keep-Alive");
	strcat(requestHeader, "\r\n");

	// Cookie
	/*if(pCookie != NULL)
	{
		strcat(requestHeader, "Set Cookie:0");
		strcat(requestHeader, pCookie);
		strcat(requestHeader, "\r\n");
	}*/

	// 请求的数据起始字节位置(断点续传的关键)
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

	// 最后一行：空行
	strcat(requestHeader, "\r\n");

	cout << endl << "发送的请求头为：" << endl << requestHeader;
}

// 创建SOCKET对象
void CVodStreamCache::Socket()
{
	if (err) return;

	struct protoent *ppe; 
	ppe=getprotobyname("tcp"); 

	sock = socket(PF_INET, SOCK_STREAM, ppe->p_proto);
	if (sock == INVALID_SOCKET)
	{
		cout << "创建Socket对象失败！" << endl;
		err = TRUE;
		return;
	}
}

// 建立连接
void CVodStreamCache::Connect(char* pHost, int port/* =80 */)
{
	if (err) return;

	// 根据主机名获取IP地址
	hostent* pHostEnt = gethostbyname(pHost);
	if (pHostEnt == NULL)
	{
		cout << "获取IP地址失败！" << endl;
		err = TRUE;
		return;
	}

	// 连接
	struct in_addr ip_addr;
	memcpy(&ip_addr, pHostEnt->h_addr_list[0], 4);	// h_addr_list[0]里4个字节,每个字节8位

	struct sockaddr_in destaddr;
	memset((void*)&destaddr, 0, sizeof(destaddr));
	destaddr.sin_family = AF_INET;
	destaddr.sin_port = htons(port);
	destaddr.sin_addr = ip_addr;

	if (0 != connect(sock, (struct sockaddr*)&destaddr, sizeof(destaddr)))
	{
		cout << "连接失败！" << endl;
		err = TRUE;
		return;
	}
}

// 发送请求头
void CVodStreamCache::SendRequest(const char* requestHeader)
{
	if (err) return;

	if (SOCKET_ERROR == send(sock, requestHeader, strlen(requestHeader), 0))
	{
		cout << "请求头发送失败！" << endl;
		err = TRUE;
		return;
	}
}

// 获取完整的返回头
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

	cout << "接收的回应头为：" << endl << responseHeader << endl;
}

// 设置接收或发送的最长时间
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
		cout << "设置失败！" << endl;
		err = TRUE;
		return;
	}
}

// 初始化Cache，获取文件长度
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