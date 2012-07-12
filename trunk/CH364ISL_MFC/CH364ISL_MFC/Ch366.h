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
	mPCH366_IO_REG m_pstuCfgRegBaseAdd;  // CH366оƬ�����üĴ���ָ��
	//mCH366_IO_REG  m_stuCfgRegBaseAdd;   //CH366оƬ�����üĴ���
	BOOL m_bIsInit;         //�Ƿ��Ѿ���ʼ��
	int m_iIsCardExist;     //���뿨�Ƿ����,0Ϊδ��ȡ��1Ϊ���ڣ�2Ϊ������
	int m_iIsIntranet;      //�Ƿ�Ϊ������0Ϊδ��ȡ��1Ϊ������2Ϊ����
	HMODULE m_hDll; 
	void Reboot();
public:
	BOOL Init();
	BOOL GetCardExist();    //��ȡ���뿨�Ƿ���ڣ��������򷵻�FALSE�����ڷ���TRUE
	BOOL GetNetWork();      //��ȡ��ǰ���绷������������TRUE����������FALSE
	BOOL SwitchWork();      //�л����绷��
};
