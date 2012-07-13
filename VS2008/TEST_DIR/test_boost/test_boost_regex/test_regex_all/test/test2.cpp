// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}


#include <iostream>
#include <boost/regex.hpp>
//#include "Regex1/regex.hpp"
using namespace std;

//限定符用来指定正则表达式的一个给定原子必须要出现多少次才能满足匹配。
//有“*”(零次或多次)、“+”(一次或多次)、“？”(零次或一次)、“{n}”(n 次)、“{n,}”(至少n 次)、“{n，m}”(最少匹配 n 次且最多匹配 m 次)共6中限定符，
//他们之间的区别主要是重复匹配的次数不同。其中"*"、“+”、“{n，}”限定符都是贪婪的 ，因为他们会尽可能多的去匹配文字。如下所示：
//  \s  匹配任何空白字符，包括空格、制表符、换页符等等。 
//   /a\s*b/     // "\s"表示空白原子，他可以匹配a b之间没有空白、有一个或者多个空白的情况
//   /a\d+b/     // "\d"表示一个数字字符。等价于 [0-9]。可以匹配ab之间有一个或者多个数字的情况
//   /a\W?b/     // "\W"非单词字符。等价于 '[^A-Za-z0-9_]'可以匹配ab之间有一个或者没有特殊字符 
//   /ax{2，}b/  //可以匹配ab之间至少有2个x字符
//默认情况下，正则表达式使用最长匹配原则（也叫贪婪匹配原则）。


//贪婪匹配
//默认情况下，正则表达式使用最长匹配原则（也叫贪婪匹配原则）。
//例如：要将"zoom"中匹配"zo?"的部 分替换成"r",替换的的结果是"rom"。如果要将"zoom"中匹配"zo*"   的部分替换成"r",替换后的结果是"rm"。
//
//非贪婪匹配
//当字符？紧随其他限定符（*、+、？、{n}、{n,}、{n,m}）之后时，匹配模式变成了最短匹配原则（也 叫非贪婪匹配原则）。
//例如：在字符串"fooood"中，"fo+?"只匹配"fo"部分，而"fo+"匹配"foooo部分。

//当正则表达式中包含能接受重复的限定符时，通常的行为是（在使整个表达式能得到匹配的前提下）匹配尽可能多的字符。
//以这个表达式为例：a.*b，它将会匹配最长的以a开始，以b结束的字符串。如果用它来搜索aabab的话，它会匹配整个字符串aabab。这被称为贪婪匹配。
//有时，我们更需要懒惰匹配，也就是匹配尽可能少的字符。前面给出的限定符都可以被转化为懒惰匹配模式，只要在它后面加上一个问号?。
//这样.*?就意味着匹配任意数量的重复，但是在能使整个匹配成功的前提下使用最少的重复。现在看看懒惰版的例子吧：
//a.*?b 匹配最短的，以a开始，以b结束的字符串。如果把它应用于aabab的话，它会匹配aab和ab。


int main(int argc, char* argv[])

