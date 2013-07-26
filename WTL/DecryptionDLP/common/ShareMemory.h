//file ShareMemory.h
/***************************************************************************************************
* 1、 File       ： ShareMemory.h
* 2、 Version    ： *.*
* 3、 Description： 文件映射共享内存封装类
* 4、 Author     ： RG (http://www.9cpp.com/)
* 5、 Created    ： 2013-4-26 15:41:26
* 6、 History    ： 
* 7、 Remark     ： 这里需要改进  1 增加读写函数  2增加读写安全同步
****************************************************************************************************/
#ifndef __SHARE_MENORY_H__
#define __SHARE_MENORY_H__
//#include "GeneralHelper.h"

namespace RG
{
	/***************************************************************************************************
	* 1、 File       ： CMapFile
	* 2、 Version    ： *.*
	* 3、 Description： 用于文件映射的打开文件类 默认为不打开文件  仅供CShareMemory调用
	*				    通过GetMapFileHandle和GetMapFileHandle文件句柄为INVALID_HANDLE_VALUE
	* 4、 Author     ： RG (http://www.9cpp.com/)
	* 5、 Created    ： 2013-4-27 15:15:39
	* 6、 History    ： 
	* 7、 Remark     ： //case GENERIC_EXECUTE: 这里暂时不考虑
	****************************************************************************************************/
	class CMapFile
	{

	public:
		CMapFile(LPCTSTR lpFileName = NULL,                    //映射文件名
			DWORD dwDesiredAccess = GENERIC_READ)             //访问方式的可读
		{
			m_hFile = INVALID_HANDLE_VALUE;
		}

		~CMapFile()
		{
			Close();
		}

	public:
		//检查文件是否存在
		BOOL CheckFileExists(LPCTSTR lpFileName)
		{
			DWORD dwAttr = 0;
			UINT iPrevErrMode = 0;
			if (!lpFileName) return false;

			iPrevErrMode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
			dwAttr = ::GetFileAttributes(lpFileName);
			::SetErrorMode(iPrevErrMode);
			return dwAttr == INVALID_FILE_ATTRIBUTES ? false : true;
		}
		//取得映射到内存的文件句柄
		HANDLE GetMapFileHandle(){return m_hFile;}

		//取得文件长度
		DWORD GetMapFileSize(LPCTSTR lpFileName = NULL,      //映射文件名
			DWORD dwDesiredAccess = GENERIC_READ)            //默认访问方式的可读
		{
			DWORD dwFileSize = INVALID_FILE_SIZE;
			if (INVALID_HANDLE_VALUE == m_hFile)
			{
				m_hFile = CreateMapFile(lpFileName, dwDesiredAccess);
			}
			//创建映射文件失败
			if (INVALID_HANDLE_VALUE == m_hFile) 
				return dwFileSize;
			
			dwFileSize = ::GetFileSize(m_hFile, NULL);
			return dwFileSize;
		}

		//创建或打开需映射的文件 成功返回 文件句柄
		HANDLE CreateMapFile(LPCTSTR lpFileName = NULL,      //映射文件名
			DWORD dwDesiredAccess = GENERIC_READ)            //默认访问方式的可读
		{
			Close();
			if (NULL == lpFileName) return m_hFile;
			if (!CheckFileExists(lpFileName)) return m_hFile;            //文件不存在

			DWORD dwShareMode = 0;      //空闲变量利用
			DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;          //默认属性
			switch(dwDesiredAccess)
			{
			case GENERIC_READ:                                           //可以从文件中读取数据
				dwShareMode = FILE_SHARE_READ;
				dwFlagsAndAttributes = FILE_FLAG_SEQUENTIAL_SCAN;        //针对连续访问对文件缓冲进行优化
				break;
			case GENERIC_WRITE:                                          //可以将数据写入文件
				dwShareMode = FILE_SHARE_WRITE;break;
			case GENERIC_READ|GENERIC_WRITE:                             //可以从文件中读取数据,也可以将数据写入文件
				dwShareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;break;
			//case GENERIC_EXECUTE: 这里暂时不考虑
			}

			m_hFile = ::CreateFile(lpFileName, dwDesiredAccess, dwShareMode, NULL, OPEN_EXISTING, dwFlagsAndAttributes, NULL);
			if (INVALID_HANDLE_VALUE == m_hFile)
			{
				//.....打印错误GetLastError();
				//::MessageBox(NULL, _T("不能打开文件！！！！！"), _T("Error"), MB_OK);
			}

			return m_hFile;
		}

		//关闭文件 把文件句柄置为INVALID_HANDLE_VALUE
		void Close()
		{
			if (INVALID_HANDLE_VALUE != m_hFile)
			{
				::CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;;
			}
		}

	private:
		HANDLE m_hFile;           //文件句柄
	};

	/***************************************************************************************************
	* 1、 File       ： CShareMemory
	* 2、 Version    ： *.*
	* 3、 Description： 共享内存封装类
	* 4、 Author     ： RG (http://www.9cpp.com/)
	* 5、 Created    ： 2013-4-27 15:21:12
	* 6、 History    ： 
	* 7、 Remark     ： 这里需要改进  增加读写安全同步
	****************************************************************************************************/
	class CShareMemory
	{
	public:
		CShareMemory()
		{
			m_hMapping = NULL;			
			m_lpBaseAddress = NULL;
			m_dwMapBufLen = INVALID_FILE_SIZE;
		}
		~CShareMemory(){Close();}

