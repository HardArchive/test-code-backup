//file DelayLoadUtil.h
/***************************************************************************************************
* 1�� File       �� DelayLoaderUtil.h
* 2�� Version    �� 1.
* 3�� Description�� DLL�ӳټ��ش�����  ��ͨ������ȡ�����ࡢ����������������
* 4�� Author     �� RG (http://www.9cpp.com/)
* 5�� Created    �� 2013-7-2 10:21:28
* 6�� History    �� 
* 7�� Remark     �� �ӳټ�����Ϊ�˱���������ʱ����DLL���³����������� 
****************************************************************************************************/
#ifndef __DELAY_LOAD_UTIL_H__
#define __DELAY_LOAD_UTIL_H__

//���û�а���tchar�����
#ifndef _INC_TCHAR
#include <tchar.h>
#endif

#ifdef _UNICODE
#ifndef _XSTRING_
#include <xstring>
#endif
#else
#ifndef _STRING_
#include <string>
#endif
#endif

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif
using namespace std;

namespace RG
{

	class TDelayLoadUtil
	{
	public:
		TDelayLoadUtil(const TCHAR* ptDllPath)
			:m_tstrDllPath(ptDllPath)
		{m_hModuleDll = NULL;}

		TDelayLoadUtil(const tstring& tstrDllPath)
			:m_tstrDllPath(tstrDllPath)
		{m_hModuleDll = NULL;}

		TDelayLoadUtil(HMODULE hModuleDll)
			:m_hModuleDll(hModuleDll)
		{m_tstrDllPath.clear();}

		~TDelayLoadUtil()
		{
			//ֱ�Ӵ���������Ĳ�ж��
			if (!m_tstrDllPath.empty())
			{
				UnloadDLL();
				m_tstrDllPath.c_str();
			}	
		}

		//ȡ����������(��)ָ��
		FARPROC GetProcAddress(const char* pProcName)
		{			
			if (NULL == m_hModuleDll)
			{
				LoadDLL();
			}
			FARPROC pfunc = NULL;
			pfunc = ::GetProcAddress(m_hModuleDll, pProcName);
			if (!pfunc)
			{
				printf("Error getting address of function '%s' in DLL: %s\n", pProcName, m_tstrDllPath.c_str());
				int iError = ::GetLastError();
				UnloadDLL();
				return false;
			}
			
			return pfunc;
		}

		//ȡ����������(��)ָ��
		FARPROC GetProcAddress(const string& strProcName)
		{			
			if (NULL == m_hModuleDll)
			{
				LoadDLL();
			}
			FARPROC pfunc = NULL;
			pfunc = ::GetProcAddress(m_hModuleDll, strProcName.c_str());

			if (!pfunc)
			{
				printf("Error getting address of function '%s' in DLL: %s\n", strProcName.c_str(), m_tstrDllPath.c_str());
				UnloadDLL();
				return false;
			}

			return pfunc;
		}

		bool LoadDLL()
		{
			if (m_hModuleDll)
				return true;

			printf("Loading DLL: %s...\n", m_tstrDllPath.c_str());


			m_hModuleDll = ::LoadLibrary(m_tstrDllPath.c_str());
			int iError = ::GetLastError();
			if (!m_hModuleDll)
			{
				printf("Error loading DLL: %s\n", m_tstrDllPath.c_str());
				return false;
			}

			return true;
		}

		void UnloadDLL()
		{
			if (m_hModuleDll)
			{
				printf("Unloading DLL: %s...\n", m_tstrDllPath.c_str());
				::FreeLibrary(m_hModuleDll);
				m_hModuleDll = NULL;
			}
		}

		bool IsLoaded() const
		{
			return NULL != m_hModuleDll;
		}

	private:
		HMODULE m_hModuleDll;
		std::tstring m_tstrDllPath;
	};
}
#endif /*__DELAY_LOAD_UTIL_H__*/

