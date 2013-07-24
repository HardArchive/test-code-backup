#pragma once
/***************************************************************************************************
* 1、 File       ： HandleCreakDLP.h
* 2、 Version    ： *.*
* 3、 Description： 解密处理操作类
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-7-24 17:41:51
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
#include <tchar.h>
#include "comm_protocol.h"
#include "safestl.h"
#include "NPipe.h"

class CHandleCreakDLP
{
public:

	CHandleCreakDLP(void)
	{
		Reset();
		//_asm int 3;
		m_clsNamePipe.Open(_T("RG_DecryptDLP"));
		
	}

	~CHandleCreakDLP(void)
	{
		Reset();
	}

	bool HandleMessage()
	{
		//::MessageBox(NULL, "DLL已经加载", "SendFile", MB_OK);
		//_asm int 3;
		m_bExitFlag = true;
		while(m_bExitFlag)
		{
			Recv();
			Sleep(1000);
		}

		return true;		
	}
	//两个线程 一个遍历目录
	//一个接收服务消息

private:
	//接收数据
	int Recv()
	{
		DATAPACKET stuDataPacket;
		stuDataPacket.Reset();
		stuDataPacket.pstuDataHead->Reset();

		int iReadLen = m_clsNamePipe.Read(stuDataPacket.pstuDataHead, sizeof(DATAHEAD));
		if (!iReadLen) return iReadLen;
		iReadLen = m_clsNamePipe.Read(stuDataPacket.szbyData+sizeof(DATAHEAD), stuDataPacket.pstuDataHead->dwPacketLen-sizeof(DATAHEAD));
		if (!iReadLen) return iReadLen;

		//服务器地址下发消息
		if (TYPE_PACKET_PATH == stuDataPacket.pstuDataHead->dwPacketType)
		{
			_tcscpy_s(m_tszECodePath, MAX_PATH, (TCHAR*)(stuDataPacket.szbyData+sizeof(DATAHEAD)));
			::MessageBox(NULL, m_tszECodePath, _T("消息提示"), MB_OK);
		}

		//服务器命令包下发消息
		if (TYPE_PACKET_COMMAND == stuDataPacket.pstuDataHead->dwPacketType)
		{
			::MessageBox(NULL, _T("收到服务器命令包下发消息"), _T("消息提示"), MB_OK);
			m_bExitFlag = false;
		}

		//文件映射消息回复 
		if (TYPE_PACKET_REPLAY_MEMSHARE == stuDataPacket.pstuDataHead->dwPacketType)
		{
			::MessageBox(NULL, _T("收到文件映射消息回复"), _T("消息提示"), MB_OK);
		}
		return iReadLen;
	}

	void Reset()
	{
		m_dwSerialNo = 0;
		m_bExitFlag = false;
		m_clsNamePipe.Close();
		memset(m_tszECodePath, 0, MAX_PATH*sizeof(TCHAR));
		m_mapSerialNoPacket.Lock();
		m_mapSerialNoPacket.clear();
		m_mapSerialNoPacket.Unlock();
	}

private:
	RG::CNamedPipe                          m_clsNamePipe;
	bool                                    m_bExitFlag;                   //线程退出标志
	DWORD                                   m_dwSerialNo;                  //流水号
	TCHAR                                   m_tszECodePath[MAX_PATH];      //加密代码路径
	RG::SAFESTL::CMap<DWORD, DATAPACKET>    m_mapSerialNoPacket;           //流水号与数据包对应 map
};
