#include "StdAfx.h"
#include "Ch366.h"

CCh366::CCh366(void)
{
	m_hDrv = NULL;
	m_bIsInit = FALSE;
	m_iIsCardExist = 0;
	m_iIsIntranet = 0;

}

CCh366::~CCh366(void)
{
	m_bIsInit = FALSE;
	m_iIsCardExist = 0;
	m_iIsIntranet = 0;
	if (m_hDrv)
	{
		CloseCH364IODrv(m_hDrv);
	}
	
}

BOOL CCh366::Init()
{
	ULONG CH366ID=0;
	ULONG ISCH366=0;
	//ZeroMemory(&m_stuCfgRegBaseAdd, sizeof(mPCH366_IO_REG));
	//mPCH366_IO_REG CfgRegBaseAdd;

	// 需要使用DLL则需要先加载
	m_hDll = LoadLibrary(L"CH364PGMDLL.dll" );
	if (!m_hDll)  // 加载DLL失败,可能未安装到系统中
	{
		MessageBox(NULL, L"无法加载CH364驱动程序的CH364PGMDLL", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		return FALSE;
	}
	if(!(SearchPath(NULL, L"CH364PGM", L".sys", NULL, NULL, NULL)))
	{
		MessageBox(NULL, L"在当前目录下没有发现CH364PGM.sys文件", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		return FALSE;
	}

	//在使用CH364函数之前先用此函数先做初始化操作,初始化成功后返回一个有效的句柄.
	if((m_hDrv = InitCH364IODrv(/*hDialog*/NULL))==NULL)
	{
		MessageBox( NULL/*hDialog*/, L"装载CH364IO.sys文件失败", L"TIPTOP_CH366", MB_ICONSTOP | MB_OK );
		return FALSE;
	}

	CH366ID = 0;
	ISCH366 = 0;

	//BOOL	WINAPI	FindCH366(         //搜索CH366芯片,获取相关信息
	//	HANDLE          DrvHandle,     //初始化函数时返回的有效句柄
	//	PULONG          CH364_ID,      //CH366芯片的位置号,可以事先指定起始芯片位置号
	//	PULONG          IS_CH366,           
	//	PVOID           CfgRegBaseAddr);//CH366配置寄存器基址
	if((FindCH366(m_hDrv, &CH366ID, &ISCH366, /*&m_stuCfgRegBaseAdd*/&m_pstuCfgRegBaseAdd)) == FALSE)
	{
		m_iIsCardExist = 2;
		MessageBox(NULL, L"没有找到CH364卡\n或者程序重复打开", L"TIPTOP_CH366", MB_ICONSTOP | MB_OK );
		return FALSE;
	}
	else
	{
		//获取芯片信息成功，判断是否为366芯片
		if(ISCH366)
		{
			m_iIsCardExist = 1;
		}else
		{
			m_iIsCardExist = 2;
			MessageBox(NULL, L"没有找到CH364卡", L"TIPTOP_CH366", MB_ICONSTOP | MB_OK );
			return FALSE;
		}
	}

	UCHAR uchRead = 0;
	PVOID pOffset = 0;
	//BOOL	WINAPI	CH364mReadIoByte(   // 从I/O端口读取一个字节
	//	HANDLE          DrvHandle,
	//	PVOID			iAddr,          // 指定I/O端口的地址
	//	PUCHAR			oByte );        // 指向一个字节单元,用于保存读取的字节数据//用来获取当前网络环境
	//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgCtrl;
	//pOffset = (PVOID)&m_pstuCfgRegBaseAdd->CH366CfgCtrl;
 	if (!CH364mReadIoByte(m_hDrv, /*pOffset*/&m_pstuCfgRegBaseAdd->CH366CfgCtrl, &uchRead))
	{
		MessageBox(NULL, L"获取网络环境失败！", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
		return FALSE;
	}	

	if((uchRead&0x2)==0)
	{
		m_iIsIntranet = 1;
	}
	else
	{
		m_iIsIntranet = 2;
	}
	m_bIsInit = TRUE;


	////test^^^^^^^^^^^^
	//uchRead = 0;
	//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgDout;
	//pOffset = &m_pstuCfgRegBaseAdd->CH366CfgDout;
	//if (!CH364mReadIoByte(m_hDrv, /*pOffset*/&m_pstuCfgRegBaseAdd->CH366CfgDout, &uchRead))
	//{
	//	MessageBox(NULL, L"获取网络环境失败！", L"TIPTOP_CH366", MB_ICONSTOP|MB_OK);
	//	return FALSE;
	//}	
	return TRUE;
}

BOOL CCh366::SwitchWork()
{
	//UCHAR t[8] = "";
	PVOID pOffset = 0;
	ULONG datavar = 0;

	HINSTANCE hInsQHuan = 0;	
	DWORD LastResult = 0;

	//mPCH366_IO_REG CfgRegBaseAdd;	
	if(1 == m_iIsCardExist)
	{
		UCHAR ch = 0;
		// 从I/O端口读取一个字节
		pOffset = &m_pstuCfgRegBaseAdd->CH366CfgCtrl;
		CH364mReadIoByte(m_hDrv, /*pOffset*/&m_pstuCfgRegBaseAdd->CH366CfgCtrl, &ch); 
		//内网切外网
		if (1 == m_iIsIntranet)
		{
			if((ch&0x2) != 0)
			{
				::MessageBox(NULL, L"您正在外网环境!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
				return FALSE;
			}
			//解除锁定
			//datavar = 0x07;
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgDout;
			//CH364mWriteIoByte(m_hDrv, pOffset, (UCHAR)datavar);// 向I/O端口写入解锁

			//外网切换成内网
			datavar = 0x2a;  
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgCtrl;  //偏移量			
			CH364mWriteIoByte(m_hDrv, &m_pstuCfgRegBaseAdd->CH366CfgCtrl/*pOffset*/, (UCHAR)datavar);// 向I/O端口写入一个字节
			//再次锁定
			//datavar = 0x17;
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgDout;
			//CH364mWriteIoByte(m_hDrv, pOffset, (UCHAR)datavar);// 向I/O端口写入锁定
		}
		else if (2 == m_iIsIntranet)
		{			
			if((ch&0x2)==0)
			{
				::MessageBox(NULL, L"您正在内网环境!", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
				return FALSE;
			}
			//解除锁定
			//datavar = 0x07;
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgDout;
			//CH364mWriteIoByte(m_hDrv, pOffset, (UCHAR)datavar);// 向I/O端口写入解锁
			//内网切换成外网
			datavar=0x10;    
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgCtrl;  //偏移量
			CH364mWriteIoByte(m_hDrv, &m_pstuCfgRegBaseAdd->CH366CfgCtrl/*pOffset*/, (UCHAR)datavar);
			//再次锁定
			//datavar = 0x17;
			//pOffset = (PVOID)m_stuCfgRegBaseAdd.CH366CfgDout;
			//CH364mWriteIoByte(m_hDrv, pOffset, (UCHAR)datavar);// 向I/O端口写入锁定
		}
		Reboot();
	}
	else
	{
		::MessageBox(NULL, L"隔离卡设备没找到!",L"TIPTOP_CH366", MB_OK|MB_ICONERROR);
	}
	return TRUE;

}

void CCh366::Reboot()
{
	HANDLE hToken; 
	TOKEN_PRIVILEGES tkp; 
	// Get a token for this process. 
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
		::MessageBox(NULL, L"OpenProcessToken", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
	// Get the LUID for the shutdown privilege. 
	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid); 
	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
	// Get the shutdown privilege for this process. 
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0); 
	// Cannot test the return value of AdjustTokenPrivileges. 
	if (GetLastError() != ERROR_SUCCESS)
		::MessageBox(NULL, L"AdjustTokenPrivileges", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
	// Shut down the system and force all applications to close. 
	if (!ExitWindowsEx(EWX_REBOOT, 0)) 
		::MessageBox(NULL, L"ExitWindowsEx", L"TIPTOP_CH366", MB_OK|MB_ICONINFORMATION);
}

BOOL CCh366::GetCardExist()
{
	if (1 == m_iIsCardExist)
		return TRUE;
	else
		return FALSE;

}

BOOL CCh366::GetNetWork()
{
	if (1 == m_iIsIntranet)
		return TRUE;
	else
		return FALSE;
}