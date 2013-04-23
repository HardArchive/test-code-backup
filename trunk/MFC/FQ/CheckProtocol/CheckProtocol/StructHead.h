#pragma  once 
#include <string>
#include <stdio.h>
using namespace std;


#define  CAP_NORMAL_MODE                  0             //ץ��ģʽ
// ������ݰ���С 1500+sizeof(ethernet_header)+sizeof(pppoe_header)+2*sizeof(vlan_header)
#define MAX_PACKET_SIZE                9168
#define PCAP_ERRBUF_SIZE                256
#define PACK_SEPARATE_NUMBER              3             //����ְ��ĸ���

#define PACK_SEPARATE_NO                  0             //���ְ�
#define PACK_SEPARATE_LOGIN               1             //�ְ���¼��־
#define PACK_SEPARATE_SEND                2             //�ְ����ͱ�־

#define PACK_FIND_USERNAME_KEY            1             //���ҵ��û���
#define PACK_FIND_PASSWORD_KEY            2             //���ҵ�����
#define PACK_FIND_SENDINFO_KEY			  4             //���ҵ�������Ϣ

#pragma region ������̫��Э������
// PPPoE��������
const u_short PPPoE_Protocol_Type = 0x6488;
// VLan��������
const u_short VLan_Protocol_Type = 0x0081;
// IP���ݰ�����
const u_short IP_Portocol_Type = 0x0008;
// IP���ݰ�����
const u_short ARP_Portocol_Type = 0x0608;
#pragma endregion

#pragma region ����Э��ͷ������
// ��̫��ͷ��
const int EtherHeaderLength = 14;
// PPPoEͷ��
const int PPPoEHeaderLength = 8;
// Vlanͷ��
const int VLanHeaderLength = 4;
#pragma endregion

#define DATA_DIRECT_UP 0		// ��������
#define DATA_DIRECT_DOWN 1		// ��������
#define DATA_DIRECT_LOCAL -2	// ����ͨ��
#define DATA_DIRECT_UNKNOW -1	// δ֪

// ������������
#define PACKE_TYPE_TCP      2   
#define PACKE_TYPE_UDP      3
#define PACKE_TYPE_PPOE     4
#define PACKE_TYEP_VALAN    5

// �����޴���
#define DECODE_SUCCESS		0
// �����޴���
#define DECODE_DEFAULT		1
// ��������
#define DECODE_ERROR		2
#pragma pack(1)
///////////////////////////////////////////////////ץ���ṹ��///////////////////////////////////////////////////////////////////////
typedef 
// IP�ỰΨһ��ʶ
struct ip_port_key
{
	// ԴIP
	in_addr sip;
	// Ŀ��IP
	in_addr dip;
	// Դ�˿�
	u_short sport;
	// Ŀ�Ķ˿�
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
// ͷ����������
struct tagHeaderDataType
{
	// PPPoE���ݡ�1=PPPoE�ỰID��2=PPPoE�ʺţ�4=PPPoE IP V4��8=PPPoE IP V6��0x8000=��չPPPoEģʽ����Ҫ���⴦��
	u_short PPPoE;
	// VLAN������0=��VLAN��1=һ��VLAN��2=˫��VLAN��0x8000=��չVLANģʽ����Ҫ���⴦��
	u_short Vlan;
}tagHeaderDataType;
typedef
// ����ƫ�Ƶ�ַ
struct tagHeaderOffset
{
	// ��̫��ͷƫ��
	u_short Eth;
	// PPPoEͷƫ��
	u_short PPPoE;
	// ���VLANƫ��
	u_short VLan1;
	// �ڲ�Vlanƫ��
	u_short Vlan2;
	// IPͷƫ��
	u_short IP;
	// TCPƫ�ƻ�UDPƫ��
	u_short IPExt;
	// ����ƫ����
	u_short DataIndex;
	//  ���ݷ���-2=����ͨ��[������]��-1=δ֪��0=����[�ɱ��ط���������������]��1=����[�ɹ������򱾵ص���������]
	u_short Direction;

