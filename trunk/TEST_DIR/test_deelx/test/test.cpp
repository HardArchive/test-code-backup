// test.cpp : 定义控制台应用程序的入口点。
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
		//GetEnd匹配成功后，获取所匹配到的子字符串的结束位置。如果匹配失败，则返回负值。
		//GetStart匹配成功后，获取所匹配到的子字符串的开始位置。如果匹配失败，则返回负值。
		printf("%.*s\n", result1.GetEnd() - result1.GetStart(), in.c_str() + result1.GetStart());
		// get next
		result1 = regexp1.Match(in.c_str(), result1.GetEnd());  //返回匹配结果 MatchResult 对象。
		// 通过 MatchResult 对象，可以得知是否匹配成功。如果成功，通过 MatchResult 对象可以获取捕获信息。

	}

	// text
	char * text = "http://www.cppprog.com/2009/0112/48.html";
	// declare
	static CRegexpT <char> regexp("\\d+");
	// loop
	MatchResult result = regexp.Match(text);
	//IsMatched返回非零值表示匹配成功，返回 0 表示匹配失败。
	while( result.IsMatched() )
	{  
		//GetEnd匹配成功后，获取所匹配到的子字符串的结束位置。如果匹配失败，则返回负值。
		//GetStart匹配成功后，获取所匹配到的子字符串的开始位置。如果匹配失败，则返回负值。
		printf("%.*s\n", result.GetEnd() - result.GetStart(), text + result.GetStart());
		// get next
		result = regexp.Match(text, result.GetEnd());  //返回匹配结果 MatchResult 对象。
		// 通过 MatchResult 对象，可以得知是否匹配成功。如果成功，通过 MatchResult 对象可以获取捕获信息。

	}
	return 0;
}

//
//// %.*s 其中的.*表示显示的精度 对字符串输出(s)类型来说就是宽度
//// 这个*代表的值由后面的参数列表中的整数型(int)值给出
//
//// 例如：
//printf("%.*s\n", 1, "abc");        // 输出a
//printf("%.*s\n", 2, "abc");        // 输出ab
//printf("%.*s\n", 3, "abc");        // 输出abc >3是一样的效果 因为输出类型type = s，遇到'\0'会结束
