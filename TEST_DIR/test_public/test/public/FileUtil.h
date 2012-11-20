//file TraceUtil.h
/***************************************************************************************************
* 1、 Class      ： CFileUtil
* 2、 Version    ： *.*
* 3、 Description： 文件读写类
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-11-13 16:46:43
* 6、 History    ： 
* 7、 Remark     ： 
****************************************************************************************************/
#ifndef __PATH_UTIL_H_
#define __PATH_UTIL_H_
#include <Windows.h>

namespace QNA
{
	class CFileUtil
	{
		CFileUtil()
		{

		}
		CFileUtil(int hFile)
		{

		}
		//nOpenFlags 共享和访问模式，指定当打开文件时进行的动作，可以将以下所列用OR(|)操作符联起来。至少应有一个访问权限和一个共享选项，modeCreate和modeNoInherit是可选的。值如下所示： · CFile::modeCreate 调用构造函数构造一个新文件，如果文件已存在，则长度变成0。  
		//	· CFile::modeNoTruncate 此值与modeCreate组合使用。如果所创建的文件已存在则其长度不变为0。因而此文件被打开，或者作为一个新文件或者作为一个已存在的文件。这将是很有用的，例如当打开一个可能存在也可能不存在的文件时。这个选项也可用于CStdioFile.  
		//	· CFile::modeRead 打开文件仅供读。  
		//	· CFile::modeReadWrite 打开文件供读写。  
		//	· CFile::modeWrite 打开文件仅供写。  
		//	· CFile::modeNoInherit 阻止文件被子进程继承。  
		//	· CFile::ShareDenyNone 不禁止其它进程读或写访问，打开文件。如果文件已被其它进程以兼容模式打开，则Create失败。  
		//	· CFile::ShareDenyRead 打开文件，禁止其它进程读此文件。如果文件已被其它进程以兼容模式打开，或被其它进程读，则Create失败。  
		//	· CFile::ShareDenyWrite 打开文件，禁止其它进程写此文件。如果文件已被其它进程以兼容模式打开，或被其它进程写，则Create失败。  
		//	· CFile::ShareExclusive 以独占模式打开文件，禁止其它进程对文件的读写。如果文件已经以其它模式打开读写（即使被当前进程），则构造失败。  
		//	· CFile::ShareCompat 此标志在32位MFC中无效。此标志在使用CFile::Open时映射为CFile::ShareExclusive。  
		//	· CFile::typeText 对回车换行设置特殊进程（仅用于派生类）。  
		//	· CFile::typeBinary 设置二进制模式（仅用于派生类）。  
		CFileUtil(TCHAR ptszFileName, UINT nOpenFlags)
		{

		}
		throw(CFileException);

	};
}
#endif

