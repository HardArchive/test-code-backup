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
#define PROTOCOL_VERSION                 0x0001       //协议版本号
//类型有以下几种级别的数据 数据头+ 数据内容                         每日数据量                   同时数据量(MAX)      长度
//1、心跳(0)-验证(16)-策略申请(8)-命令回复(0)                      (12960万+……可作15000万)            1             64
//2、经营控制命令(128)远程控制命令(460)网吧交互消息(140)            (很少)                              1             512
//3、报警(328)单个主机信息(312)上下机日志(524)                     (150万+未知 可能很多 也可能没有)   0-1500-40 0000   1024
//4、用户信息(424ImageLen[200*1024])                              (50万)                           0-1500           256*1024
//5、截屏(8+ImageLen[1024*1024])                                 (很少^几乎可忽略)                   0-1             1024*1024
#define RECVICE_DATA_MAX_LEN             1024         //接收数据最大长度1K 按级别来分的 分别*[(1-3)*1、(4)*256、(5)*1024]
#define POLICY_BUFSIZE_PERIOD            128*4        //时段策略缓冲区最大长度 4条
#define POLICY_BUFSIZE_HTTP              256*1024*8   //URL策略缓冲区最大长度 1024*8条
#define POLICY_BUFSIZE_GAME              1024*1024*4  //游戏策略缓冲区最大长度 1024*4条
//数据类型
#define TYPE_DATA_INT                    0x01       //int型
#define TYPE_DATA_DATE                   0x02       //DATE
#define TYPE_DATA_CHAR_ARRPAY            0x03       //char数组
#define TYPE_DATA_BYTE                   0x04       //BYTE
//策略类型
#define TYPE_POLICY_GAME                 0x01       //游戏策略
#define TPYE_POLICY_HTTP                 0x02       //URL策略
#define TYPE_POLICY_PERIOD               0x03       //时段策略


//数据包类型
#define TYPE_OK        0x01     //回复确认，表示已经接收或处理成功
#define TYPE_ERROR     0x02     //回复确认，表示接收或处理失败

#define TIMEOUT_MINUTE   0x01   //超时时间 测试为1分钟，正式为10分钟

//前端与后台通信类弄
#define TYPE_PACKET_VERIFY                0xA0        //验证包类型
//前端与客户端通信包类型
#define TYPE_PACKET_HEARTBEAT             0xA1        //心跳包类型
#define TYPE_PACKET_REPLAY_COMMAND        0xA2        //心跳包命令回复  表示已经收到且有命令下发  命令其后附带 无命令不回复
#define TYPE_PACKET_REQUEST_POLICY        0xA3        //申请策略
#define TYPE_PACKET_ISSUED_POLICY         0xA4        //策略下发
//客户端与后台通信
#define TYPE_PACKET_POLICY_ALERT          0xA5        //报警
#define TYPE_PACKET_SCREENSHOT     	      0xA6        //截屏


//前端与计费通信
//#define TYPE_PACKET_REQUEST_BAR_BASE_INFO       0xB0        //申请场所基本信息*已经废除*
//#define TYPE_PACKET_UPLOAD_BAR_BASE_INFO        0xB1        //上传场所基本信息*已经废除*
//#define TYPE_PACKET_REQUEST_TERMINAL_INFO       0xB2        //申请所有主机信息*已经废除*
//采用TCP短连接直接上传上需申请 
#define TYPE_PACKET_UPLOAD_ALL_TERMINAL         0xB3        //上传所有主机信息
#define TYPE_PACKET_UPLOAD_SINGLE_TERMINAL      0xB4        //上传单个主机信息
#define TYPE_PACKET_UPLOAD_USER_INFO            0xB5        //上传用户信息
#define TYPE_PACKET_UPLOAD_LOGINOUT_LOG         0xB6        //上传用户上下机日志
#define TYPE_PACKET_ISSUED_MESSAGE              0xB7        //下发消息
#define TYPE_PACKET_CONTROL_COMMAND             0xB8        //经营控制命令下发
//前端与计费通信
//时间类型为DATA类型 COleDateTime  ATL里面的 MSDN查找
//通讯协议
//########################################################################################

