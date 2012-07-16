// DNSQuery1.h: interface for the CDNSQuery class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DNSQUERY1_H__64970EE1_BDC0_40AF_9A7F_7B674079FD36__INCLUDED_)
#define AFX_DNSQUERY1_H__64970EE1_BDC0_40AF_9A7F_7B674079FD36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//查询和回应的数据位置定义
#define T_A		    1		// 主机地址
#define T_NS		2		// 认证服务器
#define T_CNAME		5		// 别名
#define T_SOA		6		// 认证区域起始
#define T_PTR		12		// 域名指针
#define T_MX		15		// 邮件路由信息

//DNS 头结构
struct DNS_HEADER1
{
	unsigned	short id;		    // 标识

	///////////////////////////报文首部的标志字段16位/////////////////////////////////
	unsigned	char rd     :1;		// 递归查询模式 
	unsigned	char tc     :1;		// 完整封包信息｛为1时表示可截断的，使用UDP时，表示总应答长度超过512字节时，只返回前512字节｝
	unsigned	char aa     :1;		// 认证回应 1
	unsigned	char opcode :4;	    // 操作码{通常值为0（标准查询），其他值为1（反向查询）和2（服务器状态请求）}
	unsigned	char qr     :1;		// 0查询/1回应标志
	
	unsigned	char rcode  :4;	    // 回应码 0没有差错、3名字差错
	unsigned	char cd     :1;	    // 检查无效
	unsigned	char ad     :1;	    // 认证数据
	unsigned	char z      :1;		// 预留
	unsigned	char ra     :1;		// 能否进行递归查询
	///////////////////////////报文首部的标志字段16位/////////////////////////////////
	

	unsigned    short q_count;	    // 问题数目
	unsigned	short ans_count;	// 回答数目
	unsigned	short auth_count;	// 权威回答数目
	unsigned	short add_count;	// 附加数目
};

//typedef struct _DNS_HEADER
//{
//	WORD    Xid;                    // 标识
//	///////////////////////////报文首部的标志字段16位/////////////////////////////////
//	BYTE    RecursionDesired : 1;   // 递归查询模式 
//	BYTE    Truncation : 1;         // 完整封包信息｛为1时表示可截断的，使用UDP时，表示总应答长度超过512字节时，只返回前512字节｝
//	BYTE    Authoritative : 1;      // 认证回应 1
//	BYTE    Opcode : 4;             // 操作码{通常值为0（标准查询），其他值为1（反向查询）和2（服务器状态请求）}
//	BYTE    IsResponse : 1;         // 0查询/1回应标志
//
//	BYTE    ResponseCode : 4;       // 回应码 0没有差错、3名字差错
//	BYTE    CheckingDisabled : 1;   // 检查无效
//	BYTE    AuthenticatedData : 1;  // 认证数据
//	BYTE    Reserved : 1;           // 预留
//	BYTE    RecursionAvailable : 1; // 能否进行递归查询
//
//	WORD    QuestionCount;          // 问题数目
//	WORD    AnswerCount;            // 回答数目
//	WORD    NameServerCount;        // 权威回答数目
//	WORD    AdditionalCount;        // 附加数目
//}
//DNS_HEADER, *PDNS_HEADER;

//查询问题结构
struct QUESTION
{
	unsigned short qtype;   // 查询的资源类型
	unsigned short qclass;  // 查询网络类型
};


//查询回应结构
#pragma pack(push, 1)
struct  R_DATA
{
	unsigned short type;    // 查询回应类型
	unsigned short _class;  // 查询回应的网络类型
	unsigned int   ttl;     // 在DNS服务器的存活时间
	unsigned short data_len;// 数据的长度
};
#pragma pack(pop)

//资源记录内容指针
struct RES_RECORD
{
	unsigned char  *name;      // 资源名称
	struct R_DATA  *resource;  // 资源数据
	unsigned char  *rdata;     // 查询结果
};

//查询结构
typedef struct
{
	unsigned char *name;       // 资源名称
	QUESTION      *ques;       // 问题
} QUERY;


class CDNSQuery  
{
public:
	CDNSQuery();
	virtual ~CDNSQuery();

	CString DNSQuery(unsigned char *host, char *dnsserver);
	unsigned char* ReadName(unsigned char* reader,unsigned char* buffer,int* count);
	void RetrieveDnsServersFromRegistry();
	void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host);

	int GetDNSServer(char* dnsserver)
	{
		int i = 0;
		for(i = 0; i < 10; i++)
		{
			if( m_DnsServers[i][0] == '\0' )
				break;
			strcpy(&dnsserver[i*100], m_DnsServers[i]);
		}

		return i;
	}
private:
	//List of DNS Servers registered on the system
	char m_DnsServers[10][100];

};

#endif // !defined(AFX_DNSQUERY1_H__64970EE1_BDC0_40AF_9A7F_7B674079FD36__INCLUDED_)
