#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
using namespace std;

void main2()
{
	try{
		boost::filesystem::path current_path = boost::filesystem::current_path();
		boost::filesystem::path file_path = current_path / "testfile";
		boost::posix_time::ptime ptime = boost::posix_time::second_clock::local_time();
		if(!boost::filesystem::exists(file_path))
		{
			boost::filesystem::create_directory(file_path);
		}
		std::string file = file_path.string() + "\\test.txt";
	
		ofstream out(file.c_str());
		out<<ptime;
		cout<<"输出文件内容："<<ptime<<endl;
		out.close();

	}
	catch(exception err)
	{
		cout<<"error"<<endl;
	}
}
