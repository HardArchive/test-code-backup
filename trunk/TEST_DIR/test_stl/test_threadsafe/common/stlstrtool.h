//file stlstrtool.h
/***************************************************************************************************
* 1、 Class      ： stlstrtool
* 2、 Version    ： 1.0
* 3、 Description： 路径相关公共函数  
* 4、 Author     ： Ray (http://www.9cpp.com/)
* 5、 Created    ： 2012-9-7 16:27:44
* 6、 History    ： CheckDirExist            检查目录是否存在 
*	                CreateMultipleDirectory  创建多级目录
*	                GetExePath               获取Exe所在目录
*	                GetFileLen               获取文件长度
*	                GetSelectDirDlg          目录选择对话框 
*	                GetFileDlg               保存或打开 文件对话框
*	                
* 7、 Remark     ：未完善  还可以添加功能  如修改文件属性  重命令文件/文件夹  删除文件 等相关函数  获取相应的系统目录  
****************************************************************************************************/
#ifndef __STL_STRING_TOOL_H_
#define __STL_STRING_TOOL_H_

#ifdef _UNICODE
#include <xstring>
#define tstring wstring
#else
#include <string>
#define tstring string
#endif

#include <iostream>
#include <vector>
using namespace std;

namespace RG
{
	namespace strtool
	{
		//功能除去字符串开头和末尾的空格或其他字符
		inline tstring trim(const tstring& str)
		{
			tstring::size_type pos = str.find_first_not_of(' ');
			if (pos == tstring::npos)
			{
				return str;
			}
			tstring::size_type pos2 = str.find_last_not_of(' ');
			if (pos2 != tstring::npos)
			{
				return str.substr(pos, pos2 - pos + 1);
			}
			return str.substr(pos);
		}

		//把一个字符串分割成字符串数组
		inline int split(const tstring& str, vector<tstring>& ret_, tstring sep = _T(","))
		{
			if (str.empty())
			{
				return 0;
			}

			tstring tmp;
			tstring::size_type pos_begin = str.find_first_not_of(sep);
			tstring::size_type comma_pos = 0;

			while (pos_begin != tstring::npos)
			{
				comma_pos = str.find(sep, pos_begin);
				if (comma_pos != tstring::npos)
				{
					tmp = str.substr(pos_begin, comma_pos - pos_begin);
					pos_begin = comma_pos + sep.length();
				}
				else
				{
					tmp = str.substr(pos_begin);
					pos_begin = comma_pos;
				}

				if (!tmp.empty())
				{
					ret_.push_back(tmp);
					tmp.clear();
				}
			}
			return 0;
		}

		//返回字符串，其中指定数目的某子字符串被替换为另一个子字符串。
		inline tstring replace(const tstring& str, const tstring& src, const tstring& dest)
		{
			tstring ret;

			tstring::size_type pos_begin = 0;
			tstring::size_type pos       = str.find(src);
			while (pos != tstring::npos)
			{
				cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
				ret.append(str.data() + pos_begin, pos - pos_begin);
				ret += dest;
				pos_begin = pos + 1;
				pos       = str.find(src, pos_begin);
			}
			if (pos_begin < str.length())
			{
				ret.append(str.begin() + pos_begin, str.end());
			}
			return ret;
		}

	}
}
#endif //__STL_STRING_TOOL_H_