// 消息头 分发协议头
typedef struct DATA_HEADTAG
{
	char  szHeadFlag[4];    // 头标识 ‘RS01’
	DWORD dwVersion;        // 版本号  0x0001
	DWORD dwSerialNo;       // 流水号  从1开始使用，到达最大值后，再从1开始【循环使用]
	DWORD dwPacketType;     // 包数据类型  最高位为1，为回复数据。
	DWORD dwPacketLen;      //sizeof(HEADTAG) + 包身
	DWORD dwReturn;     	//返回值  成功或失败
	WORD  wEncode;          // 加密算法[0=未加密；1=异或加密；]	
	WORD  wHeadLen;         // 数据头长度

	//检测数据头是否正常 包头正常返回true，否false
	inline bool Check()
	{
		return memcmp(this->szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN)?false: true;
	}
	//重置包头结构为0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}
}DATAHEAD, *PDATAHEAD;
//const int MESSAGE_HEADER_LEN = sizeof(DATAHEAD);

typedef struct DATA_PACKET_TAG
{
	bool      bIsFill;                         //是否已经填充过
	DWORD     dwCurrentSize;                   //当前szbyData的有效数据长度	
	DWORD     dwBufLen;                        //Buffer实际长度
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

	inline void Init()
	{
		memset(this, 0, sizeof(*this));
		dwBufLen = RECVICE_DATA_MAX_LEN;       //除去头之外的实际长度
		pstuDataHead = (PDATAHEAD)szbyData;		
		memcpy(pstuDataHead->szHeadFlag, PACKAGE_MARK, 4);
		pstuDataHead->dwVersion = 1;
	}

	inline void Init(DWORD dwInBufLen)
	{
		memset(szbyData, 0, dwBufLen);
		memset(this, 0, sizeof(*this));
		dwBufLen = dwInBufLen-16;       //除去头之外的实际长度
		pstuDataHead = (PDATAHEAD)szbyData;		
		memcpy(pstuDataHead->szHeadFlag, PACKAGE_MARK, 4);
		pstuDataHead->dwVersion = 1;		
	}

	//重置数据包内容为0
	inline void Reset()
	{
		memset(szbyData, 0, dwBufLen);
		memset(this, 0, sizeof(*this));		
		pstuDataHead = (PDATAHEAD)szbyData;		
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
			RG::TRACE(_T("DATAPACKET空间不足!!!!!\r\n"));			
			return -2;
		}

		memcpy(szbyData + dwCurrentSize, pbtAddRecvData, dwCopyLen);
		dwCurrentSize += dwCopyLen;
		if (!CheckRecvEnd()) return 0;
		return dwCopyLen;
	}

}DATAPACKET, *PDATAPACKET;

typedef struct CLIENT_INFO_TAG
{
	int iIsFrontend;           //0非前端 1连接时检查为前端成功(IP正确) 2 收验证包时检查前端成功(IP被修正)
	DWORD dwConnID;            //连接ID
	char szBarCode[16];        //文化场所ID
	char szCompanyID[16];      //网吧计费ID
	char szIP[16];             //场所IP

	CLIENT_INFO_TAG()
	{
		Reset();
	}
	~CLIENT_INFO_TAG()
	{
		Reset();
	}

	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
		iIsFrontend = 0;
		dwConnID = -1;
	}
}CLIENTINFO, * PCLIENTINFO;
//验证包 dwTotalLen = HEAD+DATAHEART
typedef struct DATA_VERIFY
{
	char szBarCode[16];        //所在场所
}DATAVERIFY, *PDATAVERIFY;


//下发(回复)命令 dwTotalLen = HEAD + COMMONDISSUED
typedef struct COMMOND_ISSUED
{
	int iCommandID;           //命令编号
	int iCommandType;         //命令类型
	char szBackgroundIP[64];  //后台ip
	UINT uiBackgroundPort;    //后台端口
	char szClientIP[64];      //客户端IP
}COMMONDISSUED, *PCOMMONDISSUED;



//策略申请 dwTotalLen = HEAD + POLICYREQ
typedef struct POLICY_REQUEST
{
	int iType;             //策略类型
	DATE dtTime;           //时间戳
}POLICYREQ, *PPOLICYREQ;

//策略回复acknowledge
typedef struct POLICY_ACK
{
	int iType;               //策略类型 
	int iCount;              //条数
	DATE dtTime;             //时间戳
	char szIP[64];           //后台ip
	UINT uiPort;             //后台端口
	POLICY_ACK()
	{
		Reset();
	}
	~POLICY_ACK()
	{
		Reset();
	}

	//重置数据包内容为0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}
}POLICYACK, *PPOLICYACK;

