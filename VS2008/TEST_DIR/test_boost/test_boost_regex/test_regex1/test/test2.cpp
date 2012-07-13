#include "stdafx.h"

#include <iostream>
#include <cassert>
#include <string>
#include <boost/regex.hpp>
using namespace std;
using namespace boost;

//regex������regex_match()����

int main2a()
{
	//reg�ĸ�ʽ�����ǣ��������֣�1�����ʣ�һ�������ַ���2�����ֻ����ַ���N/A��1���ո�Ȼ�����ظ���һ�����ʡ�
	//3������ һ��Ӣ���ַ���(һ���ַ����ϵ�) һ�������ַ� 2�����ֻ�N/A �ո� 
	// \num  ƥ�� num������ num ��һ����������������ȡ��ƥ������á����磬'(.)\1' ƥ��������������ͬ�ַ��� 
	regex reg("\\d{3}([a-zA-Z]+).(\\d{2}|N/A)\\s\\1");
	string correct = "123Hello N/A Hello";
	string incorrect = "123Hello 12 hello";
	assert(regex_match(correct,reg)==true);
	assert(regex_match(incorrect,reg)==false);
	return 0;
}

int main2b()
{
	cmatch what;
	//regex reg("\\w(.+)@(.+)\\.com");
	regex reg("(.+)@(.+)\\.com");    //ƥ������
	string correct = "boluo1982107@ccc122.com";
	//assert(regex_match(correct,reg)==true);

	if(regex_match(correct.c_str(), what, reg))
	{
		for(boost::cmatch::iterator itr= what.begin(); itr != what.end(); ++itr)
		{
			//       ָ���Ӵ���Ӧ��λ��        ָ���Ӵ���Ӧβλ��          �Ӵ�����
			cout << "�Ӵ���Ӧ��λ�ã�" << itr->first - correct.c_str() ;
			cout << "  �Ӵ���Ӧβλ�ã�" << itr->second - correct.c_str();
			cout << "  �Ӵ�����      ��" << *itr << endl;
		}
	}
	return 0;
}