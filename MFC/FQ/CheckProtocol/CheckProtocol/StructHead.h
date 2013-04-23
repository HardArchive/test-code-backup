#pragma  once 
#include <string>
#include <stdio.h>
using namespace std;


#define  CAP_NORMAL_MODE                  0             //抓包模式
// 最大数据包大小 1500+sizeof(ethernet_header)+sizeof(pppoe_header)+2*sizeof(vlan_header)
#define MAX_PACKET_SIZE                9168
#define PCAP_ERRBUF_SIZE                256
#define PACK_SEPARATE_NUMBER              3             //捕获分包的个数

#define PACK_SEPARATE_NO                  0             //不分包
#define PACK_SEPARATE_LOGIN               1             //分包登录标志
#define PACK_SEPARATE_SEND                2             //分包发送标志

#define PACK_FIND_USERNAME_KEY            1             //查找到用户名
#define PACK_FIND_PASSWORD_KEY            2             //查找到密码
#define PACK_FIND_SENDINFO_KEY			  4             //查找到发送信息

#pragma region 常用以太网协议类型
// PPPoE数据类型
const u_short PPPoE_Protocol_Type = 0x6488;
// VLan数据类型
const u_short VLan_Protocol_Type = 0x0081;
// IP数据包类型
const u_short IP_Portocol_Type = 0x0008;
// IP数据包类型
const u_short ARP_Portocol_Type = 0x0608;
#pragma endregion

#pragma region 常用协议头部长度
// 以太网头长
const int EtherHeaderLength = 14;
// PPPoE头长
const int PPPoEHeaderLength = 8;
// Vlan头长
const int VLanHeaderLength = 4;
#pragma endregion

#define DATA_DIRECT_UP 0		// 上行数据
#define DATA_DIRECT_DOWN 1		// 下行数据
#define DATA_DIRECT_LOCAL -2	// 本地通信
#define DATA_DIRECT_UNKNOW -1	// 未知

// 解析包的类型
#define PACKE_TYPE_TCP      2   
#define PACKE_TYPE_UDP      3
#define PACKE_TYPE_PPOE     4
#define PACKE_TYEP_VALAN    5

// 解析无错误
#define DECODE_SUCCESS		0
// 解析无错误
#define DECODE_DEFAULT		1
// 解析出错
#define DECODE_ERROR		2
#pragma pack(1)
///////////////////////////////////////////////////抓包结构体///////////////////////////////////////////////////////////////////////
typedef 
// IP会话唯一标识
struct ip_port_key
{
	// 源IP
	in_addr sip;
	// 目标IP
	in_addr dip;
	// 源端口
	u_short sport;
	// 目的端口
	u_short dport;

	bool operator < (const ip_port_key& key) const
	{
		return 0 < memcmp(this, &key, sizeof(*this));
	}

	ip_port_key(){}
	ip_port_key(const in_addr ipSrc, const in_addr ipDst, const u_short portSrc, const u_short portDst)
	{
		sip = ipSrc;
		dip = ipDst;
		sport = portSrc;
		dport = portDst;
	}

	inline void Reset(const in_addr ipSrc, const in_addr ipDst, const u_short portSrc, const u_short portDst)
	{
		sip = ipSrc;
		dip = ipDst;
		sport = portSrc;
		dport = portDst;
	}
}ip_port_key;

typedef
// 头部数据类型
struct tagHeaderDataType
{
	// PPPoE数据。1=PPPoE会话ID；2=PPPoE帐号；4=PPPoE IP V4；8=PPPoE IP V6；0x8000=扩展PPPoE模式，需要特殊处理
	u_short PPPoE;
	// VLAN层数。0=无VLAN；1=一层VLAN；2=双层VLAN；0x8000=扩展VLAN模式，需要特殊处理。
	u_short Vlan;
}tagHeaderDataType;
typedef
// 数据偏移地址
struct tagHeaderOffset
{
	// 以太网头偏移
	u_short Eth;
	// PPPoE头偏移
	u_short PPPoE;
	// 外层VLAN偏移
	u_short VLan1;
	// 内层Vlan偏移
	u_short Vlan2;
	// IP头偏移
	u_short IP;
	// TCP偏移或UDP偏移
	u_short IPExt;
	// 数据偏移量
	u_short DataIndex;
	//  数据方向。-2=本地通信[局域网]；-1=未知；0=上行[由本地发向公网的数据流向]；1=下行[由公网发向本地的数据流向]
	u_short Direction;