//本地策略缓冲区
typedef struct POLICY_CONTENT
{
	DWORD dwContentLen;        //有效数据长度
	DWORD dwBufLen;            //Buffer的实际长度
	PPOLICYACK pstuPolicyAck;	
	PBYTE pszbyData;           //策略数据 包括策略头 + N条策略数据 加策略头  最大长度为128K
	POLICY_CONTENT()
	{
		Reset();
	}
	~POLICY_CONTENT()
	{
		Reset();
	}

	//初始化
	void Init(int iType)
	{		
		dwBufLen = TYPE_POLICY_PERIOD==iType ? POLICY_BUFSIZE_PERIOD:(TPYE_POLICY_HTTP==iType ? POLICY_BUFSIZE_HTTP:POLICY_BUFSIZE_GAME);
		pszbyData = new BYTE[dwBufLen];
		memset(pszbyData, 0, dwBufLen);
		pstuPolicyAck = (PPOLICYACK)pszbyData;
		pstuPolicyAck->iType = iType;
	}

	//重置数据包内容为0
	inline void Reset()
	{
		if (pszbyData) {delete pszbyData;}
		pszbyData = NULL;
		pstuPolicyAck = NULL;	
		memset(this, 0, sizeof(*this));		
	}

	inline void RestPolicy()
	{
		memset(pszbyData + sizeof(POLICYACK), 0, dwBufLen - sizeof(POLICYACK));
	}

	//检查时间戳 相同返回true 不同返回false
	inline bool CheckdtTime(DATE dtTime)
	{
		return dtTime == pstuPolicyAck->dtTime;
	}
}POLICYCONTENT, *PPOLICYCONTENT;

//禁止游戏策略 Dic_ForbidedGames
typedef struct POLICY_CONTENT_GAME
{
	int iID;                        //策略ID
	char szGameEName[64];           //GameEName
	char szGameCName[64];           //GameCName
	char szProductsByCorp[64];      //出品公司
	char szProcessName[64];         //游戏进程名称
	char szGameType[32];            //游戏类型（网络 单机）
	int iHarmType;                  //游戏危害类型（见属性）
	char szGameVersion[32];         //版本号
	char szGameSampling[256];       //游戏样本
	char szGameSamplingFile[256];   //文件取样
	DATE dtStartTime;               //开始执行时间	StartDate	datetime
	int iCreator;
	DATE dtCreatTime;               //CreateDate
	int iEditor;
	DATE dtEditDateTime;
	BYTE byIsDeleted;
	BYTE byIsAlarm;               //是否上传报警
	int iAlarmType;				  //报警类型		
	char szGameFileName[64];      //游戏文件名
}POLICYGAME, *PPOLICYGAME;

//禁止网页策略
typedef struct POLICY_CONTENT_HTTP
{
	int iID;                        //策略ID
	char szSiteName[64];            //SiteName
	char szUrl[32];                 //URL
	int iHarmType;                  //网站危害类型（见属性）
	DATE dtStartTime;               //开始执行时间
	int iCreator;
	DATE dtCreatTime;               //CreateDate
	int iEditor;
	DATE dtEditDateTime;
	BYTE byIsDeleted;
	BYTE byIsAlarm;
	int iAlarmType;				    //报警类型
	char szSiteType[32];            //网站类别
}POLICYHTTP, *PPOLICYHTTP;

//tcp通讯
//申请策略：客户端-->dwTotalLen = HEAD + POLICYREQ -->前端
//下发策略：前端-->HEAD + POLICYACK + POLICYACK::iCount*POLICYGAME-->客户端
//下发策略：前端-->HEAD + POLICYACK + POLICYACK::iCount*POLICYHTTP-->客户端

//经营时间策略 Mgt_BarBusinessTime
typedef struct POLICY_TIME
{
	int iID;                        //策略ID
	char szCityID[8];               //地区编号
	DATE dtStartTime;               //起始时间
	DATE dtEndTime;                 //结束时间
	DATE dtCreatTime;               //CreateDate
	int iCreator;
	DATE dtEditDateTime;
	int iEditor;
	int iIsDeleted;
	int iAlarmType;                //报警类型
}POLICYTIME, *PPOLICYTIME;

