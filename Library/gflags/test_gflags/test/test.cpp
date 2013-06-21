// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "gflags/gflags.h"
#pragma comment(lib, "libgflags-debug.lib")

//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//

#include <iostream>

#include "foo.h"
//int main(int argc, char ** argv) {…}
//其中argc指明了argv数组的长度，而argv数组则空格为分割，存储命令行后用户输入的参数
//int main(int argc,char*argv[])
//{
//	std::string usage("This program does nothing.  Sample usage:\n");
//	usage += std::string(argv[0])+" --port 1234 \n or :\n -flagfile=foo.conf";
//
//	google::SetUsageMessage(usage);  //用来设定usage说明
//
//	//RegisterFlagValidator注册参数检查函数  第一个参数为之前定义的需接续的参数	第二个参数为检查函数	具体的各个函数定义可见gflags.h
//	bool port_dummy = google::RegisterFlagValidator(&FLAGS_port, &ValidatePort);
//
//	//参数定义和注册函数均完成后，调用ParseCommandLineFlags函数开始解析命令行。
//	google::ParseCommandLineFlags(&argc,&argv,true);
//
//	std::cout<<"port :"<< FLAGS_port<< std::endl;
//
//	return EXIT_SUCCESS;
//}


#include <Windows.h>
int main(int argc,char*argv[])
{
	std::string usage("This program does nothing.  Sample usage:\n");

	int iargc = 0;
	char szargv[MAX_PATH] = {0};
	usage += std::string(argv[0])+" --port 1234 \n or :\n -flagfile=foo.conf";

	google::SetUsageMessage(usage);  //用来设定usage说明

	//RegisterFlagValidator注册参数检查函数  第一个参数为之前定义的需接续的参数	第二个参数为检查函数	具体的各个函数定义可见gflags.h
	bool port_dummy = google::RegisterFlagValidator(&FLAGS_port, &ValidatePort);

	//参数定义和注册函数均完成后，调用ParseCommandLineFlags函数开始解析命令行。
	google::ParseCommandLineFlags(&argc,&argv,true);

	std::cout<<"port :"<< FLAGS_port<< std::endl;

	return EXIT_SUCCESS;
}
