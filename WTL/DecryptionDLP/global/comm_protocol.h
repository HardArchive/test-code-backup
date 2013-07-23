//file comm_protocol.h
#ifndef __COMM_PROTOCOL_H__
#define __COMM_PROTOCOL_H__
/***************************************************************************************************
* 1、 File       ： comm_protocol.h
* 2、 Version    ： 1.0
* 3、 Description： 进程通讯协议--解密工具通讯协议
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-5-2 9:56:34
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
//常量
#define PACKAGE_MARK                     "RS01"        //数据包标志
#define PACKAGE_MARK_LEN                 4             //标记长度
#define PROTOCOL_VERSION                 0x0001       //协议版本号
//命令类型
#define COMMAND_START                     0x01        //开始命令
#define COMMAND_STOP                      0x02        //停止命令
#define COMMAND_PAUSE                     0x03        //暂停命令
#define COMMAND_CONTINUE                  0x04        //继续命令
#define COMMAND_EXIT                      0x05        //退出命令

//消息类型
#define TYPE_PACKET_PATH                  0xA0        //服务器地址下发
#define TYPE_PACKET_COMMAND               0xA1        //命令包下发
#define TYPE_PACKET_REPLAY_COMMAND        0xA2        //命令包命令回复  表示已经收到且有命令下发 
#define TYPE_PACKET_MEMSHARE              0xA3        //文件映射消息
#define TYPE_PACKET_REPLAY_MEMSHARE       0xA4        //文件映射消息回复  只需将原包包头返回即可



// 消息头 分发协议头
typedef struct DATA_HEADTAG
{
	char  szHeadFlag[4];    // 头标识 ‘RG01’
	DWORD dwVersion;        // 版本号  0x0001
	DWORD dwSerialNo;       // 流水号  从1开始使用，到达最大值后，再从1开始【循环使用]
	DWORD dwPacketType;     // 包数据类型  最高位为1，为回复数据。
	DWORD dwPacketLen;      //sizeof(HEADTAG) + 包身
	DWORD dwReturn;     	//返回值  成功或失败
	WORD  wHeadLen;         //数据头长度

	//检测数据头是否正常 包头正常返回true，否false
	inline bool Check()
	{
		if (dwPacketType<0xA0 || dwPacketType>0xA2)
		{
			//TRACE(_T("DATAHEAD Check 包类型不正确; dwPacketType:0x%X!!!!!\r\n"), dwPacketType);
			return false;
		}
		if (PROTOCOL_VERSION != dwVersion) return false;
		if (wHeadLen != sizeof(DATAHEAD)) return false;
		if (dwPacketLen >sizeof(DATAHEAD)+sizeof(MEMSHAREINFO)) return false;
		return memcmp(this->szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN)?false: true;
	}

	//重置包头结构为0
	inline void Reset()
	{
		memset(this, 0, sizeof(*this));
	}

	inline void Init()
	{
		memset(this, 0, sizeof(*this));
		dwVersion = PROTOCOL_VERSION;
		wHeadLen = sizeof(DATAHEAD);
		memcmp(szHeadFlag, PACKAGE_MARK, PACKAGE_MARK_LEN);
	}
}DATAHEAD, *PDATAHEAD;

//服务器发送消息

//路径信息
typedef struct PATH_INFO
{
	TCHAR tszSourcePath[MAX_PATH];        //源文件目录
	//TCHAR tszTargetPath[MAX_PATH];        //目标目录	
	//TCHAR tszControlEventName[MAX_PATH];  //控制事件名   
}PATHINFO, *PPATHINFO;

typedef struct COMMAND
{
	int iCommand;
}COMMAND, *PCOMMAND;


//客户端发送消息

//文件信息  注意当为目录信息时（只有tszFilePath有值其他都为0）
typedef struct FILE_INFO
{
	PVOID pPoint;                   //指针地址
	DWORD dwAddrLen;                //地址指向数据长度
	//DWORD dwFileLen;                //文件长度
	DWORD dwFileAttributes;         
	//TCHAR tszFileName[MAX_PATH];    //文件名
	TCHAR tszFilePath[MAX_PATH];    //文件路径 包括文件名的子目录路径 eg:(PATH工程目录)\DecryptionDLP\DecryptionDLP.vcproj

	inline void Reset()
	{
		memset(this, 0, sizeof(FILEINFO));
	}

	inline bool IsFile()
	{
		return pPoint != NULL;
	}
}FILEINFO, *PFILEINFO;

//内存映射信息  当为目录时tszFileMapName tszEventName为空
typedef struct MEM_SHARE_INFO
{
	FILEINFO stuFileInfo;           //文件信息
	TCHAR tszFileMapName[MAX_PATH]; //文件映射对象名
	TCHAR tszEventName[MAX_PATH];   //等待事件 文件处理完毕将事件设为有信号 DLL线程则执行清理工作
}MEMSHAREINFO, *PMEMSHAREINFO;


#endif /*__COMM_PROTOCOL_H__*/  