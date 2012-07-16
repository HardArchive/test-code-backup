#pragma once
/******************************************************************************* 
* 1、 文件名称： struct_define
* 2、 版　　本： Version *.*
* 3、 描    述： 自定义数据类型头文件
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-4-20 16:46:12
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 

#define PACKET_FILTER "ip and udp"      //"ether proto \\dns"
//域名存放结构
typedef struct _DNS_DOMAIN
{
	int iDomainID;             //域名ID
	char szDomain[MAX_PATH];   //域名
}DNS_NOMAIN, *PDNS_NOMAIN;

//IP存放结构
typedef struct _DNS_IP
{
	int iIPID;            //IP ID
	char szIP[16];        //IP地址
}DNS_IP, *PDNS_IP;

//域名IP对应关系
typedef struct _DNS_DOMAIN_VS_IP
{
	int szDomainID[16];          //域名ID数组 数组的毎一元素为域名存放结构ID
	int szIPID[16];              //IP ID数组 数组的毎一元素为IP存放结构ID
	//可在这里添加认证IP包
}DNS_DOMAIN_VS_IP, *PDNS_DOMAIN_VS_IP;


typedef struct DNS_VERIFY
{
	PDNS_DOMAIN_VS_IP pDNSDomainVsIP; //域名IP对应关系
	int iVerifyStatus;                //验证状态， 0-未验证 ，1-验证通过，2-验证失败
}DNSVERIFY, *PDNSVERIFY;


//查询和回应的数据位置定义  记录类型
#define T_A		    1		// 主机地址 A记录
#define T_NS		2		// 认证服务器
#define T_CNAME		5		// 别名 CNAME记录
#define T_SOA		6		// 认证区域起始
#define T_PTR		12		// 域名指针
#define T_MX		15		// 邮件路由信息

//DNS 头结构
typedef struct _DNS_HEADER
{
	WORD    Xid;                    // 标识
	///////////////////////////报文首部的标志字段16位/////////////////////////////////
	BYTE    RecursionDesired : 1;   // 递归查询模式 
	BYTE    Truncation : 1;         // 完整封包信息｛为1时表示可截断的，使用UDP时，表示总应答长度超过512字节时，只返回前512字节｝
	BYTE    Authoritative : 1;      // 认证回应 1
	BYTE    Opcode : 4;             // 操作码{通常值为0（标准查询），其他值为1（反向查询）和2（服务器状态请求）}
	BYTE    IsResponse : 1;         // 0查询/1回应标志

	BYTE    ResponseCode : 4;       // 回应码 0没有差错、3名字差错
	BYTE    CheckingDisabled : 1;   // 检查无效
	BYTE    AuthenticatedData : 1;  // 认证数据
	BYTE    Reserved : 1;           // 预留
	BYTE    RecursionAvailable : 1; // 能否进行递归查询
	///////////////////////////报文首部的标志字段16位/////////////////////////////////

	WORD    QuestionCount;          // 问题数目
	WORD    AnswerCount;            // 回答数目
	WORD    NameServerCount;        // 权威回答数目
	WORD    AdditionalCount;        // 附加数目
}DNS_HEADER, *PDNS_HEADER;

//查询问题结构 的后面部分 不包括域名
typedef struct _DNS_WIRE_QUESTION
{
	WORD    QuestionType;  // 查询类型
	WORD    QuestionClass; // 查询类型
}DNS_WIRE_QUESTION, *PDNS_WIRE_QUESTION;

//查询回应结构-资源记录格式结构的（类型、类、生存时间、资源数据长度）
#pragma pack(push, 1)
typedef struct _DNS_WIRE_RECORD
{
	WORD    RecordType;  // 类型
	WORD    RecordClass; // 类
	DWORD   TimeToLive;  // 在DNS服务器的存活时间
	WORD    DataLength;  // 数据的长度
}DNS_WIRE_RECORD, *PDNS_WIRE_RECORD;
#pragma pack(pop)

//查询回应结构_资源记录格式结构
typedef struct _RES_RECORD
{
	BYTE *ResName;                      // 资源名称 域名
	PDNS_WIRE_RECORD ResRecord;         // 资源数据
	BYTE *ResData;                      // 查询结果 资源数据
}RES_RECORD, *PRES_RECORD;

//查询结构
typedef struct _QUERY
{
	BYTE *QueryName;                        // 资源名称
	PDNS_WIRE_QUESTION QueryQuesTion;       // 问题
} QUERY, *PQUERY;

