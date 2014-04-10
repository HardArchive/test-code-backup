//file DirectoryTraversalUtil.h
/***************************************************************************************************
* 1、 Class      ： DirectoryTraversalUtil
* 2、 Version    ： 1.0
* 3、 Description： 遍历目录 
* 4、 Author     ： Ray (http://www.9cpp.com/)
* 5、 Created    ： 2014-4-9 16:27:44
* 6、 History    ： 
* 7、 Remark     ： 还可以升级 可设计为 最多只遍历几重目录  还增加目录处理  目录不存在是否创建新的目录 多线程遍历如何处理？
* **************本类需要继承实现TraversalDoWork函数才能使用*****************
****************************************************************************************************/
#ifndef __DIEECTOR_TRAVERSAL_UTIL_H_
#define __DIEECTOR_TRAVERSAL_UTIL_H_
#include <Windows.h>
#include "TraceUtil.h"
#include "PathUtil.h"


/////////////////////////////////////Error Code/////////////////////////////////////
#define IN_PARAMETER_ERROR      -1                    //传入参数错误
#define IN_PATH_NOT_EXIST       -2                    //路径不存在
#define FIND_FILE_HANDLE_VALUE  -3                    //查找文件失败
#define DIR_TRAVERSAL_FAIL      -4                    //DirectoryTraversal处理失败
namespace RG
{
	//遍历目录设置结构
	typedef struct TRAVERSAL_TAG
	{
		bool bIsCompletePath;                  //传入处理目录是否需要完整的文件名， false 表是需要 完整路径  true 表示只需要 不包含路径的文件名
		bool bIsHandleFile;                    //遍历查找文件/文件夹  false 文件  true 文件夹  当为true时 iDirDepth值不起作用 tszFileTpye为*
		int iDirDepth;                         //遍历目录层次/深度 1表示只在ptInPath下  2 ptInPath下一层  0表示所有子目录
		int iTimeInterval;                     //多次遍历时的时间间隔  在线程中实现 比如目录监控 0表示只进行一次遍历
		TCHAR tszFileTpye[32];                 //遍历需要处理文件后缀名 *表示所有文件类型  **表示需要处理的为文件夹 
		TCHAR tszTPath[MAX_PATH];              //遍历的目录名

		TRAVERSAL_TAG(){Reset();}
		~TRAVERSAL_TAG(){Reset();}

		void Reset()
		{
			memset(this, 0, sizeof(*this));
			bIsHandleFile = false;
			bIsCompletePath = false;
			iDirDepth = 0;
			iTimeInterval = 0;
			_tcscpy_s(tszFileTpye, 32, _T("*"));
			memset(tszTPath, 0, MAX_PATH*sizeof(TCHAR));
		}

		//Init初始化遍历设置 
		// PTCHAR ptInPath               遍历的目录名
		// PTCHAR ptInFileTpye           遍历需要处理文件后缀名 *表示所有文件类型  **表示需要处理的为文件夹
		// iInDirDepth                   遍历目录层次/深度 1表示只在ptInPath下  2 ptInPath下一层  0表示所有子目录 默认0
		// int iInterval                 多次遍历时的时间间隔  在线程中实现 比如目录监控 0表示只进行一次遍历 默认0
		// bInIsHandleFile               遍历查找文件/文件夹  false 文件  true 文件夹 默认false
		void Init(const PTCHAR ptInPath, const PTCHAR ptInFileTpye = NULL, const int iInDirDepth = 0, const int iInterval = 0)
		{
			iDirDepth = iInDirDepth;
			iTimeInterval = iInterval;

			if (NULL == ptInFileTpye)
				_tcscpy_s(tszFileTpye, 32, _T(".*"));
			else
				_tcscpy_s(tszFileTpye, 32, ptInFileTpye);
			//文件夹
			if (bIsHandleFile)
			{
				_tcscpy_s(tszFileTpye, 32, _T(".*"));
				iDirDepth = 0;
			}

			_tcscpy_s(tszTPath, MAX_PATH, ptInPath);
		}
	}TRAVERSAL, * PTRAVERSAL;

	class CDirectoryTraversalUtil
	{
	public:
		CDirectoryTraversalUtil() {Reset();}
		~CDirectoryTraversalUtil(){Reset();}

	public:
		//文件/文件夹处理函数  纯虚函数 需要继承
		virtual bool TraversalDoWork(const PTCHAR ptInFilePath) = 0;  

		//初始化函数 
		// ptInPath 遍历的目录名 
		// ptInFileTpye 遍历获取的文件后缀  NULL表示所有文件类型 **表示需要处理的为文件夹
		// iInDirDepth 遍历目录的深度 
		// iInterval 遍历的时间间隔 单位 秒
		// bInIsHandleFile  遍历查找文件/文件夹  false 文件  true 文件夹 默认false
		bool Init(const PTCHAR ptInPath, const PTCHAR ptInFileTpye = NULL, const int iInDirDepth = 0, const int iInterval = 0)
		{
			//检查参数是否正确
			if (!ptInPath || _tcsclen(ptInPath)<4 || iInDirDepth<0 || iInterval<0)
				return false;

			m_stuTraversal.Init(ptInPath, ptInFileTpye, iInDirDepth, iInterval);

			return true;
		}

