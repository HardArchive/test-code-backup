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

class CDecryptDLP
{
public:
	CDecryptDLP(void)
	{
		memset(m_tszDllPath, 0, MAX_PATH*sizeof(TCHAR));
	}

	~CDecryptDLP(void)
	{
		memset(m_tszDllPath, 0, MAX_PATH*sizeof(TCHAR));
	}

	int Init(TCHAR* ptInProcessName)
	{
		//ע��DLL
		DWORD deProcessID = GetSpecifiedProcessId(tstring(ptInProcessName));
		if (!deProcessID)
		{
			::MessageBox(NULL, "δ�ҵ�ָ������,���ܽ���δ���У�����", "��ʾ", MB_OK);
			return -1;
		}

		if (!LoadLib(deProcessID, GetDllPath(m_tszDllPath)))
		{
			::MessageBox(NULL, "ע��DLLʧ�ܣ�����", "��ʾ", MB_OK);
			return -2;
		}
		m_clsNamePipe.Create("RG_DecryptDLP");

	}

	void Uninit()
	{
		m_clsNamePipe.Close();
		//ע��DLL
		DWORD deProcessID = GetSpecifiedProcessId(tstring(ptInProcessName));
		if (!deProcessID)
		{
			::MessageBox(NULL, "δ�ҵ�ָ������,���ܽ���δ���У�����", "��ʾ", MB_OK);
			return -1;
		}
		FreeLib(deProcessID, _T("F:\\Visual Studio Projects\\test-code-backup\\trunk\\WTL\\DecryptionDLP\\bin\\CreakDLP.dll"));
	}

	int SendCommand(int iCommandType)
	{


	}


	bool DecryptDLP()
	{
		DWORD deProcessID = GetSpecifiedProcessId(tstring(_T("calc.exe")));
		if (!deProcessID)
		{
			::MessageBox(NULL, "δ�ҵ�ָ������,���ܽ���δ���У�����", "��ʾ", MB_OK);
			return false;
		}

		LoadLib(deProcessID, _T("F:\\Visual Studio Projects\\test-code-backup\\trunk\\WTL\\DecryptionDLP\\bin\\CreakDLP.dll"));

		CEvt clsEventDll(true, false, _T("Global\\IPC_event_dll"));
		CEvt clsEventExe(true, false, _T("Global\\IPC_event_exe"));	

		clsEventDll.Wait(5000);

		RG::CShareMemory clsShareMemory;
		DWORD dwFileSize = 1643;

		if (!clsShareMemory.Open(_T("Global\\IPC_SHARE")))
		{
			::MessageBox(NULL, "��ӳ���ļ�ʧ�ܣ�����", "SendFile", MB_OK);
			FreeLib(deProcessID, _T("F:\\Visual Studio Projects\\test-code-backup\\trunk\\WTL\\DecryptionDLP\\bin\\CreakDLP.dll"));
			return false;
		}	
		RG::CFile clsFile;         //�ļ��򿪾��
		clsFile.Open(_T("F:\\DLP\\12345.h"), _T("wb"));
		clsFile.Write((PBYTE)clsShareMemory.GetBasePoint(), dwFileSize);
		clsFile.Close();

		clsEventExe.Set();

		FreeLib(deProcessID, _T("F:\\Visual Studio Projects\\test-code-backup\\trunk\\WTL\\DecryptionDLP\\bin\\CreakDLP.dll"));
		return true;
	}

private:
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

private:
	RG::CNamedPipe m_clsNamePipe;
	TCHAR          m_tszDllPath[MAX_PATH];
};

#endif /*__DECRYPT_DLP_H__*/