	public:
		bool Create(LPCTSTR lpMapName = NULL, DWORD dwBufSize = 0, LPCTSTR lpFileName = NULL, DWORD dwDesiredAccess = GENERIC_READ)
		{
			Close();
			//PAGE_READONLY 以只读方式打开映射
			//PAGE_READWRITE 以可读、可写方式打开映射
			//PAGE_WRITECOPY 为写操作留下备份
			//可组合使用下述一个或多个常数：
			//SEC_COMMIT 为文件映射一个小节中的所有页分配内存
			//SEC_IMAGE 文件是个可执行文件
			//SEC_RESERVE 为没有分配实际内存的一个小节保留虚拟内存空间
			//创建一个命名的文件映射对象
			m_dwMapBufLen = dwBufSize?dwBufSize:m_clsMapFile.GetMapFileSize(lpFileName);

			m_hMapping = ::CreateFileMapping(
				m_clsMapFile.CreateMapFile(lpFileName, dwDesiredAccess),   //物理文件句柄
				NULL,                                                      //安全设置 NULL使用默认的安全配置
				PAGE_READONLY | SEC_COMMIT,                                //保护设置
				0,                                                         //高位文件大小 32位机为0
				dwBufSize,                                                 //低位文件大小
				lpMapName);                                  			   //共享内存名称

			if (NULL == m_hMapping)                                              
			{
				_asm int 3;
				DWORD dwError = GetLastError();
				::MessageBox(NULL, _T("创建一个命名的文件映射对象对象失败！！！！！"), _T("Error"), MB_OK);				
				Close();
				return false;
			}

			DWORD dwMapViewDesiredAccess = FILE_MAP_READ;
			switch(dwDesiredAccess)
			{
			case GENERIC_READ:                                           //可以从文件中读取数据
				dwMapViewDesiredAccess = FILE_MAP_READ;break;
			case GENERIC_WRITE:                                          //可以将数据写入文件
				dwMapViewDesiredAccess = FILE_MAP_WRITE;;break;
			case GENERIC_READ|GENERIC_WRITE:                             //可以从文件中读取数据,也可以将数据写入文件
				dwMapViewDesiredAccess = FILE_MAP_ALL_ACCESS;break;
			//case GENERIC_EXECUTE: 这里暂时不考虑
			}

			// Map view of file into baseointer.
			//函数将此文件映射对象的视图映射进地址	空间，同时得到此映射视图的首址。###阿毛###这个就是我保存数据的地址了
			if (!(m_lpBaseAddress = ::MapViewOfFile(m_hMapping, dwMapViewDesiredAccess, 0, 0, 0)))
			{				
				::MessageBox(NULL, _T("View failed."), _T("Error"), MB_OK);
				Close();
				return false;
			}
			return true;
		}

		//打开一个已经创建的内存映射对象 
		//dwDesiredAccess
		//FILE_MAP_ALL_ACCESS Includes all access rights to a file mapping object except FILE_MAP_EXECUTE
		//FILE_MAP_COPY       Copy-on-write access.
		//FILE_MAP_EXECUTE    Execute access. 
		//FILE_MAP_READ       Read access. 
		//FILE_MAP_WRITE      Write access
		bool Open(LPCTSTR lpMapName = NULL,                   //映射对象名
			DWORD dwDesiredAccess = FILE_MAP_READ,            //默认访问方式的可读
			bool bInheritHandle = false)   
		{
			Close();
			m_hMapping = ::OpenFileMapping(dwDesiredAccess, bInheritHandle, lpMapName);

			if (NULL == m_hMapping)                                              
			{
				//::MessageBox(NULL, _T("打开一个命名的文件映射对象对象失败！！！！！"), _T("Error"), MB_OK);
				//DWORD dwError = GetLastError();
				return false;
			}

			// Map view of file into baseointer.
			//函数将此文件映射对象的视图映射进地址	空间，同时得到此映射视图的首址。###阿毛###这个就是我保存数据的地址了
			if (!(m_lpBaseAddress = ::MapViewOfFile(m_hMapping, dwDesiredAccess, 0, 0, 0)))
			{
				//::MessageBox(NULL, _T("View failed."), _T("Error"), MB_OK);
				Close();
				return false;
			}

			return true;
		}
					
		//将视图的变化刷新写入磁盘  仅在文件映射时需要用到
		//lpBaseAddress包含了刷新基本地址 dwNumberOfBytesToFlush欲刷新的字节数
		bool Flush(LPCVOID lpBaseAddress, SIZE_T dwNumberOfBytesToFlush)
		{
			if (!(m_hMapping && m_lpBaseAddress)) return false;
			return ::FlushViewOfFile(lpBaseAddress, dwNumberOfBytesToFlush);
		}

		//关闭映射
		void Close()
		{
			if (m_lpBaseAddress)
			{
				//释放视图并把变化写回文件
				::UnmapViewOfFile(m_lpBaseAddress);
				m_lpBaseAddress = NULL;
			}	

			if (m_hMapping)
			{
				::CloseHandle(m_hMapping);
				m_hMapping = NULL;
			}
			m_dwMapBufLen = 0;
			m_clsMapFile.Close();
		}

		//取得指向共享内存的指针
		PVOID GetBasePoint(){return m_lpBaseAddress;}

		//取得映射内存空间大小
		DWORD GetFileMapLen() {return m_dwMapBufLen;}

		////同步处理函数  当读写内容的时候可以先Lock 用完Unlock
		//void Lock()		{}
		//void Unlock()	{}

	private:
		DWORD m_dwMapBufLen;          //打开的共享内存长度
		HANDLE m_hMapping;            //内存映射对象句柄
		PVOID m_lpBaseAddress;		  //指向共享内存的指针
		CMapFile m_clsMapFile;        //内存映射的文件对象句柄
	};
}

#endif /*__SHARE_MENORY_H__*/