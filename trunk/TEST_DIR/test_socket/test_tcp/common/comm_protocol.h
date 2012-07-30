#ifndef _COMM_PROTOCOL_h_
#define _COMM_PROTOCOL_h_
//通讯协议
//##################################################################################################################
//分发协议头
typedef struct HeadTag
{
	//标记 恒为0x11223344
	DWORD dwFlag;
	//命令号  1请求IP段,2下发IP段,3上传已成功解析的IP段,4上传解析失败的IP段 5.确认回复
	DWORD dwCommand;
}Head;

//IP段下发
typedef struct IPSectionTag
{
	DWORD dwIPId;//IP段的ID值 标记唯一一个IP段
	DWORD dwStartIP;//开始IP值
	DWORD dwEndIP;//结束IP值
}IPSection;

//IP段确认
typedef struct IPConfirmTag
{
	DWORD dwIPId;//IP段的ID值 标记唯一一个IP段
}IPConfirm;

//##################################################################################################################

//上传协议头
typedef struct UpLoadHeadTag
{
	//标记 恒为0x11223344
	DWORD dwFlag;
	//命令号  1请求IP段,2下发IP段,3上传已成功解析的IP段,4上传解析失败的IP段 5.确认回复
	DWORD dwCommand;
	//包身所点字节长度
	DWORD dwBodyLen;
}UpLoadHead;

//域名上传
typedef struct DomainInfoTag
{
	DWORD dwIPId;//IP段的ID值 标记唯一一个IP段
	DWORD dwIP;//IP值
	//域名信息 可以根据实际域名的个数长及长度定义此缓冲区,域名信息的存放格式为 域名长度:域名
	//域名长度占一个字节,紧接着域名长度的是域名信息 然后是下一个域名长度和域名信息 按此格式一直排下去
	char buff[32];
}DomainInfo;

typedef struct NoDomainInfoTag
{
	DWORD dwIPId;//IP段的ID值 标记唯一一个IP段
	DWORD dwIPCount;//IP个数
	//存放IP信息 四个字节存一个IP地址 IP的个数由dwIPCount指定 可以根据实际需要申请buff长度
	char buff[32];
}NoDomainInfo;
//#################################################################################################################


#endif