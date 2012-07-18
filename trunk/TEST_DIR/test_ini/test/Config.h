#pragma once

#include "Ini.h"
#include "DebugTrace.h"

//服务器连接信息
typedef struct _SERVER
{
	TCHAR tszIP[64];       //IP地址
	UINT uiPort;           //端口号
}SERVER, *PSERVER;

//查询信息
typedef struct _QUERYINFO
{
	int   iSiteNum;              //网站编号
	TCHAR tszUrl[MAX_PATH];      //查询用的第三方网站的网址
	TCHAR tszRegex[MAX_PATH];    //与网址对应的正则表达式
}QUERYINFO, *PQUERYINFO;
class CConfig
{
public:
	CConfig(void)
	{
		m_bInit = false;
	}
	virtual ~CConfig(void)
	{
		m_bInit = false;
	}
	//初始化函数，未初始化之前其他函数不能执行
	bool Init()
	{
		TCHAR tszIniPath[MAX_PATH] = {0};
		if (!GetExePath(tszIniPath))
		{
			return false;
		}
		_stprintf_s(tszIniPath, MAX_PATH, _T("%s\\config.ini"), tszIniPath);

		if (!m_clsIni.SetIniFilename(tszIniPath))
		{
			return false;
		}
		m_bInit = true;
		return true;
	}
	//获取当前程序所在目录
	bool GetExePath(LPTSTR pstrPath)
	{
		LPTSTR ptTem = NULL;
		TCHAR tszTemp[MAX_PATH] = {0};
		//获取当前目录  //这里是获取当前进程文件的完整路径 
		if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && pstrPath)
			return false; 

		ptTem = _tcsrchr(tszTemp, '\\');
		memcpy(pstrPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
		return true;
	}

	//获取服务器IP和端口 stuServer 服务器IP端口， bIsDCServer是否为分发中心
	bool GetServer(SERVER& stuServer, bool bIsDCServer)
	{
		std::tstring strTem;
		if (!m_bInit)
		{
			QNA::TRACE( _T("%s的%d 行有错误!!!!"), __FILE__, __LINE__);
			QNA::TRACE(_T("QNA-GetServer 没有初始化……"));
			return false;			
		}
		//获取IP
		if (bIsDCServer)
			strTem = m_clsIni.GetString(_T("DC"), _T("IP"));   //分发中心
		else
			strTem = m_clsIni.GetString(_T("PB"), _T("IP"));   //公安后台

		if (!strTem.length())
		{
			QNA::TRACE(_T("QNA-GetServer 获取IP错误……"));
			return false;
		}
		_stprintf_s(stuServer.tszIP, 64, _T("%s"), strTem.c_str());
		//获取端口
		if (bIsDCServer)
			stuServer.uiPort = m_clsIni.GetInt(_T("DC"), _T("Port"));
		else
			stuServer.uiPort = m_clsIni.GetInt(_T("PB"), _T("Port"));

		if (!stuServer.uiPort)
		{
			QNA::TRACE(_T("QNA-GetServer 获取端口错误……"));
			return false;
		}
		return true;
	}

	/******************************************************************************* 
	1、 Function Name ： GetWebQueryInfo
	2、 Description   ： 取出下一个解析站的网址和正则
	3、 Parameters    ： stuQueryInfo用于返回 是上次取出的结构
	4、 Return Value  ： 成功1， 未初始化-1， 所有站点已经遍历-0，取网址错误-2，取正则错误-3
	5、 Dynamic memory： 无
	6、 Author        ：  QNA 
	7、 Created       ： 2012-7-18 15:58:12
	8、 Remark        ： 注意传入参数是上次调用GetWebQueryInfo所取得的参数，  第一次传空即可
	********************************************************************************/ 
	int GetWebQueryInfo(QUERYINFO& stuQueryInfo)
	{
		int iNum = 0;
		TCHAR tszTem[32] = {0};
		std::tstring strTem;
		if (!m_bInit)
		{
			QNA::TRACE( _T("%s的%d 行有错误!!!!"), __FILE__, __LINE__);
			QNA::TRACE(_T("QNA-GetWebQueryInfo 没有初始化……"));
			return -1;			
		}
		iNum = ClacNext(stuQueryInfo.iSiteNum);
		if (-1 == iNum)
		{
			return 0;
		}
		_stprintf_s(tszTem, 32, _T("SITE%d"), iNum);
		strTem = m_clsIni.GetString(tszTem, _T("Url"));    //解析站点网址
		if (!strTem.length())
		{
			QNA::TRACE(_T("QNA-GetWebQueryInfo 获取网址错误……"));
			return -2;
		}
		_stprintf_s(stuQueryInfo.tszUrl, MAX_PATH, _T("%s"), strTem.c_str());

		strTem = m_clsIni.GetString(tszTem, _T("Regex"));   //正则
		if (!strTem.length())
		{
			QNA::TRACE(_T("QNA-GetWebQueryInfo 获取正则错误……"));
			return -3;
		}
		_stprintf_s(stuQueryInfo.tszRegex, MAX_PATH, _T("%s"), strTem.c_str());
		
		stuQueryInfo.iSiteNum = iNum;
		return 1;
	}

	/******************************************************************************* 
	1、 Function Name ： ResetQuery
	2、 Description   ： 重置查询返回第一个解析站点信息，
	3、 Parameters    ： stuQueryInfo 传出第一个解析站点信息
	4、 Return Value  ： 成功true，失败false
	5、 Dynamic memory： 无
	6、 Author        ： QNA 
	7、 Created       ： 2012-7-18 16:04:35
	8、 Remark        ： 
	********************************************************************************/ 
	bool ResetQuery(QUERYINFO& stuQueryInfo)
	{
		stuQueryInfo.iSiteNum = 0;
		if (1!= GetWebQueryInfo(stuQueryInfo))
		{
			return false;
		}
		return true;
	}

private:
	//获取解析站点数量 
	int GetSiteNum()
	{
		return m_clsIni.GetInt(_T("SITENUM"), _T("Num"));
	}

	//计算下一次取值次序
	int ClacNext(int iNum)
	{
		//这应该是第一次取值
		if ((iNum<1)||iNum>GetSiteNum())
			return 1;
		//这应该是第Num(Ini值)+1次取值,所以不给取
		if (GetSiteNum() == iNum)
			return -1;
		return ++iNum;
	}

private:
	QNA::CIni m_clsIni;
	bool m_bInit;         //初始化函数

};