{    //( 1 )   ((  3  )  2 )((  5 )4)(    6    )   
	//(\w+)://((\w+\.)*\w+)((/\w*)*)(/\w+\.\w+)?
	//^协议://网址(x.x...x)/路径(n个\字串)/网页文件(xxx.xxx)
	//\w  匹配包括下划线的任何单词字符。等价于'[A-Za-z0-9_]'。 /W与之相反 匹配任何非单词字符。等价于 '[^A-Za-z0-9_]'
	//+ 匹配前面的子表达式一次或多次。( ) 标记一个子表达式的开始和结束位置   * 匹配前面的子表达式零次或多次 
	//? 匹配前面的子表达式零次或一次(等价于 {0,1})，或指明一个非贪婪限定符。 什么是非贪婪限定符 ? 。 

	//                   (\w+)://((\w+\.)*\w+)((/\w*)*)(/\w+\.\w+)?
	const char *szReg = "(\\w+)://((\\w+\\.)*\\w+)((/\\w*)*)(/\\w+\\.\\w+)?";
	const char *szStr = "http://www.cppprog.com/2009/0112/48.html";

	{    //字符串匹配
		boost::regex reg( szReg );
		bool r=boost::regex_match( szStr , reg);
		assert(r);
	}

	{    //提取子串
		boost::cmatch mat;
		boost::regex reg( szReg );
		bool r=boost::regex_match( szStr, mat, reg);
		if(r) //如果匹配成功
		{
			//显示所有子串
			for(boost::cmatch::iterator itr=mat.begin(); itr!=mat.end(); ++itr)
			{
				//       指向子串对应首位置        指向子串对应尾位置          子串内容
				cout << itr->first-szStr << ' ' << itr->second-szStr << ' ' << *itr << endl;
			}
		}
		//也可直接取指定位置信息
		if(mat[4].matched) cout << "Path is" << mat[4] << endl;
	}


	{ //查找
		const char *szReg1 = "(\\d+)";
		std::string szStr1 = "http://www.cppprog.com/2009/0112/48.html";
		boost::cmatch mat;
		boost::smatch smt;
		boost::regex reg(szReg1);    //查找字符串里的数字
		std::string::const_iterator start = szStr1.begin();
		std::string::const_iterator end = szStr1.end();
		while(boost::regex_search(start, end, smt, reg))
		{
			//cout << "searched:" << mat[0] << endl;
			//显示所有子串
			//for(boost::cmatch::iterator itr=mat.begin(); itr!=mat.end(); ++itr)
			//{
			//	//       指向子串对应首位置        指向子串对应尾位置          子串内容
			//	cout << "searched:" << *itr << endl;
			//}
			std::cout<< "Have digit：" ; 
			std::string msg(smt[1].first, smt[1].second);
			std::cout<< msg.c_str() << std::endl;
			start = smt[0].second;
		}
	}

	{ //替换
		boost::regex reg( szReg );
		string s = boost::regex_replace( string(szStr), reg, "ftp://$2$5");
		cout << "ftp site:"<< s << endl;
	}

	{ //替换2，把<>&转换成网页字符
		string s1 = "(<)|(>)|(&)";
		string s2 = "(?1&lt;)(?2&gt;)(?3&amp;)";
		boost::regex reg( s1 );
		string s = boost::regex_replace( string("cout << a&b << endl;"), reg, s2, boost::match_default | boost::format_all);
		cout << "HTML:"<< s << endl;
	}

	{ //使用迭代器找出所有数字
		boost::regex reg( "\\d+" );    //查找字符串里的数字
		boost::cregex_iterator itrBegin = make_regex_iterator(szStr,reg); //(szStr, szStr+strlen(szStr), reg);
		boost::cregex_iterator itrEnd;
		for(boost::cregex_iterator itr=itrBegin; itr!=itrEnd; ++itr)
		{
			//       指向子串对应首位置        指向子串对应尾位置          子串内容
			cout << (*itr)[0].first-szStr << ' ' << (*itr)[0].second-szStr << ' ' << *itr << endl;
		}
	}

	{ //使用迭代器拆分字符串
		boost::regex reg("/");  //按/符拆分字符串
		boost::cregex_token_iterator itrBegin = make_regex_token_iterator(szStr,reg,-1); //使用-1参数时拆分，使用其它数字时表示取第几个子串，可使用数组取多个串
		boost::cregex_token_iterator itrEnd;
		for(boost::cregex_token_iterator itr=itrBegin; itr!=itrEnd; ++itr)
		{
			cout << *itr << endl;
		}
	}

	{ //使用迭代器拆分字符串2
		boost::regex reg("(.)/(.)");  //取/的前一字符和后一字符（这个字符串形象貌似有点邪恶-_-）
		int subs[] = {1,2};        // 第一子串和第二子串
		boost::cregex_token_iterator itrBegin = make_regex_token_iterator(szStr,reg,subs); //使用-1参数时拆分，使用其它数字时表示取第几个子串，可使用数组取多个串
		boost::cregex_token_iterator itrEnd;
		for(boost::cregex_token_iterator itr=itrBegin; itr!=itrEnd; ++itr)
		{
			cout << *itr << endl;
		}
	}

	cin.get();
	return 0;

} 
