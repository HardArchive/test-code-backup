// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
#include <string>
#include <iostream>
#include <boost/program_options.hpp>
using namespace std;

int main (int ac, char* av[])
{
	std::string file1 = "1";
	std::string file2= "1";
	std::string file3= "1";
	std::string file4= "1";
	bool daemon = false;


	boost::program_options::options_description options("command line options");
	options.add_options() ("help,h", "Use -h or --help to list all arguments")
		("file1", boost::program_options::value<string>(&file1),"Provide input file name1")
		("file2", boost::program_options::value<string>(&file2),"Provide input file name2")
		("file3", boost::program_options::value<string>(&file3),"Provide input file name3")
		("file4", boost::program_options::value<string>(&file4),"Provide input file name4")
		("daemon", boost::program_options::bool_switch(&daemon), "Provide input file name5")
		;

	boost::program_options::variables_map vmap;


	try
	{
		boost::program_options::store(boost::program_options::parse_command_line(ac, av,options), vmap);
		boost::program_options::notify(vmap);
	}

	catch (std::exception & e)
	{
		std::cout << "Exception: " << e.what() << "\n";
		return -1;
	}

	cout << options << endl;
	cout << "file1: " << file1<< endl;        
	cout << "file2: " << file2<< endl;
	cout << "file3: " << file3<< endl;    
	cout << "file4: " <<file4 << endl;    
	cout << "daemon: " <<daemon << endl;
	//根据各参数值定制功能...
	{
		//...
	}

	std::cout<<"over"<<endl;
	int a;
	cin>>a;
	return 0;
}
//程序运行命令
//test.exe --file4 123 --file2 234 