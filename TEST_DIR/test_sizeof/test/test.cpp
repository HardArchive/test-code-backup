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
	DATE dtLastUpdate;  
};
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


int _tmain(int argc, _TCHAR* argv[])
{
	char szbuf[1024] = {0};
	int iLen1 = sizeof(DWORD);
	int iLen3 = sizeof(WORD);
	int iLen5 = sizeof(DATAHEAD);
	int iLen2 = sizeof(DomainInfo);
	int iLen4 = sizeof(BAR_TERMINAIL_INFO);

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

