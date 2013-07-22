/*
 *	命名管道通信类,目前支持单服务器、单客户端连接，支持双工通信。
 *	2006年1月5日 王文明
 */
#pragma once

#include <string>
#include <windows.h>
#include <stdio.h>
using namespace std;
#define BUFSIZE		4096

#define YW_EVENT_SER_CRE_OK        	0		/*! Create Server OK */
#define YW_EVENT_SER_CRE_FAIL		1		/*! Create Server FALSE */
#define YW_EVENT_CLI_CONNECT		2		/*! an client has connect */
#define YW_EVENT_CLI_DROP			3		/*! an client has left 	*/

#define YW_EVENT_CLI_CRE_OK        	4		/*! Create Client OK */
#define YW_EVENT_CLI_CRE_FAIL		5		/*! Create Client FALSE */
#define YW_EVENT_SER_CONNECT		6		/*! Connect to Server*/
#define YW_EVENT_SER_DROP			7		/*! Disconnect from Server 	*/

class CYWPipe
{
public:
	CYWPipe(void);
	virtual ~CYWPipe(void);
public:
	string GetError() {	return ErrorMessage;}
	string GetSerPipeName() {return SerPipeName;}
	string GetCliPipeName() {return CliPipeName;}
private:
	bool IsService;//true 为服务器端，false 为客户端

	HANDLE hSerPipe;//服务器管道句柄
	bool m_bSerThreadAlive;//服务器线程标志
	string SerPipeName;//管道名称
	bool CreateSerPipe();//创建服务器
	static DWORD ThrSer(LPVOID pParam);//服务器线程
	
	HANDLE hCliPipe;//客户端句柄
	bool m_bCliThreadAlive;//客户端线程句柄
	string CliPipeName;//管道名称
	bool CreateCliPipe();//创建客户端
	static DWORD ThrCli(LPVOID pParam);//客户端线程句柄
public:
	BOOL SendData(void * lpBuf, int nBufLen);//从管道发送数据
	BOOL Create(LPSTR PipeName,bool IsSer);//PipeName管道名,IsSer=true为服务器,false为客户端
	BOOL Close();//关闭管道连接

	virtual int OnRecData(LPVOID vlpPara, int len);//接收数据事件响应
	virtual int OnEvent(int EventType);//连接断开事件响应

private:
	HANDLE hSerWaitConEvent;//服务器等待连接事件
	HANDLE hSerRDEvent;//服务器读取数据事件
	HANDLE hCliWaitConEvent;//客户端等待连接事件
	HANDLE hCliRDEvent;//客户端读取数据事件
	string ErrorMessage;//出错信息
};
