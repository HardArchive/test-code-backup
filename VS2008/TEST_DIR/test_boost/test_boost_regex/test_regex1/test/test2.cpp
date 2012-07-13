#include "stdafx.h"

#include <iostream>
#include <cassert>
#include <string>
#include <boost/regex.hpp>
using namespace std;
using namespace boost;

//regex对象与regex_match()函数

int main2a()
{
	//reg的格式含义是：三个数字，1个单词，一个任意字符，2个数字或者字符串N/A，1个空格，然后再重复第一个单词。
	//3个数字 一个英文字符串(一个字符以上的) 一个任意字符 2个数字或N/A 空格 
	// \num  匹配 num，其中 num 是一个正整数。对所获取的匹配的引用。例如，'(.)\1' 匹配两个连续的相同字符。 
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
	regex reg("(.+)@(.+)\\.com");    //匹配邮箱
	string correct = "boluo1982107@ccc122.com";
	//assert(regex_match(correct,reg)==true);

	if(regex_match(correct.c_str(), what, reg))
	{
		for(boost::cmatch::iterator itr= what.begin(); itr != what.end(); ++itr)
		{
			//       指向子串对应首位置        指向子串对应尾位置          子串内容
			cout << "子串对应首位置：" << itr->first - correct.c_str() ;
			cout << "  子串对应尾位置：" << itr->second - correct.c_str();
			cout << "  子串内容      ：" << *itr << endl;
		}
	}
	return 0;
}