		//开始遍历 
		int StartTraversal()
		{
			int iRet = 0;
			int iSleepTime = 0;
			m_bExitFlag = true;
			iSleepTime = m_stuTraversal.iTimeInterval?m_stuTraversal.iTimeInterval*1000:1;
			while(m_bExitFlag)
			{
				iRet = DirectoryTraversal(m_stuTraversal.tszTPath, m_stuTraversal.tszFileTpye, m_stuTraversal.iDirDepth);
				if (iRet<0)
				{
					//...遍历失败..错误处理  暂不处理
				}
				if (!m_stuTraversal.iTimeInterval) break;
				TRACE(_T("遍历完成！！！%d秒后再次遍历！！！！！\r\n"), iSleepTime/1000);

				Sleep(iSleepTime);
			}

			return iRet;
		}

		//停止遍历
		void StopTraversal(){m_bExitFlag = false; Reset();}

		///////处理 属性设置///////
		//设置处理函数TraversalDoWork传入的是否为完整路径  默认不调用 传入完整路径  bInIsCompletePath true 表示只传入文件名
		void SetDoworkPath(bool bInIsCompletePath = true)
		{
			m_stuTraversal.bIsCompletePath = bInIsCompletePath;
		}

		//设置处理函数TraversalDoWork 遍历查找文件或者文件夹 bInIsHandleFile  false 文件  true 文件夹  
		//默认不调用 查找文件
		void SetDoWorkIsFile(bool bInIsHandleFile = true)
		{
			m_stuTraversal.bIsHandleFile = bInIsHandleFile;
		}
	private:


		//一次遍历目录
		int DirectoryTraversal(const PTCHAR ptInPath, const PTCHAR ptInFileTpye, const int iInDirDepth = 0)
		{
			HANDLE hFind = NULL;
			WIN32_FIND_DATA stuWFD = {0};	
			TCHAR tszFullPath[MAX_PATH] = {0};
			TCHAR tszFindFilter[MAX_PATH] = {0};                           //查找文件类型
			PTCHAR ptPath = NULL;
			DWORD dwAttributes = 0;

			if (!(ptInPath && ptInFileTpye))	return IN_PARAMETER_ERROR;

			//检查创建新文件夹路径
			if (!CheckDirExist(ptInPath))
			{
				return IN_PATH_NOT_EXIST;
			}

			if (!m_stuTraversal.bIsCompletePath)
			{
				ptPath = tszFullPath;
			}
			else
			{
				ptPath = stuWFD.cFileName; 
			}

			_stprintf_s(tszFindFilter, MAX_PATH, _T("%s\\*%s"), ptInPath, ptInFileTpye);    

			if ((hFind = FindFirstFile(tszFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
			{
				return FIND_FILE_HANDLE_VALUE;
			}

			do
			{
				if (!m_bExitFlag) break;
				//过滤本级目录和父目录
				if(_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0)
					continue;

				ZeroMemory(tszFullPath, sizeof(tszFullPath));
				_stprintf_s(tszFullPath, MAX_PATH, _T("%s\\%s"), ptInPath, stuWFD.cFileName);  

				//如果找到的是目录
				if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{	
					if (!m_stuTraversal.bIsHandleFile)
					{
						if (1 == iInDirDepth) continue;

						int iDirDepth = iInDirDepth?(iInDirDepth-1):0;
						if (1 != DirectoryTraversal(tszFullPath, ptInFileTpye, iDirDepth))
						{
							FindClose(hFind);
							return DIR_TRAVERSAL_FAIL;
						}

						continue;
					}

					if (!TraversalDoWork(ptPath))
					{
						FindClose(hFind);
						return DIR_TRAVERSAL_FAIL;
					}
				}
				else 
				{//找到的是文件
					if (m_stuTraversal.bIsHandleFile) continue;
					if (!TraversalDoWork(ptPath))
					{
						FindClose(hFind);
						return -5;
					}
				}	
			}while (FindNextFile(hFind, &stuWFD));

			FindClose(hFind);
			return 1;
		}

		//重置
		void Reset()
		{
			m_bExitFlag = false;                             //退出标志
			memset(&m_stuTraversal, 0, sizeof(TRAVERSAL));
			m_stuTraversal.Reset();
		}

	private:	
		bool m_bExitFlag;                                //退出标志  true 执行遍历 false 停止遍历
		TRAVERSAL m_stuTraversal;                        //目录遍历设置结构
	};
}
#endif