	// 构造函数
	tagHeaderOffset(): Eth(0), PPPoE(0), VLan1(0), Vlan2(0), IP(0), IPExt(0), DataIndex(0), Direction(DATA_DIRECT_UNKNOW)
	{

	}
}tagHeaderOffset;

typedef
// 头部登录信息
struct tagHeaderLoginInfo
{
	// PPPoE帐号
	char PPPoEKey[32];
	// PPPoE密码
	char PPPoEPass[32];
	// PPPoE会话ID
	u_int PPPoEId;

	// Vlan信息
	u_short Vlan1;	// 外层Vlan
	u_short Vlan2;	// 内层Vlan

	// 数据类型
	tagHeaderDataType DataType;

	tagHeaderLoginInfo()
	{
		memset(this, 0, sizeof(*this));
	}
}tagHeaderLoginInfo;

typedef
//数据解析输入参数
struct tagDecodeInput
{
	// 数据包初始指针
	u_char *Init;
	// 当前的指针[一般是解析到了数据头部分]
	u_char *Current;
	// 数据头长度
	int HeadLen;
	// 从当前指针开始到结束的长度
	int DataLen;
	// 数据包总长度
	int Len;
	// 抓包时间
	time_t CapTime;

	// 数据头偏移数据
	tagHeaderOffset Offset;

	// 登录信息
	tagHeaderLoginInfo Info;

	// 获取SessionKey
	ip_port_key SessionKey;

	// 基本数据复位
	inline void Reset(const time_t tmCapTime, const int nCapLen, u_char* pBuff)
	{
		memset(this, 0, sizeof(*this));

		// 传入参数复位
		this->Init = pBuff;
		this->Current = pBuff;
		this->Len = nCapLen;
		this->DataLen = nCapLen;
		this->CapTime = tmCapTime;
	}
}tagDecodeInput;

/////////////////////////////////////////////////////////////////////////////////////
typedef
struct ethernet_header
{
	// 目标MAC地址
	u_char DestMac[6];
	// 源MAC地址
	u_char SrcMac[6];
	// 协议类型
	u_short Type;
} ethernet_header;

typedef
struct ip_header{
	// header length 
	u_char	ip_hl:4,
		// version 
ip_v:4;
	// Type of service 
	u_char	tos;
	// Total length 
	u_short tlen;
	// Identification
	u_short id;
	// Flags (3 bits) + Fragment offset (13 bits)
	u_short flags_off;
	// dont fragment flag
#define IP_DF 0x4000
	// more fragments flag
#define IP_MF 0x2000
	// mask for fragmenting bits
#define IP_OFFMASK 0x1fff
	// Time to live
	u_char	ttl;
	// Protocol
	u_char	proto;
	// Header checksum
	u_short checksum;
	// Source address [source ip]
	in_addr	ip_src;
	// Destination address [destination ip]
	in_addr	ip_dst;
	// Option + Padding
	//u_int	op_pad;
}ip_header;

typedef 
// TcpHeader
struct tcp_header
{
	// source port 
	u_short	sport;
	// destination port 
	u_short	dport;
	// sequence number 
	u_long	seq;
	// acknowledgement number 
	u_long	ack;
	// (unused)
	u_char  x2:4,
		// data offset
off:4;
	// flag of TCP 
	u_char	flags;
	// 发送方字节流结束
#define TH_FIN	0x01
	// 序号同步(建立连接)
#define TH_SYN	0x02
	// 连接复位
#define TH_RST	0x04
	// 接收方应该立即将数据提交给应用程序
#define TH_PUSH	0x08
	// 确认序号(th_ack)有效
#define TH_ACK	0x10
	// 紧急数据偏移量(th_urp)有效
#define TH_URG	0x20
	// window 
	u_short	win;
	// checksum 
	u_short	checksum;
	// urgent pointer 
	u_short	urp;

