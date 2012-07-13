/******************************************************************************* 
* 1、 Class      ： CPugiXmlLog
* 2、 Version    ： 1.0
* 3、 Description： C++中的XML日志类-基于PugiXml库
* 4、 Author     ： QNA (http://www.9cpp.com/)
* 5、 Created    ： 2012-7-12 11:26:12
* 6、 History    ： 
* 7、 Remark     ： 只需要打开相应的XML文件Log.XML就可以看到一个日志表格了，
*					为此我专门写了一个XSL样式文件（XML样式XSL文件必须在XML文件同一目录下）！
*					我参考了Emilio Guijarro Cameros的CXMLProfile写XML配置文件的思想！！！
注意添加格式  时间添加  按日添加  按日清除
//Level 1：正常流程不应该发生的故障：如new操作失败，文件IO操作失败，异常退出等等；
//Level 2：参数有效性判断，以及指针NULL判断为不符和规则；
//Level 3: 函数中途异常返回；
//Level 4: 命令消息的打印输出；
//Level 5: 程序流程中重要变量内容的调试输出；
//默认日志级别是3，按天产生日志，日志记录加时间戳;如果没有设置FileByDay|FileByName选项，默认为DisplayScreen即输出到屏幕
//按天产生日志，即每天生成一个以日期为文件名的日志，还需添加可以自己清除功能（eg删除30天以前的日志）
********************************************************************************/ 
#ifndef  _PUGIXML_LOG_H_
#define  _PUGIXML_LOG_H_

#include "stdafx.h"
#include <windows.h>
//#include <pugiconfig.hpp>

#include <pugixml.hpp>
#pragma comment(lib, "../bin/pugixmld.lib")
namespace QNA
{
	class CPugiXmlLog
	{
	private:
		//获取当前程序所在目录
		bool GetExePath(LPTSTR pstrPath)
		{
			LPTSTR ptTem = NULL;
			TCHAR tszTemp[MAX_PATH] = {0};
			//获取当前目录  //这里是获取当前进程文件的完整路径 
			if (!GetModuleFileName(NULL, tszTemp, MAX_PATH) && pstrPath)
				return false; 

			ptTem = _tcsrchr(tszTemp, '\\');
			memcpy(pstrPath, tszTemp, (_tcslen(tszTemp)-_tcslen(ptTem))*sizeof(TCHAR));
			return true;
		}

	public:
		CPugiXmlLog(LPCTSTR lpszFileName = NULL)
		{
			TCHAR tszTem[MAX_PATH] = {0};
			TCHAR tszFileName[MAX_PATH] = {0};
			if (NULL == lpszFileName)
			{
				GetExePath(tszTem);
				_stprintf_s(tszFileName, _T("%s\\Log.xml"), tszTem);
			}
			else
			{
				_tcscpy_s(tszFileName, lpszFileName);
			}

			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(tszFileName);
			
			if (!result)
			{ 
				doc.load("<foo bar='baz'>hey</foo>");
				//doc.load("<?xml-stylesheet href='Log.xsl' type='text/xsl'?>");
				doc.save_file(tszFileName);
				result = doc.load_file(tszFileName);
			}

		}
		~CPugiXmlLog(void){}

		void Log(LPCTSTR lpszFilName, LPCTSTR s,...);  //Log是添加一条日志
		bool ClearAll();                      //ClearAll是清除所有日志
	};
}


#endif // (_PUGIXML_LOG_H_)