// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

//
//int _tmain(int argc, _TCHAR* argv[])
//{
//	return 0;
//}
//


#include <string>
#include <boost/filesystem.hpp>
using namespace boost;

int main(int argc, char *argv[])
{
    std::string mydir = "D:\\temp\\data\\map\\level";
	//创建多级目录
    filesystem::create_directories(mydir);
    return 0;
}