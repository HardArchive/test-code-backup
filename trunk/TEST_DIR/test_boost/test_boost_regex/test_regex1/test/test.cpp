// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//#include <iostream>
//#include <boost/regex.hpp>
//
//using namespace std;


#include <cstdlib>
#include <stdlib.h>
#include <boost/regex.hpp>
#include <string>
#include <iostream>
using namespace std;
using namespace boost;


//当我们输入“select 字符串 from 字符串”的时候，会得到输出；如果不符合这个格式，就会输出error。
//  ([a-zA-Z]*) 表示零个或多个字母（大小写都可）组成的字符串
regex expression("select ([a-zA-Z]*) from ([a-zA-Z]*)");

int main(int argc, char* argv[])
{
	std::string in;
	cmatch what;
	cout << "enter test string" << endl;
	// select aaa from bbb
	getline(cin, in);
	if(regex_match(in.c_str(), what, expression))
	{
		//for(int i=0;i<what.size();i++)
		//	cout<<"str :"<<what.str()<<endl;
		//显示所有子串
		for(boost::cmatch::iterator itr= what.begin(); itr != what.end(); ++itr)
		{
			//       指向子串对应首位置        指向子串对应尾位置          子串内容
			cout << "子串对应首位置：" << itr->first-in.c_str() ;
			cout << "  子串对应尾位置：" << itr->second-in.c_str();
			cout << "  子串内容      ：" << *itr << endl;
		}
	}
	else
	{
		cout<<"Error Input"<<endl;
	}
	return 0;
}