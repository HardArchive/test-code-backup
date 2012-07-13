#ifndef __9CPPFILE_H__
#define __9CPPFILE_H__

#pragma once

/////////////////////////////////////////////////////////////////////////////
/******************************************************************************* 
* 1、 文件名称： 9CppFile
* 2、 版　　本： Version *.*
* 3、 描    述： 文件读写操作类
* 4、 程序设计： 阿毛
* 5、 开发日期： 2012-7-5 14:05:55
* 6、 修 改 人： 
* 7、 修改日期： 
********************************************************************************/ 

namespace QNA
{
	class CFile
	{
	public:
		HANDLE m_hFile;    //文件句柄

		BOOL Open(LPCTSTR pstrFileName, 
			DWORD dwAccess = GENERIC_READ, 
			DWORD dwShareMode = FILE_SHARE_READ, 
			DWORD dwFlags = OPEN_EXISTING,
			DWORD dwAttributes = FILE_ATTRIBUTE_NORMAL)
		{
			_ASSERTE(pstrFileName!=NULL);
			Close();
			// Attempt file creation
			HANDLE hFile = ::CreateFile(pstrFileName, 
				dwAccess, 
				dwShareMode, 
				NULL,
				dwFlags, 
				dwAttributes, 
				NULL);
			if( hFile == INVALID_HANDLE_VALUE ) return FALSE;
			m_hFile = hFile;
			return TRUE;
		}
	
	};
}


#endif // __9CPPFILE_H___