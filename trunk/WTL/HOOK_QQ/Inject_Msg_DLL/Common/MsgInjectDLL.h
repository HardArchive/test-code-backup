#ifndef _MSG_INJECT_DLL_H_
#define _MSG_INJECT_DLL_H_
/***************************************************************************************************
* 1�� File       �� MsgInjectDLL.h
* 2�� Version    �� 1.0
* 3�� Description�� 
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-11-13 16:13:29
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
//////////////////////////////////////////////////////////////////////////

//ʹ��string ֧�ֿ�խ�ַ�
#include <string>
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif
using namespace std;
#include "CWindowIterator.h"
#include <TlHelp32.h>
namespace RG
{
	class CMsgInjectDLL
	{
	public:
		CMsgInjectDLL()
		{
			Release();
		};

		//1Ϊ������ 2Ϊ��������  3Ϊ���ڱ���
		CMsgInjectDLL(tstring& tstrModuleName, tstring& tstrName, int iType = 1)
		{
			m_hHook = NULL;
			m_iType = iType;
			m_tstrName = tstrName;
			m_tstrModuleName = tstrModuleName;
			m_hModule = LoadLibrary(m_tstrModuleName.c_str());			
		};

		//4Ϊ����ID�� 5Ϊ���ھ��
		CMsgInjectDLL(tstring& tstrModuleName, DWORD& dwID, int iType = 4)
		{
			m_hHook = NULL;
			m_iType = iType;
			m_tstrModuleName = tstrModuleName;
			m_hModule = LoadLibrary(m_tstrModuleName.c_str());	
		};

		~CMsgInjectDLL()
		{
			Release();
		};
	public:
		//��װ����
		bool InstallHook()
		{
			if (!m_hModule)
			{
				m_hModule = LoadLibrary(_T("spyxhk.dll"));
			}

			typedef HHOOK (__stdcall * InitHookProc_TYPE)(HWND, DWORD);
			InitHookProc_TYPE InitHookProc = (InitHookProc_TYPE)GetProcAddress(m_hModule, "InitHookProc");

			if (!InitHookProc)
			{
				DWORD dwErrorCode = GetLastError();
				return false;
			}

			if (m_hHook)
			{
				UninstallHook();
				m_hHook = NULL;
			}
			//���ҽ��� m_iType Ϊ1ʱ m_tstrNameΪ������
			DWORD dwProcessID = GetSpecifiedProcessId(m_tstrName);
			HWND hWnd = GetSpecifiedProcessHwnd(dwProcessID);

			m_hHook = InitHookProc(hWnd, GetWindowThreadProcessId(hWnd, 0));

			DWORD dwErrorCode = GetLastError();
		}


		//ж�ع���
		bool UninstallHook()
		{
			bool bRet = false;
			if (!m_hHook)
			{
				Clear();
				return bRet;
			}

			if (!m_hModule)
			{
				m_hModule = LoadLibrary(_T("spyxhk.dll"));
			}

			typedef BOOL (__stdcall* EndHook_TYPE)(HHOOK);
			EndHook_TYPE EndHook = (EndHook_TYPE)GetProcAddress(m_hModule, "EndHook");

			if (!EndHook)
			{
				DWORD dwErrorCode = GetLastError();
				Clear();
				return bRet;
			}

			bRet = (bool)EndHook(m_hHook);

			Clear();
			return bRet;
		}


	protected:
		void Clear()
		{
			if (m_hModule)
			{	
				FreeLibrary(m_hModule);
			}

			m_hHook = NULL;
			m_hModule = NULL;
		}

		void Release()
		{
			UninstallHook();
			m_iType = 0;
			m_dwID = 0;
			m_tstrName.clear();
			m_tstrModuleName.clear();			
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


		//��ȡָ�����̵Ĵ��ھ��
		HWND GetSpecifiedProcessHwnd(const DWORD dwProcessID)
		{
			if (dwProcessID<0)
			{
				return NULL;
			}
			CMainHWnd clsCMainHWnd;
			clsCMainHWnd.EnumWndsByPid(dwProcessID);
			HWND hWnd = clsCMainHWnd.GetHWnd(0);
			return hWnd;
		}

		//��ȡָ�����̵ĵ��߳�ID
		DWORD GetSpecifiedThreadID(const DWORD dwProcessID)
		{
			DWORD pTID;
			_asm 
			{ 
				mov eax, fs:[0x18] 
				add eax, 36 
					mov [pTID], eax 
			} 
			DWORD dwThreadID = 0;
			HANDLE hProcess = OpenProcess(PROCESS_VM_READ, false, dwProcessID); 
			ReadProcessMemory(hProcess, (const void *)pTID, &dwThreadID, 4, NULL); 
			CloseHandle(hProcess); 

			return dwThreadID; 
		}

	protected:
		HHOOK m_hHook;
		HMODULE m_hModule;
		int m_iType; 
		DWORD m_dwID;                    //m_iTypeֵ 4Ϊ����ID�� 5Ϊ���ھ��
		tstring m_tstrName;              //m_iTypeֵ 1Ϊ������ 2Ϊ��������  3Ϊ���ڱ���
		tstring m_tstrModuleName;
	};
}

//////////////////////////////////////////////////////////////////////////
#endif