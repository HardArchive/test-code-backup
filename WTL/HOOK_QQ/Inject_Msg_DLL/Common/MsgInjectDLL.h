#ifndef _MSG_INJECT_DLL_H_
#define _MSG_INJECT_DLL_H_
/***************************************************************************************************
* 1、 File       ： MsgInjectDLL.h
* 2、 Version    ： 1.0
* 3、 Description： 
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-11-13 16:13:29
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
//////////////////////////////////////////////////////////////////////////

//使用string 支持宽窄字符
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

		//1为进程名 2为窗口类名  3为窗口标题
		CMsgInjectDLL(tstring& tstrModuleName, tstring& tstrName, int iType = 1)
		{
			m_hHook = NULL;
			m_iType = iType;
			m_tstrName = tstrName;
			m_tstrModuleName = tstrModuleName;
			m_hModule = LoadLibrary(m_tstrModuleName.c_str());			
		};

		//4为进程ID， 5为窗口句柄
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
		//安装钩子
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
			//当且仅当 m_iType 为1时 m_tstrName为进程名
			DWORD dwProcessID = GetSpecifiedProcessId(m_tstrName);
			HWND hWnd = GetSpecifiedProcessHwnd(dwProcessID);

			m_hHook = InitHookProc(hWnd, GetWindowThreadProcessId(hWnd, 0));

			DWORD dwErrorCode = GetLastError();
		}


		//卸载钩子
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


		//获取指定进程的窗口句柄
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

		//获取指定进程的的线程ID
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
		DWORD m_dwID;                    //m_iType值 4为进程ID， 5为窗口句柄
		tstring m_tstrName;              //m_iType值 1为进程名 2为窗口类名  3为窗口标题
		tstring m_tstrModuleName;
	};
}

//////////////////////////////////////////////////////////////////////////
#endif