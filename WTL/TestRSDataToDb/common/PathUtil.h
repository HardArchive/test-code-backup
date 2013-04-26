//file TraceUtil.h
/***************************************************************************************************
* 1、 Class      ： PathUtil
* 2、 Version    ： 1.0
* 3、 Description： 
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-9-7 16:27:44
* 6、 History    ： 
* 7、 Remark     ： 
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
		//确定一个文件或目录的文件系统对象的路径是否是有效的。
		//检查当前目录需或文件是否存在,如果不存在返回false，存在返回true
		// 如果该文件存在返回TRUE，否则返回false
		bool CheckFileExists(const TCHAR* ptInPath)
		{
			return (bool)::PathFileExists(ptInPath);
		}

		//检查指定的目录是否存在. 若找到该目录返回 FILE_ATTRIBUTE_DIRECTORY	若未找到 返回FALSE.
		bool CheckDirExist(const TCHAR* ptInPath)
		{
			return (bool)::PathIsDirectory(ptInPath);
		}


		//创建多级目录，成功返回true， 失败返回false
		bool CreateMultipleDirectory(const TCHAR* ptInPath, bool bIsFilePath = false)
		{
			int iLen = 0;    
			PTCHAR ptTemp = NULL;		
			TCHAR tszPath[MAX_PATH*4] = {0};
			TCHAR tszTemPath[MAX_PATH*4] = {0};
			_tcscpy_s(tszPath, ptInPath);         //存放要创建的目录字符串

			//检查参数是否正确
			if (!tszPath || _tcsclen(tszPath)<4)
				return false;

			if (bIsFilePath)
			{
				ptTemp = _tcsrchr(tszPath, _T('\\'));  //从尾部查找字符
				iLen = _tcsclen(ptTemp);
				_tcsncpy_s(tszTemPath, tszPath, _tcsclen(tszPath)-iLen); //得到父目录路径
				_tcsncpy_s(tszPath, tszTemPath, _tcsclen(tszTemPath));
			}
			//在这里去掉尾部为'\\'的字符
			if (_T('\\') == tszPath[_tcsclen(tszPath)-1])
			{
				tszPath[_tcsclen(tszPath)-1] = 0;
			}

			ptTemp = _tcsrchr(tszPath, _T('\\'));  //从尾部查找字符
			iLen = _tcsclen(ptTemp);
			_tcsncpy_s(tszTemPath, tszPath, _tcsclen(tszPath)-iLen); //得到父目录路径

			//检验父级路径是否存在  		
			if (PathIsDirectory(tszTemPath))
			{    //检验子级路径是否存在
				if (PathIsDirectory(tszPath)) return true;
				if (!::CreateDirectory(tszPath, NULL))
				{	//创建文件夹失败
					::MessageBox(NULL, tszPath, _T("创建文件夹失败!!!!!"), MB_OK);
					return false;
				}
				SetFileAttributes(tszPath, FILE_ATTRIBUTE_NORMAL);	
				return true;		
			}	
			else
			{	//如果不存在则递归
				if (!CreateMultipleDirectory(tszTemPath))	return false;

				//父目录创建完毕则创建子目录
				if (!CreateMultipleDirectory(tszPath))	return false;						
			}

			return true;
		}

		//获取当前程序所在目录 成功返回true，失败返回false
		bool GetExePath(PTCHAR ptInPath)
		{
			PTCHAR ptTem = NULL;
			TCHAR tszTemp[MAX_PATH] = {0};
			//获取当前目录  //这里是获取当前进程文件的完整路径 
			if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && ptInPath)
				return false; 

			ptTem = _tcsrchr(tszTemp, _T('\\'));
			memcpy(ptInPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
			return true;
		}

		//获取指定文件长度（单位：字节） 
		DWORD GetFileLen(const PTCHAR ptInFilePath)
		{
			//第一种方法
			DWORD dwFileLen = 0;	
			HANDLE hFile = NULL;     
			hFile = CreateFile(ptInFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);       
			dwFileLen = GetFileSize(hFile, NULL);       
			CloseHandle(hFile);     

			////第二种方法
			//FILE* pFile = NULL;
			//pFile = ::fopen(ptszInFilePath, "rb");
			//if(pFile == NULL)
			//	return 0;
			//
			//::fseek(pFile, 0, SEEK_SET);    //定位开始
			//dwFileLen = ::ftell(pFile);
			//fseek(pFile, 0, SEEK_END);  
			//dwFileLen = ::ftell(pFile) - dwFileLen;
			//fclose(pFile);
			//pFile = NULL;

			return dwFileLen;
		}

		//设置文件属性
		//#define FILE_SHARE_READ                 0x00000001  
		//#define FILE_SHARE_WRITE                0x00000002  
		//#define FILE_SHARE_DELETE               0x00000004        
		//#define FILE_ATTRIBUTE_READONLY             0x00000001    只读
		//#define FILE_ATTRIBUTE_HIDDEN               0x00000002    隐藏
		//#define FILE_ATTRIBUTE_SYSTEM               0x00000004    系统
		//#define FILE_ATTRIBUTE_DIRECTORY            0x00000010    目录
		//#define FILE_ATTRIBUTE_ARCHIVE              0x00000020  
		//#define FILE_ATTRIBUTE_DEVICE               0x00000040    设备
		//bool SetFileAttributesEx(PTCHAR ptFileName, DWORD dwFileAttributes)
		//{
		//	::SetFileAttributes(ptFileName, dwFileAttributes); //设置隐藏属性
		//}

		//目录选择对话框 hwndOwner 主窗口句柄, nIDDlgItem 编辑框ID  
		bool GetSelectDirDlg(HWND hwndOwner, int nIDDlgItem, PTCHAR ptszTitle = _T("选择路径"))
		{
			BROWSEINFO stuBI;
			TCHAR tszPathName[MAX_PATH] = {0};
			TCHAR tszPath[MAX_PATH] = {0}; //选择的目录
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

		//保存或打开 文件对话框 hwndOwner 主窗口句柄, nIDDlgItem 编辑框ID, bIsOpen true打开 false保存
		//eg ptszFilter _T("网页文件(*.html)\0*.html\0;网页文件(*.htm)\0*.htm\0;All Files(*.*)\0*.*\0")
		bool GetFileDlg(HWND hwndOwner, int nIDDlgItem, 
			const bool bIsOpen = false,
			const PTCHAR  ptszFilter =_T("All Files(*.*)\0*.*\0"),
			const PTCHAR ptszTitle = _T("选择文件路径"))
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
		//获取桌面路径
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