//策略报警 策略碰撞记录 Mgt_TacticsMatchRecord
typedef struct POLICY_ALERT
{
	//int iID;                    //ID
	int iTacticsType;          //策略类型
	int TacticsID;             //策略编号
	char szBarCode[16];        //所在场所
	//int iBarHostName;          //所在主机
	char szBarHostIP[16];      //主机IP
	char szCusName[32];        //用户姓名
	char szCusCertNO[32];      //用户证件号码	
	char szUserBarCode[32];    //用户卡号
	DATE dtCreateDate;  //报警时间
	char szBarHostMac[32];     //主机MAC
	//抓屏数据	CaptureImg	image			FALSE	FALSE	FALSE
	char szBarCusNo[16];       //顾客账号
	int iCusCertType;          //顾客证件类型
	char szIssueOrg[64];       //发证机关
	char szAlarmType[4];       //报警类别
	char szAlarmDesc[64];      //报警描述
}POLICYALERT, *PPLICYALERT;
//tcp通讯
//发送报警：客户端-->HEAD + POLICYALERT-->后台
//回复确认：后台-->HEAD-->客户端

//截屏  图像数据附到命令表后
typedef struct SCREEN_SHOT
{
	int iID;//命令编号
	DWORD dwImageLen;     //Image数据长度Image接在本结构后面
	//抓屏数据	CaptureImg	image			FALSE	FALSE	FALSE
}SCREENSHOT, *PSCREENSHOT;
//tcp通讯
//发送截屏：客户端-->HEAD + SEREENSHOT-->后台
//回复确认：后台-->HEAD-->客户端


