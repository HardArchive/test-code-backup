//file RegUtil.h
/*************************************************************************************************** 
* 1、 Class      ： CRegUtil
* 2、 Version    ： 1.0
* 3、 Description： 注册表操作辅助类
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-9-7 11:01:50
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/ 
#ifndef __REG_UNTIL_H__
#define __REG_UNTIL_H__
namespace QNA
{

	class CRegUtil
	{
	public:
		CRegUtil(){}
		~CRegUtil(){}


	public:
		// 读取注册表项
		bool GetRegValue(LPCTSTR strItem, CString& strValue)
		{
			HKEY  hkey;
			LONG  retval;
			DWORD dwType = REG_SZ, dSize = 100;	
			char  sTemp[100];

			sTemp[0] = '\0';

			retval = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_QUERY_VALUE, &hkey);

			if (retval != 0) 
			{
				RegCloseKey(hkey);
				return false;
			}

			if(RegQueryValueEx(hkey, strItem, 0,&dwType, (LPBYTE)sTemp, (LPDWORD)&dSize) != 0)
				return false;
			strValue = (LPCSTR)sTemp;

			RegCloseKey(hkey);

			return true;
		}
		// 读取注册表项
		bool GetRegValue(LPCTSTR strItem, int& dwValue)
		{
			HKEY  hkey;
			LONG  retval;
			DWORD dwType = REG_DWORD, dSize = 100;	

			retval = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_QUERY_VALUE, &hkey);

			if (retval != 0) 
			{
				RegCloseKey(hkey);
				return false;
			}

			dSize = 4;
			if(RegQueryValueEx(hkey, strItem, NULL,&dwType, (LPBYTE)&dwValue, (LPDWORD)&dSize) != 0) return false;

			RegCloseKey(hkey);

			return true;
		}

		// 读取注册表项
		bool GetRegValue(LPCTSTR strItem, DWORD& dwValue)
		{
			HKEY  hkey;
			LONG  retval;
			DWORD dwType = REG_DWORD, dSize = 100;	

			retval = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_QUERY_VALUE, &hkey);

			if (retval != 0) 
			{
				RegCloseKey(hkey);
				return false;
			}

			dSize = 4;
			if(RegQueryValueEx(hkey, strItem, NULL,&dwType, (LPBYTE)&dwValue, (LPDWORD)&dSize) != 0) return false;

			RegCloseKey(hkey);

			return true;
		}

		bool GetRegValue( LPCTSTR strItem, char* strValue )
		{
			HKEY  hkey;
			LONG  retval;
			DWORD dwType = REG_SZ, dSize = 100;	

			strValue[0] = '\0';

			retval = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_QUERY_VALUE, &hkey);

			if (retval != 0) 
			{
				RegCloseKey(hkey);
				return false;
			}

			if(RegQueryValueEx(hkey, strItem, 0,&dwType, (LPBYTE)strValue, (LPDWORD)&dSize) != 0)
				return false;

			RegCloseKey(hkey);

			return true;
		}

		// 修改注册表项
		bool SetRegValue(LPCTSTR strItem, DWORD& dwValue)
		{
			HKEY  hkey;
			LONG  retval;
			DWORD dwType = REG_DWORD, dSize = 100;	
			char  sTemp[100];

			sTemp[0] = '\0';

			retval = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpSubKey, 0, KEY_SET_VALUE, &hkey);

			if (retval != 0) 
			{
				RegCloseKey(hkey);
				return false;
			}

			if (RegSetValueEx(hkey,strItem,0,dwType,(BYTE *)&dwValue,4) !=  ERROR_SUCCESS) return false;

			RegCloseKey(hkey);

			return true;
		}

	};
}
#endif