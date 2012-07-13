#pragma once
#include "netmysql.h"
/******************************************************************************* 
* 1、 文件名称： ConnMySql
* 2、 版　　本： Version *.*
* 3、 描    述： 数据库通信
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-2-9 15:03:43
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 

class CConnMySql
{
public:
	CConnMySql(void);
	~CConnMySql(void);

public:
	BOOL ConnectMysql(char* host,char* name,char*pwd);
	BOOL CheckUser();            //检查用户表中是否有自己的记录 如果没有则添加
private:
	
	
private:
	CNetMySql m_objNetMysql;

	BOOL m_bConnectFlag;              //MySql连接标志
};
