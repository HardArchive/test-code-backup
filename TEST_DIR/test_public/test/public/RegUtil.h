//file RegUtil.h
/*************************************************************************************************** 
* 1、 Class      ： CRegUtil
* 2、 Version    ： 1.0
* 3、 Description： 注册表操作辅助类
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2012-9-7 11:01:50
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/ 
#ifndef __REG_UNTIL_H__
#define __REG_UNTIL_H__
#include <assert.h>
namespace RG
{
	class CRegUtil
	{
	private:
		//打开一个指定的注册表键, 默认主键为HKEY_LOCAL_MACHINE, 默认键不存在不创建
		DWORD
			OpenRegKey(
			__in LPTSTR ptszRegKey,
			__out HKEY* phKey)
		{
			return OpenRegKey(
				ptszRegKey,
				FALSE,
				phKey);
		}

		//打开一个指定的注册表键, 默认主键为HKEY_LOCAL_MACHINE, bCreate为真则键不存在则创建，否则不创建
		DWORD
			OpenRegKey(
			__in const LPTSTR ptszRegKey,
			BOOL bCreate,
			__out HKEY* phKey)
		{
			return OpenRegKey(
				HKEY_LOCAL_MACHINE,
				ptszRegKey,
				bCreate,
				phKey);
		}
		
		//打开一个指定的注册表键, hHive主键名， hHive子键名， phKey得到的将要打开键的句柄
		//如果过程执行成功，返回值是 ERROR_SUCCESS。如果功能失败，返回一个非零值，
		//错误码在 Winerror.h 定义。可以使用 FormatMessage 函数 和 FORMAT_MESSAGE_FROM_SYSTEM 标记获得一个分类的错误描述。
		DWORD
			OpenRegKey(
			HKEY hHive,
			__in LPTSTR ptszRegKey,
			BOOL bCreate,        
			__out HKEY* phKey)
		{
			DWORD dwResult = ERROR_SUCCESS;

			if (hHive == NULL)
			{
				return ERROR_INVALID_PARAMETER;
			}

			dwResult = RegOpenKeyEx(
				hHive,
				ptszRegKey,
				0 /* 保留，设为0  */,
				KEY_ALL_ACCESS | KEY_WOW64_64KEY,
				phKey);

			if (dwResult != ERROR_SUCCESS)
			{
				if (bCreate == FALSE)
				{
					goto cleanup_open;
				}
				else
				{
					// 尝试创建的键
					dwResult = RegCreateKeyEx(
						hHive, 
						ptszRegKey, 
						0, 
						NULL,
						REG_OPTION_NON_VOLATILE, 
						KEY_ALL_ACCESS | KEY_WOW64_64KEY, 
						NULL,
						phKey, 
						NULL);

					if (dwResult != ERROR_SUCCESS)
					{
						goto cleanup_open;
					}
				}
			}