//########################################################################################
//经营声所基本信息 Bar_BaseInfo
typedef struct BAR_BASE_INFO
{
	char szBarCode[16];             //BarCode
	char szBarName[128];            //BarName
	char szBoss[32];                //法人代表
	char szManager[32];             //负责人
	char szNetAD[32];               //网管
	char szBarAddress[128];         //场所地址
	char szManagerTel[32];          //负责人电话
	char szADTel[32];               //网管电话
	char szBarTel[32];              //网吧固定电话
	char szLicenseNum[16];          //许可证号码
	char szIssueOrg[64];            //发生机关
	DATE dtLicenseExpiredData;      //许可证有效期
	int iServerQuantity;            //网吧服务器数量
	char szBillingName[64];         //网吧计费系统
	char szBillingVersion[32];      //计费软件型号
	int iRealTerminalQuantity;      //实际主机数
	int iFilingTerminalQuantity;    //备案主机数
	char szEmpQuantity[32];         //工作人员数量
	char szBarIP[16];               //IP地址	BarIP
	//int iBarStatus;               //营业状态*****************
	DATE dtStartDate;               //开始管理时间
	char szProvinceID[16];          //所属省份
	char szCityID[16];              //所属城市
	char szDistrctID[16];           //所属区县
	DATE dtLastUpdate;              //最后通讯时间**************
	DATE dtCreateDate;              //创建时间
	DATE dtEditDate;                //修改时间      #############
	int iCreator;                   //添加人
	int iEditor;                    //修改人
	int iIsPermit;                  //许可证状态
	int iIsOpened;                  //IsOpened
	int iIsOnLine;                  //在线状态 1/0  ##############
	int iIsInstalled;               //管理系统安装状态
}BARBASEINFO, *PBARBASEINFO;
//场所主机信息申请回复
typedef struct TERMINAIL_ACK
{
	int iCount;              //主机个数
}TERMINAILACK, *PTERMINAILACK;
#pragma pack(1)
//经营场所主机信息 经常场所主机信息Bar_TerminalInfo delete Insert
typedef struct BAR_TERMINAIL_INFO
{
	int iID;                              //主机ID
	char szBarCode[16];                   //场所编号
	char szHostIP[16];                    //主机IP
	char szHostMac[32];                   //主机MAC
	char szHostName[32];                  //主机名称
	char szHostNum[32];                   //主机编号
	int iIsCamra;                         //视频状态
	int iCurrentStatus;                   //在线状态
	DATE dtLastUpdate;                    //LastUpdate
	char szDistrictID[8];                 //区县编号  - 查场所编号
	char szCityID[8];                     //城市编号
	char szProvinceID[8];                 //省份编号
	char szClientVersion[32];             //客户端计费版本号
	char szBarCusName[32];                //用户姓名
	char szBarCusCertNO[32];              //用户证件号
	int iBarCusCertType;                  //用户证件类别
	char szBarUserNO[32];                 //用户卡号
	DATE dtBarCusLoginTime;               //上机时间
}BARTERMINALINFO, *PBARTERMINALINFO;
#pragma pack()
//上网用户信息  上网用户日志Cus_CustomerInfo  只需插入
typedef struct CUSTOMER_INFO
{
	char szBarCode[16];       //场所编号
	char szCusName[32];       //用户姓名
	int iCusGender;           //用户性别
	int iCardType;            //卡类型（临时  会员）
	DATE dtRegDate;           //登记时间
	int iCertType;            //证件类型
	char szCertNo[32];        //证件号码
	char szIssueOffice[64];   //发证单位
	char szCountryID[64];     //国别
	char szUserBarCode[32];   //顾客帐号（卡号）
	char szCusPeople[32];     //民族
	char szCusOfficeName[64]; //单位名称
	char szInfoDesc[64];      //描述
	DWORD dwImageLen;         //身份证扫描图片长度
}CUSTOMERINFO, *PCUSTOMERINFO;
//计费-->HEAD + CUSTOMERINFO + 图片--> 前端 0xB5
#pragma pack(1)
//用户上下机日志 Bar_CustomerLoginOutLog 只需插入
typedef struct CUSTOMER_LIGIN_OUT_LOG
{
	long lID;                   //ID（见属性）（改长整形）
	char szBarCode[16];         //场所编号
	char szCusName[32];         //用户姓名
	int iCusGender;             //用户性别
	int iCardType;              //卡类型
	int iCertType;              //证件类型
	char szCertNo[32];          //证件号码
	DATE dtLoginTime;           //上机时间
	char szHostIP[16];          //登录主机IP
	char szHostName[32];        //登录主机名称
	char szHostMac[32];         //主机MAC地址
	DATE dtRcvTime;             //接收时间
	DATE dtLogoutTime;          //下机时间
	int iIsUpdate;              //是否上传
	char szComment[256];        //备注
	char szCusBarCode[32];      //用户卡号
	char szCusPeople[32];       //用户民族
	CUSTOMER_LIGIN_OUT_LOG()
	{
		Reset();
	}
	~CUSTOMER_LIGIN_OUT_LOG()
	{
		Reset();
	}

	//重置数据包内容为0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));	
	}
}CUSTOMERLOGINOUTLOG, *PCUSTOMERLOGINOUTLOG;
#pragma pack()
//经营控制命令 Mgt_BarBusinessControl
typedef struct BAR_BUSINESSCONTROL
{
	int iID;                    //命令ID
	char szBarCode[16];         //BarCode
	char szBarServerIP[16];     //网吧IP地址
	char szActionType[32];      //控制类型（为拓展）
	int iActionID;              //控制编号
	DATE dtStartDate;           //控制开始时间
	DATE dtEndDate;             //控制结束时间
	int iExeState;              //控制状态（开启 结束）
	DATE dtCreateDate;          //CreateDate
	int iCreator;               //Creator
	int iEditor;                //Editor
	DATE dtEditDate;            //EditDate
	int iIsSuccess;             //发送成功标志
}BARBUSINESSCONTROL, *PBAR_BUSINESSCONTROL;


//下发消息  Mgt_MessageForBar
typedef struct ISSUED_MESSAGE
{
	int iID;            //消息编号
	char szTitle[128];  //消息标题
	char szURL[256];    //URL
	int iIsUrgency;     //紧急程度
	int iMessageType;   //消息类型
	char szBarIP[64];   //网吧IP地址	BarIP
}ISSUEDMESSAGE, *PISSUEDMESSAGE;
//前端-->HEAD + ISSUEDMESSAGE -->计费中心 0xB7
/////////////////////////////////////////////////////////////////////////////////////////////
//数据库字段信息
typedef struct FIELD_PARAMETER
{	
	int FiledLen;           //字段内容长度
	int iFiledType;         //字段类型
	char* pszFieldName;     //字段名	
	char* pszFieldDesc;     //描述
}FIELDPARAMETER, *PFIELDPARAMETER;

//命令检索结果
typedef struct COMMOND_RESULE
{
	char szPlaceIP[64];              //场所IP
	COMMONDISSUED stuCommondIssued;  //命令结构
}COMMONDRESULE,* PCOMMONDRESULE;

//extern map<string, string> g_mapCompanyIDBarcode;   //Barcode与Company对应的图
#endif  