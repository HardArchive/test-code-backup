//file DecryptDLP.h
/***************************************************************************************************
* 1�� File       �� DecryptDLP.h
* 2�� Version    �� *.*
* 3�� Description�� 
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-7-23 10:06:09
* 6�� History    �� 
* 7�� Remark     �� 
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

		//ע��DLL
		m_dwProcessID = GetSpecifiedProcessId(tstring(ptInProcessName));
		if (!m_dwProcessID)
		{
			::MessageBox(NULL, _T("δ�ҵ�ָ������,���ܽ���δ���У�����"), _T("��ʾ"), MB_OK);
			return -1;
		}

		if (!LoadLib(m_dwProcessID, GetDllPath(m_tszDllPath)))
		{
			::MessageBox(NULL, _T("ע��DLLʧ�ܣ�����"), _T("��ʾ"), MB_OK);
			return -2;
		}
		
		ShowMessage(_T("dll�Ѿ�ע�룬��ѡ��Դ��·������ʼ���ܴ��룡����"));
	}

	void Uninit()
	{
		Stop();
		Sleep(100);
		m_clsSocketServerHelper.Close();
		if (!FreeLib(m_dwProcessID, GetDllPath(m_tszDllPath)))
		{
			TRACE(_T("Server - DLLж��ʧ��,ID:%d;Path:%s!!!\r\n"), m_dwProcessID, m_tszDllPath);
		}
		ShowMessage(_T("dll�Ѿ�ж�أ�����׼���˳�������"));
	}

	//ptInPathԴ����Ŀ¼, ptInSavePath�������Ŀ¼
	bool Start(PTCHAR ptInPath, PTCHAR ptInSavePath)
	{
		bool bRet = false;

		if (m_bExitFlag)
		{
			::MessageBox(NULL, _T("�����Ѿ���ʼ��ֹͣ�ٿ�ʼ"), _T("��Ϣ��ʾ"), MB_OK);
			return false;
		}

		m_bExitFlag = true;
		QueueUserWorkItem((LPTHREAD_START_ROUTINE)WorkerThreadProc, (LPVOID)this, WT_EXECUTELONGFUNCTION);

		ShowMessage(_T("�����߳��Ѿ�������׼�����ܴ����ļ�������"));
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

		ShowMessage(_T("�����߳��Ѿ�ֹͣ��������ܿ�ʼֹͣ������"));
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
	
	//���մ������߳�
	static UINT WINAPI WorkerThreadProc( LPVOID lpThreadParameter)
	{
		CDecryptDLP* pclsDecryptDLP = (CDecryptDLP*)lpThreadParameter;

		while (pclsDecryptDLP->m_bExitFlag)
		{
			if (pclsDecryptDLP->m_clsSocketServerHelper.Accept())
				break;
		}
		TRACE(_T("Server - �ͻ������ӳɹ�����ʼ��������!\r\n"));
		while(pclsDecryptDLP->m_bExitFlag)
		{
			pclsDecryptDLP->Recv();
		}

		return 1;
	}


private:
	//�����·�
	//COMMAND_START COMMAND_STOP COMMAND_PAUSE COMMAND_CONTINUE COMMAND_EXIT
	DWORD SendCommand(int iCommandType)
	{
		COMMAND stuCommand = {0};
		stuCommand.iCommand = iCommandType;
		return SendData(TYPE_PACKET_COMMAND, (PBYTE)&stuCommand, sizeof(COMMAND));
	}
	//��ַ�·�
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

	//�ظ��ļ�ӳ�����ݰ�
	DWORD SendReplay(const DWORD dwSerialNo = 0, const DWORD dwReturn = 0)
	{
		return SendData(TYPE_PACKET_REPLAY_MEMSHARE, NULL, 0, dwSerialNo, dwReturn);
	}

	//��������
	int Recv()
	{
		DATAPACKET stuDataPacket;
		stuDataPacket.Reset();
		stuDataPacket.pstuDataHead->Reset();

		
		int iReadLen = m_clsSocketServerHelper.Recv((PBYTE)stuDataPacket.pstuDataHead, sizeof(DATAHEAD));
		//::MessageBox(NULL, (char*)stuDataPacket.pstuDataHead, "sf", MB_OK);
		if (1 != iReadLen) return iReadLen;

		//�����ļ�ӳ����Ϣ
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
				//������ɻظ�����
				SendReplay(stuDataPacket.pstuDataHead->dwSerialNo, dwReturn);
			}
		}

		//������ظ�����
		if (TYPE_PACKET_REPLAY_COMMAND == stuDataPacket.pstuDataHead->dwPacketType)
		{
			//::MessageBox(NULL, _T("�յ�������Ϣ�ظ�"), _T("��Ϣ��ʾ"), MB_OK);
		}
		return iReadLen;
	}

	//��������
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
			::MessageBox(NULL, _T("�ͻ���δ����"), _T("��Ϣ��ʾ"), MB_OK);
			return 0;
		}

		dwRet =  m_clsSocketServerHelper.Send(stuPacket.szbyData, stuPacket.pstuDataHead->dwPacketLen);
		return dwRet;
	}

	//�����ļ� 
	bool DecryptDLP(MEMSHAREINFO& stuInMemShareInfo)
	{
		bool bRet = false;
		//Ŀ¼ ����
		if (!stuInMemShareInfo.stuFileInfo.IsFile())
		{
			TCHAR tszFilePath[MAX_PATH] = {0};		
			_stprintf_s(tszFilePath, MAX_PATH, "%s%s", m_tszSavePath, stuInMemShareInfo.stuFileInfo.tszFilePath);
			//·�� Ŀ¼����
			return RG::CreateMultipleDirectory(tszFilePath, stuInMemShareInfo.stuFileInfo.dwFileAttributes);
		}

		//�ļ�����
		//CEvt clsEvent(true, false, stuInMemShareInfo.tszEventName);	
		RG::CShareMemory clsShareMemory;

		if (!clsShareMemory.Open(stuInMemShareInfo.tszFileMapName))
		{
			::MessageBox(NULL, "��ӳ���ļ�ʧ�ܣ�����", "SendFile", MB_OK);
			return bRet;
		}	
		bRet = SaveFile(stuInMemShareInfo.stuFileInfo.tszFilePath, (PBYTE)clsShareMemory.GetBasePoint(), stuInMemShareInfo.stuFileInfo.dwAddrLen, stuInMemShareInfo.stuFileInfo.dwFileAttributes);
		clsShareMemory.Close();
		//clsEvent.Set();
		return bRet;
	}

	//�����ļ�
	bool SaveFile(const PTCHAR ptInFilePath, const PBYTE pbyBuf, const size_t nBufLen, const DWORD dwFileAttributes)
	{
		TCHAR tszFilePath[MAX_PATH] = {0};		
		_stprintf_s(tszFilePath, MAX_PATH, "%s%s", m_tszSavePath, ptInFilePath);
		//����Ҫ������Ϣ �����Ի���  ������ʾ�ļ���
		RG::TRACE(_T("�յ��ļ���Ϣ�����ļ�:%s\r\n"), tszFilePath);
		ShowMessage(tszFilePath);

		RG::CFile clsFile;         //�ļ��򿪾��
		clsFile.Open(tszFilePath, _T("wb"));
		clsFile.Write(pbyBuf, nBufLen);
		clsFile.Close();
		SetFileAttributes(tszFilePath, dwFileAttributes);
		return true;
	}

	//��ȡ��Ӧ��������ID 
	DWORD GetSpecifiedProcessId(tstring& tstrProcessName) 
	{ 
		DWORD dwID = 0; 
		//���ϵͳ���վ�� (ͨ�׵Ľ�, ���ǵõ���ǰ�����н���) 
		HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
		PROCESSENTRY32 pInfo; //���ڱ��������Ϣ��һ�����ݽṹ 
		pInfo.dwSize = sizeof(pInfo); 
		//�ӿ����л�ȡ�����б� 
		Process32First(hSnapShot, &pInfo) ; //�ӵ�һ�����̿�ʼѭ�� 
		do 
		{ 
			//tstrProcessNameΪ��Ľ������� 				
			if(0 == tstrProcessName.compare(tstring(pInfo.szExeFile))) 
			{ 
				dwID = pInfo.th32ProcessID ; 
				break ; 
			} 
		}while(Process32Next(hSnapShot, &pInfo) != false); 
		return dwID; //dwID ������Ҫ�Ľ���PID ��.. 
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

	//����������ʾ��Ϣ
	void ShowMessage(const TCHAR* ptInBuf)
	{
		COPYDATASTRUCT cpd = {0}; // ��COPYDATASTRUCT�ṹ��ֵ.
		cpd.dwData = 0;
		cpd.cbData = _tcslen(ptInBuf);
		cpd.lpData = (PVOID)ptInBuf;
		SendMessage(m_hWnd, WM_COPYDATA,(LPARAM)&cpd, NULL);// ����.
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
	DWORD          m_dwProcessID;             //��ע�����ID
	bool           m_bExitFlag;               //�߳��˳���־
	HWND           m_hWnd;                    //�����ھ�� ���ڷ�����ʾ��Ϣ
};

#endif /*__DECRYPT_DLP_H__*/