cleanup_open:
			return dwResult;
		}

	public:
		//获取一个注册表项或值的数据，默认主键HKEY_LOCAL_MACHINE
		DWORD
			GetRegistryValue(
			__in LPTSTR ptszRegKey,
			__in LPTSTR ptszValueName,
			DWORD ValueType,
			__deref_out PVOID pData)
		{
			return GetRegistryValue(
				HKEY_LOCAL_MACHINE,
				ptszRegKey,
				ptszValueName,
				ValueType,
				pData);
		}

		//获取一个注册表项或值的数据
		//hHive主键名 ptszRegKey子键名 ptszValueName 值名 dwValueType值类型 pData输出值内容指针
		//零表示成功。负值都代表一个错误代码 正值表示需要的空间长度
		//注意：pData缓冲区空间是必须足够的大不然会返回错误
		DWORD 
			GetRegistryValue(
			HKEY hHive,
			__in LPTSTR ptszRegKey,
			__in LPTSTR ptszValueName,
			DWORD dwValueType,
			__deref_out PVOID pData)
		{    
			HKEY hKey = NULL;
			DWORD dwResult = ERROR_SUCCESS;
			DWORD dwValueSize = 0;       //数据的长度

			if (ptszRegKey == NULL ||ptszValueName == NULL)
			{				
				goto cleanup_getvalue;
			}

			//成功打开键取值
			dwResult = OpenRegKey(hHive, ptszRegKey, FALSE, &hKey);
			if( dwResult != ERROR_SUCCESS )	
			{
				goto cleanup_getvalue;
			}

			//查询的数据类型和 BufferSize。
			dwResult = RegQueryValueEx(
				hKey,
				ptszValueName,
				0,
				&dwValueType,
				NULL,
				&dwValueSize
				);
			if( dwResult != ERROR_SUCCESS )
			{
				goto cleanup_getvalue;
			}

			//当缓冲区指向NULL时 返回 数据长度
			if (pData == NULL) return dwValueSize;

			//读取键值到缓冲区
			dwResult  = RegQueryValueEx(
				hKey,
				ptszValueName,
				0,
				&dwValueType,
				(LPBYTE)(pData),
				&dwValueSize);

			if (dwResult != ERROR_SUCCESS)
			{
				goto cleanup_getvalue;
			}
			
cleanup_getvalue:

			if (hKey != NULL)
			{
				RegCloseKey(hKey);  
			}
			return dwResult;     
		}

		//设置键值 ，默认主键HKEY_LOCAL_MACHINE
		DWORD
			SetRegistryValue(
			__in const LPTSTR ptszKey,
			__in_opt const LPTSTR ptszSubKey,
			__in const LPTSTR ptszValueName,
			DWORD dwType,
			__in_bcount(cbData) const BYTE* pData,
			DWORD cbData)
		{
			return SetRegistryValue(
				HKEY_LOCAL_MACHINE,
				ptszKey,
				ptszSubKey,
				ptszValueName,
				dwType,
				pData,
				cbData);
		}


		DWORD
			SetRegistryValue(
			HKEY hHive,
			__in const LPTSTR ptszKey,
			__in_opt const LPTSTR ptszSubKey,
			__in_opt const LPTSTR ptszValueName,
			DWORD dwType,
			__in_bcount(cbData) const BYTE* pData,
			DWORD cbData)
			//
			// Sets value pData of type dwType to registry key 
			// hHive\ptszKey\ptszSubKey
			// If ptszSubKey is NULL then value is set to hHive\ptszKey
			// If hHive\ptszKey or ptszSubKey is not present, the keys are created.
			//

		{
			DWORD dwResult = ERROR_SUCCESS;
			HKEY hKey = NULL;	 	 		// handle to the reg key
			HKEY hSubKey = NULL;	 	 		// handle to the reg key   


			if (hHive == NULL || ptszKey == NULL)
			{
				return ERROR_INVALID_PARAMETER;
			}

			// Try to open or create the registry key
			dwResult = OpenRegKey(
				hHive,
				ptszKey,
				TRUE,
				&hKey);


			if ((dwResult != ERROR_SUCCESS) || (hKey == NULL))
			{
				goto cleanup_setvalue;                    
			}

			if (ptszSubKey != NULL)
			{
				// Need to open this key as well
				dwResult = OpenRegKey(
					hKey,
					ptszSubKey,
					TRUE,
					&hSubKey);


				if ((dwResult != ERROR_SUCCESS) || (hSubKey == NULL))
				{
					goto cleanup_setvalue;
				}
			}

			// Now set the value (if there is one).

			if (pData != NULL)
			{
				RegSetValueEx(
					(ptszSubKey == NULL)? hKey:hSubKey, 
					ptszValueName,      // if this is null, value will be set to the key itself 
					0, 
					dwType, 
					pData,
					cbData);
			}

cleanup_setvalue:
			if (hKey != NULL)
			{
				RegCloseKey(hKey);  
			}

			if (hSubKey != NULL)
			{
				RegCloseKey(hSubKey);
			}

			return dwResult;
		}

		//DWORD
		//	DeleteRegistryKey(
		//	__in const LPTSTR ptszSubKey)
		//	//
		//	// Call DeleteRegistryKey with HKEY_LOCAL_MACHINE
		//	//
		//{
		//	return DeleteRegistryKey(
		//		HKEY_LOCAL_MACHINE,
		//		ptszSubKey);
		//}

		//DWORD
		//	DeleteRegistryKey(
		//	HKEY hHive,
		//	__in const LPTSTR ptszSubKey)
		//	//
		//	//	Delete an entry in the registry of the form:
		//	//			HKEY_CLASSES_ROOT\wszKey\wszSubkey = wszValue
		//	//

		//{
		//	DWORD dwResult = ERROR_SUCCESS;

		//	if (hHive == NULL || ptszSubKey == NULL)
		//	{
		//		return ERROR_INVALID_PARAMETER;        
		//	}

		//	if (ptszSubKey != NULL)
		//	{
		//		// delete the registry key.
		//		dwResult = SHDeleteKey(
		//			hHive, 
		//			ptszSubKey);
		//	}

		//	return dwResult;
		//}


	};
}
#endif