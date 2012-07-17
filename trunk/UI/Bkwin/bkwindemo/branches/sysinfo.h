//SysInfo.h
//SysInfo支持
#pragma once
#ifndef __KISUP_SYSTEM_SYSINFO_H__
#define __KISUP_SYSTEM_SYSINFO_H__
#include "Common.h"

NS_BEGIN(KISUP)
NS_BEGIN(System)

enum enumOsPlatform
{
	enumOsNone				= 0,
	enumOsWin31				= 1,
	enumOsWin9X				= 2,
	enumOsWinMe				= 3,
	enumOsWinNT				= 4,
	enumOsWin2000			= 5,
	enumOsWinXp				= 6,
	enumOsWinXp64			= 7,
	enumOsWin2003			= 8,
	enumOsWin2003R2			= 9,
	enumOsVista				= 10,
	enumOsWin2008			= 11,
	enumOsWin7				= 12,
	enumOsWin2008R2			= 13,
};

class SysInfo
{
public:
	static int IsVistaOrLater();
	static int GetOsPlatformVersion(IN enumOsPlatform & OsPlatform);
	static int IsVistaPlatform();
	static int IsXpPlatform();
	static int IsWinNTPlatform();
	static int IsWin9XPlatform();
	static int IsSafeModel();
	static int IsSafeModelWithNetwork();
	static int Is64bitOsSystem();
	static int IsUserAdmin();
	static int GetTimeZone(OUT std::wstring & strTimeZone);
	static int SetTimeZone(IN const std::wstring & strTimeZone);
	static int SetLocalTimeZone();
	static int CalcRandom(int mod = 100);
	static DWORD GetSysTimeSpace();
	/*
	FuncName	: GetCurTime
	Brief		: 得到当前时间。
	*/
	static DWORD GetCurTime();
	/*
	FuncName	: GetCurSysTime
	Brief		: 得到当前系统时间。
	*/
	static void GetCurSysTime(LPSYSTEMTIME lpSystemTime);
public:
	// From DOS file name to device file name
	static int GetDeviceFileName(IN LPCTSTR lpFsFileName, OUT std::wstring& deviceFileName);
	// From device file name to DOS filename
	static int GetFsFileName(IN LPCTSTR lpDeviceFileName, OUT std::wstring& fsFileName);
	//Get NT version
	static DWORD GetNTMajorVersion();
protected:
private:
};
NS_END(System)
NS_END(KISUP)




#endif	//__KISUP_SYSTEM_SYSINFO_H__