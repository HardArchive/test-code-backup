// test.cpp : �������̨Ӧ�ó������ڵ㡣
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
	//�����༶Ŀ¼
    filesystem::create_directories(mydir);
    return 0;
}