	// 是否数据结束包
	inline bool IsFin() const
	{
		return (TH_FIN == (this->flags & TH_FIN));
	}

	// 是否连接同步包
	inline bool IsSyn() const
	{
		return (TH_SYN == (this->flags & TH_SYN));
	}
} tcp_header;

typedef 
// UDP header
struct udp_header
{
	// Source port
	u_short sport;
	// Destination port
	u_short dport;
	// Datagram length
	u_short len;
	// Checksum
	u_short checksum;
}udp_header;

/////////////////////////数据信息结构体///////////////////////////////////////////////////////////////////////

typedef struct INTERCEPTEDINFO                                             //Xt:数据检索匹配信息
{
	char Item[20];
	char UserName[60];
	char Password[20];
	char SendContent[1024];

	INTERCEPTEDINFO()
	{
		memset(Item,0,sizeof(Item));
		memset(UserName,0,sizeof(UserName));
		memset(Password,0,sizeof(Password));
		memset(SendContent,0,sizeof(SendContent));
	}
}InterceptedInfo;

typedef struct RADE_FILE
{
	char Item[20];
	char UserName[60];
	char Password[20];
	char SendContent[1024];
	char SrcMac [20];
	char DestMac[20];
	char PackDirection[4];
	char DestPort[10];
	char SrcPort[10];
	char ip_src[20];
	char ip_dst[20];

	RADE_FILE()
	{
		memset(Item,0,sizeof(Item));
		memset(UserName,0,sizeof(UserName));
		memset(Password,0,sizeof(Password));
		memset(SendContent,0,sizeof(SendContent));
		memset(SrcMac,0,sizeof(SrcMac));
		memset(DestMac,0,sizeof(DestMac));
		memset(PackDirection,0,sizeof(PackDirection));
		memset(DestPort,0,sizeof(DestPort));
		memset(SrcPort,0,sizeof(SrcPort));
		memset(ip_src,0,sizeof(ip_src));
		memset(ip_dst,0,sizeof(ip_dst));
	}
}Read_File;

//Xt:数据截获包信息
typedef struct PACK_INFO                                                 
{
	u_char *pPackBuff;          // 数据包信息
	u_char  DestMac[6];			// 目标MAC地址
	u_char  SrcMac [6];			// 源MAC地址
	in_addr	ip_src;             // 原IP地址
	in_addr	ip_dst;             // 目的IP地址
	u_char *pPackContentBuff;   // 包的内容信息
	char    PackDirection[4];   // 包的方向
	int     PackLength;         // 包的总长度
	int     PackHeadLength;     // 包头容长度
	int     PackContentLength;  // 包内容长度
	int     PackType;           // 是什么包(TCP or UDP)
	u_short DestPort;           // 目的端口号
	u_short SrcPort;            // 原端口号
	time_t  PackTime;           // 抓包时间

	INTERCEPTEDINFO  interceptedInfo;               
	inline void Reset(const time_t tmCapTime, const int nCapLen, u_char* pBuff)
	{
		memset(this, 0, sizeof(*this));

		// 传入参数复位
		this->pPackBuff = pBuff;
		this->pPackContentBuff = NULL;
		this->PackLength = nCapLen;
		this->PackTime = tmCapTime;
		this->PackContentLength = 0;

		memset(this->DestMac,0,sizeof(this->DestMac));
		memset(this->SrcMac,0,sizeof(this->SrcMac));
		memset(&(this->interceptedInfo),0,sizeof(this->interceptedInfo));
		memset(this->PackDirection,0,sizeof(this->PackDirection));
		pPackContentBuff = NULL;
		
	}

}pack_info;

