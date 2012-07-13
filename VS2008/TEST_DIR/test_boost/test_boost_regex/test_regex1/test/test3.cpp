#include "stdafx.h"

#include <iostream>
#include <cassert>
#include <string>
#include <boost/regex.hpp>
using namespace std;
using namespace boost;
//regex_search()函数的使用

int main3()
{
	regex reg("(new)|(delete)");
	smatch m;
	string s = "Calls to new mustbe followed by delete.  Calling simply new results in aleak!";
	int new_counter = 0;
	int delete_counter = 0;
	string::const_iterator it = s.begin();
	//    string::const_iterator end = s.end();
	while (regex_search(it, (string::const_iterator)s.end(), m, reg)) 
	{
		m[1].matched ? ++new_counter : ++delete_counter;  //查找结果
		it = m[0].second;                             //查找存放位置
	}
	if (new_counter != delete_counter)
		cout<<"Leak detected!\n";
	else
		cout<<"Seems OK...\n";
	return 0;
}
//
//在上面的例子中，统计new和delete这两个单词的数目是不是一样。
//注意：smatch对象，它其实是一个match_results类型，分 别用来记录reg中的索引是否被匹配。
//如果“new”被匹配了，那么m[1].matched就会为真(true)；m[0].second表示it向后 移动了，继续匹配剩下的字符串。
//
//m[0]，返回的是对匹配整个正则表达式的子匹配的引用，因此可以确定这个匹配的结束点通常是下次运行regex_search函数的起始点。