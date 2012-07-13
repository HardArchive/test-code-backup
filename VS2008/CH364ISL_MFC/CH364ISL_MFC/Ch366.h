#pragma once
#include "Lib/CH364PGMDLL.H"
#pragma comment(lib,"Lib/CH364PGMDLL")

class CCh366
{
public:
	CCh366(void);
	~CCh366(void);
public:
	HANDLE m_hDrv;

private:
	mPCH366_IO_REG m_pstuCfgRegBaseAdd;  // CH366芯片的配置寄存器指针
	//mCH366_IO_REG  m_stuCfgRegBaseAdd;   //CH366芯片的配置寄存器
	BOOL m_bIsInit;         //是否已经初始化
	int m_iIsCardExist;     //隔离卡是否存在,0为未获取，1为存在，2为不存在
	int m_iIsIntranet;      //是否为内网，0为未获取，1为内网，2为外网
	HMODULE m_hDll; 
	void Reboot();
public:
	BOOL Init();
	BOOL GetCardExist();    //获取隔离卡是否存在，不存在则返回FALSE，存在返回TRUE
	BOOL GetNetWork();      //获取当前网络环境，内网返回TRUE，外网返回FALSE
	BOOL SwitchWork();      //切换网络环境
};
