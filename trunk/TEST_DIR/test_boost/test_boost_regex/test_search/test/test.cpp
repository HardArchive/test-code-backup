// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <boost/regex.hpp>

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

int _tmain(int argc, _TCHAR* argv[])
{
	std::ifstream fs("C:\\Lookup\\112.127.141.86.html");
	std::string in;
	load_file(in, fs);
	fs.close();

	//std::string regstr = "(\\d+)";
	std::string regstr = "target=\"_blank\"\>(.+?)\</a\>\</td\>";
	//std::string regstr = "target=\"_blank\"\>(.*?)\</a\>\</td\>";
	//target="_blank">.*</a></td>
	boost::regex expression(regstr);
	std::string testString = "http://www.cppprog.com/2009/0112/48.html";
	boost::smatch what;
	//std::string::const_iterator start = testString.begin();
	//std::string::const_iterator end = testString.end();
	std::string::const_iterator start = in.begin();
	std::string::const_iterator end = in.end();
	while( boost::regex_search(start, end, what, expression) )
	//if (boost::regex_search(in, what, expression))
	{
		std::cout<< "Have digit：" ; 
		std::string msg(what[0].first, what[0].second);
		std::cout<< msg.c_str() << std::endl;
		
		std::cout<< "what[1].first:" <<what[1].str().c_str() <<std::endl;
		//std::cout<< "what[1].second:" << what[1].second <<std::endl;
		start = what[0].second;
	}

	std::cin.get();
	return 0;
}

