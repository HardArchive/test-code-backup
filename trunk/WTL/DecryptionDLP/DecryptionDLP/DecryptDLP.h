//file DecryptDLP.h
/***************************************************************************************************
* 1、 File       ： DecryptDLP.h
* 2、 Version    ： *.*
* 3、 Description： 
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-7-23 10:06:09
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
#ifndef __DECRYPT_DLP_H__
#define __DECRYPT_DLP_H__

#include "Injector.h"

#include "Event.h"
#include "ShareMemory.h"
#include "file.h"
#include "PathUtil.h"
#include "NPipe.h"
#include <TlHelp32.h>
#include "comm_protocol.h"
#include "SocketServerHelper.h"
class CDecryptDLP
{
public:
	CDecryptDLP(void)
	{
		Reset();
	}

	~CDecryptDLP(void)
	{
		Reset();
		m_dwProcessID = 0;
		m_clsSocketServerHelper.Close();
	}

	int Init(HWND hWnd,TCHAR* ptInProcessName)
	{
		m_hWnd = hWnd;
		//m_clsNamePipe.Create(_T("RG_DecryptDLP"));
		m_clsSocketServerHelper.Open();

		//注入DLL
		m_dwProcessID = GetSpecifiedProcessId(tstring(ptInProcessName));
		if (!m_dwProcessID)
		{
			::MessageBox(NULL, _T("未找到指定进程,可能进程未运行！！！"), _T("提示"), MB_OK);
			return -1;
		}

		if (!LoadLib(m_dwProcessID, GetDllPath(m_tszDllPath)))
		{
			::MessageBox(NULL, _T("注入DLL失败！！！"), _T("提示"), MB_OK);
			return -2;
		}
		
		ShowMessage(_T("dll已经注入，请选择源码路径，开始解密代码！！！"));
	}

	void Uninit()
	{
		Stop();
		Sleep(100);
		m_clsSocketServerHelper.Close();
		if (!FreeLib(m_dwProcessID, GetDllPath(m_tszDllPath)))
		{
			TRACE(_T("Server - DLL卸载失败,ID:%d;Path:%s!!!\r\n"), m_dwProcessID, m_tszDllPath);
		}
		ShowMessage(_T("dll已经卸载，程序准备退出！！！"));
	}

	//ptInPath源代码目录, ptInSavePath保存代码目录
	bool Start(PTCHAR ptInPath, PTCHAR ptInSavePath)
	{
		bool bRet = false;

		if (m_bExitFlag)
		{
			::MessageBox(NULL, _T("解密已经开始请停止再开始"), _T("消息提示"), MB_OK);
			return false;
		}

		m_bExitFlag = true;
		QueueUserWorkItem((LPTHREAD_START_ROUTINE)WorkerThreadProc, (LPVOID)this, WT_EXECUTELONGFUNCTION);

		ShowMessage(_T("工作线程已经启动，准备解密代码文件！！！"));
		Sleep(1000);

		bRet = SendPath(ptInPath)>0?true:false;
		_tcscpy_s(m_tszSavePath, MAX_PATH, ptInSavePath);
		if (!bRet) return false;

		return SendCommand(COMMAND_START)>0?true:false;
	}

	bool Stop()
	{
		bool bRet = false;
		if (m_clsSocketServerHelper.CheckClientStatus())
		{
			bRet = SendCommand(COMMAND_STOP)>0?true:false;
		}
		
		Sleep(200);
		m_bExitFlag = false;

		ShowMessage(_T("工作线程已经停止，代码解密开始停止！！！"));
		Reset();
		m_clsSocketServerHelper.Close();
		return bRet;
	}

	bool Pause()
	{
		return SendCommand(COMMAND_PAUSE)>0?true:false;
	}

	bool Continue()
	{
		return SendCommand(COMMAND_EXIT)>0?true:false;
	}
	
	//接收处理工作线程
	static UINT WINAPI WorkerThreadProc( LPVOID lpThreadParameter)
	{
		CDecryptDLP* pclsDecryptDLP = (CDecryptDLP*)lpThreadParameter;

		while (pclsDecryptDLP->m_bExitFlag)
		{
			if (pclsDecryptDLP->m_clsSocketServerHelper.Accept())
				break;
		}
		TRACE(_T("Server - 客户端连接成功，开始接收数据!\r\n"));
		while(pclsDecryptDLP->m_bExitFlag)
		{
			pclsDecryptDLP->Recv();
		}

		return 1;
	}


private:
	//命令下发
	//COMMAND_START COMMAND_STOP COMMAND_PAUSE COMMAND_CONTINUE COMMAND_EXIT
	DWORD SendCommand(int iCommandType)
	{
		COMMAND stuCommand = {0};
		stuCommand.iCommand = iCommandType;
		return SendData(TYPE_PACKET_COMMAND, (PBYTE)&stuCommand, sizeof(COMMAND));
	}
	//地址下发
	DWORD SendPath(PTCHAR ptInPath)
	{
		PATHINFO stuPathInfo = {0};
		if (!ptInPath)
		{
			return -1;
		}
		_tcscpy_s(stuPathInfo.tszSourcePath, MAX_PATH, ptInPath);
		return SendData(TYPE_PACKET_PATH, (PBYTE)&stuPathInfo, sizeof(PATHINFO));
	}

	//回复文件映射数据包
	DWORD SendReplay(const DWORD dwSerialNo = 0, const DWORD dwReturn = 0)
	{
		return SendData(TYPE_PACKET_REPLAY_MEMSHARE, NULL, 0, dwSerialNo, dwReturn);
	}

	//接收数据
	int Recv()
	{
		DATAPACKET stuDataPacket;
		stuDataPacket.Reset();
		stuDataPacket.pstuDataHead->Reset();

		
		int iReadLen = m_clsSocketServerHelper.Recv((PBYTE)stuDataPacket.pstuDataHead, sizeof(DATAHEAD));
		//::MessageBox(NULL, (char*)stuDataPacket.pstuDataHead, "sf", MB_OK);
		if (1 != iReadLen) return iReadLen;

		//处理文件映射信息
		if (TYPE_PACKET_MEMSHARE == stuDataPacket.pstuDataHead->dwPacketType)
		{
			iReadLen =  m_clsSocketServerHelper.Recv(stuDataPacket.szbyData+sizeof(DATAHEAD), stuDataPacket.pstuDataHead->dwPacketLen-sizeof(DATAHEAD));
			if (1 != iReadLen) return iReadLen;

			//if (sizeof(MEMSHAREINFO) == iReadLen)
			{
				MEMSHAREINFO stuMemShareInfo;
				stuMemShareInfo.Reset();
				memcpy(&stuMemShareInfo, stuDataPacket.szbyData+sizeof(DATAHEAD), stuDataPacket.pstuDataHead->dwPacketLen-sizeof(DATAHEAD));
				DWORD dwReturn = DecryptDLP(stuMemShareInfo)?TYPE_OK:TYPE_ERROR;
				//处理完成回复数据
				SendReplay(stuDataPacket.pstuDataHead->dwSerialNo, dwReturn);
			}
		}

		//命令包回复数据
		if (TYPE_PACKET_REPLAY_COMMAND == stuDataPacket.pstuDataHead->dwPacketType)
		{
			//::MessageBox(NULL, _T("收到命令消息回复"), _T("消息提示"), MB_OK);
		}
		return iReadLen;
	}

	//发送数据
	DWORD SendData(const int iMessageType, const PBYTE pbyInDateBuf = NULL, const int iBufLen = 0, const DWORD dwSerialNo = 0, const DWORD dwReturn = 0)
	{
		DWORD dwRet = 0;
		DATAPACKET stuPacket;
		stuPacket.pstuDataHead->dwPacketType = iMessageType;
		stuPacket.pstuDataHead->dwSerialNo = dwSerialNo;
		stuPacket.pstuDataHead->dwReturn = dwReturn;
		
		if (iBufLen)
		{
			memcpy(stuPacket.szbyData+stuPacket.dwBufLen, pbyInDateBuf, iBufLen);
			stuPacket.dwBufLen += iBufLen;
		}

		stuPacket.pstuDataHead->dwPacketLen = stuPacket.dwBufLen;

		if (!m_clsSocketServerHelper.CheckClientStatus())
		{
			::MessageBox(NULL, _T("客户端未连接"), _T("消息提示"), MB_OK);
			return 0;
		}

		dwRet =  m_clsSocketServerHelper.Send(stuPacket.szbyData, stuPacket.pstuDataHead->dwPacketLen);
		return dwRet;
	}

	//解密文件 
	bool DecryptDLP(MEMSHAREINFO& stuInMemShareInfo)
	{
		bool bRet = false;
		//目录 处理
		if (!stuInMemShareInfo.stuFileInfo.IsFile())
		{
			TCHAR tszFilePath[MAX_PATH] = {0};		
			_stprintf_s(tszFilePath, MAX_PATH, "%s%s", m_tszSavePath, stuInMemShareInfo.stuFileInfo.tszFilePath);
			//路径 目录属性
			return RG::CreateMultipleDirectory(tszFilePath, stuInMemShareInfo.stuFileInfo.dwFileAttributes);
		}

		//文件处理
		//CEvt clsEvent(true, false, stuInMemShareInfo.tszEventName);	
		RG::CShareMemory clsShareMemory;

		if (!clsShareMemory.Open(stuInMemShareInfo.tszFileMapName))
		{
			::MessageBox(NULL, "打开映射文件失败！！！", "SendFile", MB_OK);
			return bRet;
		}	
		bRet = SaveFile(stuInMemShareInfo.stuFileInfo.tszFilePath, (PBYTE)clsShareMemory.GetBasePoint(), stuInMemShareInfo.stuFileInfo.dwAddrLen, stuInMemShareInfo.stuFileInfo.dwFileAttributes);
		clsShareMemory.Close();
		//clsEvent.Set();
		return bRet;
	}

	//保存文件
	bool SaveFile(const PTCHAR ptInFilePath, const PBYTE pbyBuf, const size_t nBufLen, const DWORD dwFileAttributes)
	{
		TCHAR tszFilePath[MAX_PATH] = {0};		
		_stprintf_s(tszFilePath, MAX_PATH, "%s%s", m_tszSavePath, ptInFilePath);
		//这里要发送消息 给主对话框  用以显示文件名
		RG::TRACE(_T("收到文件信息保存文件:%s\r\n"), tszFilePath);
		ShowMessage(tszFilePath);

		RG::CFile clsFile;         //文件打开句柄
		clsFile.Open(tszFilePath, _T("wb"));
		clsFile.Write(pbyBuf, nBufLen);
		clsFile.Close();
		SetFileAttributes(tszFilePath, dwFileAttributes);
		return true;
	}

	//获取对应进程名的ID 
	DWORD GetSpecifiedProcessId(tstring& tstrProcessName) 
	{ 
		DWORD dwID = 0; 
		//获得系统快照句柄 (通俗的讲, 就是得到当前的所有进程) 
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
		PROCESSENTRY32 pInfo; //用于保存进程信息的一个数据结构 
		pInfo.dwSize = sizeof(pInfo); 
		//从快照中获取进程列表 
		Process32First(hSnapShot, &pInfo) ; //从第一个进程开始循环 
		do 
		{ 
			//tstrProcessName为你的进程名称 				
			if(0 == tstrProcessName.compare(tstring(pInfo.szExeFile))) 
			{ 
				dwID = pInfo.th32ProcessID ; 
				break ; 
			} 
		}while(Process32Next(hSnapShot, &pInfo) != false); 
		return dwID; //dwID 就是你要的进程PID 了.. 
	}

	TCHAR* GetDllPath(TCHAR* ptInPath)
	{
		if (_tcslen(ptInPath))
		{
			return ptInPath;
		}
		if (RG::GetExePath(ptInPath))
		{
			_tcscat_s(ptInPath, MAX_PATH-_tcslen(ptInPath), _T("\\CreakDLP.dll"));
		}		
		return ptInPath;
	}

	//在主窗口显示消息
	void ShowMessage(const TCHAR* ptInBuf)
	{
		COPYDATASTRUCT cpd = {0}; // 给COPYDATASTRUCT结构赋值.
		cpd.dwData = 0;
		cpd.cbData = _tcslen(ptInBuf);
		cpd.lpData = (PVOID)ptInBuf;
		SendMessage(m_hWnd, WM_COPYDATA,(LPARAM)&cpd, NULL);// 发送.
	}

	void Reset()
	{
		m_hWnd = NULL;
		//m_dwProcessID = 0;
		m_bExitFlag = false;		
		memset(m_tszDllPath, 0, MAX_PATH*sizeof(TCHAR));
		memset(m_tszSavePath, 0, MAX_PATH*sizeof(TCHAR));
	}

private:
	//RG::CNamedPipe m_clsNamePipe;
	CSocketServerHelper m_clsSocketServerHelper;
	TCHAR          m_tszDllPath[MAX_PATH];
	TCHAR          m_tszSavePath[MAX_PATH];
	DWORD          m_dwProcessID;             //需注入进程ID
	bool           m_bExitFlag;               //线程退出标志
	HWND           m_hWnd;                    //主窗口句柄 用于发用显示消息
};

#endif /*__DECRYPT_DLP_H__*/