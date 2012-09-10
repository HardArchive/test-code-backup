//file IniUtil.h
/*************************************************************************************************** 
* 1、 Class      ： CIniUtil
* 2、 Version    ： 1.1
* 3、 Description： INI文件操作辅助类
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-7-18 10:30:07
* 6、 History    ： 
* 7、 Remark     ： 支持宽字节和多字节
****************************************************************************************************/  
#ifndef __INI_UNTIL_H__
#define __INI_UNTIL_H__

#include <iostream>
#include <windows.h>
#include <assert.h>

namespace QNA
{
#ifdef _UNICODE
#define tstring wstring
#else
#define tstring string
#endif
//禁止BOOL强制转化提示
#pragma warning(disable: 4800)

	class CIniUtil
	{
	public:
		CIniUtil()
		{
			m_IniFilename.clear();
		}

		CIniUtil(LPCTSTR IniFilename)
		{
			SetIniFilename(IniFilename);
		}

		virtual ~CIniUtil()
		{
			::WritePrivateProfileString(NULL, NULL, NULL, m_IniFilename.c_str());
		}

		//方法
	public:
		//设置当前的ini文件使用
		bool SetIniFilename(LPCTSTR IniFilename)
		{
			assert(IniFilename);
			m_IniFilename = IniFilename;
			if(m_IniFilename.empty()) 
				return false;

			return true;
		}

		// 从INI文件读取整数
		UINT GetInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nDefault=0)
		{
			TCHAR tszDefault[MAX_PATH/2] = {0};
			if(m_IniFilename.empty()) 
				return 0; 

			_stprintf_s(tszDefault, MAX_PATH/2, _T("%d"), nDefault);

			std::tstring s = GetString(lpszSection, lpszEntry, tszDefault);
			return _ttol(s.c_str());
		}

		//读取bool值 
		bool GetBoolean(LPCTSTR lpszSection, LPCTSTR lpszEntry, bool bDefault=false)
		{
			TCHAR tszDefault[MAX_PATH/2] = {0};
			assert(lpszSection);
			assert(lpszEntry);
			if(m_IniFilename.empty()) 
				return 0; // error
			_stprintf_s(tszDefault, MAX_PATH/2, _T("%d"), bDefault);
			return !GetString(lpszSection, lpszEntry, tszDefault).compare(_T("Y"));
		}

		//从INI文件读取一个字符串
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

		//在指定字段指定键写入整型值
		bool WriteInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue)
		{
			TCHAR tszValue[MAX_PATH/2] = {0};
			assert(lpszSection);
			assert(lpszEntry);
			_stprintf_s(tszValue, MAX_PATH/2, _T("%d"), nValue);
			return WriteString(lpszSection, lpszEntry, tszValue);
		}

		//在指定字段指定键写入bool值
		bool WriteBoolean(LPCTSTR lpszSection, LPCTSTR lpszEntry, bool bValue)
		{
			std::tstring strTem;
			bValue ? strTem = _T("Y") : strTem = _T("N");
			return WriteString(lpszSection, lpszEntry, strTem.c_str());
		}

		//在指定字段指定键写入字符串
		bool WriteString(LPCTSTR lpszSection, LPCTSTR lpszEntry, LPCTSTR lpszValue)
		{
			assert(lpszSection);
			assert(lpszEntry);
			if(m_IniFilename.empty())
				return false;
			return ::WritePrivateProfileString(lpszSection, lpszEntry, lpszValue, m_IniFilename.c_str());
		}

		//删除键
		bool DeleteKey(LPCTSTR lpszSection, LPCTSTR lpszEntry)
		{
			assert(lpszSection);
			assert(lpszEntry);
			if(m_IniFilename.empty())
				return false;
			return ::WritePrivateProfileString(lpszSection, lpszEntry, NULL, m_IniFilename.c_str());
		}

		//字段
		bool DeleteSection(LPCTSTR lpszSection)
		{
			assert(lpszSection);
			if(m_IniFilename.empty())
				return false;
			return ::WritePrivateProfileString(lpszSection, NULL, NULL, m_IniFilename.c_str());
		}

		//获取当前程序所在目录
		bool GetExePath(LPTSTR pstrPath)
		{
			LPTSTR ptTem = NULL;
			TCHAR tszTemp[MAX_PATH] = {0};
			//获取当前目录  //这里是获取当前进程文件的完整路径 
			if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && pstrPath)
				return false; 

			ptTem = _tcsrchr(tszTemp, '\\');
			memcpy(pstrPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
			return true;
		}

		//变量
	protected:
		std::tstring m_IniFilename;  //INI文件名
	};
}
#endif