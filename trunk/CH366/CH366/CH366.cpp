// CH366.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include <tchar.h>
//#include "lib/CH364PGMDLL.H"
//#include <Shellapi.h>
#include "lib/CH364PGM.h"

//#pragma comment(lib,"Lib/CH364PGMDLL")
//
#pragma comment(lib,"Powrprof.lib")

extern "C" { 
#include <powrprof.h> 
}

typedef	struct	_CH366_IO_REG {	         // CH364芯片的配置寄存器
	UCHAR CH366CfgDout;                // 00H 输出寄存器
	UCHAR CH366CfgCtrl;                // 01H 控制寄存器
	UCHAR CH366CfgDin;                 // 02H 输入寄存器
	UCHAR CH366CfgSet;                 // 03H 配置寄存器
	ULONG rev[61];
	UCHAR CH366CfgAux;				    // F8H 辅助寄存器
} mCH366_IO_REG, *mPCH366_IO_REG;



HANDLE g_hDrv = NULL;   //驱动句柄
HMODULE g_hDll = NULL;        //CH364PGMDLL.dll动态库句柄 

ULONG CH366ID = 0;
ULONG ISCH366 = 0;
BOOL  g_Ch366Open = FALSE;                  //CH366芯片打开状态
mPCH366_IO_REG g_pstuCfgRegBaseAdd = NULL;    //CH366芯片的配置寄存器指针

//自定义TRACE函数
extern "C" __declspec(dllexport) void MyTRACE(WCHAR *fmt, ...)
{
//#ifdef _DEBUG
	WCHAR out[1024];
	va_list body;
	va_start(body,fmt);
	vswprintf_s(out,fmt,body);
	va_end(body);
	OutputDebugStringW(out);
//#endif
}
//显示错误信息函数
void MyShowError(void)
{
	ULONG ulError = 0;
	TCHAR szErrorMsg[128] = {0}, szOutput[256] = {0};

	ulError = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, ulError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), szErrorMsg, sizeof(szErrorMsg), 0);
	//FormatMessage功能就是将GetLastError得到的错误信息（这个错误信息是数字代号）转化成字符串信息的函数 

	_stprintf(szOutput, L"错误代码:0x%x:%s", ulError, szErrorMsg);
	::MessageBox(NULL, szOutput, L"TIPTOP_CH366", MB_ICONWARNING | MB_OK);
}

//关闭366设备
extern "C" __declspec(dllexport) void CloseCH366(void)
{
	if (g_hDrv)
	{
		CloseCH364IODrv(g_hDrv);
	}	

	ReleaseCh364PGM();

	g_hDrv = NULL;
	g_hDll = NULL;
	CH366ID = 0;
	ISCH366 = 0;
	g_Ch366Open = FALSE;
	g_pstuCfgRegBaseAdd = NULL;    //CH366芯片的配置寄存器指针
}