	// ���캯��
	tagHeaderOffset(): Eth(0), PPPoE(0), VLan1(0), Vlan2(0), IP(0), IPExt(0), DataIndex(0), Direction(DATA_DIRECT_UNKNOW)
	{

	}
}tagHeaderOffset;

typedef
// ͷ����¼��Ϣ
struct tagHeaderLoginInfo
{
	// PPPoE�ʺ�
	char PPPoEKey[32];
	// PPPoE����
	char PPPoEPass[32];
	// PPPoE�ỰID
	u_int PPPoEId;

	// Vlan��Ϣ
	u_short Vlan1;	// ���Vlan
	u_short Vlan2;	// �ڲ�Vlan

	// ��������
	tagHeaderDataType DataType;

	tagHeaderLoginInfo()
	{
		memset(this, 0, sizeof(*this));
	}
}tagHeaderLoginInfo;

typedef
//���ݽ����������
struct tagDecodeInput
{
	// ���ݰ���ʼָ��
	u_char *Init;
	// ��ǰ��ָ��[һ���ǽ�����������ͷ����]
	u_char *Current;
	// ����ͷ����
	int HeadLen;
	// �ӵ�ǰָ�뿪ʼ�������ĳ���
	int DataLen;
	// ���ݰ��ܳ���
	int Len;
	// ץ��ʱ��
	time_t CapTime;

	// ����ͷƫ������
	tagHeaderOffset Offset;

	// ��¼��Ϣ
	tagHeaderLoginInfo Info;

	// ��ȡSessionKey
	ip_port_key SessionKey;

	// �������ݸ�λ
	inline void Reset(const time_t tmCapTime, const int nCapLen, u_char* pBuff)
	{
		memset(this, 0, sizeof(*this));

		// ���������λ
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
	// Ŀ��MAC��ַ
	u_char DestMac[6];
	// ԴMAC��ַ
	u_char SrcMac[6];
	// Э������
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
	// ���ͷ��ֽ�������
#define TH_FIN	0x01
	// ���ͬ��(��������)
#define TH_SYN	0x02
	// ���Ӹ�λ
#define TH_RST	0x04
	// ���շ�Ӧ�������������ύ��Ӧ�ó���
#define TH_PUSH	0x08
	// ȷ�����(th_ack)��Ч
#define TH_ACK	0x10
	// ��������ƫ����(th_urp)��Ч
#define TH_URG	0x20
	// window 
	u_short	win;
	// checksum 
	u_short	checksum;
	// urgent pointer 
	u_short	urp;

	// �Ƿ����ݽ�����
	inline bool IsFin() const
	{
		return (TH_FIN == (this->flags & TH_FIN));
	}

	// �Ƿ�����ͬ����
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

/////////////////////////������Ϣ�ṹ��///////////////////////////////////////////////////////////////////////

typedef struct INTERCEPTEDINFO                                             //Xt:���ݼ���ƥ����Ϣ
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

//Xt:���ݽػ����Ϣ
typedef struct PACK_INFO                                                 
{
	u_char *pPackBuff;          // ���ݰ���Ϣ
	u_char  DestMac[6];			// Ŀ��MAC��ַ
	u_char  SrcMac [6];			// ԴMAC��ַ
	in_addr	ip_src;             // ԭIP��ַ
	in_addr	ip_dst;             // Ŀ��IP��ַ
	u_char *pPackContentBuff;   // ����������Ϣ
	char    PackDirection[4];   // ���ķ���
	int     PackLength;         // �����ܳ���
	int     PackHeadLength;     // ��ͷ�ݳ���
	int     PackContentLength;  // �����ݳ���
	int     PackType;           // ��ʲô��(TCP or UDP)
	u_short DestPort;           // Ŀ�Ķ˿ں�
	u_short SrcPort;            // ԭ�˿ں�
	time_t  PackTime;           // ץ��ʱ��

	INTERCEPTEDINFO  interceptedInfo;               
	inline void Reset(const time_t tmCapTime, const int nCapLen, u_char* pBuff)
	{
		memset(this, 0, sizeof(*this));

		// ���������λ
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

//Xt:HTTP��¼�ؼ����ļ�
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

//Xt:���ݰ������ؼ����ļ���Ϣ    
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

///////////////////////������ṹ//////////////////////////////////////
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