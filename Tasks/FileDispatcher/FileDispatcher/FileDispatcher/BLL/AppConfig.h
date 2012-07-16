/*******************************************************************************
 * Copyright (C), 2011-2020, 李  照
 * FileName: AppConfig.h
 * Author: 李  照    Version: 1.0    Date: 2011-08-18
 * Description: 应用程序配置类
 * Version: 1.0     // 版本信息
 * Function List:   // 主要函数及其功能
 * 1. 
 * History:         // 历史修改记录
 * <author>   <time>   <version>   <desc>
 *  李  照  2011-08-18    1.0    build this moudle  
 ******************************************************************************/
#pragma once

#ifndef APP_CONFIG_H_
#define APP_CONFIG_H_
#include <vector>

using namespace std;


// 工作线程基类
class CAppConfig
{
public:
	// 初始化
	static bool Init();
	// 获取应用程序配置实例
	static CAppConfig** GetInstance();
	// 释放资源
	static void Free();
	// 空串指针
	static char* Empty();
	// 加载配置数据
	bool LoadData();
public:
	// 应用程序目录
	char AppDir[MAX_PATH];
	////// 源数据路径，包含'\\'字符
	////char SourcePath[MAX_PATH];
	////// 目标数据路径，包含'\\'字符
	////char DestPath[MAX_PATH];
	////// 以文件保存的数据路径[目前主要是电子邮件的附件]，包含'\\'字符
	////char FilePath[MAX_PATH];
	////// 错误文件路径，包含'\\'字符
	////char ErrorPath[MAX_PATH];
	////// 第三方文件路径，包含'\\'字符
	////char OtherPath[MAX_PATH];
	//监控目录路径
	char m_szWatchDirPath[MAX_PATH];
	//目录移动目标路径
	char m_szDestDirPath[MAX_PATH];   
	//文件移动目录路径
	char m_szDestFilePath[MAX_PATH]; 


	// 程序运行模式。小于2时，单进程模式；大于2时，表示启动的子进程数。默认为单进程模式
	DWORD m_RunMode;
private:
	// 构造函数
	CAppConfig();
	// 析构函数
	~CAppConfig();

	// 字符串写入注册表
	void WriteStringRegistry(LPCTSTR strName, const string & strValue);
	// 从注册表载入配置信息
	bool LoadDataFromRegister();

	// 获取注册表存储的文件路径，并默认添加路径最后的'\\'字符
	inline int GetRegPath(HKEY hKey, const char* pKeyName, char* pValue, const char* pDefault);

	// 
	DWORD IpStr2Dword(const char* pSrc);


	// 实例对象
	static CAppConfig *m_instance;
	// 空串指针
	static char m_empty;
};

#define EMPTY_POINTER CAppConfig::Empty()
#endif