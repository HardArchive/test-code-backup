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

	//线程函数
private:
	static DWORD WINAPI _ListenThreadProc(LPVOID lpParam);  //监听线程 - 接受连接线程
	static DWORD WINAPI _WorkerThreadProc(LPVOID lpParam);  //工作线程 - 接收处理数据线程


public:
	bool Init(int nPort = 4567);     //初始化函数
	bool UnInit();   //退出清理函数


protected:

public:

};


