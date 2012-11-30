#pragma once

/***************************************************************************************************
* 1、 class      ： SockeHelperIOCP
* 2、 Version    ： *.*
* 3、 Description： 完成端口处理类
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-11-30 17:06:01
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
extern bool g_bExit;   //全局退出函数

class CSockeHelperIOCP
{
public:
	CSockeHelperIOCP(void);
	~CSockeHelperIOCP(void);
	static DWORD WINAPI AcceptConnetProc(LPVOID lpParam);  //接收连接线程
	//接收探针数据
	static DWORD WINAPI ReceiveDataProc(LPVOID lpParam);   //接收处理数据线程

public:
	bool Init();     //初始化函数
	bool UnInit();   //退出清理函数


protected:

public:

};


