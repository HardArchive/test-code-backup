//file stlstrtool.h
/***************************************************************************************************
* 1�� Class      �� stlstrtool
* 2�� Version    �� 1.0
* 3�� Description�� ·����ع�������  
* 4�� Author     �� Ray (http://www.9cpp.com/)
* 5�� Created    �� 2012-9-7 16:27:44
* 6�� History    �� CheckDirExist            ���Ŀ¼�Ƿ���� 
*	                CreateMultipleDirectory  �����༶Ŀ¼
*	                GetExePath               ��ȡExe����Ŀ¼
*	                GetFileLen               ��ȡ�ļ�����
*	                GetSelectDirDlg          Ŀ¼ѡ��Ի��� 
*	                GetFileDlg               ������ �ļ��Ի���
*	                
* 7�� Remark     ��δ����  ��������ӹ���  ���޸��ļ�����  �������ļ�/�ļ���  ɾ���ļ� ����غ���  ��ȡ��Ӧ��ϵͳĿ¼  
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
		//���ܳ�ȥ�ַ�����ͷ��ĩβ�Ŀո�������ַ�
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

		//��һ���ַ����ָ���ַ�������
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

		//�����ַ���������ָ����Ŀ��ĳ���ַ������滻Ϊ��һ�����ַ�����
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

