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

#define CPathUtil_Administrative_Tools 0x01                                //「开始」菜单\程序\管理工具
#define CPathUtil_AppData              0x02                                //应用数据
#define CPathUtil_Cache                0x03                                //缓存
#define CPathUtil_CD_Burning           0x04                                //CD刻录
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
#define CPathUtil_Start_Menu           0x15                                //「开始」菜单\程序
#define CPathUtil_SendTo               0x16                                //「开始」菜单
#define CPathUtil_Startup              0x17                                //「开始」菜单\程序\启动
#define CPathUtil_Templates            0x18                                //模板
//系统壁纸目录  
//打印缓存目录
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

		//遍历目录
		int DirectoryTraversal(const PTCHAR ptInPath, const PTCHAR ptInFileTpye)
		{
			HANDLE hFind = NULL;
			WIN32_FIND_DATA stuWFD = {0};	
			TCHAR tszFullPath[MAX_PATH] = {0};
			TCHAR tszFindFilter[MAX_PATH] = {0};
			DWORD dwAttributes = 0;

			if (!(ptInPath && ptInFileTpye))	return -1;
			//检查创建新文件夹路径
			if (!ChickDirExist(ptInPath))
			{
				if (!CreateMultipleDirectory(ptInPath))
					return -2;				
			}
			_stprintf_s(tszFindFilter, MAX_PATH, _T("%s\\*%s"), ptInPath, ptInFileTpye);    //查找文件类型

			if ((hFind = FindFirstFile(tszFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
			{
				return -3;
			}

			do
			{
				//过滤本级目录和父目录
				if(_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0)
					continue;
				ZeroMemory(tszFullPath, sizeof(tszFullPath));
				_stprintf_s(tszFullPath, MAX_PATH, _T("%s\\%s"), ptInPath, stuWFD.cFileName);  //得到文件(夹)完整路径

				//如果找到的是目录，则进入此目录进行递归
				if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{	
					if (1 != DirectoryTraversal(tszFullPath, ptInFileTpye))
					{
						FindClose(hFind);
						return -4;
					}

				}
				else 
				{//找到的是文件
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

		//****************************删除文件夹**************************************************************************
		
		//检查当前目录需要的文件夹是否存在,如果不存在返回false，存在返回true
		bool ChickDirExist(const PTCHAR ptInPath)
		{
			if (!ptInPath || _tcsclen(ptInPath)<2)	return false;

			//检验路径是否存在	如果不存在则创建
			if (GetFileAttributes(ptInPath) != FILE_ATTRIBUTE_DIRECTORY)	return false;
			return true;
		}

		//创建多级目录，成功返回true， 失败返回false
		bool CreateMultipleDirectory(const PTCHAR ptInPath)
		{
			int iLen = 0;    
			PTCHAR ptTemp = NULL;		
			TCHAR tszPath[MAX_PATH] = {0};
			TCHAR tszTemPath[MAX_PATH] = {0};
			_tcscpy_s(tszPath, ptInPath);         //存放要创建的目录字符串

			//检查参数是否正确
			if (!tszPath || _tcsclen(tszPath)<4)
				return false;

			//在这里去掉尾部为'\\'的字符
			if (_T('\\') == tszPath[_tcsclen(tszPath)-1])
			{
				tszPath[_tcsclen(tszPath)-1] = 0;
			}

			ptTemp = _tcsrchr(tszPath, _T('\\'));  //从尾部查找字符
			iLen = _tcsclen(ptTemp);
			_tcsncpy_s(tszTemPath, tszPath, _tcsclen(tszPath)-iLen); //得到父目录路径

			//检验父级路径是否存在  		
			if (ChickDirExist(tszTemPath))
			{   //如果存在则创建子目录
				if (!CreateDirectory(tszPath, NULL))
				{	//创建文件夹失败
					//::MessageBox(NULL, tszPath, _T("创建文件夹失败!!!!!"), MB_OK);
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

