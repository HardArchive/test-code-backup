// test.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "deelx.h"

#include <iostream>
#include <fstream>

void load_file(std::string& s, std::istream& is)
{
	s.erase();
	if(is.bad()) return;
	s.reserve(is.rdbuf()->in_avail());
	char c;
	while(is.get(c))
	{
		if(s.capacity() == s.size())
			s.reserve(s.capacity() * 3);
		s.append(1, c);
	}
}

int main(int argc, char * argv[])
{
	std::ifstream fs("C:\\Lookup\\112.127.141.86.html");
	std::string in;
	load_file(in, fs);
	fs.close();
	//static CRegexpT <char> regexp1("\\d+");
	static CRegexpT <char> regexp1("target=\\\"_blank\\\"\\>(.+?)\\</a\\>\\</td\\>");
	// loop
	MatchResult result1 = regexp1.Match(in.c_str());

	while( result1.IsMatched() )
	{  
		//GetEndƥ��ɹ��󣬻�ȡ��ƥ�䵽�����ַ����Ľ���λ�á����ƥ��ʧ�ܣ��򷵻ظ�ֵ��
		//GetStartƥ��ɹ��󣬻�ȡ��ƥ�䵽�����ַ����Ŀ�ʼλ�á����ƥ��ʧ�ܣ��򷵻ظ�ֵ��
		printf("%.*s\n", result1.GetEnd() - result1.GetStart(), in.c_str() + result1.GetStart());
		// get next
		result1 = regexp1.Match(in.c_str(), result1.GetEnd());  //����ƥ���� MatchResult ����
		// ͨ�� MatchResult ���󣬿��Ե�֪�Ƿ�ƥ��ɹ�������ɹ���ͨ�� MatchResult ������Ի�ȡ������Ϣ��

	}

	// text
	char * text = "http://www.cppprog.com/2009/0112/48.html";
	// declare
	static CRegexpT <char> regexp("\\d+");
	// loop
	MatchResult result = regexp.Match(text);
	//IsMatched���ط���ֵ��ʾƥ��ɹ������� 0 ��ʾƥ��ʧ�ܡ�
	while( result.IsMatched() )
	{  
		//GetEndƥ��ɹ��󣬻�ȡ��ƥ�䵽�����ַ����Ľ���λ�á����ƥ��ʧ�ܣ��򷵻ظ�ֵ��
		//GetStartƥ��ɹ��󣬻�ȡ��ƥ�䵽�����ַ����Ŀ�ʼλ�á����ƥ��ʧ�ܣ��򷵻ظ�ֵ��
		printf("%.*s\n", result.GetEnd() - result.GetStart(), text + result.GetStart());
		// get next
		result = regexp.Match(text, result.GetEnd());  //����ƥ���� MatchResult ����
		// ͨ�� MatchResult ���󣬿��Ե�֪�Ƿ�ƥ��ɹ�������ɹ���ͨ�� MatchResult ������Ի�ȡ������Ϣ��

	}
	return 0;
}

//
//// %.*s ���е�.*��ʾ��ʾ�ľ��� ���ַ������(s)������˵���ǿ��
//// ���*�����ֵ�ɺ���Ĳ����б��е�������(int)ֵ����
//
//// ���磺
//printf("%.*s\n", 1, "abc");        // ���a
//printf("%.*s\n", 2, "abc");        // ���ab
//printf("%.*s\n", 3, "abc");        // ���abc >3��һ����Ч�� ��Ϊ�������type = s������'\0'�����
