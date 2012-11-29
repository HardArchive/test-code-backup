//file TraceUtil.h
/***************************************************************************************************
* 1�� Class      �� PathUtil
* 2�� Version    �� 1.0
* 3�� Description�� 
* 4�� Author     �� QNA (http://www.9cpp.com/)
* 5�� Created    �� 2012-9-7 16:27:44
* 6�� History    �� 
* 7�� Remark     �� 
****************************************************************************************************/
#ifndef __PATH_UTIL_H_
#define __PATH_UTIL_H_
#include <Windows.h>

namespace QNA
{
	class CPathUtil
	{
	public:
		virtual bool TraversalDoWork(const TCHAR* ptInFilePath)
		{
			//doSomethine			
		}

		//����Ŀ¼
		int DirectoryTraversal(const TCHAR* ptInPath, const TCHAR* ptInFileTpye)
		{
			HANDLE hFind = NULL;
			WIN32_FIND_DATA stuWFD = {0};	
			TCHAR tszFullPath[MAX_PATH] = {0};
			TCHAR tszFindFilter[MAX_PATH] = {0};
			DWORD dwAttributes = 0;

			if (!(ptInPath && ptInFileTpye))	return -1;
			//��鴴�����ļ���·��
			if (!ChickDirExist(ptInPath))
			{
				if (!CreateMultipleDirectory(ptInPath))
					return -2;				
			}
			_stprintf(tszFindFilter, _T("%s\\*.cpp"), ptInPath);    //�����ļ�����

			if ((hFind = FindFirstFile(tszFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
			{
				return -3;
			}

			do
			{
				//���˱���Ŀ¼�͸�Ŀ¼
				if(_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0)
					continue;
				ZeroMemory(tszFullPath, sizeof(tszFullPath));
				_stprintf(tszFullPath, _T("%s\\%s"), ptInPath, stuWFD.cFileName);  //�õ��ļ�(��)����·��

				//����ҵ�����Ŀ¼��������Ŀ¼���еݹ�
				if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{	
					if (1 != DirectoryTraversal(tszFullPath, ptInFileTpye))
					{
						FindClose(hFind);
						return -4;
					}

				}
				else 
				{//�ҵ������ļ�
					if (!TraversalDoWork(tszFullPath))
					{
						FindClose(hFind);
						return -5;
					}
				}	
			}while (FindNextFile(hFind, &stuWFD));

			FindClose(hFind);

			return 1;



		}
		//ɾ���ļ���
		//��鵱ǰĿ¼��Ҫ���ļ����Ƿ����,��������ڷ���false�����ڷ���true
		bool ChickDirExist(const TCHAR* ptInPath)
		{
			if (!ptInPath || _tcsclen(ptInPath)<2)	return false;

			//����·���Ƿ����	����������򴴽�
			if (GetFileAttributes(ptInPath) != FILE_ATTRIBUTE_DIRECTORY)	return false;
			return true;
		}

		//�����༶Ŀ¼���ɹ�����true�� ʧ�ܷ���false
		bool CreateMultipleDirectory(const TCHAR* ptInPath)
		{
			int iLen = 0;    
			TCHAR* ptTemp = NULL;		
			TCHAR tszPath[MAX_PATH] = {0};
			TCHAR tszTemPath[MAX_PATH] = {0};
			_tcscpy_s(tszPath, ptInPath);         //���Ҫ������Ŀ¼�ַ���

			//�������Ƿ���ȷ
			if (!tszPath || _tcsclen(tszPath)<4)
				return false;

			//������ȥ��β��Ϊ'\\'���ַ�
			if (_T('\\') == tszPath[_tcsclen(tszPath)-1])
			{
				tszPath[_tcsclen(tszPath)-1] = 0;
			}

			ptTemp = _tcsrchr(tszPath, _T('\\'));  //��β�������ַ�
			iLen = _tcsclen(ptTemp);
			_tcsncpy_s(tszTemPath, tszPath, _tcsclen(tszPath)-iLen); //�õ���Ŀ¼·��

			//���鸸��·���Ƿ����  		
			if (ChickDirExist(tszTemPath))
			{   //��������򴴽���Ŀ¼
				if (!CreateDirectory(tszPath, NULL))
				{	//�����ļ���ʧ��
					::MessageBox(NULL, tszPath, _T("�����ļ���ʧ��!!!!!"), MB_OK);
					return false;
				}
				else
				{
					SetFileAttributes(tszPath, FILE_ATTRIBUTE_NORMAL);	
					return true;
				}
				return true;
			}	
			else
			{	//�����������ݹ�
				if (!CreateMultipleDirectory(tszTemPath))
				{
					return false;
				}
				else
				{   //��Ŀ¼��������򴴽���Ŀ¼
					if (!CreateMultipleDirectory(tszPath))
						return false;		
				}
			}

			return true;
		}

		//��ȡ��ǰ��������Ŀ¼ �ɹ�����true��ʧ�ܷ���false
		bool GetExePath(TCHAR* ptInPath)
		{
			TCHAR* ptTem = NULL;
			TCHAR tszTemp[MAX_PATH] = {0};
			//��ȡ��ǰĿ¼  //�����ǻ�ȡ��ǰ�����ļ�������·�� 
			if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptInPath)
				return false; 

			ptTem = _tcsrchr(tszTemp, _T('\\'));
			memcpy(ptInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
			return true;
		}

		//��ȡָ���ļ����ȣ���λ���ֽڣ� 
		DWORD GetFileLen(const TCHAR* ptInFilePath)
		{
			//��һ�ַ���
			DWORD dwFileLen = 0;	
			HANDLE hFile = NULL;     
			hFile = CreateFile(ptInFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);       
			dwFileLen = GetFileSize(hFile, NULL);       
			CloseHandle(hFile);     

			////�ڶ��ַ���
			//FILE* pFile = NULL;
			//pFile = ::fopen(ptszInFilePath, "rb");
			//if(pFile == NULL)
			//	return 0;
			//
			//::fseek(pFile, 0, SEEK_SET);    //��λ��ʼ
			//dwFileLen = ::ftell(pFile);
			//fseek(pFile, 0, SEEK_END);  
			//dwFileLen = ::ftell(pFile) - dwFileLen;
			//fclose(pFile);
			//pFile = NULL;

			return dwFileLen;
		}
		//��ȡ����·��
		bool GetDesktopPath(TCHAR* ptOutDestopPath)
		{
			// Get the directory for Windows Desktop. This is
			// stored in the Registry under HKEY_CURRENT_USER\Software\
			// Microsoft\Windows\CurrentVersion\Explorer\Shell Folders\Desktop.
			HKEY hCU;
			DWORD lpType;
			ULONG ulSize = MAX_PATH;
			TCHAR tszPath[MAX_PATH] = {0};
			const TCHAR tszRegKey[] = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Shell Folders");
			if (!ptOutDestopPath) return false;

			if(ERROR_SUCCESS != ::RegOpenKeyEx(HKEY_CURRENT_USER, tszRegKey, 0, KEY_QUERY_VALUE, &hCU))	return false;

			if(ERROR_SUCCESS != ::RegQueryValueEx(hCU, _T("Desktop"), NULL, &lpType, (BYTE *)&tszPath, &ulSize))
			{
				RegCloseKey(hCU);
				return false;
			}

			_tcscpy_s(ptOutDestopPath, _tcslen(tszPath), tszPath);
			return true;
		}

	};


}
#endif

