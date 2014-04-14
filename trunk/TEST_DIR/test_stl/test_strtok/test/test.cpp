// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}

//��Cʵ��
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

//C++ ʹ��STL string ʵ�ֵ�split��trim��replace-�޶�
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
	//���ܳ�ȥ�ַ�����ͷ��ĩβ�Ŀո�������ַ�
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

	//��һ���ַ����ָ���ַ�������
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

	//�����ַ���������ָ����Ŀ��ĳ���ַ������滻Ϊ��һ�����ַ�����
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
��ܰ��ʾ���������Ż��н��ͻ��ѳ�ֵ����Ϣ��������ƭ��\r\n\
���Ż����  11:47:00\r\n\
��л��ע���Ż���֣����ǽ������ڷ����н�����������⣡���κ���������������ԡ� \r\n\
----------------------\r\n\
�� A �鿴 ���»����\r\n\
�� B �鿴 ���ڻ����\r\n\
�� M ���ػ�ӭҳ\r\n\
-----------------------\r\n\
����ע���롿\r\n\
����  http://gz.feixin.10086.cn/400123 ת������ĺ��ѣ������ǲ���Ӯȡ��Ʒ��\r\n\
\r\n\
-----------------------------\r\n\
�ظ�\"Z\"������У�칫ͨ��\r\n\
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


