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

#define CPathUtil_Administrative_Tools 0x01                                //����ʼ���˵�\����\������
#define CPathUtil_AppData              0x02                                //Ӧ������
#define CPathUtil_Cache                0x03                                //����
#define CPathUtil_CD_Burning           0x04                                //CD��¼
#define CPathUtil_Cookies              0x05                                //Cookies
#define CPathUtil_Desktop              0x06                                //Desktop
#define CPathUtil_Favorites            0x07                                //Favorites
#define CPathUtil_Fonts                0x08                                //Fonts
#define CPathUtil_History              0x09                                //History
#define CPathUtil_Local_AppData        0x0A                                //
#define CPathUtil_Local_Settings       0x0B                                //
#define CPathUtil_My_Music             0x0C                                //
#define CPathUtil_My_Pictures          0x0D                                //
#define CPathUtil_My_Video             0x0E                                //
#define CPathUtil_NetHood              0x0F                                //
#define CPathUtil_Personal             0x10                                //
#define CPathUtil_PrintHood            0x11                                //
#define CPathUtil_Programs             0x12                                //
#define CPathUtil_Recent               0x13                                //
#define CPathUtil_SendTo               0x14                                //
#define CPathUtil_Start_Menu           0x15                                //����ʼ���˵�\����
#define CPathUtil_SendTo               0x16                                //����ʼ���˵�
#define CPathUtil_Startup              0x17                                //����ʼ���˵�\����\����
#define CPathUtil_Templates            0x18                                //ģ��
//ϵͳ��ֽĿ¼  
//��ӡ����Ŀ¼
//
namespace RG
{
	class CPathUtil
	{
	public:
		virtual bool TraversalDoWork(const PTCHAR ptInFilePath)
		{
			//doSomethine		
			return true;
		}

		//����Ŀ¼
		int DirectoryTraversal(const PTCHAR ptInPath, const PTCHAR ptInFileTpye)
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
			_stprintf_s(tszFindFilter, MAX_PATH, _T("%s\\*%s"), ptInPath, ptInFileTpye);    //�����ļ�����

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
				_stprintf_s(tszFullPath, MAX_PATH, _T("%s\\%s"), ptInPath, stuWFD.cFileName);  //�õ��ļ�(��)����·��

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

		//****************************ɾ���ļ���**************************************************************************
		
		//��鵱ǰĿ¼��Ҫ���ļ����Ƿ����,��������ڷ���false�����ڷ���true
		bool ChickDirExist(const PTCHAR ptInPath)
		{
			if (!ptInPath || _tcsclen(ptInPath)<2)	return false;

			//����·���Ƿ����	����������򴴽�
			if (GetFileAttributes(ptInPath) != FILE_ATTRIBUTE_DIRECTORY)	return false;
			return true;
		}

		//�����༶Ŀ¼���ɹ�����true�� ʧ�ܷ���false
		bool CreateMultipleDirectory(const PTCHAR ptInPath)
		{
			int iLen = 0;    
			PTCHAR ptTemp = NULL;		
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
					//::MessageBox(NULL, tszPath, _T("�����ļ���ʧ��!!!!!"), MB_OK);
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

