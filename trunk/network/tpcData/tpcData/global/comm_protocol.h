#ifndef _COMM_PROTOCOL_h_
#define _COMM_PROTOCOL_h_
/******************************************************************************* 
* 1、 Class      ： comm_protocol
* 2、 Version    ： 1.00
* 3、 Description： CultureBJ通讯协议
* 4、 Author     ： 
* 5、 Created    ： 2012-11-20 17:14:30
* 6、 History    ： 1.00					
* 7、 Remark     ： 
********************************************************************************/ 

//常量
#define PACKAGE_MARK                     "RS01"        //数据包标志
#define PACKAGE_MARK_LEN                 4             //标记长度
#define RECVICE_DATA_MAX_LEN             1024         //接收数据最大长度1K 按级别来分的 分别*[(1-3)*1、(4)*256、(5)*1024]
//数据包类型
#define TYPE_OK        0x01     //回复确认，表示已经接收或处理成功
#define TYPE_ERROR     0x02     //回复确认，表示接收或处理失败

#define TIMEOUT_MINUTE   0x01   //超时时间 测试为1分钟，正式为10分钟

extern bool g_bIsRujia;

//时间类型为DATA类型 COleDateTime  ATL里面的 MSDN查找
//通讯协议
//########################################################################################
#pragma pack(1)
// 消息头 分发协议头
typedef struct DATA_HEADTAG
{
	char  szHeadFlag[4];    // 头标识 ‘RS01’
	DWORD dwSerialNo;       // 流水号  从1开始使用，到达最大值后，再从1开始【循环使用]
	DWORD dwPacketLen;      // 包头+包身(XML文件长度)
	DWORD dwReturn;     	// 返回值  成功或失败
	//检测数据头是否正常 包头正常返回true，否false
	inline bool Check()
	{
		//if (g_bIsRujia)	NtohlEx();
		if (dwPacketLen >50*1024*1024) return false;
		return memcmp(this->szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN)?false: true;
	}
  //重置包头结构为0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}

	////把unsigned long类型从网络字节顺序转换到主机字节顺序
	//inline void NtohlEx()
	//{
	//	dwSerialNo = ntohl(dwSerialNo); 
	//	dwPacketLen = ntohl(dwPacketLen);
	//	dwReturn = ntohl(dwReturn);
	//}

	////把unsigned long类型从主机字节顺序转换到网络字节顺序
	//inline void HtonlEx()
	//{
	//	dwSerialNo = htonl(dwSerialNo); 
	//	dwPacketLen = htonl(dwPacketLen);
	//	dwReturn = htonl(dwReturn);
	//}
}DATAHEAD, *PDATAHEAD;
//const int MESSAGE_HEADER_LEN = sizeof(DATAHEAD);

typedef struct DATA_PACKET_TAG
{
	bool      bIsFill;                         //是否已经填充过 false第一次  true 非第一次
	DWORD     dwCurrentSize;                   //当前szbyData的有效数据长度	
	DWORD     dwBufLen;                        //szbyData实际长度
	PDATAHEAD pstuDataHead;                    //数据头
	BYTE      szbyData[RECVICE_DATA_MAX_LEN];  //用来存放包头和包身 最多只能存放一个包
	DATA_PACKET_TAG()
	{
		Reset();
	}
	~DATA_PACKET_TAG()
	{
		Reset();
	}

	inline void Init(DWORD dwInBufLen = RECVICE_DATA_MAX_LEN+16)
	{
		memset(this, 0, sizeof(*this));	
		dwBufLen = dwInBufLen-16;       //除去头之外的实际长度
		memset(szbyData, 0, dwBufLen);
		pstuDataHead = (PDATAHEAD)szbyData;		
		memcpy(pstuDataHead->szHeadFlag, PACKAGE_MARK, 4);	
	}

	//重置数据包内容为0
	inline void Reset()
	{
		DWORD dwTem = dwBufLen;
		memset(this, 0, sizeof(*this));	
		dwBufLen = dwTem;
		memset(szbyData, 0, dwBufLen);	
		pstuDataHead = (PDATAHEAD)szbyData;	
		memcpy(pstuDataHead->szHeadFlag, PACKAGE_MARK, 4);	
	}

	//检查数据是否已经接收完成 true 已经接收完成， false 未接收完整
	inline bool CheckRecvEnd()
	{
		return dwCurrentSize == pstuDataHead->dwPacketLen;
	}

	//用接收数据将szbyData填充为一整个数据包 返回实际填充数据长度 失败返回负值
	//-1 传入参数错误 -2 空间不足 0 未接收完成  大于零 返回本次增加的数据长度
	int AddRecvData(PBYTE pbtAddRecvData, int iAddRecvDataLen)
	{
		DWORD dwCopyLen = 0;       //本次操作的数据长度
		DWORD dwPacketLen = 0;     //应接收的包长度

		if (!pbtAddRecvData) return -1;
		//判断是否为第一次填充
		if (!bIsFill)
		{
			dwPacketLen = ((PDATAHEAD)pbtAddRecvData)->dwPacketLen;			  
			bIsFill = true;
		}
		else
		{  //再次填充
			dwPacketLen = pstuDataHead->dwPacketLen - dwCurrentSize;
		}

		dwCopyLen = (DWORD)iAddRecvDataLen < dwPacketLen ? iAddRecvDataLen:dwPacketLen;
		if (dwBufLen< dwCurrentSize + dwCopyLen)
		{
			TRACE(_T("DATAPACKET空间不足!!!!!\r\n"));			
			return -2;
		}

		memcpy(szbyData + dwCurrentSize, pbtAddRecvData, dwCopyLen);
		dwCurrentSize += dwCopyLen;
		if (!CheckRecvEnd()) return 0;
		return dwCopyLen;
	}

}DATAPACKET, *PDATAPACKET;

