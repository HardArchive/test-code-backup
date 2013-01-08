//file RegUtil.h
/*************************************************************************************************** 
* 1�� Class      �� CRegUtil
* 2�� Version    �� 1.0
* 3�� Description�� ע������������
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-9-7 11:01:50
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/ 
#ifndef __REG_UNTIL_H__
#define __REG_UNTIL_H__
#include <assert.h>
namespace QNA
{
	class CRegUtil
	{
	public:
		//��һ��ָ����ע����, Ĭ������ΪHKEY_LOCAL_MACHINE, Ĭ�ϼ������ڲ�����
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

		//��һ��ָ����ע����, Ĭ������ΪHKEY_LOCAL_MACHINE, bCreateΪ������������򴴽������򲻴���
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
		
		//��һ��ָ����ע����, hHive�������� hHive�Ӽ����� phKey�õ��Ľ�Ҫ�򿪼��ľ��
		//�������ִ�гɹ�������ֵ�� ERROR_SUCCESS���������ʧ�ܣ�����һ������ֵ��
		//�������� Winerror.h ���塣����ʹ�� FormatMessage ���� �� FORMAT_MESSAGE_FROM_SYSTEM ��ǻ��һ������Ĵ���������
		DWORD
			OpenRegKey(
			HKEY hHive,
			__in LPTSTR ptszRegKey,
			BOOL bCreate,        
			__out HKEY* phKey)
		{
			DWORD result = ERROR_SUCCESS;

			if (hHive == NULL)
			{
				return ERROR_INVALID_PARAMETER;
			}

			result = RegOpenKeyEx(
				hHive,
				ptszRegKey,
				0 /* ��������Ϊ0  */,
				KEY_ALL_ACCESS | KEY_WOW64_64KEY,
				phKey);

			if (result != ERROR_SUCCESS)
			{
				if (bCreate == FALSE)
				{
					goto cleanup;
				}
				else
				{
					// ���Դ����ļ�
					result = RegCreateKeyEx(
						hHive, 
						ptszRegKey, 
						0, 
						NULL,
						REG_OPTION_NON_VOLATILE, 
						KEY_ALL_ACCESS | KEY_WOW64_64KEY, 
						NULL,
						phKey, 
						NULL);

					if (result != ERROR_SUCCESS)
					{
						goto cleanup;
					}
				}
			}

cleanup:
			return result;
		}

		//��ȡһ��ע������ֵ�����ݣ�Ĭ������HKEY_LOCAL_MACHINE
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

		//��ȡһ��ע������ֵ������
		//hHive������ ptszRegKey�Ӽ��� ptszValueName ֵ�� dwValueTypeֵ���� pData���ֵ����ָ��
		//���ʾ�ɹ�����ֵ������һ��������� ��ֵ��ʾ��Ҫ�Ŀռ䳤��
		//ע�⣺pData�������ռ��Ǳ����㹻�Ĵ�Ȼ�᷵�ش���
		DWORD 
			GetRegistryValue(
			HKEY hHive,
			__in LPTSTR ptszRegKey,
			__in LPTSTR ptszValueName,
			DWORD dwValueType,
			__deref_out PVOID pData)
		{    
			HKEY hKey = NULL;
			DWORD dwResult = 0;
			DWORD dwValueSize = 0;       //���ݵĳ���

			if (ptszRegKey == NULL ||ptszValueName == NULL)	return -1;

			//�ɹ��򿪼�ȡֵ
			dwResult = OpenRegKey(hHive, ptszRegKey, FALSE, &hKey);
			if( dwResult != ERROR_SUCCESS ) return -2;			

			//��ѯ���������ͺ� BufferSize��
			dwResult = RegQueryValueEx(
				hKey,
				ptszValueName,
				0,
				&dwValueType,
				NULL,
				&dwValueSize
				);
			if( dwResult != ERROR_SUCCESS ) return -3;

			//��������ָ��NULLʱ ���� ���ݳ���
			if (pData == NULL) return dwValueSize;

			//��ȡ��ֵ��������
			dwResult  = RegQueryValueEx(
				hKey,
				ptszValueName,
				0,
				&dwValueType,
				(LPBYTE)(pData),
				&dwValueSize);

			if (dwResult != ERROR_SUCCESS) return -4;

			return 0;        
		}

		//���ü�ֵ ��Ĭ������HKEY_LOCAL_MACHINE
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
			DWORD result = ERROR_SUCCESS;
			HKEY hKey = NULL;	 	 		// handle to the reg key
			HKEY hSubKey = NULL;	 	 		// handle to the reg key   


			if (hHive == NULL || ptszKey == NULL)
			{
				return ERROR_INVALID_PARAMETER;
			}

			// Try to open or create the registry key

			result = OpenRegKey(
				hHive,
				ptszKey,
				TRUE,
				&hKey);


			if ((result != ERROR_SUCCESS) || (hKey == NULL))
			{
				goto cleanup;                    
			}

			if (ptszSubKey != NULL)
			{
				// Need to open this key as well
				result = OpenRegKey(
					hKey,
					ptszSubKey,
					TRUE,
					&hSubKey);


				if ((result != ERROR_SUCCESS) || (hSubKey == NULL))
				{
					goto cleanup;
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

cleanup:

			if (hKey != NULL)
			{
				RegCloseKey(hKey);  
			}

			if (hSubKey != NULL)
			{
				RegCloseKey(hSubKey);
			}


			return result;

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
		//	DWORD result = ERROR_SUCCESS;

		//	if (hHive == NULL || ptszSubKey == NULL)
		//	{
		//		return ERROR_INVALID_PARAMETER;        
		//	}

		//	if (ptszSubKey != NULL)
		//	{
		//		// delete the registry key.
		//		result = SHDeleteKey(
		//			hHive, 
		//			ptszSubKey);
		//	}

		//	return result;
		//}


	};
}
#endif