//查找366卡 成功返回1失败返回0
int FindCard(void)
{	
	CH366ID = 0;
	ISCH366 = 0;
	if(g_hDrv)
	{
		if((FindCH366(g_hDrv, &CH366ID, &ISCH366, &g_pstuCfgRegBaseAdd))==FALSE)
		{
#ifdef _DEBUG
			OutputDebugString(L"FindCard_没有找到CH364卡或者程序重复打开\r\n");
#endif	
			MessageBox( NULL, L"FindCard_没有找到CH364卡或者程序重复打开", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
			g_Ch366Open = FALSE;
			return 0;    //找不到CH366卡
		}
		else
		{
			if(ISCH366)
			{
				g_Ch366Open = TRUE;
				return 1;
			}
			else
			{
#ifdef _DEBUG
				OutputDebugString(L"FindCard_没有找到CH364卡\r\n");
#endif	
				MessageBox( NULL, L"FindCard_没有找到CH364卡", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
				g_Ch366Open = FALSE;
				return 0;
			}
		}
	}	

	return 0;
}

/******************************************************************************* 
1、 函数名称： OpenTP366
2、 功能描述： 打开366设备
3、 输入参数： 无
4、 返 回 值： -1   DLL加载失败
			 -2    驱动加载失败
			 -3    未找到卡
			 1     打开成功 
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-9-1 14:42:46
8、 备    注： 
********************************************************************************/ 
extern "C" __declspec(dllexport) int OpenCH366(void)
{	
	if (g_Ch366Open&&g_hDll&&g_hDrv)
	{
#ifdef _DEBUG
		OutputDebugString(L"卡已经打开……，不需再次打开\r\n");
#endif	
		return 1;
	}
	CloseCH366();

	//需要使用ch364GPMDLL则需要先加载	

	if (!Init364PGMDLL())
	{
		return -1;
	}

	if(!(SearchPath(NULL, L"CH364PGM", L".sys", NULL, NULL, NULL)))
	{
#ifdef _DEBUG
		OutputDebugString(L"在当前目录下没有发现CH364PGM.sys文件\r\n");
#endif	
		MessageBox(NULL, L"在当前目录下没有发现CH364PGM.sys文件", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		CloseCH366();
		return -2;
	}

	//在使用CH364函数之前先用此函数先做初始化操作,初始化成功后返回一个有效的句柄
	g_hDrv = InitCH364IODrv(NULL);
	if(NULL==g_hDrv)
	{
#ifdef _DEBUG
		OutputDebugString(L"装载CH364IO.sys文件失败\r\n");
#endif	
		MessageBox( NULL, L"装载CH364IO.sys文件失败", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		CloseCH366();

		return -2;
	}

	if(FindCard()==0) 
	{
#ifdef _DEBUG
		OutputDebugString(L"找不到CH366卡\r\n");
#endif		
		CloseCH366();
		return -3;
	}
	return 1;
}

/******************************************************************************* 
1、 函数名称： GetNet
2、 功能描述： 得到当前的网络类型
3、 输入参数：  无
4、 返 回 值： 返回2为外网，1为内网 o为失败
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-9-1 15:09:16
8、 备    注： 
********************************************************************************/ 
extern "C" __declspec(dllexport) int GetNet(void)
{
	UCHAR uchRead = 0;
	PVOID pOffset = 0;

	if(NULL == g_hDrv)
	{	
#ifdef _DEBUG
		OutputDebugString(L"Error : ChangeNet handle == NULL ");
#endif	
		CloseCH366();
		if (1 != OpenCH366())
		{
			::MessageBoxW(NULL, L"获取网络状态时当前卡状态不正确，重新打开失败，请确认卡已经插好……", L"TIPTOP_CH366", MB_OK);
			return 0; //获取网络环境失败
		}
	}

	if (!CH364mReadIoByte(g_hDrv, &g_pstuCfgRegBaseAdd->CH366CfgCtrl, &uchRead))
	{
		MessageBox(NULL, L"获取网络环境失败！", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		return 0;
	}
	MyTRACE(L"#####获取网络环境g_pstuCfgRegBaseAdd->CH366CfgCtrl，其值uchRead：0x%X#########################", uchRead);

	if((uchRead&0x2)==0)     
	{
		return 1;      //内网环境
	}
	else
	{			
		return 2;      //外网环境	
	}
}

//判断系统版本
BOOL IsWIN32_NT(void)
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);
	return osv.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

//作关机的预备工作  取得权限
BOOL PrepareShutDown(void)
{
	//  此处所调用的函数大多为API函数；
	HANDLE hToken = NULL;                   //与进程有关的访问标记句柄；
	HANDLE hProcess =  NULL;				//定义进程句柄；	
	LPVOID lpMsgBuf = NULL;
	TOKEN_PRIVILEGES  tp;			// 定义优先级标记结构体的一个实例；
	LUID luid;						//  定义查找ID结构体的一个实例;
	if(!IsWIN32_NT())
	{
		::MessageBox(0, L"当前系统为非WIN_32系统", L"TIPTOP_CH366", MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}

	hProcess = GetCurrentProcess();
	if(!hProcess)
	{
		::MessageBox(0, L"获取当前进程句柄失败……", L"TIPTOP_CH366", MB_OK);
		return FALSE;
	}
	//  得到一个与进程有关的访问标记句柄；	
	if(!OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
	{	
		MyShowError();
		::LocalFree(lpMsgBuf);
		return FALSE;
	}		

	//  给luid赋制值；
	if(!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid))  
	{
		MyShowError();
		::LocalFree(lpMsgBuf);
		return FALSE;
	}	

	tp.PrivilegeCount = 1;       //  给结构体实例赋值；
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if(!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		MyShowError();
		::LocalFree(lpMsgBuf);
		return FALSE;
	}	
	return TRUE;
}

//EWX_FORCE  强制注销
//　　强制终止进程。
//	当此标志设置，Windows不会发送消息WM_QUERYENDSESSION和WM_ENDSESSION的消息给目前在系统中运行的程序。这可能会导致应用程序丢失数据。
//	因此，你应该只在紧急情况下使用此标志。
//EWX_LOGOFF 　　关闭所有进程，然后注销用户。
//EWX_POWEROFF 　　
//	关闭系统并关闭电源。该系统必须支持断电。 　　
//	Windows要求： 　　Windows NT中调用进程必须有 SE_SHUTDOWN_NAME 特权。 　　Windows 9X中：可以直接调用。
//EWX_REBOOT 　　
//	关闭系统，然后重新启动系统。 　　
//	Windows要求： 　　Windows NT中：调用进程必须有SE_SHUTDOWN_NAME特权。 　　Windows 9X中：可以直接调用。
//EWX_SHUTDOWN 　　
//	关闭系统，安全地关闭电源。所有文件缓冲区已经刷新到磁盘上，所有正在运行的进程已经停止。 　　 //侍机
//	Windows要求： 　　Windows NT中：调用进程必须有SE_SHUTDOWN_NAME特权。 　　Windows 9X中：可以直接调用。
//侍机 paternity machine
//关机注销重启操作函数
BOOL PoweroffEx(UINT uFlags)
{
	LPVOID lpMsgBuf = NULL;
	if(0==ExitWindowsEx(uFlags, 0))
	{	
		MyShowError();
		//OutputDebugString(lpMsgBuf);
		::LocalFree(lpMsgBuf);
		return FALSE;
	}
	return TRUE;
}


//重启
BOOL Reboot(void)
{
	if(!PrepareShutDown())
	{
		MessageBox(NULL, L"PrepareShutDown", L"SHUTDOWN", MB_OK);	
		return FALSE;
	}

	if (!PoweroffEx(EWX_REBOOT))
	{
		return FALSE;
	}

	return TRUE;
}

//关闭计算机
BOOL ShutDown(void) 
{
	if(!PrepareShutDown())
	{
		MessageBox(NULL, L"PrepareShutDown", L"SHUTDOWN", MB_OK);	
		return FALSE;
	}
	if (!PoweroffEx(EWX_SHUTDOWN))
	{
		return FALSE;
	}
	return TRUE;
}

//休眠
BOOL Hibernate(void)
{
	BOOL bRet = FALSE;                    //返回值

	if (!IsPwrHibernateAllowed())
	{
		::MessageBoxW(NULL, L"请先启用系统休眠功能……", L"TIPTOP_366", MB_OK);
		return FALSE;
	}

	if(!PrepareShutDown())
	{
		MessageBox(NULL, L"PrepareShutDown", L"SHUTDOWN", MB_OK);	
		return FALSE;
	}

	if(!IsWIN32_NT())
	{
		::MessageBox(0, _T("当前系统为非WIN_32系统"), _T("Error"), MB_OK | MB_ICONINFORMATION);
		return FALSE;
	}
	//if (!SetSuspendState(1,0,0))
	//{
	//	return FALSE;
	//}

	//SetThreadExecutionState(ES_DISPLAY_REQUIRED);
	//Sleep(1000);
	//CloseCH366();
	//OpenCH366();

	return SetSuspendState(1,0,0);	
}

/******************************************************************************* 
1、 函数名称： ChangeNet
2、 功能描述： 
3、 输入参数： bNetBit    为1时，切换到外网;否则切换到内网。
			 iPowerOff    为1时，重启计算机，为2休眠计算机。如果是休眠的话，计算机将自动唤醒 为3时关闭计算机
			 bInterFace 为1时，开机时出现选择界面，否则不出选择界面。
4、 返 回 值：  0、成功切换
			   1、未找到卡
			   2、获取网络环境失败
			   3、休眠切换读出数据失败
			   4、休眠切换写入换醒数据失败
			   5、参数输入有误
			   6、重启、休眠、关机出错
			   7、切换网络类型出错
5、 动态内存： 无
6、 代码设计：  阿毛
7、 开发日期： 2011-9-23 17:48:03
8、 备    注： 此处未加处出处理代码
********************************************************************************/ 
extern "C" __declspec(dllexport) int ChangeNet(BOOL bNetBit, int iPowerOff, BOOL bInterFace)
{
	int  iNetStatue = 0;
	UCHAR uchRead = 0;
	if (!g_Ch366Open)
	{
		CloseCH366();
		//Sleep(500);
		if (1 != OpenCH366())
		{
			return 1;
		}
	}

	//判断网络切换是否正确
	iNetStatue = GetNet();
	if (1 == iNetStatue)  //当前为内网
	{
		if (!bNetBit)
		{
			::MessageBox(NULL, L"当前网络已经为外网……", L"TIPTOP_366", MB_OK);	   	     
			return 7;
		}
		//uchRead = 0x00;
		uchRead = 0xA8;
	}
	else if (2 == iNetStatue)  //当前为外网
	{
		if (bNetBit)
		{
			::MessageBox(NULL, L"当前网络已经为内网……", L"TIPTOP_366", MB_OK);	   	     
			return 7;
		}
		//uchRead = 0xA;
		uchRead = 0x92;
	}
	else
	{
		return 2;  //获取网络环境失败
	}

	//获取网络环境
	if(!CH364mReadIoByte(g_hDrv, &g_pstuCfgRegBaseAdd->CH366CfgCtrl, &uchRead))
	{
		MessageBox(NULL, L"读CH366获取网络环境出错!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
		return 2;
	}
	MyTRACE(L"重初始化配置寄存器&g_pstuCfgRegBaseAdd->CH366CfgCtrl读出数据,uchRead：0x%X", uchRead);

	if(IDOK == MessageBox(NULL, L"是否要切换当前网络环境? \n(切换前必须重新启动或休眠计算机!)\n", 
		L"TIPTOP_CH366", MB_OKCANCEL | MB_ICONQUESTION ))
	{
		uchRead = uchRead | 0x80;

		if( bNetBit == 1 )
			uchRead = uchRead | 0x08;		//位3置1，系统关机后继电器将选择切换到外网
		else
			uchRead = uchRead & 0x0F7;    //位3置0，系统关机后继电器将选择切换到内网

		if(bInterFace)
		{
			uchRead = uchRead & 0x0CF;	   //位4与位5相同时，出现选择界面，这里全设置为0
		}
		else
		{
			if( bNetBit == 1 )	          //位5与位3相同,位4与位3相反时不出选择界面
				uchRead = uchRead | 0x20;
			else
				uchRead = uchRead | 0x10;
		}
		MyTRACE(L"正在切换隔离卡，切换参数，bNetBit：%d;iPowerOff:%d;bInterFace:%d;", bNetBit, iPowerOff, bInterFace);
		MyTRACE(L"配置寄存器&g_pstuCfgRegBaseAdd->CH366CfgCtrl写入数据,uchRead：0x%X", uchRead);

		//将切换信息写入隔离卡 
		if( !CH364mWriteIoByte( g_hDrv,&g_pstuCfgRegBaseAdd->CH366CfgCtrl,(UCHAR)uchRead) )
		{
			MessageBox(NULL, L"写CH366IO数据出错!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
			return 3;
		}

		if(2 == iPowerOff) //休眠切换   这里应该是写入换醒数据
		{	
			//uchRead = 0xB8;
			if(!CH364mReadIoByte(g_hDrv, &g_pstuCfgRegBaseAdd->CH366CfgAux, &uchRead))
			{
				//_stprintf( buf, L"Write before CH366CfgAux:%02X "), ch );
				//MessageBox( NULL, buf, L"TIPTOP_CH366", MB_ICONWARNING | MB_OK );
				MessageBox(NULL, L"读CH366辅助寄存器数据出错!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
				return 3;
			}
			MyTRACE(L"辅助寄存器&g_pstuCfgRegBaseAdd->CH366CfgAux读取数据，uchRead：0x%X", uchRead);

			uchRead = uchRead & 0x0FE ;
			MyTRACE(L"辅助寄存器&g_pstuCfgRegBaseAdd->CH366CfgAux写入数据，uchRead：0x%X", uchRead);
			if(!CH364mWriteIoByte(g_hDrv, &g_pstuCfgRegBaseAdd->CH366CfgAux, (UCHAR)uchRead))
			{
				MessageBox(NULL, L"写CH364IO数据!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
				return 4;
			}
		}

		Sleep(500);
		if(1 == iPowerOff)
		{
			if (!Reboot())
			{
				return 6;
			}

		}
		else if (2 == iPowerOff)
		{
			if (!Hibernate())
			{
				return 6;
			}
		}
		else if (3 == iPowerOff)
		{
			if (!ShutDown())
			{
				return 6;
			}				
		}
		else
		{
			return 5;
		}				
	}

	return 0;
}
