//file FixMemory.h
/***************************************************************************************************
* 1、 File       ： FixMemory.h
* 2、 Version    ： 1.0
* 3、 Description： 读写查找指定进程的内存值
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-4-26 16:44:05
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
#ifndef __FIX_MENORY_H__
#define __FIX_MENORY_H__
#include "GeneralHelper.h"


namespace RG
{
	class CFixMemory
	{
	public:
		CFixMemory()
		{
			m_hProcess;        //进程ID
			m_lpBaseAddress;   //基本地址
		}
		//CFixMemory(tstring& tstr)
		~CFixMemory(){Delete();}

	public:
		bool New(tstring& tstrProcessName, DWORD dwBufLen)
		{
			bool bRet = false;
			// 分配虚拟内存.
			m_lpBaseAddress = VirtualAllocEx(Open(tstrProcessName), 0, dwBufLen, 
				MEM_COMMIT, PAGE_READWRITE);

			if (m_lpBaseAddress) bRet = true;
			return bRet;
		}

		bool New(HWND hWnd, DWORD dwBufLen)
		{
			bool bRet = false;
			// 分配虚拟内存.
			m_lpBaseAddress = VirtualAllocEx(Open(hWnd), 0, dwBufLen, 
				MEM_COMMIT, PAGE_READWRITE);

			if (m_lpBaseAddress) bRet = true;
			return bRet;
		}

		//清除申请的空间
		void Delete()
		{
			if (m_lpBaseAddress)
			{
				// 释放虚拟内存.
				if (m_hProcess)
				{
					VirtualFreeEx(m_hProcess, m_lpBaseAddress, 0, MEM_RELEASE);
					Close();
				}
				m_lpBaseAddress = NULL;				
			}

			if (m_hProcess)	Close();
		}
		bool Write(PVOID pBuffer, DWORD dwBufLen)
		{
			// 把字符串写入hProcess进程的内存.
			return WriteProcessMemory(m_hProcess, m_lpBaseAddress, pBuffer, dwBufLen, NULL);
		}
		bool Read(PVOID pBuffer, DWORD dwBufLen)
		{
			return ReadProcessMemory(m_hProcess, m_lpBaseAddress, pBuffer, dwBufLen, NULL);
		}

	private:
		//跟据进程名打开进程并取得进程句柄
		HANDLE Open(tstring& tstrProcessName)
		{
			return OpenProcess(
				PROCESS_ALL_ACCESS, 
				FALSE, 
				GetSpecifiedProcessId(tstrProcessName)
				);
		}

		//跟据进程的窗口句柄打开进程并取得进程句柄
		HANDLE Open(HWND hWnd)
		{
			DWORD dwPID = 0;
			GetWindowThreadProcessId(hWnd, (DWORD*)&dwPID);
			if (0 == dwPID)	return NULL;
			return OpenProcess (PROCESS_ALL_ACCESS,FALSE, dwPID);
		}

		void Close()
		{
			if (NULL != m_hProcess)
			{
				CloseHandle(m_hProcess);
				m_hProcess = NULL;
			}
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
	private:
		HANDLE m_hProcess;        //进程ID
		LPVOID m_lpBaseAddress;   //基本地址
	};
}

#endif /*__FIX_MENORY_H__*/