typedef struct CLIENT_BASE_INFO_TAG
{   //作废 ————0非前端 1连接时检查为前端成功(IP正确) 2 收验证包时检查前端成功(IP被修正) 
	DWORD dwConnID;            //连接ID          .....连接时赋值
	char szIP[16];             //连接IP           .....连接时赋值

	CLIENT_BASE_INFO_TAG()
	{
		Reset();
	}
	~CLIENT_BASE_INFO_TAG()
	{
		Reset();
	}

public:
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
		dwConnID = -1;
	}
}CLIENTBASEINFO, * PCLIENTBASEINFO;

enum TransmissionProtocolType { Tcp, Udp };
//用户状态
enum USER_STATUS
{
	ErrorStatus,  //错误
	CheckIn,      //入住
	CheckOut,     //退房
	OnLine,       //上线
	OutLine       //下线
};

//证件类型
enum CARD_TYPE
{
	GID,	//工作证
	ID,		//身份证
	JID,	//军人证
	JLZ,	//外国人居留证
	JZ,		//驾照
	TXZ,	//通行证
	VSA,	//护照
	XZ,		//回乡证
	ZQT		//其他证件
};
////证件类型
//enum CARD_TYPE
//{
//	"GID",     //工作证
//	"ID",      //身份证
//	"JID",     //军人证
//	"JLZ",     //外国人居留证
//	"JZ",      //驾照
//	"TXZ",     //通行证
//	"VSA",     //护照
//	"XZ",      //回乡证
//	"ZQT"      //其他证件
//};

//用户状态信息
typedef struct USER_STATUS_INFO
{
	USER_STATUS emUserStatus;          //用户状态
	TCHAR tszDescription[128];         //状态描述
	//CARD_TYPE emCardType;            //证件类型
	TCHAR tszCardType[8];
	TCHAR tszCardID[128];              //证件号码
	TCHAR tszUserName[MAX_PATH];       //用户姓名
	TCHAR tszRoomName[8];              //房间号
	TCHAR tszIP[32];                   //IP
	TCHAR tszMAC[32];                  //MAC
	DATE dtTime;                     //时间
	//TCHAR tszTime[32];                 //时间

	inline bool Check()
	{
		if (emUserStatus>ZQT || emUserStatus<GID)
		{
			TRACE(_T("用户状态信息检查；用户状态出错;emUserStatus:%d\r\n"), emUserStatus);
			return false;
		}
		//if (!(_tcslen(tszCardType) && _tcslen(tszCardID) && _tcslen(tszUserName) && _tcslen(tszIP) && _tcslen(tszMAC) && _tcslen(tszTime)))
		//{
		//	TRACE(_T("用户状态信息检查；数据长度出错;tszCardType:%d; tszCardID:%d; tszUserName:%d; tszIP:%d; tszMAC:%d; tszTime:%d;\r\n"),
		//		_tcslen(tszCardType),  _tcslen(tszCardID), _tcslen(tszUserName), _tcslen(tszIP), _tcslen(tszMAC), _tcslen(tszTime));
		//	return false;
		//}
		//if (!(!_tcscmp(tszCardType, _T("GID")) || !_tcscmp(tszCardType, _T("ID")) || !_tcscmp(tszCardType, _T("JID"))  || 
		//	!_tcscmp(tszCardType, _T("JLZ")) || !_tcscmp(tszCardType, _T("JZ")) || !_tcscmp(tszCardType, _T("TXZ")) || 
		//	!_tcscmp(tszCardType, _T("VSA"))  || !_tcscmp(tszCardType, _T("XZ")) || !_tcscmp(tszCardType, _T("ZQT"))))
		//{
		//	TRACE(_T("用户状态信息检查；证件类型检查出错;tszCardType:%s\r\n"), tszCardType);
		//	return false;
		//}
		//if (iRoomID<0) 
		//{
		//	TRACE(_T("用户状态信息检查；房间号检查出错;iRoomID:%s\r\n"), iRoomID);
		//	return false;
		//}

		return true;
	}
}USERSTATUSINFO, * PUSERSTATUSINFO;


#define DateTimeToString(dt) COleDateTime(dt).Format(_T("%Y-%m-%d %H:%M:%S"))
#define DateToString(dt) COleDateTime(dt).Format(_T("%Y-%m-%d"))
#define CURRENTDATE COleDateTime::GetCurrentTime().Format(_T("%Y%m%d"))
#define CURRENTTIME COleDateTime::GetCurrentTime().Format(_T("%H%M"))
#define GETCURRENTTIME COleDateTime::GetCurrentTime().Format(_T("%Y-%m-%d %H:%M:%S"))
#endif  