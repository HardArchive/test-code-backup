#ifndef __INI_H__
#define __INI_H__
/******************************************************************************* 
* 1�� Class      �� CIni
* 2�� Version    �� *.*
* 3�� Description�� INI�ļ���д������
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-7-18 10:30:07
* 6�� History    �� 
* 7�� Remark     �� ֧�ֿ��ֽںͶ��ֽ�
********************************************************************************/ 

#include <iostream>
#include <windows.h>
#define DEBUG
#include <assert.h>

#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif


namespace QNA
{
#pragma warning(disable: 4800)

	class CIni
	{
	public:
		CIni()
		{
			m_IniFilename.clear();
		}
		CIni(LPCTSTR IniFilename)
		{
			SetIniFilename(IniFilename);
		}
		virtual ~CIni()
		{
			::WritePrivateProfileString(NULL, NULL, NULL, m_IniFilename.c_str());
		}

		//����
	public:
		//���õ�ǰ��ini�ļ�ʹ��
		bool SetIniFilename(LPCTSTR IniFilename)
		{
			assert(IniFilename);
			m_IniFilename = IniFilename;
			if(m_IniFilename.empty()) 
				return false;
			return true;
		}

		// ��INI�ļ���ȡ����
		UINT GetInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault=0)
		{
			TCHAR tszDefault[MAX_PATH/2] = {0};
			if(m_IniFilename.empty()) 
				return 0; 

			_stprintf_s(tszDefault, sizeof(nDefault), _T("%d"), nDefault);

			std::tstring s = GetString(lpszSection, lpszEntry, tszDefault);
			return _ttol(s.c_str());
		}

		//��ȡboolֵ 
		bool GetBoolean(LPCTSTR lpszSection, LPCTSTR lpszEntry, bool bDefault=false)
		{
			TCHAR tszDefault[MAX_PATH*2] = {0};
			assert(lpszSection);
			assert(lpszEntry);
			if(m_IniFilename.empty()) 
				return 0; // error
			_stprintf_s(tszDefault, sizeof(bDefault), _T("%d"), bDefault);
			return !GetString(lpszSection, lpszEntry, tszDefault).compare(_T("Y"));
		}

		//��INI�ļ���ȡһ���ַ���
		std::tstring GetString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszDefault=NULL)
		{
			long ret = 0;
			TCHAR tszTem[MAX_PATH*2] = {0};

			assert(lpszSection);
			assert(lpszEntry);

			if(m_IniFilename.empty())
				return std::tstring();		
			ret = ::GetPrivateProfileString(lpszSection, lpszEntry, lpszDefault, tszTem, MAX_PATH*2, m_IniFilename.c_str());

			if(0 == ret) 
				return std::tstring(lpszDefault);
			return std::tstring(tszTem);
		}
		//��ָ���ֶ�ָ����д������ֵ
		bool WriteInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
		{
			TCHAR tszValue[MAX_PATH*2] = {0};
			assert(lpszSection);
			assert(lpszEntry);
			_stprintf_s(tszValue, sizeof(nValue), _T("%d"), nValue);
			return WriteString(lpszSection, lpszEntry, tszValue);
		}
		//��ָ���ֶ�ָ����д��boolֵ
		bool WriteBoolean(LPCTSTR lpszSection, LPCTSTR lpszEntry, bool bValue)
		{
			std::tstring strTem;
			bValue ? strTem = _T("Y") : strTem = _T("N");
			return WriteString(lpszSection, lpszEntry, strTem.c_str());
		}
		//��ָ���ֶ�ָ����д���ַ���
		bool WriteString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
		{
			assert(lpszSection);
			assert(lpszEntry);
			if(m_IniFilename.empty())
				return false;
			return ::WritePrivateProfileString(lpszSection, lpszEntry, lpszValue, m_IniFilename.c_str());
		}	
		//ɾ����
		bool DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszEntry)
		{
			assert(lpszSection);
			assert(lpszEntry);
			if(m_IniFilename.empty())
				return false;
			return ::WritePrivateProfileString(lpszSection, lpszEntry, NULL, m_IniFilename.c_str());
		}
		//�ֶ�
		bool DeleteSection(LPCTSTR lpszSection)
		{
			assert(lpszSection);
			if(m_IniFilename.empty())
				return false;
			return ::WritePrivateProfileString(lpszSection, NULL, NULL, m_IniFilename.c_str());
		}

		//����
	protected:
		std::tstring m_IniFilename;  //INI�ļ���
	};
}

#endif