#ifndef _rs_header_
#define _rs_header_

//协议头
typedef struct HeadTag
{
	DWORD dwFlag;//标记 恒为0x11223344
	DWORD dwCommand;//命令号 1请求IP段,2下发IP段,3上传已成功解析的IP段,4上传解析失败的IP段
};

//IP段下发
typedef struct IPSectionTag
{
	DWORD dwIPId;//IP段的ID值 标记唯一一个IP段
	DWORD dwStartIP;//开始IP值
	DWORD dwEndIP;//结束IP值
}IPSection;

//域名上传
typedef struct DomainInfoTag
{
	DWORD dwIPId;//IP段的ID值 标记唯一一个IP段
	DWORD dwIP;//IP值
	DWORD dwBytes;//所有域名所占字节数
	//域名信息 可以根据实际域名的个数长及长度定义此缓冲区,域名信息的存放格式为 域名长度:域名 
	char buff[32];
}DomainInfo;

typedef struct NoDomainInfoTag
{
	DWORD dwIPId;//IP段的ID值 标记唯一一个IP段
	DWORD dwIPCount;//IP个数
	char buff[32];  //IP值
}NoDomainInfo;

//IP段确认
typedef struct IPConfirmTag
{
	DWORD dwIPId;//IP段的ID值 标记唯一一个IP段
}IPConfirm;

#endif