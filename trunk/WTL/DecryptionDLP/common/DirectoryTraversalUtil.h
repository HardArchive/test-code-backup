//file DirectoryTraversalUtil.h
/***************************************************************************************************
* 1、 Class      ： DirectoryTraversalUtil
* 2、 Version    ： 1.0.1
* 3、 Description： 遍历目录 
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-9-7 16:27:44
* 6、 History    ： 修改为支持文件夹处理  
* 7、 Remark     ： 还可以升级 可设计为 最多只遍历几重目录  还增加目录处理  多线程遍历如何处理？
****************************************************************************************************/
#ifndef __DIEECTOR_TRAVERSAL_UTIL_H_
#define __DIEECTOR_TRAVERSAL_UTIL_H_
#include <Windows.h>
#include "TraceUtil.h"
#include "PathUtil.h"

namespace RG
{
	class CDirectoryTraversalUtil
	{
	public:
		CDirectoryTraversalUtil() {Reset();}
		~CDirectoryTraversalUtil(){Reset();}

	public:
		//文件处理函数
		virtual bool TraversalFileDoWork(const PTCHAR ptInFilePath) = 0;  //处理文件
		//文件夹处理函数
		virtual bool TraversalFolderDoWork(const PTCHAR ptInFilePath) = 0;  //处理文件夹

		//初始化函数 ptInPath 遍历的目录名 ptInFileTpye 遍历获取的文件后缀 iInterval 遍历的时间间隔
		void Init(const PTCHAR ptInPath, const PTCHAR ptInFileTpye = _T(".*"), const int iInterval = 0)
		{
			m_iInterval = iInterval;
			_tcscpy_s(m_tszFileType, 16, ptInFileTpye);
			_tcscpy_s(m_tszDirectoryTraversalPath, MAX_PATH, ptInPath);
		}

		//开始遍历 
		int StartTraversal()
		{
			int iRet = 0;
			int iSleepTime = 0;
			m_bExitFlag = true;
			iSleepTime = m_iInterval?m_iInterval*1000:1;
			//多次遍历 
			while(m_bExitFlag)
			{
				iRet = DirectoryTraversal(m_tszDirectoryTraversalPath, m_tszFileType);
				if (iRet<0)
				{
					//.....错误处理
				}

				//时间间隔为零时只遍历一次
				if (!m_iInterval)	break;
				//TRACE("拷贝完成！！！%d秒后再拷！！！！！", iSleepTime/1000);

				Sleep(iSleepTime);
			}

			return iRet;
		}
		//停止遍历
		void StopTraversal(){m_bExitFlag = false; Reset();}

	private:
		void Reset()
		{
			m_iInterval = 0;                                 //时间间隔
			m_bExitFlag = false;                             //退出标志
			memset(m_tszFileType, 0, sizeof(TCHAR)*16);
			memset(m_tszDirectoryTraversalPath, 0, sizeof(TCHAR)*MAX_PATH);
		}

		//一次遍历目录
		int DirectoryTraversal(const PTCHAR ptInPath, const PTCHAR ptInFileTpye = _T(".*"))
		{
			HANDLE hFind = NULL;
			WIN32_FIND_DATA stuWFD = {0};	
			TCHAR tszFullPath[MAX_PATH] = {0};
			TCHAR tszFindFilter[MAX_PATH] = {0};
			DWORD dwAttributes = 0;

			if (!(ptInPath && ptInFileTpye))	return -1;

			//检查创建新文件夹路径			
			if (!RG::ChickDirExist(ptInPath))
			{
				//if (!RG::CreateMultipleDirectory(ptInPath))
					return -2;				
			}

			_stprintf_s(tszFindFilter, MAX_PATH, _T("%s\\*%s"), ptInPath, ptInFileTpye);    //查找文件类型

			if ((hFind = FindFirstFile(tszFindFilter, &stuWFD)) == INVALID_HANDLE_VALUE)
			{
				return -3;
			}

			do
			{
				if (!m_bExitFlag) break;
				//过滤本级目录和父目录
				if(_tcscmp(stuWFD.cFileName, _T(".")) == 0 || _tcscmp(stuWFD.cFileName, _T("..")) == 0)
					continue;
				ZeroMemory(tszFullPath, sizeof(tszFullPath));
				_stprintf_s(tszFullPath, MAX_PATH, _T("%s\\%s"), ptInPath, stuWFD.cFileName);  //得到文件(夹)完整路径

				//如果找到的是目录，则进入此目录进行递归
				if(stuWFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{	//处理目录
					if (!TraversalFolderDoWork(tszFullPath))
					{
						FindClose(hFind);
						return -5;
					}

					if (1 != DirectoryTraversal(tszFullPath, ptInFileTpye))
					{
						FindClose(hFind);
						return -4;
					}
				}
				else 
				{    //找到的是文件
					if (!TraversalFileDoWork(tszFullPath))
					{
						FindClose(hFind);
						return -5;
					}
				}	
			}while (FindNextFile(hFind, &stuWFD));

			FindClose(hFind);
			return 1;
		}

	public:
		bool m_bExitFlag;                                //退出标志  true 执行遍历 false 停止遍历
		CPathUtil m_clsPathUtil;
	private:		
		int m_iInterval;                                 //多次遍历的时间间隔   如果是一次遍历则时间为0
		TCHAR m_tszFileType[16];                         //遍历获取的文件后缀
		TCHAR m_tszDirectoryTraversalPath[MAX_PATH];     //遍历的目录名		
	};
}
#endif

