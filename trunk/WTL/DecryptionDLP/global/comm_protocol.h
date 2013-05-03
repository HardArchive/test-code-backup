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
//地址信息
typedef struct ADDR_INFO
{
	PVOID pPoint;           //指针地址
	DWORD dwAddrLen;        //地址指向数据长度
}ADDRINFO, *PADDRINFO;

//路径信息
typedef struct PATH_INFO
{
	TCHAR tszSourcePath[MAX_PATH];        //源目录
	TCHAR tszTargetPath[MAX_PATH];        //目标目录	
	TCHAR tszControlEventName[MAX_PATH];  //控制事件名
}PATHINFO, *PPATHINFO;

//文件信息
typedef struct FILE_INFO
{
	DWORD dwFileLen;                //文件长度
	TCHAR tszFileName[MAX_PATH];    //文件名
	TCHAR tszFilePath[MAX_PATH];    //文件路径 子目录路径
	//这里也许需要加一个文件属性
	TCHAR tszFileMapName[MAX_PATH]; //文件映射对象名
	TCHAR tszEventName[MAX_PATH];   //等待事件 文件处理完毕将事件设为有信号 DLL线程则执行清理工作
}FILEINFO, *PFILENIFO;


#endif /*__COMM_PROTOCOL_H__*/  