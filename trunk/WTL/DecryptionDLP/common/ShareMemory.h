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
#include "GeneralHelper.h"


namespace RG
{
	class CShareMemory
	{
	public:
		CShareMemory()
		{
			m_hFile = INVALID_HANDLE_VALUE;
			m_hMapping = NULL;
			m_lpBaseAddress = NULL;
		}
		~CShareMemory(){Close();}

	public:
		bool Open(tstring& MapName = tstring(_T("")), DWORD dwBufSize = 0, tstring& tstrFileName = tstring(_T("")), DWORD dwShareMode = FILE_SHARE_READ)
		{
			m_hFile = GetFileHandle(tstrFileName, dwShareMode);
			//创建一个命名的文件映射对象
			if (!(m_hMapping = CreateFileMapping(
				m_hFile, 
				0, 
				dwShareMode | SEC_COMMIT,
				0,
				dwBufSize, 
				0)))
			{
				::MessageBox(NULL, _T("创建一个命名的文件映射对象对象失败！！！！！"), _T("Error"), MB_OK);
				Close();
				return false;
			}

			// Map view of file into baseointer.
			//函数将此文件映射对象的视图映射进地址	空间，同时得到此映射视图的首址。###阿毛###这个就是我保存数据的地址了
			if (!(m_lpBaseAddress = MapViewOfFile(m_hMapping, FILE_MAP_READ,0,0,0)))
			{
				::MessageBox(NULL, _T("View failed."), _T("Error"), MB_OK);
				Close();
				return false;
			}
			return true;
		}

		void Close()
		{
			if (m_hMapping)
			{
				CloseHandle(m_hMapping);
				m_hMapping = NULL;
			}
			if (m_hFile)
			{
				CloseHandle(m_hFile);
				m_hFile = INVALID_HANDLE_VALUE;;
			}

			if (m_lpBaseAddress)
			{
				UnmapViewOfFile(m_lpBaseAddress);
				m_lpBaseAddress = NULL;
			}	
		}

		//取得指向共享内存的指针
		PVOID GetBasePoint(){return m_lpBaseAddress;}

	private:
		//打开文件共享内存tstrFileName, dwShareMode访问模式 默认读   dwCreationDisposition 文件存在
		HANDLE GetFileHandle(tstring& tstrFileName, DWORD dwShareMode = FILE_SHARE_READ, DWORD dwCreationDisposition = OPEN_EXISTING)
		{
			HANDLE hFile = INVALID_HANDLE_VALUE;
			if (!tstrFileName.length())	return hFile;

			// 创建文件对象
			if ((hFile = ::CreateFile(tstrFileName.c_str(), 
				GENERIC_READ, 
				dwShareMode, 
				0, 
				dwCreationDisposition, 
				FILE_FLAG_SEQUENTIAL_SCAN,
				0)) == INVALID_HANDLE_VALUE)
			{
				//.....打印错误GetLastError();
				::MessageBox(NULL, _T("不能打开文件！！！！！"), _T("Error"), MB_OK);
			}
			return hFile;
		}

	private:
		HANDLE m_hFile;           //文件句柄
	    HANDLE m_hMapping;        //内存映射对象句柄
		PVOID m_lpBaseAddress;		  //指向共享内存的指针
	};
}

#endif /*__SHARE_MENORY_H__*/