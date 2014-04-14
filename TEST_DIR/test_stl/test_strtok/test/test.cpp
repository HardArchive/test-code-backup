// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

//纯C实现
/* strtok example */
//#include <stdio.h>
//#include <string.h>
////#include <string>
//
//int main ()
//{
//	char str[] ="- This, a sample string.";
//	char * pch;
//	printf ("Splitting string \"%s\" into tokens:\n",str);
//	pch = strtok (str," ,.-");
//	while (pch != NULL)
//	{
//		printf ("%s\n",pch);
//		pch = strtok (NULL, " ,.-");
//	}
//	return 0;
//}

//C++ 使用STL string 实现的split，trim，replace-修订
//http://www.cnblogs.com/zhiranok/archive/2011/12/08/stl_string_trim_split_replace.html

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
using namespace std;


namespace strtool
{
	//功能除去字符串开头和末尾的空格或其他字符
	tstring trim(const tstring& str)
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
	int split(const tstring& str, vector<tstring>& ret_, tstring sep = _T(","))
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
	tstring replace(const tstring& str, const tstring& src, const tstring& dest)
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

#define TEMP_STR _T("\
温馨提示：请勿轻信汇款、中奖和话费充值等信息，谨防受骗。\r\n\
飞信活动助手  11:47:00\r\n\
感谢关注飞信活动助手，我们将不定期发布有奖活动，敬请留意！有任何问题请给我们留言。 \r\n\
----------------------\r\n\
回 A 查看 最新活动详情\r\n\
回 B 查看 往期活动详情\r\n\
回 M 返回欢迎页\r\n\
-----------------------\r\n\
【关注邀请】\r\n\
复制  http://gz.feixin.10086.cn/400123 转发给你的好友，让他们参与活动赢取奖品！\r\n\
\r\n\
-----------------------------\r\n\
回复\"Z\"进入企校办公通信\r\n\
")



int main(int argc, char* argv[])
{
	tstring strTemp1 = strtool::trim(_T(" nihao sadfasdf  sdfasfdadsd  "));
	cout << strTemp1.c_str()<<"\n";

	vector<tstring> vt;
	strtool::split(_T(",o h,,,nice,,,,,,,"), vt);
	for (size_t i = 0; i < vt.size(); ++ i)
	{
		cout <<"out:" << vt[i].c_str() <<"\n";
	}
	vt.clear();

	tstring strTemp = TEMP_STR;


	strtool::split(strTemp, vt, _T("\r\n"));
	for (size_t i = 0; i < vt.size(); ++ i)
	{
		cout <<"out:" << vt[i].c_str() <<"\n";
	}

	tstring ret = strtool::replace(_T("xxAxxxAxxAxx"), _T("A"), _T("B"));
	cout <<"replace:" << ret.c_str() <<"\n";
	return 0;
}


