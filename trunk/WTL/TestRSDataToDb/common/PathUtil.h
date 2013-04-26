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
#include <ShlObj.h>
#include <Shlwapi.h>
#pragma  comment(lib,"Shlwapi.lib")  
namespace RG
{
	class CPathUtil
	{
	public:
		//ȷ��һ���ļ���Ŀ¼���ļ�ϵͳ�����·���Ƿ�����Ч�ġ�
		//��鵱ǰĿ¼����ļ��Ƿ����,��������ڷ���false�����ڷ���true
		// ������ļ����ڷ���TRUE�����򷵻�false
		bool CheckFileExists(const TCHAR* ptInPath)
		{
			return (bool)::PathFileExists(ptInPath);
		}

		//���ָ����Ŀ¼�Ƿ����. ���ҵ���Ŀ¼���� FILE_ATTRIBUTE_DIRECTORY	��δ�ҵ� ����FALSE.
		bool CheckDirExist(const TCHAR* ptInPath)
		{
			return (bool)::PathIsDirectory(ptInPath);
		}


		//�����༶Ŀ¼���ɹ�����true�� ʧ�ܷ���false
		bool CreateMultipleDirectory(const TCHAR* ptInPath, bool bIsFilePath = false)
		{
			int iLen = 0;    
			PTCHAR ptTemp = NULL;		
			TCHAR tszPath[MAX_PATH*4] = {0};
			TCHAR tszTemPath[MAX_PATH*4] = {0};
			_tcscpy_s(tszPath, ptInPath);         //���Ҫ������Ŀ¼�ַ���

			//�������Ƿ���ȷ
			if (!tszPath || _tcsclen(tszPath)<4)
				return false;

			if (bIsFilePath)
			{
				ptTemp = _tcsrchr(tszPath, _T('\\'));  //��β�������ַ�
				iLen = _tcsclen(ptTemp);
				_tcsncpy_s(tszTemPath, tszPath, _tcsclen(tszPath)-iLen); //�õ���Ŀ¼·��
				_tcsncpy_s(tszPath, tszTemPath, _tcsclen(tszTemPath));
			}
			//������ȥ��β��Ϊ'\\'���ַ�
			if (_T('\\') == tszPath[_tcsclen(tszPath)-1])
			{
				tszPath[_tcsclen(tszPath)-1] = 0;
			}

			ptTemp = _tcsrchr(tszPath, _T('\\'));  //��β�������ַ�
			iLen = _tcsclen(ptTemp);
			_tcsncpy_s(tszTemPath, tszPath, _tcsclen(tszPath)-iLen); //�õ���Ŀ¼·��

			//���鸸��·���Ƿ����  		
			if (PathIsDirectory(tszTemPath))
			{    //�����Ӽ�·���Ƿ����
				if (PathIsDirectory(tszPath)) return true;
				if (!::CreateDirectory(tszPath, NULL))
				{	//�����ļ���ʧ��
					::MessageBox(NULL, tszPath, _T("�����ļ���ʧ��!!!!!"), MB_OK);
					return false;
				}
				SetFileAttributes(tszPath, FILE_ATTRIBUTE_NORMAL);	
				return true;		
			}	
			else
			{	//�����������ݹ�
				if (!CreateMultipleDirectory(tszTemPath))	return false;

				//��Ŀ¼��������򴴽���Ŀ¼
				if (!CreateMultipleDirectory(tszPath))	return false;						
			}

			return true;
		}

		//��ȡ��ǰ��������Ŀ¼ �ɹ�����true��ʧ�ܷ���false
		bool GetExePath(PTCHAR ptInPath)
		{
			PTCHAR ptTem = NULL;
			TCHAR tszTemp[MAX_PATH] = {0};
			//��ȡ��ǰĿ¼  //�����ǻ�ȡ��ǰ�����ļ�������·�� 
			if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptInPath)
				return false; 

			ptTem = _tcsrchr(tszTemp, _T('\\'));
			memcpy(ptInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
			return true;
		}

		//��ȡָ���ļ����ȣ���λ���ֽڣ� 
		DWORD GetFileLen(const PTCHAR ptInFilePath)
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

		//�����ļ�����
		//#define FILE_SHARE_READ                 0x00000001  
		//#define FILE_SHARE_WRITE                0x00000002  
		//#define FILE_SHARE_DELETE               0x00000004        
		//#define FILE_ATTRIBUTE_READONLY             0x00000001    ֻ��
		//#define FILE_ATTRIBUTE_HIDDEN               0x00000002    ����
		//#define FILE_ATTRIBUTE_SYSTEM               0x00000004    ϵͳ
		//#define FILE_ATTRIBUTE_DIRECTORY            0x00000010    Ŀ¼
		//#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
		//#define FILE_ATTRIBUTE_DEVICE               0x00000040    �豸
		//bool SetFileAttributesEx(PTCHAR ptFileName, DWORD dwFileAttributes)
		//{
		//	::SetFileAttributes(ptFileName, dwFileAttributes); //������������
		//}

		//Ŀ¼ѡ��Ի��� hwndOwner �����ھ��, nIDDlgItem �༭��ID  
		bool GetSelectDirDlg(HWND hwndOwner, int nIDDlgItem, PTCHAR ptszTitle = _T("ѡ��·��"))
		{
			BROWSEINFO stuBI;
			TCHAR tszPathName[MAX_PATH] = {0};
			TCHAR tszPath[MAX_PATH] = {0}; //ѡ���Ŀ¼
			ZeroMemory(&stuBI, sizeof(BROWSEINFO));

			stuBI.hwndOwner = hwndOwner;
			stuBI.pszDisplayName = tszPathName;
			stuBI.lpszTitle = ptszTitle;
			stuBI.ulFlags = 0x0040 ;

			LPITEMIDLIST idl = SHBrowseForFolder(&stuBI);
			if(idl == NULL) return false;
			SHGetPathFromIDList(idl, tszPath);
			SetDlgItemText(hwndOwner, nIDDlgItem, tszPath);
			return true;
		}

		//������ �ļ��Ի��� hwndOwner �����ھ��, nIDDlgItem �༭��ID, bIsOpen true�� false����
		//eg ptszFilter _T("��ҳ�ļ�(*.html)\0*.html\0;��ҳ�ļ�(*.htm)\0*.htm\0;All Files(*.*)\0*.*\0")
		bool GetFileDlg(HWND hwndOwner, int nIDDlgItem, 
			const bool bIsOpen = false,
			const PTCHAR  ptszFilter =_T("All Files(*.*)\0*.*\0"),
			const PTCHAR ptszTitle = _T("ѡ���ļ�·��"))
		{
			TCHAR tszFilePath[MAX_PATH] = {0};

			OPENFILENAME ofn = {0};       
			TCHAR tszFile[MAX_PATH] = {0};       
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwndOwner;
			ofn.lpstrFile = tszFile;
			ofn.lpstrFile[0] = ' ';
			ofn.nMaxFile = sizeof(tszFile);
			ofn.lpstrFilter = ptszFilter;
			ofn.nFilterIndex = 1;
			ofn.lpstrTitle = ptszTitle;

			if (bIsOpen)
			{
				if (false == GetOpenFileName(&ofn)) return false;
			}
			else
			{
				if (false == GetSaveFileName(&ofn))	return false;
			}
			SetDlgItemText(hwndOwner, nIDDlgItem, ofn.lpstrFile);
		}
		//��ȡ����·��
		bool GetDesktopPath(PTCHAR ptOutDestopPath)
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

