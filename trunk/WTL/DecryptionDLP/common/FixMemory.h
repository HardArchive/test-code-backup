//file FixMemory.h
/***************************************************************************************************
* 1�� File       �� FixMemory.h
* 2�� Version    �� 1.0
* 3�� Description�� ��д����ָ�����̵��ڴ�ֵ
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-4-26 16:44:05
* 6�� History    �� 
* 7�� Remark     �� 
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
			m_hProcess;        //����ID
			m_lpBaseAddress;   //������ַ
		}
		//CFixMemory(tstring& tstr)
		~CFixMemory(){Delete();}

	public:
		bool New(tstring& tstrProcessName, DWORD dwBufLen)
		{
			bool bRet = false;
			// ���������ڴ�.
			m_lpBaseAddress = VirtualAllocEx(Open(tstrProcessName), 0, dwBufLen, 
				MEM_COMMIT, PAGE_READWRITE);

			if (m_lpBaseAddress) bRet = true;
			return bRet;
		}

		bool New(HWND hWnd, DWORD dwBufLen)
		{
			bool bRet = false;
			// ���������ڴ�.
			m_lpBaseAddress = VirtualAllocEx(Open(hWnd), 0, dwBufLen, 
				MEM_COMMIT, PAGE_READWRITE);

			if (m_lpBaseAddress) bRet = true;
			return bRet;
		}

		//�������Ŀռ�
		void Delete()
		{
			if (m_lpBaseAddress)
			{
				// �ͷ������ڴ�.
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
			// ���ַ���д��hProcess���̵��ڴ�.
			return WriteProcessMemory(m_hProcess, m_lpBaseAddress, pBuffer, dwBufLen, NULL);
		}
		bool Read(PVOID pBuffer, DWORD dwBufLen)
		{
			return ReadProcessMemory(m_hProcess, m_lpBaseAddress, pBuffer, dwBufLen, NULL);
		}

	private:
		//���ݽ������򿪽��̲�ȡ�ý��̾��
		HANDLE Open(tstring& tstrProcessName)
		{
			return OpenProcess(
				PROCESS_ALL_ACCESS, 
				FALSE, 
				GetSpecifiedProcessId(tstrProcessName)
				);
		}

		//���ݽ��̵Ĵ��ھ���򿪽��̲�ȡ�ý��̾��
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
	private:
		HANDLE m_hProcess;        //����ID
		LPVOID m_lpBaseAddress;   //������ַ
	};
}

#endif /*__FIX_MENORY_H__*/
