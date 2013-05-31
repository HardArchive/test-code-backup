#pragma once 
#include "define.h"

#ifdef WIN32
#if defined DLLCLASS_EXPORTS
#define DLLCLASS_EXPORTS __declspec(dllexport)
#else
#define DLLCLASS_EXPORTS __declspec(dllimport)
#endif
#else
#define DLLCLASS_EXPORTS
#endif 


class DLLCLASS_EXPORTS CPacketParse
{
public:
	CPacketParse(void);
	~CPacketParse(void);

	void Init();

	uint32 GetPacketHeadLen();                        //得到的包头长度（未解密）
	uint32 GetPacketDataLen();                        //得到包体长度(未解密)
	uint16 GetPacketCommandID();                      //得到数据包命令字

  const char* GetPacketVersion();                   //得到包解析模块的版本

	bool GetIsHead();                                 //得到包头标记位，如果当前是包头，则返回True，否则返回False

	ACE_Message_Block* GetMessageHead();              //得到解密好的数据块（包头）
	ACE_Message_Block* GetMessageBody();              //得到解密好的数据块（包体）
	bool SetMessageHead(ACE_Message_Block* pmbHead);  //设置数据块信息（包头）
	bool SetMessageBody(ACE_Message_Block* pmbBody);  //设置数据块信息（包体）

	bool SetPacketHead(char* pData, uint32 u4Len);    //设置原始包头入口，在这里将数据还原成ACE_Message_Block解密解密好的包体
	bool SetPacketData(char* pData, uint32 u4Len);    //设置原始包体入口，在这里将数据还原成ACE_Message_Block解密解密好的包体

	uint32 GetPacketHeadSrcLen();                     //得到数据包原始包头长度
	uint32 GetPacketBodySrcLen();                     //得到数据包原始包体长度

	//拼接数据返回包
	bool MakePacket(const char* pData, uint32 u4Len, ACE_Message_Block* pMbData);   //返回的数据包，你可以在这里添加你的加密方法
	uint32 MakePacketLength(uint32 u4DataLen);                                      //获得加密后包的长度

	void Close();
};
