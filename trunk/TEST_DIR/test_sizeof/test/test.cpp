// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <Windows.h>
typedef struct DomainInfoTag
{
	DWORD dwIPId;//IP段的ID值 标记唯一一个IP段
	DWORD dwIP;//IP值
	//域名信息 可以根据实际域名的个数长及长度定义此缓冲区,域名信息的存放格式为 域名长度:域名
	//域名长度占一个字节,紧接着域名长度的是域名信息 然后是下一个域名长度和域名信息 按此格式一直排下去
	char buff[29];  //这个buff长度不固定
	WORD AA;

}DomainInfo;

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
#define PACKAGE_MARK                     "RS01"        //数据包标志
#define PACKAGE_MARK_LEN                 4             //标记长度
#define PROTOCOL_VERSION                 0x0001       //协议版本号

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
		if (dwPacketType<0xA0 || dwPacketType>0xB8)
		{
			//TRACE(_T("DATAHEAD Check 包类型不正确; dwPacketType:0x%X!!!!!\r\n"), dwPacketType);
			return false;
		}
		if (PROTOCOL_VERSION != dwVersion) return false;
		if (wHeadLen != sizeof(DATAHEAD)) return false;
		if (dwPacketLen >50*1024*1024) return false;
		return memcmp(this->szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN)?false: true;
	}
	//重置包头结构为0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}
}DATAHEAD, *PDATAHEAD;
//const int MESSAGE_HEADER_LEN = sizeof(DATAHEAD);


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
	char szAlarmType[8];       //报警类别
	char szAlarmDesc[64];      //报警描述
}POLICYALERT, *PPLICYALERT;


int _tmain(int argc, _TCHAR* argv[])
{
	char szbuf[1024] = {0};
	int iLen1 = sizeof(DWORD);
	int iLen3 = sizeof(WORD);
	int iLen5 = sizeof(DATAHEAD);
	int iLen2 = sizeof(DomainInfo);
	int iLen4 = sizeof(BAR_TERMINAIL_INFO);
	int iLen6 = sizeof(CUSTOMERINFO);
	int iLen7 = sizeof(CUSTOMERLOGINOUTLOG);
	int iLen8 = sizeof(POLICYALERT);
	struct BAR_TERMINAIL_INFO aa = {0};
	memset(&aa, 0xaa, sizeof(aa));
	memset(&aa.dtLastUpdate, 0xff, 8);

	DomainInfo* pDomainInfo = (DomainInfo*)szbuf;

	pDomainInfo->dwIPId = 100;
	pDomainInfo->dwIP = 115;

	strcpy(pDomainInfo->buff, "asdgkladfjghkladfjgkldfgorthdfkalgafklgkldfgkldf");
	strcpy(pDomainInfo->buff, "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");

	//有效长度
	iLen3 = sizeof(DomainInfo)-32 + strlen("WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW");



	return 0;
}