//Xt:HTTP登录关键字文件
typedef struct LoginFileContentInfo					                      
{ 

	char Item[20];
	char URL[512];
	char UserNameMark[30];
	char PassWordMark[30];
	char ButtonName[20];
	char UserName[30];
	char Password[30];
	char SendContent[1024];
	int  OperateItem;
	char SendKey[30];
	bool bSelect;
	LoginFileContentInfo()
	{
		memset(Item,0,sizeof(Item));
		memset(URL, 0,sizeof(URL));
		memset(UserNameMark,0,sizeof(UserNameMark));
		memset(PassWordMark,0,sizeof(PassWordMark));
		memset(ButtonName,0,sizeof(ButtonName));
		memset(UserName,0,sizeof(UserName));
		memset(Password,0,sizeof(Password));
		memset(SendKey,0,sizeof(SendKey));
		memset(SendContent,0,sizeof(SendContent));
		OperateItem = 0;
		bSelect = 0;
	}

}LoginFileContentInfo;

//Xt:数据包检索关键字文件信息    
typedef struct PACKRULEFILE                                                                  
{
	char Item[20];
	char PackHead[100];
	char Host[256];
	char Referer[256];
	char UserNameKey[30];
	char UserEndNameKey[20];
	char PassWordKey[30];
	char EndPassWordKey[20];
	char SendContentKey[30];
	char EndSengContentKey[20];
	char CodeType[10];
	bool bSelect;
	PACKRULEFILE()
	{
		memset(Item,0,sizeof(Item));
		memset(PackHead, 0,sizeof(PackHead));
		memset(Host,0,sizeof(Host));
		memset(Referer,0,sizeof(Referer));
		memset(UserNameKey,0,sizeof(UserNameKey));
		memset(UserEndNameKey,0,sizeof(UserEndNameKey));
		memset(PassWordKey,0,sizeof(PassWordKey));
		memset(EndPassWordKey,0,sizeof(EndPassWordKey));
		memset(SendContentKey,0,sizeof(SendContentKey));
		memset(EndSengContentKey,0,sizeof(EndSengContentKey));
		memset(CodeType,0,sizeof(CodeType));
		bSelect = 0;
	}
}PackRuleFile;

///////////////////////包规则结构//////////////////////////////////////
typedef struct _FINDINFO
{
	char userName[100];
	char Password[20];
	char SendContent[200];
	char Type[7];

	_FINDINFO()
	{
		memset(userName,0,sizeof(userName));
		memset(Password,0,sizeof(Password));
		memset(SendContent,0,sizeof(SendContent));
		memcpy(Type,"G-",sizeof("G-"));
	}
}FindInfo;

typedef struct _FINDPACKRULE
{
	char packHead[30];
	char packHost[30];
	char packReferer[30];
	char userNameKeyBegin[20];
	char userNameKeyEnd[20];
	char PasswordBegin[20];
	char PasswordEnd[20];
	char SendContentBegin[20];
	char SendContentEnd[20];

	_FINDPACKRULE()
	{
		memset(packHead,0,sizeof(packHead));
		memset(packHost,0,sizeof(packHost));
		memset(packReferer,0,sizeof(packReferer));
		memset(userNameKeyBegin,0,sizeof(userNameKeyBegin));
		memset(userNameKeyEnd,0,sizeof(userNameKeyEnd));
		memset(PasswordBegin,0,sizeof(PasswordBegin));
		memset(PasswordEnd,0,sizeof(PasswordEnd));
		memset(SendContentBegin,0,sizeof(SendContentBegin));
		memset(SendContentEnd,0,sizeof(SendContentEnd));
	}
}FindPackRule;

#pragma pack (1)
typedef struct DATAPACK
{
	char	         cType;
	unsigned int     nPackLen;
	unsigned int     nID;            
	unsigned long    nContentLen;               
	unsigned long    nFileLocation;              
}DataPackHead,*pDataPackHead;